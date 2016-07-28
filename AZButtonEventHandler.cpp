// Handle Xadow M0 on-board button press event
//
//  AZButtonEventHandler.cpp
//  AZOS
//
//  Created by Apollo Zhu on 6/19/16.
//  Copyright © 2016 WWITDC. All rights reserved.
//

#include "mbed.h"
#include "pinmap.h"
#include "AZButtonEventHandler.h"
#include "LED.h"

#define tapTimeInterval 80
#define holdTimeInterval 1500

enum AZButtonPressType{
    tap,  // 100 < time <=3000 ms, override with tapTimeInterval
    hold,  // time > 3000 ms, override with holdTimeInterval
    none
};

struct AZButtonResponse{
    int count;
    AZButtonPressType type;
};

InterruptIn button(P0_1);       // On-board button of Xadow M0

bool buttonPressed = false;
void didPressButton() { buttonPressed = true; }; // Change status of button
bool isButtonPressed(){ return !button.read(); };

AZButtonResponse currentButtonResponse(){
	pin_function(P0_0, 1);      // Need to figure out
	button.fall(didPressButton); // Call `didPressButton` when button is presssed down

    AZButtonResponse response;
    int count = 0;
    AZButtonPressType pressType = none;

    if (buttonPressed) {
        // Start of actual function
        int downInterval = 0;
        int upInterval = 0;
        bool waitingForNextPress = true;
        while (waitingForNextPress){
            if (isButtonPressed()){
                downInterval++;
            } else {
                if (downInterval){
                    #ifdef holdTimeInterval
                    if ((downInterval > holdTimeInterval)
                    #else
                    if ((downInterval > 3000)
                    #endif
                    && (pressType != tap)){
                        pressType = hold;
                    } else 
                    #ifdef tapTimeInterval
                    if ((downInterval > tapTimeInterval)
                    #else
                    if ((downInterval > 100)
                    #endif
                    && (pressType != hold)) {
                        pressType = tap;
                    } else {
	                    // ignore
                        waitingForNextPress = false;
                        count--;
                    }
                    count++;
                    downInterval = 0;
                    upInterval = 0;
                } else {
                    #ifdef holdTimeInterval
                    if (upInterval > holdTimeInterval)
                    #else
                    if (upInterval > 3000)
                    #endif
                    {
                        waitingForNextPress = false;
                    }
                }
                upInterval++;
            }
            wait_ms(1);
        }
        // End of actual function
        buttonPressed = false;
    }

    response.count = count;
    response.type = pressType;
    return response;
};

void handleButtonResponse(){
    AZButtonResponse response = currentButtonResponse();
    if (response.count){
        if (response.type == tap){
            for (int i = 0; i < response.count; i++){
                blueLED = 1;
                wait(0.3);
                blueLED = 0;
                wait(0.2);
            }
        } else if (response.type == hold) {
            if (response.count == 1){
                blueLED = 1;
                whiteLED = 1;
                wait (1);
                NVIC_SystemReset();
            } else {
                for (int i = 0; i < response.count; i++){
                    whiteLED = 1;
                    wait(0.3);
                    whiteLED = 0;
                    wait(0.2);
                }  
            }  
        }
    }
};