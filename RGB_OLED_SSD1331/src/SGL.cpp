/*
* SGL.cpp
* A library for Seeed Graphical library
*
* Copyright (c) 2014 seeed technology inc.
* Author        :   lawliet.zou(lawliet.zou@gmail.com)
* Create Time   :   Jun 06, 2014
* Change Log    :
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include <stdlib.h>
#include "SGL.h"
#include "SimpleFont.h"

//template class SGL<uint8_t>;
//template class SGL<uint16_t>;

//---------------------------------------------------------------------------------------
template <class T>
SGL<T>::SGL(T width, T height):
    _width(width), _height(height),
    _currentFont(0),     
    _fontWidth(0),
    _fontHeight(0),
    _fontStart(0),
    _fontStop(0)
{}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawLine(T x0, T y0, T x1, T y1, uint16_t color)
{
    T x   = x1-x0;
    T y   = y1-y0;
    T dx  = abs(x),  sx = x0<x1 ? 1 : -1;
    T dy  = -abs(y), sy = y0<y1 ? 1 : -1;
    T err = dx+dy, e2;
    for (;;)
    {
        drawPixel(x0, y0,color);
        e2 = 2*err;
        if (e2 >= dy) {
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawVLine(T x, T y, T length,uint16_t color)
{
    T y1 = MIN(y+length,_height-1);
    for(T i = y; i < y1; ++i)
    {
        drawPixel(x, i, color);
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawHLine(T x, T y, T length, uint16_t color)
{
    T x1 = MIN(x+length,_width-1);
    for(T i = x; i < x1; ++i)
    {
        drawPixel(i, y, color);
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawRect(T x, T y, T width, T height, uint16_t color)
{
    drawHLine(x, y, width, color);
    drawHLine(x, y+height, width, color);
    drawVLine(x, y, height, color);
    drawVLine(x+width, y, height, color);
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::fillRect(T x, T y, T width, T height, uint16_t color)
{
    for(uint8_t i = 0; i < height; ++i)
    {
        for(uint8_t j = 0; j < width; ++j)
        {
            drawPixel( x+j, y+i, color);
        }
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawCircle(T poX, T poY, T r, uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do
    {
        drawPixel(poX-x, poY+y,color);
        drawPixel(poX+x, poY+y,color);
        drawPixel(poX+x, poY-y,color);
        drawPixel(poX-x, poY-y,color);
        e2 = err;
        if(e2 <= y)
        {
            err += ++y*2+1;
            if(-x == y && e2 <= x) e2 = 0;
        }
        if(e2 > x) err += ++x*2+1;
    } while(x <= 0);
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::fillCircle(T poX, T poY, T r, uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do
    {
        drawVLine(poX-x, poY-y, 2*y, color);
        drawVLine(poX+x, poY-y, 2*y, color);
        e2 = err;
        if(e2 <= y)
        {
            err += ++y*2+1;
            if(-x == y && e2 <= x) e2 = 0;
        }
        if(e2 > x) err += ++x*2+1;
    }while(x <= 0);
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawTriangle(T x0, T y0, T x1, T y1, T x2, T y2, uint16_t color)
{
    drawLine(x0, y0, x1, y1,color);
    drawLine(x1, y1, x2, y2,color);
    drawLine(x2, y2, x0, y0,color);
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::fillTriangle(T x0, T y0, T x1, T y1, T x2, T y2, uint16_t color)
{
    T a, b, y, last;

    if(y0 > y1){ swap(&y0, &y1); swap(&x0, &x1); }
    if(y1 > y2){ swap(&y2, &y1); swap(&x2, &x1); }
    if(y0 > y1){ swap(&y1, &y0); swap(&x1, &x0); }

    if(y0 == y2){
        x0 = MIN(x0,x1)<x2?MIN(x0,x1):x2;
        x2 = MAX(x0,x1)>x2?MAX(x0,x1):x2;
        drawHLine(x0, y0, x2-x0, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0,
            dx02 = x2 - x0, dy02 = y2 - y0,
            dx12 = x2 - x1, dy12 = y2 - y1;
    int16_t sa   = 0,       sb   = 0;

    if(y1 == y2) 
        last = y1;
    else 
        last = y1-1;

    for(y = y0; y <= last; ++y) 
    {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if(a > b) swap(&a,&b);
        drawHLine(a, y, b-a+1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; ++y)
    {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if(a > b) swap(&a,&b);
        drawHLine(a, y, b-a+1, color);
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawChar(uint8_t ascii, T x, T y, uint16_t color, float zoom)
{
    if(!_currentFont && !*_currentFont)
        return;
        
    if((ascii < _fontStart)||(ascii > _fontStop)){
        return;
    }

    for(uint8_t i = 0; i < _fontWidth; ++i )
    {
        uint8_t temp   = simpleFont[ascii-0x20][i];
        uint8_t inrun  = 0;
        uint8_t runlen = 0;
        uint8_t endrun = 0;

        for(uint8_t f = 0; f < _fontHeight; ++f)
        {
            if((temp>>f)&0x01)
            {
                if (inrun) runlen += 1;
                else 
                {
                    inrun  = 1;
                    runlen = 1;
                }
            }
            else if (inrun) 
            {
                endrun = 1;
                inrun  = 0;
            }

            if (f == _fontHeight - 1 && inrun) 
            {
                endrun = 1;
                // need the +1 b/c we this code is normally
                // only triggered  when f == FONT_Y, due to the
                // edge-triggered nature of this algorithm
                f += 1;
            }
            
            if (endrun) 
            {
                fillRect(x+i*zoom, y+(f-runlen)*zoom, zoom, runlen*zoom, color);
                inrun = 0;
                runlen = 0;
                endrun = 0;
            }
        }
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawString(const char *string, T x, T y, uint16_t color, float zoom, uint8_t fontSpace)
{
    if(!_currentFont && !*_currentFont)
        return;
        
    while(*string)
    {
        drawChar(*string, x, y, color, zoom);
        *string++;
        x += fontSpace*zoom;
        if(x >= _width-1)
        {
            y += _fontHeight*zoom;
            x = 0;
        }
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::drawBitMap(T x, T y, const uint8_t *bitmap, T width, T height, uint16_t color)
{
    T byteWidth = (width + 7) / 8;
    
    for(T j = 0; j < height; ++j)
    {
        for(T i = 0; i < width; ++i ) 
        {
            if( *(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)) ) 
            {
                drawPixel(x+i, y+j, color);
            }
        }
    }
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::fillScreen(uint16_t color)
{
    fillRect(0, 0, _width, _height, color);
}

//---------------------------------------------------------------------------------------
template <class T>
void SGL<T>::setFont(FontType font, uint8_t width, uint8_t height, uint8_t asciiStart, uint8_t asciiStop)
{
    if(!font && !*font)
        return;
    
    _currentFont= font;
    _fontWidth  = width;
    _fontHeight = height;
    _fontStart  = asciiStart;
    _fontStop   = asciiStop;    
}