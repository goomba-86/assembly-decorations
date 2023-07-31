#include <iostream>
#include "gpio-controller-impl.h"
#include "file-io-impl.h"
#include <vector>
#include <chrono>
#include <thread>

#include <wiringPiI2C.h>
#include <wiringPi.h>

#define LCD_ADDRESS 0x27
#define LCD_DATA_MODE 0x1
#define LCD_COMMAND_MODE 0x0
#define LCD_BACKLIGHT 0x8
#define LINE1 0x80
#define LINE2 0xC0

#define ENABLE 0x4

void LcdInit();
void LcdSendByte(int byte, int mode);
void LcdToggleEnable(int byte);
void LcdTypeLine(const std::string& line);
void LcdTypeChar(char c);
void LcdClear();
void LcdSetLine(int line);

int fd;

int main()
{
    std::cout << "Running Assembly decoration program." << std::endl;

    std::vector<GpioControllerImpl> ledColumns;
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 20));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 16));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 12));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 21));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 19));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 25));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 26));
    ledColumns.push_back(GpioControllerImpl(std::make_shared<FileIoImpl>(), Direction::Out, 6));


    if (wiringPiSetup() == -1)
    {
        std::cout << "Wiring pi setup failed." << std::endl;
        return 1;
    }

    fd = wiringPiI2CSetup(LCD_ADDRESS);
    std::cout << "Wiring PI I2C setup ready." << std::endl;

    LcdInit();
    std::cout << "LCR init ready" << std::endl;
    LcdSetLine(LINE1);
    std::cout << "LCD line set." << std::endl;
    LcdTypeLine("CCTF loves you!");
    std::cout << "Line typed." << std::endl;

    // LCD I2C pin mappings
    // rs=0, rw=1, en=2, backlight=3, d4=4, d5=5, d6=6, d7=7


    size_t column = 0;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for(size_t i = 0; i < ledColumns.size(); i++)
        {
            if(i == column)
            {
                ledColumns[i].Write(PinValue::High);
            }
            else
            {
                ledColumns[i].Write(PinValue::Low);
            }
        }

        column++;
        column %= 8;
    }
    return 0;
}


void LcdInit()
{
    LcdSendByte(0x33, LCD_COMMAND_MODE);
    LcdSendByte(0x32, LCD_COMMAND_MODE);
    LcdSendByte(0x06, LCD_COMMAND_MODE);
    LcdSendByte(0x0C, LCD_COMMAND_MODE);
    LcdSendByte(0x28, LCD_COMMAND_MODE);
    LcdSendByte(0x01, LCD_COMMAND_MODE);
    std::this_thread::sleep_for(std::chrono::microseconds(500));
}

void LcdSendByte(int byte, int mode) 
{
    int bitsHigh;
    int bitsLow;

    bitsHigh = mode | (byte & 0xF0) | LCD_BACKLIGHT;
    bitsLow = mode | ((byte << 4) & 0xF0) | LCD_BACKLIGHT;

    wiringPiI2CReadReg8(fd, bitsHigh);
    LcdToggleEnable(bitsHigh);
    wiringPiI2CReadReg8(fd, bitsLow);
    LcdToggleEnable(bitsLow);
}

void LcdToggleEnable(int byte) 
{
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    wiringPiI2CReadReg8(fd, (byte | ENABLE));
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    wiringPiI2CReadReg8(fd, (byte & ~ENABLE));
    std::this_thread::sleep_for(std::chrono::microseconds(500));

}

void LcdTypeLine(const std::string& line) 
{
    for(auto iter = line.begin(); iter != line.end(); iter++)
    {
        LcdSendByte(*iter, LCD_DATA_MODE);
    }
}

void LcdClear() 
{
    LcdSendByte(0x01, LCD_COMMAND_MODE);
    LcdSendByte(0x02, LCD_COMMAND_MODE);
}

void LcdSetLine(int line)
{
    LcdSendByte(line, LCD_COMMAND_MODE);
}
