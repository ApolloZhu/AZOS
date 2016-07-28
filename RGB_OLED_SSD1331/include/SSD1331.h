/*
* SSD1331.h
* A library for RGB OLED module
*
* Copyright (c) 2015 Juergen Messerer
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

#ifndef _SSD1331_H_
#define _SSD1331_H_

#include "DigitalOut.h"
#include "PinNames.h"
#include "SGL.cpp" // Because of the template class we have to include this cpp
#include "Stream.h"


#define RGB_OLED_WIDTH                      96
#define RGB_OLED_HEIGHT                     64
#define RGB_OLED_XMAX                       RGB_OLED_WIDTH-1
#define RGB_OLED_YMAX                       RGB_OLED_HEIGHT-1

#define CMD_DRAW_LINE                       0x21
#define CMD_DRAW_RECTANGLE                  0x22
#define CMD_COPY_WINDOW                     0x23
#define CMD_DIM_WINDOW                      0x24
#define CMD_CLEAR_WINDOW                    0x25
#define CMD_FILL_WINDOW                     0x26
#define DISABLE_FILL                        0x00
#define ENABLE_FILL                         0x01
#define CMD_CONTINUOUS_SCROLLING_SETUP      0x27
#define CMD_DEACTIVE_SCROLLING              0x2E
#define CMD_ACTIVE_SCROLLING                0x2F

#define CMD_SET_COLUMN_ADDRESS              0x15
#define CMD_SET_ROW_ADDRESS                 0x75
#define CMD_SET_CONTRAST_A                  0x81
#define CMD_SET_CONTRAST_B                  0x82
#define CMD_SET_CONTRAST_C                  0x83
#define CMD_MASTER_CURRENT_CONTROL          0x87
#define CMD_SET_PRECHARGE_SPEED_A           0x8A
#define CMD_SET_PRECHARGE_SPEED_B           0x8B
#define CMD_SET_PRECHARGE_SPEED_C           0x8C
#define CMD_SET_REMAP                       0xA0
#define CMD_SET_DISPLAY_START_LINE          0xA1
#define CMD_SET_DISPLAY_OFFSET              0xA2
#define CMD_NORMAL_DISPLAY                  0xA4
#define CMD_ENTIRE_DISPLAY_ON               0xA5
#define CMD_ENTIRE_DISPLAY_OFF              0xA6
#define CMD_INVERSE_DISPLAY                 0xA7
#define CMD_SET_MULTIPLEX_RATIO             0xA8
#define CMD_DIM_MODE_SETTING                0xAB
#define CMD_SET_MASTER_CONFIGURE            0xAD
#define CMD_DIM_MODE_DISPLAY_ON             0xAC
#define CMD_DISPLAY_OFF                     0xAE
#define CMD_NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define CMD_POWER_SAVE_MODE                 0xB0
#define CMD_PHASE_PERIOD_ADJUSTMENT         0xB1
#define CMD_DISPLAY_CLOCK_DIV               0xB3
#define CMD_SET_GRAy_SCALE_TABLE            0xB8
#define CMD_ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define CMD_SET_PRECHARGE_VOLTAGE           0xBB
#define CMD_SET_V_VOLTAGE                   0xBE

#define LOW 0
#define HIGH 1


/* some 16bit RGB color definitions  */
enum Color{                                             
 Black           = 0x0000,      /*   0,   0,   0 */
 Navy            = 0x000F,      /*   0,   0, 128 */
 DarkGreen       = 0x03E0,      /*   0, 128,   0 */
 DarkCyan        = 0x03EF,      /*   0, 128, 128 */
 Maroon          = 0x7800,      /* 128,   0,   0 */
 Purple          = 0x780F,      /* 128,   0, 128 */
 Olive           = 0x7BE0,      /* 128, 128,   0 */
 LightGrey       = 0xC618,      /* 192, 192, 192 */
 DarkGrey        = 0x7BEF,      /* 128, 128, 128 */
 Blue            = 0x001F,      /*   0,   0, 255 */
 Green           = 0x07E0,      /*   0, 255,   0 */
 Cyan            = 0x07FF,      /*   0, 255, 255 */
 Red             = 0xF800,      /* 255,   0,   0 */
 Magenta         = 0xF81F,      /* 255,   0, 255 */
 Yellow          = 0xFFE0,      /* 255, 255,   0 */
 White           = 0xFFFF,      /* 255, 255, 255 */
 Orange          = 0xFD20,      /* 255, 165,   0 */
 GreenYellow     = 0xAFE5,      /* 173, 255,  47 */
 Pink            = 0xF81F
};

enum DisplayMode{
    //reset the above effect and turn the data to ON at the corresponding gray level.
    DM_NormalDisplay   = 0xA4,
    //forces the entire display to be at "GS63"
    DM_DisplayOn       = 0xA5,
    //forces the entire display to be at gray level "GS0"
    DM_DisplayOff      = 0xA6,
    //swap the gray level of display data
    DM_InverseDisplay  = 0xA7
};

enum PowerMode{
    PM_SleepMode       = 0xAE,
    PM_NormalMode      = 0xAF
};

enum ScrollDirection{
    SD_Horizontal      = 0x00,
    SD_Vertical        = 0x01,
    SD_Diagonal        = 0x02
};

enum ScrollInterval{
    SI_6_Frames   = 0x00,
    SI_10_Frames  = 0x01,
    SI_100_Frames = 0x02,
    SI_200_Frames = 0x03
};

