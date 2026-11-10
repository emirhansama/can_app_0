#include "can_comm.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_def.h"
#include "can_app_config.h"

#define CAN_STD_ID_SHIFT        (5)
#define CAN_STD_ID_MASK         (0x7F8)
#define CAN_RTR_BIT_MASK        (0x02)
#define CAN_IDE_BIT_MASK        (0x04)

bool can_comm_init(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0; //can 1 in hangi filter banki için bu struct dolduruluyor 0-14 arası (0 14 slave start kısmında güncellenebiliyor)
    filter.FilterMode = CAN_FILTERMODE_IDMASK; // ID list direkt hangi id ler kabul ediliyor listesi oluşturur idmask ise önemli bitleri eslesen herkesi kabul eder
    filter.FilterScale = CAN_FILTERSCALE_32BIT; // filtrede id ler maskler 32 bit mi olcak 16 bit mi toplam 64 alan var bana 1 maske 1 id lazım ondan 32 secmemde sakınca yok. ama extended mode kullanmadıgım icin id ler de en fazla 11 bit olacagından 16 kullanmanın da sakıncası yok.
    filter.FilterFIFOAssignment = CAN_RX_FIFO0; //bu filtredekiler hangi fifoya gidecek
    filter.FilterActivation = ENABLE; //filtre acma kapama.
    filter.SlaveStartFilterBank = 14; //bankleri nereden itibaran can 2 ye bırakıyoruz.

    filter.FilterIdHigh = (CAN_ID_ACCEL_X << CAN_STD_ID_SHIFT); // 0x100 değerini 0100 0000 0000 örnek id olarak belirliyoruz 16 bite uydurmak icin kaydırdık.
    filter.FilterIdLow = CAN_RTR_BIT_MASK; // rtr kısmı 1 olanları istiyoruz biz f746 talebiyle sadece mesaj bascaz cunku.

    filter.FilterMaskIdHigh = (CAN_STD_ID_MASK << CAN_STD_ID_SHIFT); // maske olarak 0111 1111 1000 degerini sectik ki son üc bit önemsiz olsıun 0x100 0x108 ararsı can mesajlarını kabul edelim.
    filter.FilterMaskIdLow = CAN_RTR_BIT_MASK | CAN_IDE_BIT_MASK; // rtr ve ide bitleri bit 1 ve bit 2 ide rtr bizle eşleşmeli. can2.0 a formatı ve rtr leri kabul ediyoruz bir tek


    if(HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK) {
        return false;
    }

    if(HAL_CAN_Start(&hcan1) != HAL_OK) {
        return false;
    }

    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        return false;
    }

    return true;
}

HAL_StatusTypeDef can_comm_send(uint16_t id, const uint8_t data[8], uint8_t dlc)
{
    CAN_TxHeaderTypeDef header = {0};
    uint32_t mailbox;

    header.StdId = id; //gönderilmek istenen mesajın id si.
    header.IDE = CAN_ID_STD; //can 2.0A B secimi biz a sectik
    header.RTR = CAN_RTR_DATA; //data mı gönderiyoruz uzaktan mesaj talebimi secimi
    header.DLC = dlc; // data length belirttigimiz kısım fonksiyon parama eşitleniyor.
    header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox);
}

HAL_StatusTypeDef can_comm_receive(CAN_RxHeaderTypeDef *header, uint8_t data[8])
{
    return HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, header, data);
}