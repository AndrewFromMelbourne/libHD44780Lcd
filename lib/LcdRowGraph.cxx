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

#include "LcdRowGraph.h"

//-------------------------------------------------------------------------

HD44780::LcdRowGraph::LcdRowGraph(
    HD44780::Lcd& lcd,
    int32_t min,
    int32_t max,
    uint8_t row,
    uint8_t firstColumn,
    uint8_t lastColumn)
:
    _lcd(lcd),
    _min(min),
    _max(max),
    _row(row),
    _firstColumn(firstColumn),
    _columns(lastColumn - firstColumn + 1),
    _setup(false)
{
}


//-------------------------------------------------------------------------

void
HD44780::LcdRowGraph::setup()
{
    for (uint8_t letter = 0 ; letter < 5 ; ++letter)
    {
        uint8_t pixelRow = 0;

        for (int16_t bit = 5 ; bit > (4 - letter) ; --bit)
        {
            pixelRow |=  1 << (bit-1);
        }

        std::array<uint8_t, 8> pixels;

        for (uint8_t i = 0 ; i < 8 ; ++i)
        {
            pixels[i] = pixelRow;
        }

        _lcd.createChar(letter, pixels);
    }

    _setup = true;
}

//-------------------------------------------------------------------------

void
HD44780::LcdRowGraph::unscaled(
    uint8_t value)
{
    if (not _setup)
    {
        setup();
    }

    uint8_t letters = value / 5;
    uint8_t bar = value % 5;
    uint8_t letter = 0;

    _lcd.setCursor(letter + _firstColumn, _row);

    for ( ; letter < letters ; ++letter)
    {
        _lcd.write((uint8_t)4);
    }

    if (bar)
    {
        ++letter;
        _lcd.write(bar-1);
    }

    for ( ; letter < _columns ; ++letter)
    {
        _lcd.write(' ');
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdRowGraph::setColumns(
    uint8_t first,
    uint8_t last)
{
    _firstColumn = first;
    _columns = last - first + 1;
}

