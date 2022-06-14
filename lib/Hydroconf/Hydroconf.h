#ifndef Hydroconf_h
#define Hydroconf_h
    
#define DHTPIN 4
#define DHTTYPE DHT11
#define LightSensorPin 33       // LDR sensor
#define ReadDataInterval 10000  //Read data every 10 secs
#define oneWireBus 25           // GPIO where the DS18B20 is connected to
#define TDSPIN 35
#define ADCRESOLUTION 4096
#define VREF 5.0                // analog reference voltage(Volt) of the ADC
#define SCOUNT  30              // sum of sample point for tds sensor
#define WATERPUMPPIN 12
#define PH_UP_PIN 13
#define PH_DOWN_PIN 27
#define EC_UP_PIN 26
#define MIN_PH 5.2              
#define MAX_PH 7.0                // 6.5 but because of the sensor --> 7              
#define MIN_EC 450              // K = 0.65
#define MAX_EC 950              // buffer 1.413mS/cm --> 707 ppm
#define PH_PIN 32
#define PH_offset 0.00          //deviation compensate
#define PH_IDEAL 700
#define PH_ADJ_TIME 400        //1.5 sec
#define LED_RING_PIN   23
#define NUMPIXELS      61
#define LED_START_HOUR_TIME 6
#define LED_START_MIN_TIME  30
#define LED_STOP_HOUR_TIME  20
#define LED_STOP_MIN_TIME   30 
#define PH_samplingInterval 20
#define PH_printInterval 800
#define PH_ArrayLenth  40       //times of collection

#endif