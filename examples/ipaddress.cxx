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

#include <cstring> 
#include <iostream>

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <sys/types.h>

#include "LcdI2C.h"

//-------------------------------------------------------------------------

int
main()
{
    try
    {
        HD44780::LcdI2C lcd {"/dev/i2c-1", 0x27, 20, 4, true};

        lcd.clear();

        struct ifaddrs *ifAddrStruct = NULL;
        getifaddrs(&ifAddrStruct);

        for (struct ifaddrs* ifa = ifAddrStruct ;
             ifa != NULL ;
             ifa = ifa->ifa_next)
        {
            struct sockaddr* ifa_addr = ifa->ifa_addr;

            if (ifa_addr->sa_family == AF_INET)
            {
                // is a valid IP4 Address

                void *tmpAddrPtr = &((struct sockaddr_in *)ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

                if (strncmp(ifa->ifa_name, "eth", 3) == 0)
                {
                    lcd.setCursor(0, 0);
                    lcd.write('E');
                    lcd.write(addressBuffer);
                }
                else if (strncmp(ifa->ifa_name, "wlan", 4) == 0)
                {
                    lcd.setCursor(0, 1);
                    lcd.write('W');
                    lcd.write(addressBuffer);
                }
            }
        }

        if (ifAddrStruct != NULL)
        {
            freeifaddrs(ifAddrStruct);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
