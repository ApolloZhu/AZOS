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

DigitalOut blue(P0_20);
DigitalOut white(P0_23);

int main()
{
    while (true) {
        AZButtonResponse response = currentButtonResponse();
        if (response.count){
            if (response.type == tap){
                for (int i = 0; i < response.count; i++){
                    blue = 1;
                    wait(1);
                    blue = 0;
                    wait(1);
                }
            } else if (response.type == hold) {
                if (response.count == 1){
                    blue = 1;
                    white = 1;
                    wait (1);
                    NVIC_SystemReset();
                } else {
                    for (int i = 0; i < response.count; i++){
                        white = 1;
                        wait(1);
                        white = 0;
                        wait(1);
                    }  
                }  
            }
        }
    }
}
