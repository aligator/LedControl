#ifndef UTIL_H_
#define UTIL_H_

#include <Arduino.h>

class Util {
public:
    static boolean isValidFloat(String str);
    static boolean isValidInt(String str);
    static String split(String s, char parser, int index);
};
#endif /* UTIL_H_ */