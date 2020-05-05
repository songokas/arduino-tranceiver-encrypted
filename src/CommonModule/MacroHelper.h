#ifndef COMMON_MODULE_MACRO_HELPER_H
#define COMMON_MODULE_MACRO_HELPER_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

#ifndef DEBUG_LEVEL 
#define DEBUG_LEVEL 1
#endif

#if DEBUG_LEVEL > 0
    #define LOG(level, levelName, message, ...) if (level >= DEBUG_LEVEL) { Serial.print(millis()); Serial.print(F(levelName)); printf_P(PSTR(message),##__VA_ARGS__); } 
#else
    #define LOG(...)
#endif

// @TODO functions
#define debug(...) LOG(1, " debug ", __VA_ARGS__)
#define info(...) LOG(2, " info ", __VA_ARGS__)
#define warning(...) LOG(3, " warning ", __VA_ARGS__)
#define error(...) LOG(4, " error ", __VA_ARGS__)

#endif