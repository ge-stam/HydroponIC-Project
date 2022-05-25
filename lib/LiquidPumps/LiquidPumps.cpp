#include "Arduino.h"
#include "LiquidPumps.h"
#include "Hydroconf.h"
#include "Sensors.h"

static Sensors s;
static unsigned long currentMillis;
static unsigned long previousMillis;

LiquidPumps::LiquidPumps(){
}

void LiquidPumps::Init_LiquidPumps(){
    pinMode(PH_UP_PIN, OUTPUT);
    pinMode(PH_DOWN_PIN, OUTPUT);
    pinMode(EC_UP_PIN, OUTPUT);
    Serial.println("************        INITIALISE LIQUID PUMPS           ************\n");
}

int LiquidPumps::get_PH_UP_state(){
    return PH_UP_state;
}

int LiquidPumps::get_PH_DOWN_state(){
    return PH_DOWN_state;
}

int LiquidPumps::get_EC_UP_state(){
    return EC_UP_state;
}

void LiquidPumps::set_PH_UP_PUMP_ON(){
    digitalWrite(PH_UP_PIN, HIGH);
    Serial.println("PH UP PUMP STATE: ON ");
}

void LiquidPumps::set_PH_DOWN_PUMP_ON(){
    digitalWrite(PH_DOWN_PIN, HIGH);
    Serial.println("PH DOWN PUMP STATE: ON ");
}

void LiquidPumps::set_EC_UP_PUMP_ON(){
    digitalWrite(EC_UP_PIN, HIGH);
    Serial.println("PH UP PUMP STATE: ON ");
}

void LiquidPumps::set_PH_UP_PUMP_OFF(){
    digitalWrite(PH_UP_PIN, LOW);
    Serial.println("PH UP PUMP STATE: OFF ");
}

void LiquidPumps::set_PH_DOWN_PUMP_OFF(){
    digitalWrite(PH_DOWN_PIN, LOW);
    Serial.println("PH DOWN PUMP STATE: OFF ");
}
void LiquidPumps::set_EC_UP_PUMP_OFF(){
    digitalWrite(EC_UP_PIN, LOW);
    Serial.println("EC UP PUMP STATE: OFF");
}


void LiquidPumps::raise_PH(){ 
    Serial.println("LOW pH level");
    set_PH_UP_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_PH_UP_PUMP_OFF();
    Serial.println("Completed");       
}

void LiquidPumps::lower_PH(){
    Serial.println("HIGH pH level");
    set_PH_DOWN_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_PH_DOWN_PUMP_OFF();
    Serial.println("Completed");
}

void LiquidPumps::raise_EC(){
    Serial.println("LOW EC level");
    set_EC_UP_PUMP_ON();
    currentMillis = millis();
    previousMillis = millis();     
    while (currentMillis - previousMillis < PH_ADJ_TIME) { 
        currentMillis = millis(); 
    }  
    set_EC_UP_PUMP_OFF();
    Serial.println("Completed");
}

void LiquidPumps::check_ph(){
    float phv = s.Read_PH();
    if (phv > MAX_PH) { 
        lower_PH();
    }
    else if(phv < MIN_PH){
        raise_PH();
    }
}

void LiquidPumps::check_ec(){
    float ecv = s.Read_TDS();
    if (ecv < MIN_EC) { 
        raise_EC();
    }
}
