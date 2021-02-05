#ifndef COMMON_MODULE_MACRO_HELPER_H
#define COMMON_MODULE_MACRO_HELPER_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#endif

#if ARDUINO

inline void serialPrintf(HardwareSerial &serial, const char *fmt, ...)
{
    va_list argv;
    va_start(argv, fmt);
    uint8_t strLength = strlen_P(fmt);

    for (int i = 0; i < strLength; i++)
    {
        char letter = pgm_read_byte_near(fmt + i);
        if (letter == '%')
        {
            // Look for specification of number of decimal places
            char nextLetter = pgm_read_byte_near(fmt + (i + 1));
            int places = 2;
            if (nextLetter >= '0' && nextLetter <= '9')
            {
                places = nextLetter - '0';
                i++;
            }

            i++;
            letter = pgm_read_byte_near(fmt + i);

            switch (letter)
            {
            case 'B':
                serial.print("0b"); // Fall through intended
            case 'b':
                serial.print(va_arg(argv, int), BIN);
                break;
            case 'c':
                serial.print((char)va_arg(argv, int));
                break;
            case 'd':
            case 'i':
                serial.print(va_arg(argv, int), DEC);
                break;
            case 'f':
                serial.print(va_arg(argv, double), places);
                break;
            case 'l':
                serial.print(va_arg(argv, long), DEC);
                break;
            case 'o':
                serial.print(va_arg(argv, int) == 0 ? "off" : "on");
                break;
            case 's':
                serial.print(va_arg(argv, const char *));
                break;
            case 'X':
                serial.print("0x"); // Fall through intended
            case 'x':
                serial.print(va_arg(argv, int), HEX);
                break;
            case '%':
                serial.print(letter);
                break;
            default:
                serial.print("?");
                break;
            }
        }
        else
        {
            serial.print(letter);
        }
    }
    va_end(argv);
}

#endif

#if DEBUG_LEVEL > 0 && ARDUINO
#define LOG(level, levelName, message, ...)                  \
    if (level >= DEBUG_LEVEL)                                \
    {                                                        \
        Serial.print(millis());                              \
        Serial.print(F(levelName));                          \
        serialPrintf(Serial, PSTR(message), ##__VA_ARGS__);  \
        Serial.println();                                    \
    }
#else
#define LOG(...)
#endif

// @TODO functions
#define debug(...) LOG(1, " debug ", __VA_ARGS__)
#define info(...) LOG(2, " info ", __VA_ARGS__)
#define warning(...) LOG(3, " warning ", __VA_ARGS__)
#define error(...) LOG(4, " error ", __VA_ARGS__)

#endif