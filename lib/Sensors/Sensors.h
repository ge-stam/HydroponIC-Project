#ifndef Sensors_h
#define Sensors_h

#include "DHT.h"
    
class Sensors{

  public:

  Sensors();
  void Init_DHT();
  void Init_Light();
  void Init_DS18B20();
  void Init_TDS();
  void Init_PH();
  void Init_sensors();
  void Read_sensors();
  float Read_Temp();
  float Read_Hum();
  float Read_Light();  
  float Read_DS18B20();
  float Read_TDS(); 
  float Read_PH(); 
  };

#endif

