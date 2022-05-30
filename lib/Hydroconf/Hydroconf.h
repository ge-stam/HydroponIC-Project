#ifndef Hydroconf_h
#define Hydroconf_h
    
#define DHTPIN 4
#define DHTTYPE DHT11
#define LightSensorPin 33 // LDR sensor
#define ReadDataInterval 10000  //Read data every 10 secs
#define oneWireBus 25     // GPIO where the DS18B20 is connected to
#define TDSPIN 35
#define ADCRESOLUTION 4096
#define VREF 5.0          // analog reference voltage(Volt) of the ADC
#define SCOUNT  30        // sum of sample point for tds sensor
#define WATERPUMPPIN 12
#define PH_UP_PIN 14
#define PH_DOWN_PIN 27
#define EC_UP_PIN 26
#define MIN_PH 5.5    //MUST BE CHANGED
#define MAX_PH 6    //MUST BE CHANGED
#define MIN_EC 550
#define MAX_EC 850
#define PH_PIN 34
#define PH_IDEAL 700
#define PH_ADJ_TIME 1000 //1 sec
#define LED_RING_PIN   22
#define NUMPIXELS      12

#endif