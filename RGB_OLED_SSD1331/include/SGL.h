/*
* SGL.h
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

#ifndef _SGL_H_ //Seeed Graphics Library
#define _SGL_H_

#include <stdint.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef const unsigned char** FontType;

template <class T>
class SGL {
 
public:
    SGL(T width, T height);
    virtual void drawPixel(T x, T y, uint16_t color) = 0; // implemented by subclass
    virtual void drawLine(T x0, T y0, T x1, T y1, uint16_t color);
    virtual void drawVLine(T x, T y, T length,uint16_t color);
    virtual void drawHLine(T x, T y, T length, uint16_t color);
    virtual void drawRect(T x, T y, T width, T height, uint16_t color);
    virtual void fillRect(T x, T y, T width, T height, uint16_t color);
    virtual void drawCircle(T x, T y, T r, uint16_t color);
    virtual void fillCircle(T x, T y, T r, uint16_t color);
    virtual void drawTriangle(T x0, T y0, T x1, T y1, T x2, T y2, uint16_t color);
    virtual void fillTriangle(T x0, T y0, T x1, T y1, T x2, T y2, uint16_t color);    
    
    // The zoom factor works at the moment only with integer values. Float values will create bad fonts 
    virtual void drawChar(uint8_t ascii, T x, T y, uint16_t color, float zoom=1);
    virtual void drawString(const char *string, T x, T y, uint16_t color, float zoom=1, uint8_t fontSpace=6);
    
    virtual void drawBitMap(T x, T y, const uint8_t *bitmap, T width, T height, uint16_t color);
    virtual void fillScreen(uint16_t color);
    virtual void setFont(FontType font, uint8_t width, uint8_t height, uint8_t asciiStart, uint8_t asciiStop);
    
private:
    void swap(T* a, T* b){ 
        T t = *a; *a = *b; *b = t; 
    };
    
    T _width;
    T _height;
    FontType _currentFont;
    uint8_t _fontWidth;
    uint8_t _fontHeight;
    uint8_t _fontStart;
    uint8_t _fontStop;
};

#endif
