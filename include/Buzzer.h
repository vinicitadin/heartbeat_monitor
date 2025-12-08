#ifndef __BUZZER__H__
#define __BUZZER__H__

#include <Arduino.h>

class Buzzer
{
private:

public:
    Buzzer();
    void begin();
    void setData(String data);
};

#endif  //!__BUZZER__H__