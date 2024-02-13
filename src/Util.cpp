#include "Util.h"

boolean Util::isValidInt(String str) {
    boolean isNum=false;
    for(byte i=0; i<str.length(); i++)
    {
        isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
        if(!isNum) {
            return false;
        }
    }
    return isNum;
}

boolean Util::isValidFloat(String str) {
    boolean isNum=false;
    bool foundDot = false;
    for(byte i=0; i<str.length(); i++)
    {
        if(str.charAt(i) == '.') {
            if(foundDot) {
                return false;
            }
            foundDot = true;
            continue;
        }
        isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
        if(!isNum) {
            return false;
        }
    }
    return isNum;
}

String Util::split(String s, char parser, int index) {
  String rs="";
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;
  while (index >= parserCnt) {
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);
    if (index == parserCnt) {
      if (rToIndex == 0 || rToIndex == -1) return "";
      return s.substring(rFromIndex,rToIndex);
    } else parserCnt++;
  }
  return rs;
}