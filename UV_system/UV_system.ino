#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <Adafruit_VEML6070.h>


#include <Adafruit_BME280.h>


// Sensors
Adafruit_BME280 bme; // I2C for barometric sensor
Adafruit_VEML6070 uv = Adafruit_VEML6070();


float UV_value = 0;
unsigned int adc_value = 0;

void setup() {
  Serial.begin(9600);
  
  
  bme.begin();
  uv.begin(VEML6070_1_T);
}
 

void read_uv_index() {
  adc_value = adcAverage();
  UV_value = ((adc_value * 0.0341) - 1.3894);
  
  if (UV_value > 11) {
    UV_value = 11;
  } else if (UV_value <= 0)   {
    UV_value = 0;
  }
}

  
void loop() {
  sensors_event_t event;
  adcAverage();
  read_uv_index();

  
  float bmepres = bme.readPressure();
    float bmehum = bme.readHumidity();
  
  String Temp_BME = (String)bme.readTemperature();
  String Hum_BME = (String)bme.readHumidity();
  String Pres_BME = (String)(bme.readPressure() / 100);
  String UV_index = (String)UV_value;

    
     int NData = 6;
      String str_Payload;

 str_Payload += NData;
  str_Payload += " " + Temp_BME;
  str_Payload += " " + Hum_BME;
  str_Payload += " " + Pres_BME; 
  str_Payload += " " + UV_index;
  str_Payload += " ";
  
  byte Payload[str_Payload.length()];
  int LengthFrameAPI = 18 + sizeof(Payload);
  int LengthPayload = sizeof(Payload);
  
  Serial.println(str_Payload);
    
  delay(60000);
}

unsigned int adcAverage() {
  unsigned char samples = 64;
  unsigned long avg = 0;
  while (samples > 0) {
    avg = (avg + analogRead(A0));
    delayMicroseconds(4);
    samples--;
  }
  avg >>= 6;
  return avg;
}