/** The SSD1331 class drives via the SPI port oled displays with the SSD1331 chip such as the RGB oled from Seeedstudio
 *
 *  The class supports various drawinf and scrolling fuctions
 *
 *
 * Example:
 * @code
 * // Draw a line at the rgb old of Seeedstudio
 *
 * #include "mbed.h"
 * #include "SSD1331.h"
 *
 * int main() {
 *     SSD1331 oled(P0_15, NC, P0_4, P0_9, NC, P0_10);
 *     // or
 *     SSD1331 oled(P1_19, NC, P0_7, P0_21, NC, P1_15);
 *     oled.drawLine(0, 0, 95, 0, Yellow);
 * }
 * @endcode
 */
class SSD1331 : public mbed::Stream, public virtual SGL<uint8_t>
{
    public:
        /** Create a SPI master connected to the specified pins
        *
        *
        *  mosi or miso can be specfied as NC if not used
        *
        *  @param cs   Chip select pin
        *  @param rst  Reset pin
        *  @param dc   Data / #Command pin 
        *  @param mosi SPI Master Out, Slave In pin
        *  @param miso SPI Master In,  Slave Out pin
        *  @param sclk SPI Clock pin  
        */
        SSD1331(PinName cs, PinName rst, PinName dc, PinName mosi, PinName miso, PinName sclk);

        /** Draws a pixel in a given color on the screen. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x     Position x
        *  @param y     Position y
        *  @param color 16bit Color of the pixel
        */
        virtual void drawPixel(uint8_t x, uint8_t y, uint16_t color);
        
        /** Draws a line from one position to another position. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Start Position x 
        *  @param y0      Start Position y
        *  @param x1      End Position x
        *  @param y1      End Position y 
        *  @param color   16bit Color of the pixel
        */
        virtual void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
        
        /** Draws a rectangle with no filling. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param left      Position x
        *  @param top       Position y
        *  @param width     Width of the rectangle
        *  @param height    Height of the rectangle
        *  @param color     16bit Color of the rectangle lines
        */
        virtual void drawRect(uint8_t left, uint8_t top, uint8_t width, uint8_t height, uint16_t color);
        
        /** Draws a filled rectangle with a seperate border line. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param left_x        Position x
        *  @param top_y         Position y
        *  @param right_x       Position x
        *  @param bottom_y      Position y
        *  @param frameColor    16bit Color of the rectangle lines
        *  @param fillColor     16bit Color of the filling
        */
        void drawRect(uint8_t left_x, uint8_t top_y, uint8_t righ_x, uint8_t bottom_y, uint16_t borderColor, uint16_t fillColor);
        
        /** Copys an area from a given position to a certain position. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Position x top left corner
        *  @param y0      Position y top left corner
        *  @param x1      Position x bottom right corner
        *  @param y1      Position y bottom right corner
        *  @param x2      New Position x topleft corner
        *  @param y2      New Position y topleft corner
        */
        void copyArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
        
        /** Dims an area from a given start position to a given end position. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Position x top left corner
        *  @param y0      Position y top left corner
        *  @param x1      Position x bottom right corner
        *  @param y1      Position y bottom right corner
        */
        void dimArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
        
        /** Clears an area from a given start position to a given end position. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Position x top left corner
        *  @param y0      Position y top left corner
        *  @param x1      Position x bottom right corner
        *  @param y1      Position y bottom right corner
        */
        void clearArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
        
        /** Moves an area from a given start position to a given end position. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Position x top left corner
        *  @param y0      Position y top left corner
        *  @param x1      Position x bottom right corner
        *  @param y1      Position y bottom right corner
        */
        void moveArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
        
        /** Fills the screen with a given color. Larger coordinates than the screen will be set to RGB_OLED_XMAX and RGB_OLED_YMAX
        *
        *  @param x0      Position x top left corner
        *  @param y0      Position y top left corner
        *  @param x1      Position x bottom right corner
        *  @param y1      Position y bottom right corner
        */
        virtual void fillScreen(uint16_t color);
        
        /** Sets the direction and starts the scrolling of the display content.
        *
        *  @param direction    Direction of the scrolling SD_Horizontal, SD_Vertical or SD_Diagonal
        *  @param rowAddr      Position y top left corner
        *  @param rowNum       Position x bottom right corner
        *  @param interval     Speed of the scrolling either 6, 10 100 or 200 frames
        */
        void setScrolling(ScrollDirection dir, uint8_t rowAddr, uint8_t rowNum, ScrollInterval interval);
        
        /** Enables or disables the scrolling
        *
        *  @param enable   Set true or flase for enabling or disabling the scrolling
        */
        void enableScrolling(bool enable);
        
        /** Sets the display mode of the oled screen
        *
        *  @param mode      DM_NormalDisplay, DM_DisplayOn, DM_DisplayOff or DM_InverseDisplay
        */
        void setDisplayMode(DisplayMode mode);
        
        /** Sets the power mode of the display
        *
        *  @param mode   PM_SleepMode or PM_NormalMode
        */
        void setPowerMode(PowerMode mode);
        
    protected:
         // Stream implementation functions
         virtual int _putc( int c );
         virtual int _getc();
        
    private:
        void init(void);
        void sendCmd(uint8_t cmd);
        void sendCmd(uint8_t cmd,  uint8_t arg1);
        void sendCmd(uint8_t cmd,  uint8_t arg1, uint8_t arg2);
        void sendCmd(uint8_t *cmd, uint8_t count);
        void sendData(uint16_t data);
        
    private:
        mbed::DigitalOut _cs;
        mbed::DigitalOut _dc;
        mbed::SPI        _spiPort;
};

#endif