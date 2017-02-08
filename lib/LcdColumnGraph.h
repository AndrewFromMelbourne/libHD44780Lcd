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

#ifndef LCD_COLUMN_GRAPH_H
#define LCD_COLUMN_GRAPH_H

//-------------------------------------------------------------------------

#include <cstdint>
#include "Lcd.h"

//-------------------------------------------------------------------------

namespace HD44780
{

//-------------------------------------------------------------------------

class LcdColumnGraph 
{
public:

    LcdColumnGraph(
        Lcd& lcd,
        int32_t min,
        int32_t max,
        uint8_t column,
        uint8_t firstRow = 0,
        uint8_t lastRow = 1);

    void value(int32_t value)
    {
        unscaled(map(value, _min, _max, 0, _rows * 8));
    }

    void unscaled(uint8_t value);

    uint8_t getColumn() const { return _column; }
    void setColumn(uint8_t column) { _column = column; }

    int32_t getMin() const { return _min; }
    int32_t getMax() const { return _max; }

    void setMinMax(int32_t min, int32_t max) { _min = min; _max = max; }

private:

    long map(long x, long inMin, long inMax, long outMin, long outMax)
    {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    Lcd& _lcd;
    int32_t _min;
    int32_t _max;
    uint8_t _column;
    uint8_t _lastRow;
    uint8_t _rows;
    bool _setup;

    void setup();
};

//-------------------------------------------------------------------------

} // namespace HD44780

//-------------------------------------------------------------------------

#endif
