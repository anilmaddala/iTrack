#include "Adafruit_FONA.h"

// standard pins for the shield, adjust as necessary
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

boolean DEBUG = false;
int led = 13;

void setup() {
  if(DEBUG) {
    while (! Serial);
    Serial.begin(115200);
  }
  
  pinMode(led, OUTPUT);

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    if(DEBUG) Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  if(DEBUG) Serial.println(F("FONA is OK"));
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

char * append(char * str1, char * str2) {
  char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
  strcpy(str3, str1);
  strcat(str3, str2);
  return str3;
}

void loop() {
  delay(2000);

  float latitude, longitude, speed_kph, heading, speed_mph, altitude;

  // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
  if(DEBUG) Serial.println(F("Enabling GPS..."));
  fona.enableGPS(true);
  boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading);

  if (gps_success) {
    if(DEBUG) Serial.println(F("Enabling GPRS..."));
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);

    String getURL;
    char sLatitude[141];
    char sLongitude[141];
    
    dtostrf(latitude, 6, 6, sLatitude);
    dtostrf(longitude, 6, 6, sLongitude);
    
    String baseURL = String("ec2-54-90-96-132.compute-1.amazonaws.com");
    
    getURL = baseURL + String("/?latitude=");
    getURL = getURL + sLatitude;
    getURL = getURL + String("&longitude=");
    getURL = getURL + sLongitude;
    
    uint16_t statuscode;
    int16_t length;

    if (!fona.HTTP_GET_start((char*)getURL.c_str(), &statuscode, (uint16_t *)&length)) {
          if(DEBUG) Serial.println("Failed!");
        }
    fona.HTTP_GET_end();

  } else {
    if(DEBUG) Serial.println("Waiting for FONA GPS 3D fix...");
  }
}

