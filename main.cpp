//
//  main.cpp
//  AZOS
//
//  Created by Apollo Zhu on 6/19/16.
//  Copyright © 2016 WWITDC. All rights reserved.
//

// Attension: If you want to enter ISP mode, press and hold P0_0 and then connect to computer

#include "mbed.h"
#include "AZButtonEventHandler.h"
#include "LED.h"

#include "SSD1331.h"

#define startupEventHandler 0
#define buttonTouchEventHandler 1

#define cs      P0_15
#define rst     NC
#define dc      P0_4
#define mosi    P0_9
#define miso    NC
#define sclk    P0_10

SSD1331 oled = SSD1331(cs, rst, dc, mosi, miso, sclk);  

event_callback_t eventHandlers;

DigitalOut blueLED(LED1);
DigitalOut whiteLED(LED2);

void startup(){
    blueLED = 0;
    whiteLED = 0;
    wait(0.1);
    blueLED = 1;
    whiteLED = 1;
    wait(1);
    blueLED = 0;
    whiteLED = 0;
}

void handle(int flag){
    switch (flag){
        case 0:{
            startup();
            break;
        }
        case 1:{
            handleButtonResponse();
            break;
        }
        default:
            break;
    }   
}

void setup(){
    eventHandlers.attach(handle);
    eventHandlers.call(startupEventHandler);
    oled.drawLine(3,10,80,21,Yellow);
}

int main()
{
    setup();
    while (true) {
        eventHandlers.call(buttonTouchEventHandler);
    }
}