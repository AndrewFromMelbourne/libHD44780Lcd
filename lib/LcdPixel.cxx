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
    _lcd(lcd),
    _row(row),
    _column(column),
    _changed(255),
    _setup(false)
{
    for (uint8_t i = 0 ; i < 64 ; ++i)
    {
        _pixels[i] = 0;
    }
}

//-------------------------------------------------------------------------

HD44780::LcdPixel::LcdPixel(
    const HD44780::LcdPixel& lcdpixels)
:
    _lcd(lcdpixels._lcd),
    _row(lcdpixels._row),
    _column(lcdpixels._column),
    _changed(lcdpixels._changed)
{
    for (uint8_t i = 0 ; i < 64 ; ++i)
    {
        _pixels[i] = lcdpixels._pixels[i];
    }
}

//-------------------------------------------------------------------------

HD44780::LcdPixel&
HD44780::LcdPixel::operator=(
    const HD44780::LcdPixel& lcdpixels)
{
    if (this != &lcdpixels)
    {
        _row = lcdpixels._row;
        _column = lcdpixels._column;
        _changed = lcdpixels._changed;

        for (uint8_t i = 0 ; i < 64 ; ++i)
        {
            _pixels[i] = lcdpixels._pixels[i];
        }
    }

    return *this;
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::clear()
{
    for (uint8_t j = 0 ;  j < 8 ; ++j)
    {
        for (uint8_t i = 0 ; i < 8 ; ++i)
        {
            if (_pixels[(j * 8) + i] != 0)
            {
                _pixels[(j * 8) + i] = 0;

                _changed |= (1 << j);
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

    if (_pixels[(ch * 8) + row] & (1 << bit))
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

    if ((_pixels[(ch * 8) + row] & (1 << bit)) == 0)
    {
        _pixels[(ch * 8) + row] |= (1 << bit);
        _changed |= (1 << ch);
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

    if ((_pixels[(ch * 8) + row] & (1 << bit)) != 0)
    {
        _pixels[(ch * 8) + row] &= ~(1 << bit);
        _changed |= (1 << ch);
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::update()
{
    for (uint8_t ch = 0 ; ch < 8 ; ++ch)
    {
        if (_changed & (1 << ch))
        {
            _lcd.createChar(ch, _pixels + (ch * 8));
        }
    }

    _changed = 0;

    if (_setup == false)
    {
        setup();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setColumn(
    uint8_t column)
{
    if (column != _column)
    {
        _column = column;
        print();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setRow(
    uint8_t row)
{
    if (row != _row)
    {
        _row = row;
        print();
    }
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::setup()
{
    _setup = true;
    print();
}

//-------------------------------------------------------------------------

void
HD44780::LcdPixel::print() const
{
    _lcd.clear();

    uint8_t i = 0;
    for (uint8_t row = _row ; row < _row + 2 ; ++row)
    {
        _lcd.setCursor(_column, row);

        for (uint8_t column = _column ; column < _column + 4 ; ++column)
        {
            _lcd.write(i++);
        }
    }
}

//-------------------------------------------------------------------------

