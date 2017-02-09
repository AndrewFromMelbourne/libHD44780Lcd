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

#include "LcdColumnGraph.h"

//-------------------------------------------------------------------------

HD44780::LcdColumnGraph::LcdColumnGraph(
    HD44780::Lcd& lcd,
    int32_t min,
    int32_t max,
    uint8_t column,
    uint8_t firstRow,
    uint8_t lastRow)
:
    _lcd(lcd),
    _min(min),
    _max(max),
    _column(column),
    _lastRow(lastRow),
    _rows(lastRow - firstRow + 1),
    _setup(false)
{
}

//-------------------------------------------------------------------------

void
HD44780::LcdColumnGraph::setup()
{
    for (uint8_t letter = 0 ; letter < 8 ; ++letter)
    {
        std::array<uint8_t, 8> pixels;

        for (uint8_t i = 0 ; i < 8 ; ++i)
        {
            if ((letter + 1) > (7 - i))
            {
                pixels[i] = 0b11111;
            }
            else
            {
                pixels[i] = 0;
            }
        }

        _lcd.createChar(letter, pixels);
    }

    _setup = true;
}

//-------------------------------------------------------------------------

void
HD44780::LcdColumnGraph::unscaled(
    uint8_t value)
{
    if (not _setup)
    {
        setup();
    }

    uint8_t letters = value / 8;
    uint8_t bar = value % 8;
    uint8_t letter = 0;


    for ( ; letter < letters ; ++letter)
    {
        _lcd.setCursor(_column, _lastRow - letter);
        _lcd.write((uint8_t)7);
    }

    if (bar)
    {
        _lcd.setCursor(_column, _lastRow - letter);
        _lcd.write(bar-1);
        ++letter;
    }

    for ( ; letter < _rows ; ++letter)
    {
        _lcd.setCursor(_column, _lastRow - letter);
        _lcd.write(' ');
    }
}

