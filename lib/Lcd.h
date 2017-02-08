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

#ifndef LCD_H
#define LCD_H

//-------------------------------------------------------------------------

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>

//-------------------------------------------------------------------------

namespace HD44780
{

//-------------------------------------------------------------------------

class Lcd
{
public:

    Lcd(
        uint8_t columns,
        uint8_t rows,
        bool linger);

    virtual ~Lcd() = 0;

    Lcd(const Lcd&) = delete;
    Lcd& operator= (const Lcd&) = delete;

    //---------------------------------------------------------------------

    virtual void clear() const {};
    virtual void home() const {};

    virtual void noDisplay() {};
    virtual void display() {};

    virtual void noBlink() {};
    virtual void blink() {};

    virtual void noCursor() {};
    virtual void cursor() {};

    virtual void scrollDisplayLeft() {};
    virtual void scrollDisplayRight() {};

    virtual void leftToRight() {};
    virtual void rightToLeft() {};

    virtual void autoscroll() {};
    virtual void noAutoscroll() {};

    virtual void backlight() {};
    virtual void noBacklight() {};

    void linger()
    {
        linger_ = true;
    }

    void noLinger()
    {
        linger_ = false;
    }

    virtual void createChar(uint8_t number, uint8_t* data) const {};
    virtual void setCursor(uint8_t column, uint8_t row) const {};
    virtual size_t write(uint8_t value) const = 0;

    //---------------------------------------------------------------------

    size_t
    write(
        const std::string& s) const
    {
        std::for_each(s.begin(), s.end(), [this](char c){ write(c); });
        return s.length();
    }

    //---------------------------------------------------------------------

protected:

    const uint8_t columns_;
    const uint8_t rows_;

    bool linger_;

private:

    size_t printNumber(unsigned long, uint8_t) const;
    size_t printFloat(double, uint8_t) const;

};

//-------------------------------------------------------------------------

} // namespace HD44780

//-------------------------------------------------------------------------
#endif
