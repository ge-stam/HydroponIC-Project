#include "DHT.h"
#include "Arduino.h"
#include "Sensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GravityTDS.h>
#include "Hydroconf.h"

/* ===============================================================================================  */
/*                                VARIABLES & FUNCTIONS DECLARATIONS                                */
/* ===============================================================================================  */
static int analogBuffer[SCOUNT]; // store the analog value of tds sensor in the array, read from ADC    
static int analogBufferTemp[SCOUNT];
static int analogBufferIndex = 0, copyIndex = 0;
static float averageVoltage = 0, tdsValue = 0, temperature = 25;
static float voltage;
static float pHValue;
static int pHArray[PH_ArrayLenth];   //Store the average value of the sensor feedback
static int pHArrayIndex=0;

static DHT dht(DHTPIN, DHTTYPE);
static OneWire oneWire(oneWireBus); // Setup a oneWire instance to communicate with any OneWire devices
static DallasTemperature DS18B20_sensor(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor  
static GravityTDS gravityTds;

static double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

/**
 * @brief Constructor of Sensors
 */
Sensors::Sensors(){
}

/**
 * @brief Initialise DHT Sensor (Temperature & Humidity)
 */
void Sensors::Init_DHT(){
  dht.begin();
  Serial.println("************ INITIALISE TEMPERATURE & HUMIDITY SENSOR ************");
}

/**
 * @brief Initilise Light Intensity Sensor
 */
void Sensors::Init_Light(){
  pinMode(LightSensorPin, INPUT);
  Serial.println("************    INITIALISE LIGHT INTENSITY SENSOR     ************");
}

/**
 * @brief Initialise DS18B20 Sensor (Waterproof Temperature Sensor)
 */
void Sensors::Init_DS18B20(){
  // Start the DS18B20 sensor
  DS18B20_sensor.begin();
  Serial.print("************        INITIALISE DS18B20 SENSOR         ************\n");
}

/**
 * @brief Initialise TDS Sensor
 */
void Sensors::Init_TDS(){
  gravityTds.setPin(TDSPIN);
  gravityTds.setAref(VREF);                //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(ADCRESOLUTION);  //1024 for 10bit ADC; 4096 for 12bit ADC
  gravityTds.begin();                     //initialization
  Serial.println("************          INITIALISE TDS SENSOR           ************");
}

/**
 * @brief Initialise PH Sensor
 */
void Sensors::Init_PH(){
  Serial.println("************          INITIALISE PH SENSOR            ************");
  //ph.begin();
}

/**
 * @brief Initialise all Sensors
 */
void Sensors::Init_sensors(){
  Init_Light();
  Init_DHT();
  Init_DS18B20();
  Init_TDS();
  Init_PH();
}

/**
 * @brief Read Temperature Sensor (DHT)
 * @return float
 */
float Sensors::Read_Temp(){
  return dht.readTemperature();  // Read temperature as Celsius
}

/**
 * @brief Read Humidity Sensor (DHT)
 * @return float
 */
float Sensors::Read_Hum(){
  return dht.readHumidity();        // Read humidity
}

/**
 * @brief Read Light Intensity Sensor
 * @return float
 */
float Sensors::Read_Light(){
  return analogRead(LightSensorPin);   // Read light level
}

/**
 * @brief Read DS18B20 Sensor
 * @return float
 */
float Sensors::Read_DS18B20(){
  DS18B20_sensor.requestTemperatures(); 
  return DS18B20_sensor.getTempCByIndex(0);
}

/**
 * @brief Read TDS Sensor
 * @return float
 */
float Sensors::Read_TDS(){
  Serial.print("TDS: ");
  Serial.println(tdsValue);
  gravityTds.setTemperature(Read_Temp()); // set the temperature and execute temperature compensation
  gravityTds.update();                    //sample and calculate 
  tdsValue = gravityTds.getTdsValue();    // then get the value
  myTDS = tdsValue;
  return tdsValue;
}

/**
 * @brief Read PH Sensor
 * @return float
 */
float Sensors::Read_PH(){
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > PH_samplingInterval)
  {
    pHArray[pHArrayIndex++]=analogRead(PH_PIN);
    if(pHArrayIndex==PH_ArrayLenth)pHArrayIndex=0;
    voltage = avergearray(pHArray, PH_ArrayLenth)*3.54/4096;
    pHValue = 4.1*voltage + PH_offset;
    myPH = pHValue;
    samplingTime=millis();
  }
  if(millis() - printTime > PH_printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage,2);
    Serial.print("    pH value: ");
    Serial.println(pHValue,2);
    printTime=millis();
  }
  return pHValue;
}

/**
 * @brief Read Temperature, Humidity, Water-Temperature and Light 
 *        Initensity Sensor
 */
void Sensors::Read_high_freq_sensors(){
  float light = Read_Light();       // Read light level
  float humidity = Read_Hum();      // Read humidity
  float temperature = Read_Temp();  // Read temperature as Celsius
  float water_temp = Read_DS18B20(); // Read water temperature
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
 
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  ");
  Serial.print("Light Level: ");
  Serial.print(light * 0.244140625); // calculate Light in Lux
  Serial.println(" Lux");
  Serial.print("Water Temperature: ");
  Serial.print(water_temp);
  Serial.println("°C ");
  }

/**
 * @brief Read TDS and PH Sensor
 */
void Sensors::Read_low_freq_sensors(){
  float tds_value = Read_TDS();     // Read TDS value
  float ph_value = Read_PH();       // Read PH Value

  Serial.print("TDS Value: ");
  Serial.print(tds_value);
  Serial.print("ppm ");
  Serial.print("PH Value: ");
  Serial.print(ph_value,2);
  Serial.println(" ");
  Serial.println("");
}

/**
 * @brief Get the value of PH Sensor
 * @return float
 */
float Sensors::get_my_PH(){
  return myPH;
}
  
/**
 * @brief Get the value of TDS Sensor
 * @return float
 */
float Sensors::get_my_TDS(){
  return myTDS;
}
