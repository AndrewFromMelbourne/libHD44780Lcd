//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <system_error>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "LcdI2C.h"

//-------------------------------------------------------------------------

namespace
{
    std::array<uint8_t, 4> lcdRowOffset{ { 0x00, 0x40, 0x14, 0x54 } };

    static const uint8_t lcdEnableBit{ 0x04 };

    enum Commands
    {
        CLEAR = 0x01,
        HOME = 0x02,
        SET_ENTRY_MODE = 0x04,
        SET_DISPLAY_CONTROL = 0x08,
        CURSOR_SHIFT = 0x10,
        SET_FUNCTION = 0x20,
        SET_CGRAM_ADDRESS = 0x40,
        SET_DDRAM_ADDRESS = 0x80
    };

    enum EntryDirection
    {
        ENTRY_RIGHT = 0x00,
        ENTRY_LEFT = 0x02,
    };

    enum EntryShift
    {
        ENTRY_SHIFT_DECREMENT = 0x00,
        ENTRY_SHIFT_INCREMENT = 0x01
    };

    enum DisplayControl
    {
        BLINK = 0x01,
        CURSOR = 0x02,
        DISPLAY = 0x04
    };

    enum MoveType
    {
        CURSOR_MOVE = 0x00,
        DISPLAY_MOVE = 0x08
    };

    enum MoveDirection
    {
        MOVE_LEFT = 0x00,
        MOVE_RIGHT = 0x04
    };

    enum Mode
    {
        MODE_4BIT = 0x00,
        MODE_8BIT = 0x10
    };

    enum Lines
    {
        LINES_1 = 0x00,
        LINES_2 = 0x08
    };
    
    enum Dots
    {
        DOTS_5x8 = 0x00,
        DOTS_5x10 = 0x04
    };

    enum Backlight
    {
        BACKLIGHT_OFF = 0x00,
        BACKLIGHT_ON = 0x08
    };

};

//-------------------------------------------------------------------------

HD44780::LcdI2C::LcdI2C(
    const std::string& device,
    uint8_t address,
    uint8_t columns,
    uint8_t rows,
    bool linger)
:
    HD44780::Lcd{ columns, rows, linger },
    fd_{-1},
    backlight_{BACKLIGHT_ON},
    displayControl_{ DISPLAY },
    displayFunction_{ MODE_4BIT | LINES_2 | DOTS_5x8 },
    displayMode_{ ENTRY_LEFT | ENTRY_SHIFT_DECREMENT }
{
    fd_ = FileDescriptor{::open(device.c_str(), O_RDWR)};

    if (fd_.fd() == -1)
    {
        std::string what( "open "
                        + device
                        + " " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }

    if (ioctl(fd_.fd(), I2C_SLAVE, address) == -1)
    {
        std::string what( "ioctl I2C_SLAVE " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }

    usleep(50);

    // reset expander and turn off backlight

    if (i2c_smbus_write_byte(fd_.fd(), backlight_) == -1)
    {
        std::string what( "write " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }
    usleep(1000);

    sendLcdNibble(0x03 << 4);
    usleep(4500);

    sendLcdNibble(0x03 << 4);
    usleep(4500);
    
    sendLcdNibble(0x03 << 4);
    usleep(150);
    
    sendLcdNibble(0x02 << 4);
    sendLcdCommand(SET_FUNCTION | displayFunction_);

    display();
    clear();

    leftToRight();

    home();
}

//-------------------------------------------------------------------------

HD44780::LcdI2C::~LcdI2C()
{
    if (Lcd::linger_ == false)
    {
        noBacklight();
        noDisplay();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::clear() const
{
    sendLcdCommand(CLEAR);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::home() const
{
    sendLcdCommand(HOME);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::noDisplay()
{
    displayControl_ &= ~DISPLAY;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::display()
{
    displayControl_ |= DISPLAY;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::noBlink()
{
    displayControl_ &= ~BLINK;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::blink()
{
    displayControl_ |= BLINK;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::noCursor()
{
    displayControl_ &= ~CURSOR;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::cursor()
{
    displayControl_ |= CURSOR;
    sendLcdCommand(SET_DISPLAY_CONTROL | displayControl_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::scrollDisplayLeft()
{
    sendLcdCommand(CURSOR_SHIFT | DISPLAY_MOVE | MOVE_LEFT);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::scrollDisplayRight()
{
    sendLcdCommand(CURSOR_SHIFT | DISPLAY_MOVE | MOVE_RIGHT);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::leftToRight()
{
    displayMode_ |= ENTRY_LEFT;
    sendLcdCommand(SET_ENTRY_MODE | displayMode_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::rightToLeft()
{
    displayMode_ &= ~ENTRY_LEFT;
    sendLcdCommand(SET_ENTRY_MODE | displayMode_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::autoscroll()
{
    displayMode_ |= ENTRY_SHIFT_INCREMENT;
    sendLcdCommand(SET_ENTRY_MODE | displayMode_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::noAutoscroll()
{
    displayMode_ &= ~ENTRY_SHIFT_INCREMENT;
    sendLcdCommand(SET_ENTRY_MODE | displayMode_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::backlight()
{
    backlight_ = BACKLIGHT_ON;

    sendLcdCommand(0);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::noBacklight()
{
    backlight_ = BACKLIGHT_OFF;

    sendLcdCommand(0);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::createChar(
    uint8_t number,
    uint8_t* data) const
{
    sendLcdCommand(SET_CGRAM_ADDRESS | (number << 3));

    for (int i = 0 ; i < 8 ; ++i)
    {
        sendLcdData(data[i]);
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::setCursor(
    uint8_t column,
    uint8_t row) const
{
    if ((column >= Lcd::columns_) || (row >= Lcd::rows_))
    {
        return;
    }

    sendLcdCommand(SET_DDRAM_ADDRESS | (column + lcdRowOffset[row])); 
}

//-------------------------------------------------------------------------

size_t
HD44780::LcdI2C::write(
    uint8_t value) const
{
    sendLcdData(value);

    return 1;
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::sendLcdCommand(
    uint8_t value) const
{
    sendLcd(value, COMMAND);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::sendLcdData(
    uint8_t value) const
{
    sendLcd(value, DATA);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::sendLcd(
    uint8_t value,
    Mode mode) const
{
    sendLcdNibble(mode | (value & 0xF0));
    sendLcdNibble(mode | ((value << 4) & 0xF0));
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::sendLcdNibble(
    uint8_t value) const
{
    if (i2c_smbus_write_byte(fd_.fd(), value | backlight_) == -1)
    {
        std::string what( "write " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }
    toggleLcdEnableBit(value | backlight_);
}

//-------------------------------------------------------------------------

void
HD44780::LcdI2C::toggleLcdEnableBit(
    uint8_t value) const
{
    if (i2c_smbus_write_byte(fd_.fd(), value | lcdEnableBit) == -1)
    {
        std::string what( "write " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }
    usleep(500);
    if (i2c_smbus_write_byte(fd_.fd(), value & ~lcdEnableBit) == -1)
    {
        std::string what( "write " __FILE__ "("
                        + std::to_string(__LINE__)
                        + ")" );
        throw std::system_error(errno, std::system_category(), what);
    }
    usleep(500);
}

