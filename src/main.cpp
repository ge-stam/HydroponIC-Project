/*  HydroponIC Project
 * 
 * This is a project created to develop a multi-sensor integrated system appropriate
 * for Hydroponic Greenhouses based on Internet Of Things Technology (IoT).
 * This is the implementation of my Diploma Thesis at Electrical and Computer Engineering 
 * dept. of National Technical University of Athens (NTUA).
 * 
 *
 * Copyright (c) 2022 Georgia Stamou <stamou.geor@gmail.com>
 * All rights reserved.
 * 
 */
#include <Arduino.h>
#include "Hydroconf.h"
#include "WaterPump.h"
#include "Sensors.h"
#include "LiquidPumps.h"
#include "EEPROM.h"
#include <Adafruit_NeoPixel.h>
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID           "TMPLavWBd3a0"
//#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_TEMPLATE_ID "TMPLavWBd3a0"
#define BLYNK_DEVICE_NAME "HydroponIC"
#define BLYNK_AUTH_TOKEN            "z0p0eN7f2abNmWPNWtUybrCJ1hFQKk3h"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "time.h"


char auth[] = BLYNK_AUTH_TOKEN;
char date_buffer[100];
char hour_buffer[5];
char min_buffer[5];
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Sensors";
//char pass[] = "sensorslab";
char ssid[] = "WIND_2.4G_12CA41";
char pass[] = "U4T78YGG";
//char ssid[] = "HUAWEI Mate 20 lite";
//char pass[] = "e6c007a066d2";

static float voltage;
static unsigned long previous_time;
static unsigned long current_time;
static Sensors s;
static WaterPump wp;
static LiquidPumps lp;
static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_RING_PIN, NEO_GRB + NEO_KHZ800);

BlynkTimer timer;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

void printLocalDate()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  sprintf(date_buffer, "%d-%02d-%02d \n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  }

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  sprintf(hour_buffer, "%d \n", timeinfo.tm_hour);
  //Serial.println(hour_buffer);
  sprintf(min_buffer, "%d \n", timeinfo.tm_min);
  }
/*
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  Blynk.virtualWrite(V1, value);
}*/

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V11)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  if(value){
    Blynk.virtualWrite(V10, "ON");
    wp.Set_WP_ON();
    }
   else{
    Blynk.virtualWrite(V10, "OFF");
    wp.Set_WP_OFF();
    }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V13)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  if(value){
    Blynk.virtualWrite(V16, "ON");
    lp.set_PH_UP_PUMP_ON();
    }
   else{
    Blynk.virtualWrite(V16, "OFF");
    lp.set_PH_UP_PUMP_OFF();
    }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V14)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  if(value){
    Blynk.virtualWrite(V17, "ON");
    lp.set_PH_DOWN_PUMP_ON();
    }
   else{
    Blynk.virtualWrite(V17, "OFF");
    lp.set_PH_DOWN_PUMP_OFF();
    }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V15)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  if(value){
    Blynk.virtualWrite(V18, "ON");
    lp.set_EC_UP_PUMP_ON();
    }
   else{
    Blynk.virtualWrite(V18, "OFF");
    lp.set_EC_UP_PUMP_OFF();
    }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
/*
void UltraHighFreqData(){
  if(wp.Get_WP_State()){
    Blynk.virtualWrite(V10, "ON");
    }
   else{
    Blynk.virtualWrite(V10, "OFF");
    }
  if(lp.get_PH_UP_state()){
    Blynk.virtualWrite(V16, "ON");
    }
   else{
    Blynk.virtualWrite(V16, "OFF");
    }
  if(lp.get_PH_DOWN_state()){
    Blynk.virtualWrite(V17, "ON");
    }
   else{
    Blynk.virtualWrite(V17, "OFF");
    }
  if(lp.get_EC_UP_state()){
    Blynk.virtualWrite(V18, "ON");
    }
   else{
    Blynk.virtualWrite(V18, "OFF");
    }
    Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, s.Read_Temp());
  Blynk.virtualWrite(V5, s.Read_Hum());
  Blynk.virtualWrite(V6, s.Read_Light());
  Blynk.virtualWrite(V7, s.Read_DS18B20());
  Blynk.virtualWrite(V12, date_buffer);
}*/

