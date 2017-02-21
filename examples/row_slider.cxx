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

#include "LcdI2C.h"
#include "LcdRowSlider.h"
#include "LinuxKeys.h"

#include <cmath>
#include <iostream>

//-------------------------------------------------------------------------

int
main()
{
    try
    {
        HD44780::LcdI2C lcd {"/dev/i2c-1", 0x27, 20, 4};

        HD44780::LcdRowSlider lrs(lcd, 0, 255, 0, 0, 19);

        lcd.clear();

        int value = 0;
        int inc = 1;
        LinuxKeys linuxKeys;
        LinuxKeys::PressedKey key;

        //-----------------------------------------------------------------

        while(key.key != 27)
        {
            key = linuxKeys.pressed();

            lrs.unscaled(value);

            value += inc;

            if (value < 0)
            {
                value = 0;
                inc = 1;
            }
            else if (value > 99)
            {
                value = 99;
                inc = -1;
            }
        }

        //-----------------------------------------------------------------

        lcd.clear();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

