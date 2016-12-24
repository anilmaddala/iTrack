/**
 *  ___ ___  _  _   _     ___  __  ___    ___ ___  ___
 * | __/ _ \| \| | /_\   ( _ )/  \( _ )  / __| _ \/ __|
 * | _| (_) | .` |/ _ \  / _ \ () / _ \ | (_ |  _/\__ \
 * |_| \___/|_|\_/_/ \_\ \___/\__/\___/  \___|_|  |___/
 *
 * This example is meant to work with the Adafruit
 * FONA 808 or 3G Shield or Breakout
 *
 * Copyright: 2015 Adafruit
 * Author: Todd Treece
 * Licence: MIT
 *
 */
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

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Have a FONA 3G? use this object type instead
//Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);


void setup() {

  while (! Serial);

  Serial.begin(115200);
  Serial.println(F("Adafruit FONA 808 & 3G GPS demo"));
  Serial.println(F("Initializing FONA... (May take a few seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));
  // Try to enable GPRS
  

  Serial.println(F("Enabling GPS..."));
  fona.enableGPS(true);
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
  boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

  if (gps_success) {
    
    char sendto[11] = "8074489404";
    char * message;
    char sLatitude[141];
    char sLongitude[141];
    
    dtostrf(latitude, 6, 6, sLatitude);
    dtostrf(longitude, 6, 6, sLongitude);
    message = append(sLatitude, "+");
    message = append(message, sLongitude);
    Serial.println(message);
    
    if (!fona.sendSMS(sendto, message)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }

  } else {
    Serial.println("Waiting for FONA GPS 3D fix...");
  }
}

