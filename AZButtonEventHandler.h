//
//  AZButtonEventHandler.h
//  AZOS
//
//  Created by Apollo Zhu on 6/19/16.
//  Copyright © 2016 WWITDC. All rights reserved.
//

#ifndef AZButtonEventHandler_H
#define AZButtonEventHandler_H

enum AZButtonPressType{
    tap,  // 100 < time <=3000 ms, override with tapTimeInterval
    hold,  // time > 3000 ms, override with holdTimeInterval
    none
};

struct AZButtonResponse{
    int count;
    AZButtonPressType type;
};

AZButtonResponse currentButtonResponse();

#endif