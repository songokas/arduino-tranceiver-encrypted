#ifndef COMMON_MODULE_STRING_HELPER_H
#define COMMON_MODULE_STRING_HELPER_H

#include <Arduino.h>

namespace CommonModule
{
    inline uint16_t findNextPos(const char * str, uint16_t maxLength, char c, uint8_t occurence = 1)
    {
        if (maxLength > strlen(str)) {
            return 0;
        }
        for (uint16_t i = 0; i < maxLength; i++) {
            if (str[i] == c) {
                occurence--;
                if (occurence == 0) {
                    return i+1 < maxLength ? i + 1 : 0;
                }
            }
            if (str[i] == '\0') {
                return 0;
            }
        }
        return 0;
    }

    inline uint16_t findPosFromEnd(const char * str, uint16_t fromLength, char c)
    {
        if (fromLength > strlen(str)) {
            return 0;
        }
        for (uint16_t i = fromLength; i > 0; i--) {
            if (str[i] == c) {
                return i;
            }
        }
        return 0;
    }
}

#endif
