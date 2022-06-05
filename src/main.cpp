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

#define BLYNK_TEMPLATE_ID "TMPLavWBd3a0"
#define BLYNK_DEVICE_NAME "HydroponIC"
#define BLYNK_AUTH_TOKEN  "z0p0eN7f2abNmWPNWtUybrCJ1hFQKk3h"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "time.h"

/* ===============================================================================================  */
/*                                VARIABLES & FUNCTIONS DECLARATIONS                                */
/* ===============================================================================================  */
char auth[] = BLYNK_AUTH_TOKEN;
char date_buffer[100];
char hour_buffer[5];
char min_buffer[5];
// WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Sensors";
//char pass[] = "sensorslab";
char ssid[] = "WIND_2.4G_12CA41";
char pass[] = "U4T78YGG";

static float voltage;
static unsigned long previous_time;
static unsigned long current_time;
static Sensors s;
static WaterPump wp;
static LiquidPumps lp(s);
static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_RING_PIN, NEO_GRB + NEO_KHZ800);
BlynkTimer timer;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

/**
 * @brief Prints Local Date
 */
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

/**
 * @brief Prints Local Time
 */
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  sprintf(hour_buffer, "%d \n", timeinfo.tm_hour);
  sprintf(min_buffer, "%d \n", timeinfo.tm_min);
}

/**
 * @brief Connect to WiFi and get date and time
 */
void connectWifi_getTime(){ 
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
}

/**
 * @brief Get the value of WaterPump Switch from Blynk
 * @param V11 Datastream from Blynk
 */
BLYNK_WRITE(V11)
{
  int value = param.asInt();
  if(value){
    Blynk.virtualWrite(V10, "ON");
    wp.Set_WP_ON();
  }
  else{
    Blynk.virtualWrite(V10, "OFF");
    wp.Set_WP_OFF();
  }
}

/**
 * @brief Get channel's cayenne datatype
 * @param V13 Datastream from Blynk
 */
BLYNK_WRITE(V13)
{
  int value = param.asInt();
  if(value){
    lp.set_PH_UP_PUMP_ON();
  }
  else{
    lp.set_PH_UP_PUMP_OFF();
  }
}

/**
 * @brief Get channel's cayenne datatype
 * @param V14 Datastream from Blynk
 */
BLYNK_WRITE(V14)
{
  int value = param.asInt();
  if(value){
    Blynk.virtualWrite(V17, "ON");
    lp.set_PH_DOWN_PUMP_ON();
  }
  else{
    Blynk.virtualWrite(V17, "OFF");
    lp.set_PH_DOWN_PUMP_OFF();
  }
}

/**
 * @brief Get channel's cayenne datatype
 * @param V15 Datastream from Blynk
 */
BLYNK_WRITE(V15)
{
  int value = param.asInt();
  if(value){
    Blynk.virtualWrite(V18, "ON");
    lp.set_EC_UP_PUMP_ON();
  }
  else{
    Blynk.virtualWrite(V18, "OFF");
    lp.set_EC_UP_PUMP_OFF();
  }
}

/**
 * @brief Connection State in Blynk
 */
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

/**
 * @brief Reads sensors objects and upload their values to Blynk
 */
void HighFreqData()
{
  s.Read_high_freq_sensors(); 
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, s.Read_Temp());
  Blynk.virtualWrite(V5, s.Read_Hum());
  Blynk.virtualWrite(V6, s.Read_Light());
  Blynk.virtualWrite(V7, s.Read_DS18B20());
  Blynk.virtualWrite(V12, date_buffer);
}

/**
 * @brief Reads PH nad TDS sensors, upload their values to Blynk and 
 *        checks if correction is needed
 */
void LowFreqData()
{
  s.Read_low_freq_sensors();
  lp.check_ph();
  lp.check_ec(); 

  float tds_val = s.get_my_TDS();
  Blynk.virtualWrite(V8, tds_val);
  if(tds_val > MAX_EC || tds_val < MIN_EC){
    Blynk.logEvent("ec_check");
  }
  float ph_val = s.get_my_PH();
  Blynk.virtualWrite(V9, ph_val);
  if(ph_val > MAX_PH || ph_val < MIN_PH) {
    Blynk.logEvent("ph_check");
  }
}

/**
 * @brief Checks if LED must be on or off 
 */
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
          pixels.setPixelColor(i, pixels.Color(0,0,0)); 
          pixels.show(); 
        }
      }
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512); // Initialise EEPROM
  Blynk.begin(auth, ssid, pass); //Initialise Blynk
  connectWifi_getTime();
  pixels.begin(); // This initializes the NeoPixel library.
  /* Initialize sensors and parts */
  s.Init_sensors();
  wp.Init_WaterPump();
  wp.Set_WP_ON();
  lp.Init_LiquidPumps();
  Serial.println(date_buffer);
  timer.setInterval(900000L, LowFreqData); // reads data every 15 minutes
  timer.setInterval(2000L, HighFreqData);  // reads data every 2 secs
  timer.setInterval(120000L, Check_Led);   // checks every 2 mins
}

void loop()
{
  Blynk.run();
  timer.run();
}