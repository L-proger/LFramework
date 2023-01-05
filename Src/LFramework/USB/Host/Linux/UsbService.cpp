#include "UsbService.h"

namespace LFramework::USB {

IUsbService* createUsbService(){
    return new UsbService();
}

}