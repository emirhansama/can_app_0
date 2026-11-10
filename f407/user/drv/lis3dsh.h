#ifndef LIS3DSH_H
#define LIS3DSH_H

#include "main.h"
#include <stdbool.h>

#define LIS3DSH_WHO_AM_I                    (0x0F)
#define LIS3DSH_DEVICE_ID                   (0x3F)
#define LIS3DSH_READ_MASK                   (0x80)

#define LIS3DSH_CTRL_REG4                   (0x20)
#define LIS3DSH_CTRL_REG5                   (0x24)

#define LIS3DSH_ODR_MASK                    (0xF0)
#define LIS3DSH_ODR_50HZ_MASK               (0x50)
#define LIS3DSH_ODR_100HZ_MASK              (0x60)
#define LIS3DSH_ODR_400HZ_MASK              (0x70)
#define LIS3DSH_ODR_800HZ_MASK              (0x80)
#define LIS3DSH_ODR_1600HZ_MASK             (0x90)

#define LIS3DSH_BDU_MASK                    (0x08)
#define LIS3DSH_XYZ_ENABLE_MASK             (0x07)

#define LIS3DSH_FS_MASK                     (0x38)
#define LIS3DSH_FS_2G_MASK                  (0x00)
#define LIS3DSH_FS_4G_MASK                  (0x08)
#define LIS3DSH_FS_6G_MASK                  (0x10)
#define LIS3DSH_FS_8G_MASK                  (0x18)
#define LIS3DSH_FS_16G_MASK                 (0x20)

#define LIS3DSH_BW_MASK                     (0xC0)
#define LIS3DSH_BW_50HZ_MASK                (0xC0)
#define LIS3DSH_OUT_X_L                     (0x28)
#define LIS3DSH_OUT_X_H                     (0x29)
#define LIS3DSH_OUT_Y_L                     (0x2A)
#define LIS3DSH_OUT_Y_H                     (0x2B)
#define LIS3DSH_OUT_Z_L                     (0x2C)
#define LIS3DSH_OUT_Z_H                     (0x2D)

#define LIS3DSH_SENS_2G                     (0.000060f)
#define LIS3DSH_SENS_4G                     (0.000120f)
#define LIS3DSH_SENS_6G                     (0.000180f)
#define LIS3DSH_SENS_8G                     (0.000240f)
#define LIS3DSH_SENS_16G                    (0.000730f)

#define LIS3DSH_CS_LOW()                    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_RESET)
#define LIS3DSH_CS_HIGH()                   HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_SET)

bool lis3dsh_check_id(void);
bool lis3dsh_init(void);

HAL_StatusTypeDef lis3dsh_set_odr(uint8_t odr);
HAL_StatusTypeDef lis3dsh_set_scale(uint8_t scale);
HAL_StatusTypeDef lis3dsh_set_bandwidth(uint8_t bandwidth);
HAL_StatusTypeDef lis3dsh_read_xyz(float *x, float *y, float *z);

#endif