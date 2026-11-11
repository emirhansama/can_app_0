#include "can_comm.h"
#include "can.h"
#include "can_app_config.h"
#include "main.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal_can.h"
#include "stm32f7xx_hal_def.h"

#define CAN_STD_ID_SHIFT        (5)
#define CAN_STD_ID_MASK         (0x7F8)
#define CAN_RTR_BIT_MASK        (0x02)
#define CAN_IDE_BIT_MASK        (0x04)

bool can_comm_init(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterActivation = ENABLE;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.SlaveStartFilterBank = 14;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;

    filter.FilterMaskIdHigh = (CAN_STD_ID_MASK << CAN_STD_ID_SHIFT);
    filter.FilterMaskIdLow = (CAN_RTR_BIT_MASK | CAN_IDE_BIT_MASK);
    
    filter.FilterIdHigh = (CAN_ID_ACCEL_X << CAN_STD_ID_SHIFT);
    filter.FilterIdLow = 0;

    if(HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
        return false;
    }

    if(HAL_CAN_Start(&hcan1) != HAL_OK){
        return false;
    }

    if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
        return false;
    }

    return true;
}

HAL_StatusTypeDef can_comm_send_request(uint16_t id, uint8_t dlc)
{
    CAN_TxHeaderTypeDef header = {0};
    uint32_t mailbox;
    uint8_t data[8] = {0};

    header.StdId = id,
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_REMOTE;
    header.DLC = dlc;
    header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox);
}

HAL_StatusTypeDef can_comm_receive(CAN_RxHeaderTypeDef *header, uint8_t *data)
{
    return HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, header, data);
}