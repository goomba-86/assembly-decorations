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
void LcdStoreSmallHeartsToGCRam();
void LcdStoreLargeHeartToGCRam();

void FillScreenWithHearts();
void FallingHeartsRoutine();
void LargePumpingHeartsRoutine();
void CctfLovesYouRoutine();
void CreateSmallHeart(int position);
void CreateLargeHeart(int position);

int fd;

int main()
{
    std::cout << "Running Assembly decoration program." << std::endl;


    if (wiringPiSetup() == -1)
    {
        std::cout << "Wiring pi setup failed." << std::endl;
        return 1;
    }

    fd = wiringPiI2CSetup(LCD_ADDRESS);
    std::cout << "Wiring PI I2C setup ready." << std::endl;

    LcdInit();

    // LCD I2C pin mappings
    // rs=0, rw=1, en=2, backlight=3, d4=4, d5=5, d6=6, d7=7

    while(true)
    {
        FillScreenWithHearts();
        LcdClear();
        LargePumpingHeartsRoutine();
        LcdClear();
        FallingHeartsRoutine();
        LcdClear();
        CctfLovesYouRoutine();
        LcdClear();
    }
    return 0;
}

void FillScreenWithHearts()
{
    LcdStoreSmallHeartsToGCRam();
    LcdSetLine(LINE1);
    for(int i = 0; i < 16; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        LcdSendByte(0x01, LCD_DATA_MODE);
    }
    LcdSetLine(LINE2);
    for(int i = 0; i < 16; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        LcdSendByte(0x01, LCD_DATA_MODE);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void LargePumpingHeartsRoutine()
{
    LcdStoreLargeHeartToGCRam();
    for(int i = 0; i < 16; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        if(i % 2 == 0)
        {
            CreateLargeHeart(0);
            CreateLargeHeart(3);
            CreateLargeHeart(6);
            CreateLargeHeart(9);
            CreateLargeHeart(12);
        }
        else
        {
            CreateSmallHeart(0);
            CreateSmallHeart(3);
            CreateSmallHeart(6);
            CreateSmallHeart(9);
            CreateSmallHeart(12);
        }
    }

}

void CreateSmallHeart(int position)
{
    LcdSetLine(LINE1 + position);
    LcdSendByte(0x03, LCD_DATA_MODE);
    LcdSetLine(LINE1 + position + 1);
    LcdSendByte(0x04, LCD_DATA_MODE);
    LcdSetLine(LINE2 + position);
    LcdSendByte(0x05, LCD_DATA_MODE);
    LcdSetLine(LINE2 + position + 1);
    LcdSendByte(0x06, LCD_DATA_MODE);
}

void CreateLargeHeart(int position)
{
    LcdSetLine(LINE1 + position);
    LcdSendByte(0x00, LCD_DATA_MODE);
    LcdSetLine(LINE1 + position + 1);
    LcdSendByte(0x00, LCD_DATA_MODE);
    LcdSetLine(LINE2 + position);
    LcdSendByte(0x01, LCD_DATA_MODE);
    LcdSetLine(LINE2 + position + 1);
    LcdSendByte(0x02, LCD_DATA_MODE);
}

void FallingHeartsRoutine()
{
    size_t animationFrame = 0;
    size_t nextAnimationFrame = 2;

    LcdStoreSmallHeartsToGCRam();

    for(int i = 0; i < 16; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        for(int character = 0; character < 16; character+=2)
        {
            auto isEvenCharacter = character % 4 == 0; 
            if(isEvenCharacter)
            {
                LcdSetLine(LINE1 + character);
                LcdSendByte(animationFrame, LCD_DATA_MODE);
                LcdSetLine(LINE2 + character);
                LcdSendByte(animationFrame, LCD_DATA_MODE);
            }
            else
            {
                LcdSetLine(LINE1 + character);
                LcdSendByte(nextAnimationFrame, LCD_DATA_MODE);
                LcdSetLine(LINE2 + character);
                LcdSendByte(nextAnimationFrame, LCD_DATA_MODE);
            }
        }

        animationFrame++;
        animationFrame %= 4;

        nextAnimationFrame++;
        nextAnimationFrame %= 4;
    }
}

void CctfLovesYouRoutine()
{
    size_t animationFrame = 0;
    LcdSetLine(LINE1 + 1);
    LcdTypeLine("CCTF loves you");

    LcdStoreSmallHeartsToGCRam();

    for(int i = 0; i < 16; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        LcdSetLine(LINE1);
        LcdSendByte(animationFrame, LCD_DATA_MODE);
        LcdSetLine(LINE1 + 15);
        LcdSendByte(animationFrame, LCD_DATA_MODE);
        LcdSetLine(LINE2);
        LcdSendByte(animationFrame, LCD_DATA_MODE);
        LcdSetLine(LINE2 + 15);
        LcdSendByte(animationFrame, LCD_DATA_MODE);

        animationFrame++;
        animationFrame %= 4;
    }
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

void LcdStoreSmallHeartsToGCRam()
{
    //Heart lines
    const std::vector<char> heartLines = 
    {
        /******/
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        0b00000,
        0b00000,
        /******/
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        /******/
        0b00100,
        0b00000,
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        /******/
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        /******/
    };

    //Set CGRAM address
    LcdSendByte(0x40, LCD_COMMAND_MODE);

    //Set heart lines to CGRAM memory
    for(auto line = heartLines.begin(); line != heartLines.end(); line++)
    {
        LcdSendByte(*line, LCD_DATA_MODE);
    }
}

void LcdStoreLargeHeartToGCRam()
{
    //Heart lines
    const std::vector<char> heartLines = 
    {
        /******/
        0b00000,
        0b00000,
        0b00000,
        0b01110,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        /******/
        0b11111,
        0b01111,
        0b00111,
        0b00011,
        0b00001,
        0b00000,
        0b00000,
        0b00000,
        /******/
        0b11111,
        0b11110,
        0b11100,
        0b11000,
        0b10000,
        0b00000,
        0b00000,
        0b00000,
        /******/
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00110,
        0b01111,
        0b01111,
        0b01111,
        /******/
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b01100,
        0b11110,
        0b11110,
        0b11110,
        /******/
        0b01111,
        0b00111,
        0b00011,
        0b00001,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        /******/
        0b11110,
        0b11100,
        0b11000,
        0b10000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    //Set CGRAM address
    LcdSendByte(0x40, LCD_COMMAND_MODE);

    //Set heart lines to CGRAM memory
    for(auto line = heartLines.begin(); line != heartLines.end(); line++)
    {
        LcdSendByte(*line, LCD_DATA_MODE);
    }
}

