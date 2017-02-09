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

#ifndef LCD_I2C_H
#define LCD_I2C_H

//-------------------------------------------------------------------------

#include <cstdint>
#include <cstring>
#include <string>

#include "FileDescriptor.h"
#include "Lcd.h"

//-------------------------------------------------------------------------

namespace HD44780
{

//-------------------------------------------------------------------------

class LcdI2C
:
    public Lcd
{
public:

    enum Mode
    {
        COMMAND = 0x00,
        DATA = 0x01
    };

    //---------------------------------------------------------------------

    LcdI2C(
        const std::string& device,
        uint8_t address,
        uint8_t columns,
        uint8_t rows,
        bool linger = false);

    ~LcdI2C();

    LcdI2C(const LcdI2C&) = delete;
    LcdI2C& operator= (const LcdI2C&) = delete;

    //---------------------------------------------------------------------

    void clear() const override;
    void home() const override;

    void noDisplay() override;
    void display() override;

    void noBlink() override;
    void blink() override;

    void noCursor() override;
    void cursor() override;

    void scrollDisplayLeft() override;
    void scrollDisplayRight() override;

    void leftToRight() override;
    void rightToLeft() override;

    void autoscroll() override;
    void noAutoscroll() override;

    void backlight() override;
    void noBacklight() override;

    void
    createChar(
        uint8_t number,
        const std::array<uint8_t, 8>& data) const override;

    void setCursor(uint8_t column, uint8_t row) const override;

    using Lcd::write;

    size_t write(uint8_t value) const override;

    //---------------------------------------------------------------------

private:

    void sendLcdCommand(uint8_t value) const;
    void sendLcdData(uint8_t value) const;
    void sendLcd(uint8_t value, Mode mode) const;
    void sendLcdNibble(uint8_t value) const;
    void toggleLcdEnableBit(uint8_t value) const;

    FileDescriptor fd_;
    uint8_t backlight_;
    uint8_t displayControl_;
    uint8_t displayFunction_;
    uint8_t displayMode_;
};

//-------------------------------------------------------------------------

} // namespace HD44780

//-------------------------------------------------------------------------

#endif
