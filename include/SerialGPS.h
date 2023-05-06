#ifndef SERIALGPS_H
#define SERIALGPS_H

unsigned char _SERIALINDEX;              // byte _SERIALINDEX of message
unsigned char UARTBUFFER[80]; // buffer for recieving data
unsigned char GPSRAW[80];
unsigned char TEMP;           // temp data from SCI port
unsigned char FLAG;

struct GPSDATA
{
   double timestamp;
   int isValid;
   double latitude;
   double longitude;
}gps;




void Serial_Init(void);

void serialISR(void);

void GPS_parseData(void);

float GPS_calcDistanceFargo(float currentLat, float currentLon, float targetLat, float targetLon);




#endif