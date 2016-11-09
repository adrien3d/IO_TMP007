/***************************************************************************
  This is a library for the TMP007 InfraRed temperature sensor

  Designed to work with all kinds of TMP007 Breakout Boards

  These sensors use I2C, 2 pins are required to interface.

  Written by Adrien Chapelet for Iotech
 ***************************************************************************/

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

// uncomment for debugging!
//#define TMP007_DEBUG 1

#define TMP007_VOBJ       0x00
#define TMP007_TDIE       0x01
#define TMP007_CONFIG     0x02
#define TMP007_TOBJ       0x03
#define TMP007_STATUS     0x04
#define TMP007_STATMASK   0x05

#define TMP007_CFG_RESET    0x8000
#define TMP007_CFG_MODEON   0x1000
#define TMP007_CFG_1SAMPLE  0x0000
#define TMP007_CFG_2SAMPLE  0x0200
#define TMP007_CFG_4SAMPLE  0x0400
#define TMP007_CFG_8SAMPLE  0x0600
#define TMP007_CFG_16SAMPLE 0x0800
#define TMP007_CFG_ALERTEN  0x0100
#define TMP007_CFG_ALERTF   0x0080
#define TMP007_CFG_TRANSC   0x0040

#define TMP007_STAT_ALERTEN 0x8000
#define TMP007_STAT_CRTEN   0x4000

#define TMP007_I2CADDR 0x40
#define TMP007_DEVID 0x1F



class EQS_TMP007  {
 public:
  EQS_TMP007(uint8_t addr = TMP007_I2CADDR);
  boolean begin(uint8_t samplerate = TMP007_CFG_16SAMPLE);  // by default go highres

  int16_t readRawDieTemperature(void);
  int16_t readRawVoltage(void);
  double readObjTempC(void);
  double readDieTempC(void);

 private:
  uint8_t _addr;
  uint16_t read16(uint8_t addr);
  void write16(uint8_t addr, uint16_t data);
};

