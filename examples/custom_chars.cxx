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

#include <iostream>

#include <unistd.h>

#include "LcdI2C.h"
#include "LinuxKeys.h"

//-------------------------------------------------------------------------

int
main()
{
    try
    {
        HD44780::LcdI2C lcd {"/dev/i2c-1", 0x27, 20, 4};

        //-----------------------------------------------------------------

        lcd.clear();

        //-----------------------------------------------------------------

        lcd.setCursor(0, 1);
        lcd.write("test");

        std::array<uint8_t, 8> bell  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
        std::array<uint8_t, 8> note  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
        std::array<uint8_t, 8> clock = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
        std::array<uint8_t, 8> heart = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
        std::array<uint8_t, 8> duck  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
        std::array<uint8_t, 8> check = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
        std::array<uint8_t, 8> cross = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
        std::array<uint8_t, 8> retarrow = { 0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

        lcd.createChar(0, bell);
        lcd.createChar(1, note);
        lcd.createChar(2, clock);
        lcd.createChar(3, heart);
        lcd.createChar(4, duck);
        lcd.createChar(5, check);
        lcd.createChar(6, cross);
        lcd.createChar(7, retarrow);

        lcd.setCursor(0, 0);

        for (uint8_t i = 0 ; i < 8 ; ++i)
        {
            lcd.write(i);
        }

        LinuxKeys linuxKeys;
        LinuxKeys::PressedKey key;

        //-----------------------------------------------------------------

        while(key.key != 27)
        {
            key = linuxKeys.pressed();
            usleep(100000);
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

