#ifndef MOCK_RTC_DS3231_H
#define MOCK_RTC_DS3231_H

#include <Arduino.h>
#include "RtcDateTime.h"

template <class A>
class RtcDS3231
{
    public:
        virtual RtcDateTime GetDateTime() = 0;
};

#endif
