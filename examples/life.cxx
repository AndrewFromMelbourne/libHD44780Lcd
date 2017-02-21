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
#include "LcdI2C.h"
#include "LinuxKeys.h"

#include <cmath>
#include <iostream>

//-------------------------------------------------------------------------

int
newLife(
    HD44780::LcdPixel& pixel)
{
    int population = 0;

    for (uint8_t row = 0 ; row < 20 ; ++row)
    {
        for (uint8_t col = 0 ; col < 16 ; ++col)
        {
            if (rand() > (RAND_MAX / 2))
            {
                ++population;

                pixel.setPixel(col, row);
            }
            else
            {
                pixel.unsetPixel(col, row);
            }
        }
    }

    pixel.update();

    return population;
}

//-------------------------------------------------------------------------

uint8_t
neighbours(
    HD44780::LcdPixel& pixel,
    uint8_t x,
    uint8_t y)
{
    int neighbours = 0;

    for (int8_t dy = -1 ; dy <= 1 ;  ++dy)
    {
        for (int8_t dx = -1 ; dx <= 1 ;  ++dx)
        {
            if ((dx == 0) && (dy == 0))
            {
                continue;
            }

            uint8_t i = x + dx;
            uint8_t j = y + dy;

            if (i < 0)
            {
                i = 19;
            }
            else if (i > 19)
            {
                i = 0;
            }

            if (j < 0)
            {
                j = 15;
            }
            else if (j > 15)
            {
                j = 0;
            }

            if (pixel.getPixel(i, j))
            {
                ++neighbours;
            }
        }
    }

    return neighbours;
}

//-------------------------------------------------------------------------

int
iterateLife(
    HD44780::LcdPixel& pixel)
{
    int population = 0;
    HD44780::LcdPixel last = pixel;

    for (uint8_t row = 0 ; row < 16 ; ++row)
    {
        for (uint8_t col = 0 ; col < 20 ; ++col)
        {
            bool live = (last.getPixel(col, row) != 0);
            int n = neighbours(last, col, row);

            if (n < 2)
            {
                live = false;
            }
            else if (live && (n == 2))
            {
                live = true;
            }
            else if (n == 3)
            {
                live = true;
            }
            else
            {
                live = false;
            }

            if (live)
            {
                ++population;

                pixel.setPixel(col, row);
            }
            else
            {
                pixel.unsetPixel(col, row);
            }
        }
    }

    pixel.update();

    return population;
}

//-------------------------------------------------------------------------

int
main()
{
    try
    {
        HD44780::LcdI2C lcd {"/dev/i2c-1", 0x27, 20, 4};

        HD44780::LcdPixel pixel(lcd, 8, 1);

        newLife(pixel);

        LinuxKeys linuxKeys;
        LinuxKeys::PressedKey key;

        //-----------------------------------------------------------------

        while(key.key != 27)
        {
            key = linuxKeys.pressed();
        
            if (key.isPressed)
            {
                switch (key.key)
                {
                case ' ':

                    newLife(pixel);
                    break;

                case '+':

                    lcd.backlight();
                    break;

                case '-':

                    lcd.noBacklight();
                    break;
                }
            }

            iterateLife(pixel);
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

