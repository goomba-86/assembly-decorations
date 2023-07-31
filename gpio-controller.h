#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

#include <string>

enum class Direction
{
    In,
    Out
};

enum class PinValue
{
    Low,
    High
};

class GpioController
{
public:
    virtual ~GpioController() = default;
    virtual void Write(PinValue value) const = 0;
    virtual std::string Read() const = 0;
    virtual void SetDirection(Direction direction) = 0;

    inline static const std::string gpioSysfsPath = "/sys/class/gpio";
};

#endif
