#include "Arduino.h"
#include "LiquidPumps.h"
#include "Hydroconf.h"
#include "Sensors.h"

static unsigned long currentMillis;
static unsigned long previousMillis;

/**
 * @brief Constructor of LiquidPumps
 * @param An object of class Sensors
 */
LiquidPumps::LiquidPumps(){
}

/**
 * @brief Initialise LiquidPumps Object
 */
void LiquidPumps::Init_LiquidPumps(){
    pinMode(PH_UP_PIN, OUTPUT);
    pinMode(PH_DOWN_PIN, OUTPUT);
    pinMode(EC_UP_PIN, OUTPUT);
    set_EC_UP_PUMP_OFF();
    set_PH_UP_PUMP_OFF();
    set_PH_DOWN_PUMP_OFF();
    Serial.println("************        INITIALISE LIQUID PUMPS           ************\n");
}

/**
 * @brief Get the state of the pump that contains PH UP Liquid
 * @return int
 */
int LiquidPumps::get_PH_UP_state(){
    return PH_UP_state;
}

/**
 * @brief Get the state of the pump that contains PH DOWN Liquid
 * @return int
 */
int LiquidPumps::get_PH_DOWN_state(){
    return PH_DOWN_state;
}

/**
 * @brief Get the state of the pump that contains EC UP Liquid
 * @return int
 */
int LiquidPumps::get_EC_UP_state(){
    return EC_UP_state;
}

/**
 * @brief Set the pump containing PH UP Liquid ON
 */
void LiquidPumps::set_PH_UP_PUMP_ON(){
    digitalWrite(PH_UP_PIN, HIGH);
    PH_UP_state = 1;
    Serial.println("PH UP PUMP STATE: ON ");
}


/**
 * @brief Set the pump containing PH DOWN Liquid ON
 */
void LiquidPumps::set_PH_DOWN_PUMP_ON(){
    digitalWrite(PH_DOWN_PIN, HIGH);
    PH_DOWN_state = 1;
    Serial.println("PH DOWN PUMP STATE: ON ");
}


/**
 * @brief Set the pump containing EC UP Liquid ON
 */
void LiquidPumps::set_EC_UP_PUMP_ON(){
    digitalWrite(EC_UP_PIN, HIGH);
    EC_UP_state = 1;
    Serial.println("EC UP PUMP STATE: ON ");
}


/**
 * @brief Set the pump containing PH UP Liquid OFF
 */
void LiquidPumps::set_PH_UP_PUMP_OFF(){
    digitalWrite(PH_UP_PIN, LOW);
    PH_UP_state = 0;
    Serial.println("PH UP PUMP STATE: OFF ");
}


/**
 * @brief Set the pump containing PH DOWN Liquid OFF
 */
void LiquidPumps::set_PH_DOWN_PUMP_OFF(){
    digitalWrite(PH_DOWN_PIN, LOW);
    PH_DOWN_state = 0;
    Serial.println("PH DOWN PUMP STATE: OFF ");
}


/**
 * @brief Set the pump containing EC UP Liquid OFF
 */
void LiquidPumps::set_EC_UP_PUMP_OFF(){
    digitalWrite(EC_UP_PIN, LOW);
    EC_UP_state = 0;
    Serial.println("EC UP PUMP STATE: OFF");
}


/**
 * @brief Raise PH by setting the pump that contains PH UP Liquid ON for a while
 */
/*void LiquidPumps::raise_PH(){ 
    Serial.println("LOW pH level");
    set_PH_UP_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_PH_UP_PUMP_OFF();
    Serial.println("Completed");       
}*/

/**
 * @brief Lower PH by setting the pump that contains PH DOWN Liquid ON for a while
 */
/*void LiquidPumps::lower_PH(){
    Serial.println("HIGH pH level");
    set_PH_DOWN_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_PH_DOWN_PUMP_OFF();
    Serial.println("Completed");
}*/

/**
 * @brief Raise EC by setting the pump that contains EC UP Liquid ON for a while
 */
/*void LiquidPumps::raise_EC(){
    Serial.println("LOW EC level");
    set_EC_UP_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_EC_UP_PUMP_OFF();
    Serial.println("Completed");
}*/


/**
 * @brief Check if PH needs configuration
 */
/*void LiquidPumps::check_ph(){
    float phv = my_sens.get_my_PH();
    if (phv > MAX_PH) { 
        lower_PH();
    }
    else if(phv < MIN_PH){
        raise_PH();
    }
}*/

/**
 * @brief Check if EC needs configuration
 */
/*void LiquidPumps::check_ec(){
    float ecv = my_sens.get_my_TDS();
    if (ecv < MIN_EC) { 
        raise_EC();
    }
}*/
