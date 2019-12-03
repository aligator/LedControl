#ifndef UTIL_H_
#define UTIL_H_

#include <Arduino.h>

class Util {
public:
    static boolean isValidNumber(String str);
    static uint16_t convert48BitTo16BitColor(int color);
};
#endif /* UTIL_H_ */