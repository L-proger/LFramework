#ifndef Stm32FsPhy_h__
#define Stm32FsPhy_h__

namespace LFramework {

	template<typename Child>
	class Stm32FsPhy {
	public:
		/*void UsbDevice::start() {
			HAL_PCD_Start(_pcd);
		}*/

		//UsbDevice(PCD_HandleTypeDef* pcd);

		/*void initPHY() {

			_pcd->Instance = USB_OTG_FS;
			_pcd->Init.dev_endpoints = 1;
			_pcd->Init.speed = PCD_SPEED_FULL;
			_pcd->Init.dma_enable = DISABLE;
			_pcd->Init.Sof_enable = DISABLE;
			_pcd->Init.ep0_mps = 64;
			_pcd->Init.phy_itface = PCD_PHY_EMBEDDED;
			_pcd->Init.low_power_enable = DISABLE;
			_pcd->Init.lpm_enable = DISABLE;
			_pcd->Init.vbus_sensing_enable = DISABLE;
			_pcd->Init.use_dedicated_ep1 = DISABLE;
			//_pcd->Init.battery_charging_enable = DISABLE;
			HAL_PCD_Init(_pcd);

			//HAL_PCD_SetRxFiFo(&USBD_PCD, 0x80);
			//HAL_PCD_SetTxFiFo(&USBD_PCD, 0, 0x20);
			//HAL_PCD_SetTxFiFo(&USBD_PCD, 1, 0x40);
			
			HAL_PCD_SetRxFiFo(_pcd, 0x80);
			HAL_PCD_SetTxFiFo(_pcd, 0, 0x40);
			HAL_PCD_SetTxFiFo(_pcd, 1, 0x80);
		}*/

		void connect() {
			//HAL_PCD_DevConnect(_pcd);
		}

		void disconnect() {
			//HAL_PCD_DevDisconnect(_pcd);
		}

		//PCD_HandleTypeDef* _pcd;
	};
}
#endif // Stm32FsPhy_h__
