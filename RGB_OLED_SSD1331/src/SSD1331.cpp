/*
* SSD1331.cpp
* A library for RGB OLED module
*
* Copyright (c) 2014 seeed technology inc.
* Copyright (c) 2012, Adafruit Industries.
*
* All rights reserved.
*
* This library is based on Adafruit's SSD1331-OLED-Driver-Library. Thanks to 
* their contribution to the code, we modify it and add more interface to 
* support our Seeed's Xadow RGB OLED 96*64 module.
*
* Below is the introduction of Adafruit's Color OLED module, we add it to here
* to express our thanks to them.
*
* ****************************************************************************
* This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
*
*  Pick one up today in the adafruit shop!
*  ------> http://www.adafruit.com/products/684
*
* These displays use SPI to communicate.
*
* Adafruit invests time and resources providing this open source code, 
* please support Adafruit and open-source hardware by purchasing 
* products from Adafruit!
*
* Written by Limor Fried/Ladyada for Adafruit Industries.
* Modifed by lawliet for Seeed Studio's RGB OLED module.
* BSD license, all text above must be included in any redistribution
* ******************************************************************************
*
* Software License Agreement (BSD License)
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holders nor the
* names of its contributors may be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "DigitalOut.h"
#include "SPI.h"
#include "SSD1331.h"
#include "wait_api.h"

SSD1331::SSD1331(PinName cs, PinName rst, PinName dc, PinName mosi, PinName miso, PinName sclk)
    :SGL<uint8_t>(RGB_OLED_WIDTH, RGB_OLED_HEIGHT), 
    _cs(cs), 
    _dc(dc),
    _spiPort(mosi, miso, sclk)
{
    _spiPort.format(8,3); //8bit frame and POL=1 /PHA=1(UpEdge Sampled)
    _spiPort.frequency(25000000); // modify later
    
    init();
};

//------------------------------------------------------------------------------------
void SSD1331::init(void)
{
    // initialize sequence
    sendCmd(CMD_DISPLAY_OFF);    //OLED display OFF
    
    //Row Address
    sendCmd(CMD_SET_ROW_ADDRESS,    0x00, 0x3f); //Set Row Address with start=0,end=63
    sendCmd(CMD_SET_COLUMN_ADDRESS, 0x00, 0x5f); //Set Column Address with start=0,end=95
    
     //Contrast
    sendCmd(CMD_SET_REMAP,               0x72); //Set remap & data format 0111 0000 // 65k Color/8bit buswidth/format1 0x72
    sendCmd(CMD_SET_DISPLAY_START_LINE,  0x00); //Set display start row RAM
    sendCmd(CMD_SET_DISPLAY_OFFSET,      0x00); //Set dispaly offset
    sendCmd(CMD_NORMAL_DISPLAY);                //Set Display Mode
    sendCmd(CMD_SET_MULTIPLEX_RATIO,     0x3f); //Set Multiplex Ratio
    sendCmd(CMD_SET_MASTER_CONFIGURE,    0x8e); //Set Master Configuration (External VCC Supply Selected) 0x8f or 0x8e
    sendCmd(CMD_POWER_SAVE_MODE,         0x1a); //Set Power Saving Mode
    sendCmd(CMD_PHASE_PERIOD_ADJUSTMENT, 0x31); //Set Phase 1 & 2 Period Adjustment 0x74 or 0x31
    sendCmd(CMD_DISPLAY_CLOCK_DIV,       0xf0); //Set Display Clock Divide Ratio / Oscillator Frequency 0xd0 or 0xf0
    sendCmd(CMD_SET_PRECHARGE_SPEED_A,   0x64); //Set Second Pre-charge Speed of Color A 0x81 or 0x64
    sendCmd(CMD_SET_PRECHARGE_SPEED_B,   0x78); //Set Second Pre-charge Speed of Color B 0x82 or 0x78
    sendCmd(CMD_SET_PRECHARGE_SPEED_C,   0x64); //Set Second Pre-charge Speed of Color C 0x83 or 0x64
    sendCmd(CMD_SET_PRECHARGE_VOLTAGE,   0x3a); //Set Pre-charge Level 3e or 3a
    sendCmd(CMD_SET_V_VOLTAGE,           0x3e); //Set VCOMH
    sendCmd(CMD_MASTER_CURRENT_CONTROL,  0x06); //Set Master Current Control 0x6 or 0xf
    sendCmd(CMD_SET_CONTRAST_A,          0x91); //Set Contrast Control for Color &#129;gA&#129;h 0x80 or 0x91
    sendCmd(CMD_SET_CONTRAST_B,          0x50); //Set Contrast Control for Color &#129;gB&#129;h 0x80 or 0x50   
    sendCmd(CMD_SET_CONTRAST_C,          0x7d); //Set Contrast Control for Color &#129;gC&#129;h 0x80 or 0x7D
    
    clearArea(0,0, RGB_OLED_XMAX, RGB_OLED_YMAX);
    sendCmd(CMD_NORMAL_BRIGHTNESS_DISPLAY_ON);    //display ON
    wait(0.1);            
}

//------------------------------------------------------------------------------------
void SSD1331::drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    if (x >= RGB_OLED_WIDTH)  x = RGB_OLED_XMAX;
    if (y >= RGB_OLED_HEIGHT) y = RGB_OLED_YMAX;
    //                         set column point               set row point
    uint8_t cmdBuffer[6] = { CMD_SET_COLUMN_ADDRESS, x, x,  CMD_SET_ROW_ADDRESS, y, y };
    sendCmd(cmdBuffer, 6); //Send buffer  
    sendData(color); //fill 16bit colour
}

//------------------------------------------------------------------------------------
void SSD1331::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_XMAX;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_YMAX;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_XMAX;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_YMAX;
    
    uint8_t cmd[8] = { CMD_DRAW_LINE, x0, y0, x1, y1, (uint8_t)(((color>>11)&0x1F)<<1), (uint8_t)((color>>5)&0x3F), (uint8_t)((color&0x1F)<<1) };
    sendCmd(cmd, 8);
}

//------------------------------------------------------------------------------------
void SSD1331::drawRect(uint8_t left_x, uint8_t top_y, uint8_t width, uint8_t height, uint16_t color)
{
    if (left_x >= RGB_OLED_WIDTH)  left_x = RGB_OLED_XMAX;
    if (top_y  >= RGB_OLED_HEIGHT) top_y  = RGB_OLED_YMAX;

    drawLine(left_x, top_y, left_x+width, top_y, color); // horizontal
    drawLine(left_x, top_y, left_x, top_y+height, color); // vertical
    drawLine(left_x, top_y+height, left_x+width, top_y+height, color); //horizontal
    drawLine(left_x+width, top_y, left_x+width, top_y+height, color); // vertical
}

//------------------------------------------------------------------------------------
void SSD1331::drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t outColor, uint16_t fillColor)
{
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_XMAX;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_YMAX;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_XMAX;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_YMAX;
    
    uint8_t cmd[11] = { CMD_DRAW_RECTANGLE, x0, y0, x1, y1, 
                     (uint8_t)(((outColor>>11)&0x1F)<<1), (uint8_t)((outColor>>5)&0x3F), (uint8_t)((outColor&0x1F)<<1),
                     (uint8_t)(((fillColor>>11)&0x1F)<<1), (uint8_t)((fillColor>>5)&0x3F), (uint8_t)((fillColor&0x1F)<<1)};

    sendCmd(CMD_FILL_WINDOW, ENABLE_FILL);//fill window
    sendCmd(cmd, 11);
    wait(0.05); 
}

//------------------------------------------------------------------------------------
void SSD1331::copyArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2)
{
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_XMAX;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_YMAX;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_XMAX;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_YMAX;
    if (x2 >= RGB_OLED_WIDTH)  x2 = RGB_OLED_XMAX;
    if (y2 >= RGB_OLED_HEIGHT) y2 = RGB_OLED_YMAX;
    
    uint8_t cmd[7] = { CMD_COPY_WINDOW, x0, y0, x1, y1, x2, y2};
    sendCmd(cmd, 7);
    wait(0.05); 
}

//------------------------------------------------------------------------------------
void SSD1331::dimArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_XMAX;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_YMAX;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_XMAX;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_YMAX;
    
    uint8_t cmdBuffer[5] = { CMD_DIM_WINDOW, x0, y0, x1, y1 };
    sendCmd(cmdBuffer, 5); //Send buffer
    wait(0.05); 
}

//------------------------------------------------------------------------------------
void SSD1331::clearArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_XMAX;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_YMAX;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_XMAX;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_YMAX;
    
    uint8_t cmdBuffer[5] = { CMD_CLEAR_WINDOW, x0, y0, x1, y1 };
    sendCmd(cmdBuffer, 5); //Send buffer
    wait(0.05);  
}

//------------------------------------------------------------------------------------
void SSD1331::moveArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{  
    copyArea(x0, y0, x1, y1, x2, y2);
    clearArea(x0, y0, x1, y1);
}

//------------------------------------------------------------------------------------
void SSD1331::fillScreen(uint16_t color)
{
    drawRect(0, 0, RGB_OLED_XMAX, RGB_OLED_YMAX, color, color);         
}

//------------------------------------------------------------------------------------
void SSD1331::setScrolling(ScrollDirection direction, uint8_t rowAddr, uint8_t rowNum, ScrollInterval interval)
{
    uint8_t scolling_horizontal = 0x0;
    uint8_t scolling_vertical = 0x0;
    switch(direction){
        case SD_Horizontal:
            scolling_horizontal = 0x01;
            scolling_vertical   = 0x00;
            break;
        case SD_Vertical:
            scolling_horizontal = 0x00;
            scolling_vertical   = 0x01;
            break;
        case SD_Diagonal:
            scolling_horizontal = 0x01;
            scolling_vertical   = 0x01;
            break;
        default:
            break;
    }
    
    uint8_t cmdBuffer[6] = { CMD_CONTINUOUS_SCROLLING_SETUP, scolling_horizontal, rowAddr, rowNum, scolling_vertical, interval };
    sendCmd(cmdBuffer, 6); //Send buffer
    sendCmd(CMD_ACTIVE_SCROLLING);
    wait(0.05);  
}

//------------------------------------------------------------------------------------
void SSD1331::enableScrolling(bool enable)
{
    if(enable)
        sendCmd(CMD_ACTIVE_SCROLLING);
    else
        sendCmd(CMD_DEACTIVE_SCROLLING);
}

//------------------------------------------------------------------------------------
void SSD1331::setDisplayMode(DisplayMode mode)
{
    sendCmd(mode);
}

//------------------------------------------------------------------------------------
void SSD1331::setPowerMode(PowerMode power)
{
    sendCmd(power);
}

//------------------------------------------------------------------------------------
// Private member functions
//------------------------------------------------------------------------------------
void SSD1331::sendCmd(uint8_t cmd)
{
    _dc = 0;
    _cs = 0;
    _spiPort.write(cmd);
    _cs = 1;
}

//------------------------------------------------------------------------------------
void SSD1331::sendCmd(uint8_t cmd, uint8_t arg1)
{
    _dc = 0;
    _cs = 0;
    _spiPort.write(cmd);
    _spiPort.write(arg1);
    _cs = 1;
}

//------------------------------------------------------------------------------------    
void SSD1331::sendCmd(uint8_t cmd, uint8_t arg1, uint8_t arg2)
{
    _dc = 0;
    _cs = 0;
    _spiPort.write(cmd);
    _spiPort.write(arg1);
    _spiPort.write(arg2);
    _cs = 1;
}    

//------------------------------------------------------------------------------------
void  SSD1331::sendCmd(uint8_t *cmd, uint8_t count)
{
    if(!cmd) return;
    
    _dc = 0;
    _cs = 0;
    for(uint8_t i=0; i<count; ++i)
       _spiPort.write(*cmd++);
    _cs = 1;
}

//------------------------------------------------------------------------------------
void  SSD1331::sendData(uint16_t data)
{
    _dc = HIGH;
    _cs = LOW;
    _spiPort.write(data >> 8);
    _spiPort.write((data & 0x00ff));
    _cs = HIGH;
}

//------------------------------------------------------------------------------------
int SSD1331::_getc() 
{
    return -1;
}

//------------------------------------------------------------------------------------
int SSD1331::_putc( int c )
{  
    return -1;
}