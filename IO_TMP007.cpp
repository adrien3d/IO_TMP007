/***************************************************************************
  This is a library for the TMP007 InfraRed temperature sensor

  Designed to work with all kinds of TMP007 Breakout Boards

  These sensors use I2C, 2 pins are required to interface.

  Written by Adrien Chapelet for Iotech
 ***************************************************************************/

#include "EQS_TMP007.h"
//#include <util/delay.h>

//#define TESTDIE 0x0C78
//#define TESTVOLT 0xFEED

EQS_TMP007::EQS_TMP007(uint8_t i2caddr) {
  _addr = i2caddr;
}


boolean EQS_TMP007::begin(uint8_t samplerate) {
  Wire.begin();

  write16(TMP007_CONFIG, TMP007_CFG_MODEON | TMP007_CFG_ALERTEN | 
	  TMP007_CFG_TRANSC | samplerate);
  write16(TMP007_STATMASK, TMP007_STAT_ALERTEN |TMP007_STAT_CRTEN);
  // enable conversion ready alert

  uint16_t did;
  did = read16(TMP007_DEVID);
#ifdef TMP007_DEBUG
  Serial.print("did = 0x"); Serial.println(did, HEX);
#endif
  if (did != 0x78) return false;
  return true;
}

//////////////////////////////////////////////////////

double EQS_TMP007::readDieTempC(void) {
   double Tdie = readRawDieTemperature();
   Tdie *= 0.03125; // convert to celsius
#ifdef TMP007_DEBUG
   Serial.print("Tdie = "); Serial.print(Tdie); Serial.println(" C");
#endif
   return Tdie;
}

double EQS_TMP007::readObjTempC(void) {
  int16_t raw = read16(TMP007_TOBJ);
  // invalid
  if (raw & 0x1) return NAN;
  raw >>=2;

  double Tobj = raw;
  Tobj *= 0.03125; // convert to celsius
#ifdef TMP007_DEBUG
   Serial.print("Tobj = "); Serial.print(Tobj); Serial.println(" C");
#endif
   return Tobj;
}



int16_t EQS_TMP007::readRawDieTemperature(void) {
  int16_t raw = read16(TMP007_TDIE);

#if TMP007_DEBUG == 1

#ifdef TESTDIE
  raw = TESTDIE;
#endif

  Serial.print("Raw Tambient: 0x"); Serial.print (raw, HEX);
  

  float v = raw/4;
  v *= 0.03125;
  Serial.print(" ("); Serial.print(v); Serial.println(" *C)");
#endif
  raw >>= 2;
  return raw;
}

int16_t EQS_TMP007::readRawVoltage(void) {
  int16_t raw;

  raw = read16(TMP007_VOBJ);

#if TMP007_DEBUG == 1

#ifdef TESTVOLT
  raw = TESTVOLT;
#endif

  Serial.print("Raw voltage: 0x"); Serial.print (raw, HEX);
  float v = raw;
  v *= 156.25;
  v /= 1000;
  Serial.print(" ("); Serial.print(v); Serial.println(" uV)");
#endif
  return raw; 
}


/*********************************************************************/

uint16_t EQS_TMP007::read16(uint8_t a) {
  uint16_t ret;

  Wire.beginTransmission(_addr); // start transmission to device 
#if (ARDUINO >= 100)
  Wire.write(a); // sends register address to read from
#else
  Wire.send(a); // sends register address to read from
#endif
  Wire.endTransmission(); // end transmission
  
  Wire.beginTransmission(_addr); // start transmission to device 
  Wire.requestFrom(_addr, (uint8_t)2);// send data n-bytes read
#if (ARDUINO >= 100)
  ret = Wire.read(); // receive DATA
  ret <<= 8;
  ret |= Wire.read(); // receive DATA
#else
  ret = Wire.receive(); // receive DATA
  ret <<= 8;
  ret |= Wire.receive(); // receive DATA
#endif
  Wire.endTransmission(); // end transmission

  return ret;
}

void EQS_TMP007::write16(uint8_t a, uint16_t d) {
  Wire.beginTransmission(_addr); // start transmission to device 
#if (ARDUINO >= 100)
  Wire.write(a); // sends register address to read from
  Wire.write(d>>8);  // write data
  Wire.write(d);  // write data
#else
  Wire.send(a); // sends register address to read from
  Wire.send(d>>8);  // write data
  Wire.send(d);  // write data
#endif
  Wire.endTransmission(); // end transmission
}

