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

#include "LcdPixel.h"

//-------------------------------------------------------------------------

HD44780::LcdPixel::LcdPixel(
    HD44780::Lcd& lcd,
    uint8_t column,
    uint8_t row)
:
    lcd_(lcd),
    row_(row),
    column_(column),
    changed_(255),
    setup_(false),
	pixels_{}
{
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::clear()
{
    for (uint8_t ch = 0 ;  ch < 8 ; ++ch)
    {
        for (auto& row : pixels_[ch])
        {
            if (row != 0)
            {
                row = 0;
                changed_ |= (1 << ch);
            }
        }
    }
}

//-------------------------------------------------------------------------

uint8_t
HD44780::LcdPixel::getPixel(
    uint8_t x,
    uint8_t y) const
{
    x %= 20;
    y %= 16;

    uint8_t ch = ((y / 8) * 4) + (x / 5);
    uint8_t row = y % 8;
    uint8_t bit = 4 - (x % 5);

    if (pixels_[ch][row] & (1 << bit))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setPixel(
    uint8_t x,
    uint8_t y)
{
    x %= 20;
    y %= 16;

    uint8_t ch = ((y / 8) * 4) + (x / 5);
    uint8_t row = y % 8;
    uint8_t bit = 4 - (x % 5);

    if ((pixels_[ch][row] & (1 << bit)) == 0)
    {
        pixels_[ch][row] |= (1 << bit);
        changed_ |= (1 << ch);
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::unsetPixel(
    uint8_t x,
    uint8_t y)
{
    x %= 20;
    y %= 16;

    uint8_t ch = ((y / 8) * 4) + (x / 5);
    uint8_t row = y % 8;
    uint8_t bit = 4 - (x % 5);

    if ((pixels_[ch][row] & (1 << bit)) != 0)
    {
        pixels_[ch][row] &= ~(1 << bit);
        changed_ |= (1 << ch);
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::update()
{
    for (uint8_t ch = 0 ; ch < 8 ; ++ch)
    {
        if (changed_ & (1 << ch))
        {
            lcd_.createChar(ch, pixels_[ch]);
        }
    }

    changed_ = 0;

    if (setup_ == false)
    {
        setup();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setColumn(
    uint8_t column)
{
    if (column != column_)
    {
        column_ = column;
        print();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setRow(
    uint8_t row)
{
    if (row != row_)
    {
        row_ = row;
        print();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setup()
{
    setup_ = true;
    print();
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::print() const
{
    lcd_.clear();

    uint8_t i = 0;
    for (uint8_t row = row_ ; row < row_ + 2 ; ++row)
    {
        lcd_.setCursor(column_, row);

        for (uint8_t column = column_ ; column < column_ + 4 ; ++column)
        {
            lcd_.write(i++);
        }
    }
}

//-------------------------------------------------------------------------

