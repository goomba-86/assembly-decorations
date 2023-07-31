#include "gpio-controller-impl.h"

GpioControllerImpl::GpioControllerImpl(std::shared_ptr<FileIo> fileIo, Direction direction, int pinNumber) : 
        fileIo(fileIo), direction(direction), gpioPinPath(gpioSysfsPath + "/gpio" + std::to_string(pinNumber))
{
    fileIo->Write(gpioSysfsPath + "/export", std::to_string(pinNumber));  
    SetDirection(direction);
}


void GpioControllerImpl::Write(PinValue value) const
{
    auto pinValue = value == PinValue::High ? "1" : "0";
    fileIo->Write(gpioPinPath + "/value", pinValue);
}

std::string GpioControllerImpl::Read() const
{
    return fileIo->Read(gpioPinPath + "/value");
}


void GpioControllerImpl::SetDirection(Direction direction)
{
    if(direction == Direction::In)
    {
        fileIo->Write(gpioPinPath + "/direction", "in");
    }
    else
    {
        fileIo->Write(gpioPinPath + "/direction", "out");
    }
}
