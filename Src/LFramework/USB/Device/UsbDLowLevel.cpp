#ifndef _WIN32

#include <LFrameworkConfig.h>

#ifdef LF_USB_DEVICE

#include <stm32f7xx_hal.h>
#include <usbd_def.h>
#include <usbd_core.h>
#include "UsbDDevice.h"

using namespace LFramework::USB;

extern "C" {
  extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
}

extern "C" USBD_StatusTypeDef halToUsbdStatus(HAL_StatusTypeDef status){
	if(status == HAL_OK){
		return USBD_OK;
	}else if(status == HAL_ERROR){
		return USBD_FAIL;
	}else if((status == HAL_BUSY) || (status == HAL_TIMEOUT)){
		return USBD_BUSY;
	}
	return USBD_FAIL;
}

extern "C" USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps){
	USBD_StatusTypeDef status = USBD_LL_FlushEP(pdev, ep_addr);
	if(status != USBD_OK){
		return status;
	}
	return HAL_PCD_EP_Open((PCD_HandleTypeDef*)pdev->pData,ep_addr, ep_mps, ep_type) == HAL_OK ? USBD_OK : USBD_FAIL;
}

USBD_StatusTypeDef  USBD_LL_FlushEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr){
#if defined(STM32L4)//Fix for set start NACK status
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData;
	uint32_t USBx_BASE = (uint32_t)hpcd->Instance;
	if((ep_addr & 0x80) != 0){
		USBx_INEP(ep_addr & 0x7f)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
	}else{
		USBx_OUTEP(ep_addr & 0x7f)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
	}

#endif
	return HAL_PCD_EP_Flush((PCD_HandleTypeDef*)pdev->pData, ep_addr) == HAL_OK ? USBD_OK : USBD_FAIL;
}

USBD_StatusTypeDef  USBD_LL_CloseEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr){
#if defined(STM32F4) || defined(STM32L4)
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData;
	USB_OTG_GlobalTypeDef *USBx = (USB_OTG_GlobalTypeDef *)hpcd->Instance;
	uint32_t USBx_BASE = (uint32_t)USBx;
	if((ep_addr & 0x80) != 0){
		USBx_INEP(ep_addr & 0x7f)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
	}else{
		USBx_OUTEP(ep_addr & 0x7f)->DOEPCTL |= USB_OTG_DOEPCTL_EPDIS;
	}
#endif
	return HAL_PCD_EP_Close((PCD_HandleTypeDef*)pdev->pData, ep_addr) == HAL_OK ? USBD_OK : USBD_FAIL;
}

extern "C" USBD_StatusTypeDef  USBD_LL_Start(USBD_HandleTypeDef *pdev){
	return HAL_PCD_Start((PCD_HandleTypeDef*)pdev->pData) == HAL_OK ? USBD_OK : USBD_FAIL;
}

USBD_StatusTypeDef  USBD_LL_Stop (USBD_HandleTypeDef *pdev){
	return HAL_PCD_Stop((PCD_HandleTypeDef*)pdev->pData) == HAL_OK ? USBD_OK : USBD_FAIL;
}

extern "C" USBD_StatusTypeDef  USBD_LL_StallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr){
	return HAL_PCD_EP_SetStall((PCD_HandleTypeDef*)pdev->pData, ep_addr) == HAL_OK ? USBD_OK : USBD_FAIL;
}

extern "C" USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr,uint8_t *pbuf, uint16_t size){
	return halToUsbdStatus(HAL_PCD_EP_Receive((PCD_HandleTypeDef*)pdev->pData, ep_addr, pbuf, size));
}

extern "C" USBD_StatusTypeDef  USBD_LL_ClearStallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr){
/*#if defined(STM32F0)
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData;
	PCD_EPTypeDef *ep;

	if ((0x80 & ep_addr) == 0x80){
		ep = &hpcd->IN_ep[ep_addr & 0x7F];
	}
	else{
		ep = &hpcd->OUT_ep[ep_addr];
	}

	ep->is_stall = 0;
	ep->num   = ep_addr & 0x7F;
	ep->is_in = ((ep_addr & 0x80) == 0x80);

	__HAL_LOCK(hpcd);

	if (ep->is_in){
		PCD_CLEAR_TX_DTOG(hpcd->Instance, ep->num);
		//PCD_SET_EP_TX_STATUS(hpcd->Instance, ep->num, USB_EP_TX_NAK);
	} else{
		PCD_CLEAR_RX_DTOG(hpcd->Instance, ep->num);
		//PCD_SET_EP_RX_STATUS(hpcd->Instance, ep->num, USB_EP_RX_NAK);
	}
	__HAL_UNLOCK(hpcd);
#elif defined(STM32F4) || defined(STM32L4)*/
	HAL_PCD_EP_ClrStall((PCD_HandleTypeDef*)pdev->pData, ep_addr);
	//HAL_PCD_EP_ClrStall_Ex((PCD_HandleTypeDef*)pdev->pData, ep_addr);
/*#else

#error "Not supported controller"
#endif*/
	return USBD_OK;
}

extern "C" uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr){
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef*)pdev->pData;
	if((ep_addr & 0x80) == 0x80){
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	} else{
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}



extern "C" USBD_StatusTypeDef USBD_LL_SetUSBAddress (USBD_HandleTypeDef *pdev, uint8_t dev_addr){
	return halToUsbdStatus(HAL_PCD_SetAddress((PCD_HandleTypeDef*)pdev->pData, dev_addr));
}

extern "C" USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr,uint8_t *pbuf, uint16_t size){
	return halToUsbdStatus(HAL_PCD_EP_Transmit((PCD_HandleTypeDef*)pdev->pData, ep_addr, pbuf, size));
}

extern "C" uint32_t USBD_LL_GetRxDataSize  (USBD_HandleTypeDef *pdev, uint8_t  ep_addr){
	return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef*)pdev->pData, ep_addr);
}

extern "C" void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum){
	USBD_LL_DataOutStage((USBD_HandleTypeDef*)hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

extern "C" void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum){
	USBD_LL_DataInStage((USBD_HandleTypeDef*)hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

extern "C" USBD_StatusTypeDef USBD_LL_Init (USBD_HandleTypeDef* pdev){
	auto* device = UsbDevice::getContext();

	  /* Link the driver to the stack. */
	  hpcd_USB_OTG_FS.pData = pdev;
	  pdev->pData = &hpcd_USB_OTG_FS;

	  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
	  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
	  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
	  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
	  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
	  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
	  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
	  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
	  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
	  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
	  {
	     for(;;){
	    	 asm("nop");
	     }
	  }

	  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x80);
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x40);
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x80);

	  return USBD_OK;
}

extern "C"  USBD_StatusTypeDef  USBD_LL_DeInit (USBD_HandleTypeDef *pdev){
	return HAL_PCD_DeInit((PCD_HandleTypeDef*)pdev->pData) == HAL_OK ? USBD_OK : USBD_FAIL;
}

#endif
#endif
