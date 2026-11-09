
#include "lis3dsh.h"
#include "spi.h"

static float sensitivity = LIS3DSH_SENS_2G;

static HAL_StatusTypeDef lis3dsh_read_reg(uint8_t addr, uint8_t *value)
{
    uint8_t data[2] = {addr | LIS3DSH_READ_MASK, 0};

    LIS3DSH_CS_LOW();

    HAL_StatusTypeDef result = HAL_SPI_TransmitReceive(&hspi1, data, data, 2, 100);

    LIS3DSH_CS_HIGH();

    if (result == HAL_OK) {
        *value = data[1];
    }

    return result;
}

static HAL_StatusTypeDef lis3dsh_write_reg(uint8_t addr, uint8_t value)
{
    uint8_t data[2] = {addr, value};

    LIS3DSH_CS_LOW();

    HAL_StatusTypeDef result = HAL_SPI_Transmit(&hspi1, data, 2, 100);

    LIS3DSH_CS_HIGH();

    return result;
}

bool lis3dsh_check_id(void)
{
    uint8_t id = 0;
     
    HAL_StatusTypeDef result = lis3dsh_read_reg(LIS3DSH_WHO_AM_I, &id);

    return result == HAL_OK && id == LIS3DSH_DEVICE_ID;
}

HAL_StatusTypeDef lis3dsh_set_odr(uint8_t odr)
{
    uint8_t value;

    HAL_StatusTypeDef result = lis3dsh_read_reg(LIS3DSH_CTRL_REG4, &value);

    if (result != HAL_OK) {
        return result;
    }

    value &= ~LIS3DSH_ODR_MASK;
    value |= odr & LIS3DSH_ODR_MASK;

    value |= LIS3DSH_BDU_MASK;
    value |= LIS3DSH_XYZ_ENABLE_MASK;

    return lis3dsh_write_reg(LIS3DSH_CTRL_REG4, value);
}

HAL_StatusTypeDef lis3dsh_set_bandwidth(uint8_t bandwidth)
{
    uint8_t value;

    HAL_StatusTypeDef result = lis3dsh_read_reg(LIS3DSH_CTRL_REG5, &value);

    if (result != HAL_OK) {
        return result;
    }

    value &= ~LIS3DSH_BW_MASK;
    value |= bandwidth & LIS3DSH_BW_MASK;

    return lis3dsh_write_reg(LIS3DSH_CTRL_REG5, value);
}

bool lis3dsh_init(void)
{
    if (lis3dsh_set_bandwidth(LIS3DSH_BW_50HZ_MASK) != HAL_OK) {
        return false;
    }

    if (lis3dsh_set_scale(LIS3DSH_FS_2G_MASK) != HAL_OK) {
        return false;
    }

    if (lis3dsh_set_odr(LIS3DSH_ODR_100HZ_MASK) != HAL_OK) {
        return false;
    }

    return true;
}


HAL_StatusTypeDef lis3dsh_read_xyz(float *x, float *y, float *z)
{
    if (x == NULL || y == NULL || z == NULL) {
        return HAL_ERROR;
    }

    uint8_t data[6];

    for (uint8_t i = 0; i < 6; i++) {
        HAL_StatusTypeDef result = lis3dsh_read_reg(LIS3DSH_OUT_X_L + i, &data[i]);

        if (result != HAL_OK) {
            return result;
        }
    }

    int16_t raw_x = (int16_t)(((uint16_t)data[1] << 8) | data[0]);
    int16_t raw_y = (int16_t)(((uint16_t)data[3] << 8) | data[2]);
    int16_t raw_z = (int16_t)(((uint16_t)data[5] << 8) | data[4]);

    *x = raw_x * sensitivity;
    *y = raw_y * sensitivity;
    *z = raw_z * sensitivity;

    return HAL_OK;
}


HAL_StatusTypeDef lis3dsh_set_scale(uint8_t scale)
{
    float new_sensitivity;

    switch (scale) {
        case LIS3DSH_FS_2G_MASK:  new_sensitivity = LIS3DSH_SENS_2G;  break;
        case LIS3DSH_FS_4G_MASK:  new_sensitivity = LIS3DSH_SENS_4G;  break;
        case LIS3DSH_FS_6G_MASK:  new_sensitivity = LIS3DSH_SENS_6G;  break;
        case LIS3DSH_FS_8G_MASK:  new_sensitivity = LIS3DSH_SENS_8G;  break;
        case LIS3DSH_FS_16G_MASK: new_sensitivity = LIS3DSH_SENS_16G; break;
        default: return HAL_ERROR;
    }

    uint8_t value;

    HAL_StatusTypeDef result = lis3dsh_read_reg(LIS3DSH_CTRL_REG5, &value);

    if (result != HAL_OK) {
        return result;
    }

    value &= ~LIS3DSH_FS_MASK;
    value |= scale & LIS3DSH_FS_MASK;

    result = lis3dsh_write_reg(LIS3DSH_CTRL_REG5, value);

    if (result == HAL_OK) {
        sensitivity = new_sensitivity;
    }

    return result;
}
