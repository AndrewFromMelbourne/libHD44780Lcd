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

#ifndef LCD_PIXELS_H
#define LCD_PIXELS_H

//-------------------------------------------------------------------------

#include <cstdint>

#include "Lcd.h"

//-------------------------------------------------------------------------

namespace HD44780
{

//-------------------------------------------------------------------------

class LcdPixel
{
public:

    LcdPixel(
        Lcd& lcd,
        uint8_t column = 0,
        uint8_t row = 0);

    LcdPixel(const LcdPixel& lcdpixel);

    LcdPixel& operator=(const LcdPixel& lcdpixel);

    void clear();
    uint8_t getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y);
    void unsetPixel(uint8_t x, uint8_t y);

    void update();

    void setColumn(uint8_t column);
    void setRow(uint8_t row);

private: 

    void print() const;
    void setup();

    Lcd& _lcd;
    uint8_t _row;
    uint8_t _column;
    uint8_t _changed;
    bool _setup;
    uint8_t _pixels[64];
};

//-------------------------------------------------------------------------

} // namespace HD44780

//-------------------------------------------------------------------------

#endif
