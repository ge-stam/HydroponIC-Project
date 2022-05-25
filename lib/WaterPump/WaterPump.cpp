#include "WaterPump.h"
#include "Arduino.h"
#include "Hydroconf.h"

WaterPump::WaterPump(){}

void WaterPump::Init_WaterPump(){
    pinMode(WATERPUMPPIN, OUTPUT);
    //digitalWrite(WATERPUMPPIN, LOW);
    Serial.println("************          INITIALISE WATER PUMP           ************");
}

void WaterPump::Set_WP_ON(){
    digitalWrite(WATERPUMPPIN, HIGH);
    WaterPump_state = true;
    Serial.println("************          WATER PUMP STATE: ON            ************");
}

void WaterPump::Set_WP_OFF(){
    digitalWrite(WATERPUMPPIN, LOW);
    WaterPump_state = false;
    Serial.println("************          WATER PUMP STATE: OFF           ************");
}

bool WaterPump::Get_WP_State(){
    return WaterPump_state;
}