// This function sends Arduino's uptime every second to Virtual Pin 2.
void HighFreqData()
{
  s.Read_high_freq_sensors(); 
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, s.Read_Temp());
  Blynk.virtualWrite(V5, s.Read_Hum());
  Blynk.virtualWrite(V6, s.Read_Light());
  Blynk.virtualWrite(V7, s.Read_DS18B20());
  Blynk.virtualWrite(V12, date_buffer);
  
}

void LowFreqData()
{
  s.Read_low_freq_sensors();
  lp.check_ph();
  lp.check_ec(); 

  float tds_val = s.Read_TDS();
  Blynk.virtualWrite(V8, tds_val);
  if(tds_val > MAX_EC || tds_val < MIN_EC){
    Blynk.logEvent("ec_check");
  }
  float ph_val = s.Read_PH();
  Blynk.virtualWrite(V9, ph_val);
  if(ph_val > MAX_PH || ph_val < MIN_PH) {
    Blynk.logEvent("ph_check");
  }
}

void CloseOpenPumps()
{
  if(lp.get_PH_UP_state()){
    lp.set_PH_UP_PUMP_OFF();
    //Blynk.virtualWrite(V16, "OFF");
  }
  if(lp.get_PH_DOWN_state()){
    lp.set_PH_DOWN_PUMP_OFF();
    //Blynk.virtualWrite(V17, "OFF");
  }
  if(lp.get_EC_UP_state()){
    lp.set_EC_UP_PUMP_OFF();
    //Blynk.virtualWrite(V18, "OFF");
  }
}

void Check_Led()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  if((atoi(hour_buffer) > LED_START_HOUR_TIME || (atoi(hour_buffer) == LED_START_HOUR_TIME && atoi(min_buffer) >= LED_START_MIN_TIME))
      && (atoi(hour_buffer) < LED_STOP_HOUR_TIME || (atoi(hour_buffer) == LED_STOP_HOUR_TIME && atoi(min_buffer) <= LED_STOP_MIN_TIME))){
        for(int i=0;i<NUMPIXELS;i++)
        {
          pixels.setPixelColor(i, pixels.Color(160,20,20)); // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255 
          pixels.show(); // This sends the updated pixel color to the hardware.
        }
      }
      else{
       for(int i=0;i<NUMPIXELS;i++)
        {
          pixels.setPixelColor(i, pixels.Color(0,0,0)); // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255 
          pixels.show(); // This sends the updated pixel color to the hardware.
        }
      }
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512); // Initialise EEPROM
  Blynk.begin(auth, ssid, pass); //Initialise Blynk
  
  // connect to WiFi and get date and time 
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalDate();
  
  pixels.begin(); // This initializes the NeoPixel library.

  /* Initialize sensors and parts */
  s.Init_sensors();
  wp.Init_WaterPump();
  wp.Set_WP_ON();
  lp.Init_LiquidPumps();

  Serial.println(date_buffer);
  // Setup a function to be called every second for Blynk
  timer.setInterval(10000L, LowFreqData);
  timer.setInterval(13000L, CloseOpenPumps);
  //timer.setInterval(1000L, UltraHighFreqData);
  timer.setInterval(2000L, HighFreqData);
  timer.setInterval(120000L, Check_Led);
}

void loop()
{
  Blynk.run();
  timer.run();
  //pixels.show();
  //s.Ph_cali(analogRead(PH_PIN)/4096.0*5000, s.Read_Temp());
  //voltage = analogRead(PH_PIN)/4096.0*5000;  // read the voltage
  //Serial.println(voltage);
  //phValue = ph.readPH(voltage, Read_Temp());  // convert voltage to pH with temperature compensation
  //s.Ph_cali(voltage,25.0);           // calibration process by Serail CMD
  }