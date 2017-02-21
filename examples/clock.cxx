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
#include <string>

#include "LcdI2C.h"
#include "LcdRowSlider.h"
#include "LinuxKeys.h"

#include <time.h>
#include <unistd.h>
#include <sys/time.h>

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

        HD44780::LcdRowSlider lrs(lcd, 0, 59, 0, 4, 15);

        time_t now;
        time(&now);
        struct tm* tm = localtime(&now);

        lrs.value(tm->tm_sec);

        lcd.setCursor(6, 1);
        if (tm->tm_hour < 10) lcd.write('0');
        lcd.write(std::to_string(tm->tm_hour));
        lcd.write(':');
        if (tm->tm_min < 10) lcd.write('0');
        lcd.write(std::to_string(tm->tm_min));
        lcd.write(':');
        if (tm->tm_sec < 10) lcd.write('0');
        lcd.write(std::to_string(tm->tm_sec));

        lcd.setCursor(5, 2);
        if (tm->tm_mday < 10) lcd.write('0');
        lcd.write(std::to_string(tm->tm_mday));
        lcd.write('/');
        if (tm->tm_mon < 9) lcd.write('0');
        lcd.write(std::to_string(tm->tm_mon + 1));
        lcd.write('/');
        lcd.write(std::to_string(tm->tm_year + 1900));

        struct tm before;
        memcpy(&before, tm, sizeof(*tm));

        //-----------------------------------------------------------------

        LinuxKeys linuxKeys;
        LinuxKeys::PressedKey key;

        while(key.key != 27)
        {
            //-------------------------------------------------------------

            key = linuxKeys.pressed();

            if (key.isPressed)
            {
                switch (key.key)
                {
                case '+':

                    lcd.backlight();

                    break;

                case '-':

                    lcd.noBacklight();

                    break;
                }
            }

            //-------------------------------------------------------------

            time(&now);
            tm = localtime(&now);

            if (tm->tm_hour != before.tm_hour)
            {
                lcd.setCursor(6, 1);
                if (tm->tm_hour < 10) lcd.write('0');
                lcd.write(std::to_string(tm->tm_hour));
            }

            if (tm->tm_min != before.tm_min)
            {
                lcd.setCursor(9, 1);
                if (tm->tm_min < 10) lcd.write('0');
                lcd.write(std::to_string(tm->tm_min));
            }

            if (tm->tm_sec != before.tm_sec)
            {
                lrs.value(tm->tm_sec);
                lcd.setCursor(12, 1);
                if (tm->tm_sec < 10) lcd.write('0');
                lcd.write(std::to_string(tm->tm_sec));
            }

            if (tm->tm_mday != before.tm_mday)
            {
                lcd.setCursor(5, 2);
                if (tm->tm_mday < 10) lcd.write('0');
                lcd.write(std::to_string(tm->tm_mday));
            }

            if (tm->tm_mon != before.tm_mon)
            {
                lcd.setCursor(8, 2);
                if (tm->tm_mon < 9) lcd.write('0');
                lcd.write(std::to_string(tm->tm_mon + 1));
            }

            if (tm->tm_year != before.tm_year)
            {
                lcd.setCursor(11, 2);
                lcd.write(std::to_string(tm->tm_year + 1900));
            }

            memcpy(&before, tm, sizeof(*tm));

            //-------------------------------------------------------------

            usleep(100000);
        }

        lcd.clear();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

