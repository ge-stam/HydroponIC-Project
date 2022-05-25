#include "DHT.h"
#include "Arduino.h"
#include "Sensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GravityTDS.h>
#include <DFRobot_PH.h>
#include "Hydroconf.h"

// store the analog value of tds sensor in the array, read from ADC
static int analogBuffer[SCOUNT];    
static int analogBufferTemp[SCOUNT];
static int analogBufferIndex = 0, copyIndex = 0;
static float averageVoltage = 0, tdsValue = 0, temperature = 25;
static float voltage;
static float phValue;

static DHT dht(DHTPIN, DHTTYPE);
// Setup a oneWire instance to communicate with any OneWire devices
static OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
static DallasTemperature DS18B20_sensor(&oneWire); 
static GravityTDS gravityTds;
static DFRobot_PH ph;


Sensors::Sensors(){
  }

void Sensors::Init_DHT(){
  dht.begin();
  Serial.println("************ INITIALISE TEMPERATURE & HUMIDITY SENSOR ************");
}

void Sensors::Init_Light(){
  pinMode(LightSensorPin, INPUT);
  Serial.println("************    INITIALISE LIGHT INTENSITY SENSOR     ************");
}

void Sensors::Init_DS18B20(){
  // Start the DS18B20 sensor
  DS18B20_sensor.begin();
  Serial.print("************        INITIALISE DS18B20 SENSOR         ************\n");
}

void Sensors::Init_TDS(){
  gravityTds.setPin(TDSPIN);
  gravityTds.setAref(VREF);                //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(ADCRESOLUTION);  //1024 for 10bit ADC; 4096 for 12bit ADC
  gravityTds.begin();                     //initialization
  Serial.println("************          INITIALISE TDS SENSOR           ************");
}

void Sensors::Init_PH(){
  ph.begin();
}

void Sensors::Init_sensors(){
  Init_Light();
  Init_DHT();
  Init_DS18B20();
  Init_TDS();
  Init_PH();
  }

float Sensors::Read_Temp(){
  return dht.readTemperature();  // Read temperature as Celsius
}

float Sensors::Read_Hum(){
  return dht.readHumidity();        // Read humidity
}

float Sensors::Read_Light(){
  return analogRead(LightSensorPin);   // Read light level
}

float Sensors::Read_DS18B20(){
  DS18B20_sensor.requestTemperatures(); 
  return DS18B20_sensor.getTempCByIndex(0);
}

float Sensors::Read_TDS(){
  gravityTds.setTemperature(Read_Temp()); // set the temperature and execute temperature compensation
  gravityTds.update();                    //sample and calculate 
  tdsValue = gravityTds.getTdsValue();    // then get the value
  return tdsValue;
}

float Sensors::Read_PH(){
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U){                  //time interval: 1s
      timepoint = millis();
      voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
      phValue = ph.readPH(voltage, Read_Temp());  // convert voltage to pH with temperature compensation
  }
  ph.calibration(voltage,temperature);           // calibration process by Serail CMD
  return phValue;
}

void Sensors::Read_sensors(){
  float light = Read_Light();       // Read light level
  float humidity = Read_Hum();      // Read humidity
  float temperature = Read_Temp();  // Read temperature as Celsius
  float water_temp = Read_DS18B20(); // Read water temperature
  float tds_value = Read_TDS();     // Read TDS value
  float ph_value = Read_PH();
  
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
  Serial.println(light);
  Serial.print("Water Temperature: ");
  Serial.print(water_temp);
  Serial.print("°C ");
  Serial.print("TDS Value: ");
  Serial.print(tds_value);
  Serial.print("ppm ");
  Serial.print("PH Value: ");
  Serial.print(ph_value);
  Serial.println("ppm ");
  Serial.println("");
  }
