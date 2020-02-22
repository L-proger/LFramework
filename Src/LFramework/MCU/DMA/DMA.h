/*
 * DMA.h
 *
 *  Created on: 25 сент. 2017 г.
 *      Author: l-pro
 */

#ifndef LFRAMEWORK_MCU_DMA_DMA_H_
#define LFRAMEWORK_MCU_DMA_DMA_H_


namespace EmbeddedFramework {

class DMA{

};

template<int Number, int Channel>
class DmaIrqHandler{

};

template<int Number, int Channel>
class DmaCallback {
public:
	/*static void error();
	static void complete();
	static void halfComplete();*/
};


#endif /* LFRAMEWORK_MCU_DMA_DMA_H_ */
