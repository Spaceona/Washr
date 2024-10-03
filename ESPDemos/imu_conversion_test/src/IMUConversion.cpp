//
// Created by Robbie on 9/8/24.
//

#include "IMUConversion.h"
#include <Arduino.h>


double accelToMps2(double accelData, int accelRange) {

    switch(accelRange) {
        case 2:
            return (accelData / 16384.0) * 9.81;
        case 4:
            return (accelData / 8192.0) * 9.81;
        case 8:
            return (accelData / 4096.0) * 9.81;
        case 16:
            return (accelData / 2048.0) * 9.81;
        default:
            return -1; // Error
    }
}
