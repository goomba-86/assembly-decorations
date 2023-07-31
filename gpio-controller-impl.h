#ifndef GPIOCONTROLLERIMPL_H
#define GPIOCONTROLLERIMPL_H

#include "gpio-controller.h"
#include "file-io.h"
#include <memory>

class GpioControllerImpl : public GpioController
{
public:
    GpioControllerImpl(std::shared_ptr<FileIo> fileIo, Direction direction, int pinNumber);
    void Write(PinValue value) const override;
    std::string Read() const override;
    void SetDirection(Direction direction) override;
private:
    std::shared_ptr<FileIo> fileIo;
    Direction direction;
    std::string gpioPinPath;

};

#endif
