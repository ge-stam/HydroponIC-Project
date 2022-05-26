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
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Sensors";
//char pass[] = "sensorslab";
char ssid[] = "HUAWEI Mate 20 lite";
char pass[] = "e6c007a066d2";

static unsigned long previous_time;
static unsigned long current_time;
static Sensors s;
static WaterPump wp;
static LiquidPumps lp;

BlynkTimer timer;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //sprintf(date_buffer, "%d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  sprintf(date_buffer, "%d-%02d-%02d \n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
}


// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

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

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

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
  if(wp.Get_WP_State()){
    Blynk.virtualWrite(V10, "ON");
  }
  else{
    Blynk.virtualWrite(V10, "OFF");
  }
}

void LowFreqData()
{
  s.Read_low_freq_sensors(); 
  lp.check_ph();
  lp.check_ec();

  Blynk.virtualWrite(V8, s.Read_TDS());
  Blynk.virtualWrite(V9, s.Read_PH());
}

void CloseOpenPumps()
{
  if(lp.get_PH_UP_state()){
    lp.set_PH_UP_PUMP_OFF();
  }
  if(lp.get_PH_DOWN_state()){
    lp.set_PH_DOWN_PUMP_OFF();
  }
  if(lp.get_EC_UP_state()){
    lp.set_EC_UP_PUMP_OFF();
  }
}

void setup()
{
  Serial.begin(115200);
  /*Initialise Blynk*/
  Blynk.begin(auth, ssid, pass); 
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(147,102,21,114), 80);
  
  // connect to WiFi and get datte and time 
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
  
  /* Initialize sensors and parts */
  s.Init_sensors();
  wp.Init_WaterPump();
  wp.Set_WP_OFF();
  lp.Init_LiquidPumps();

  Serial.println(date_buffer);
  // Setup a function to be called every second for Blynk
  timer.setInterval(2000L, HighFreqData);
  timer.setInterval(10000L, LowFreqData);
  timer.setInterval(11000L, CloseOpenPumps);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  /*current_time = millis();
  if(current_time >= previous_time + ReadDataInterval){
    s.Read_sensors(); 
    lp.check_ph();
    lp.check_ec();
    previous_time = current_time; 
  }*/
  
}