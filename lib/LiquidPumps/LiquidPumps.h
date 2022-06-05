#ifndef LiquidPumps_h
#define LiquidPumps_h

#include "Sensors.h"

class LiquidPumps{

    public: 
    LiquidPumps(Sensors &sens);
    void Init_LiquidPumps();
    int get_PH_UP_state();
    int get_PH_DOWN_state();
    int get_EC_UP_state();
    void set_PH_UP_PUMP_ON();
    void set_PH_DOWN_PUMP_ON();
    void set_EC_UP_PUMP_ON();
    void set_PH_UP_PUMP_OFF();
    void set_PH_DOWN_PUMP_OFF();
    void set_EC_UP_PUMP_OFF();
    void raise_PH();
    void lower_PH();
    void raise_EC();
    void check_ph();
    void check_ec();
    Sensors my_sens;

    private:
    int PH_UP_state = 0;
    int PH_DOWN_state = 0;
    int EC_UP_state = 0;

};

#endif