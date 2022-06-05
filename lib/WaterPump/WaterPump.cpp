#include "WaterPump.h"
#include "Arduino.h"
#include "Hydroconf.h"

/**
 * @brief Constructor of WaterPump
 */
WaterPump::WaterPump(){}

/**
 * @brief Initialise WaterPump Object
 */
void WaterPump::Init_WaterPump(){
    pinMode(WATERPUMPPIN, OUTPUT);
    Serial.println("************          INITIALISE WATER PUMP           ************");
}

/**
 * @brief Set WaterPump ON
 */
void WaterPump::Set_WP_ON(){
    digitalWrite(WATERPUMPPIN, HIGH);
    WaterPump_state = true;
    Serial.println("************          WATER PUMP STATE: ON            ************");
}

/**
 * @brief Set WaterPump OFF
 */
void WaterPump::Set_WP_OFF(){
    digitalWrite(WATERPUMPPIN, LOW);
    WaterPump_state = false;
    Serial.println("************          WATER PUMP STATE: OFF           ************");
}

/**
 * @brief Get the state of WaterPump
 * @return bool
 */
bool WaterPump::Get_WP_State(){
    return WaterPump_state;
}