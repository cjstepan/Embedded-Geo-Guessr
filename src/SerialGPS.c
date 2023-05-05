#include <pic18.h>
#include <math.h>



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



void Serial_Init(void){
       // Initialize Serial Port to 9600 baud
   TRISC = TRISC | 0xC0;
   TXIE = 0;
   RCIE = 1;
   BRGH = 1;
   BRG16 = 1;
   SYNC = 0;
   SPBRG = 255;
   SPBRGH = 0;
   TXSTA = 0x22;
   RCSTA = 0x90;
   PEIE = 1;

   // Turn on all interrupts
   GIE = 1;
}

void serialISR(void){
   if (RCIF)
   {
      TEMP = RCREG; // Read the serial register bit
      // TXREG = TEMP; // data in, data out
      if (TEMP > 20)
         UARTBUFFER[_SERIALINDEX++] = TEMP; // store filtered ascii values to buffer, ignore control bits
      if (_SERIALINDEX > 79)
         _SERIALINDEX = 79; // prevent buffer overflow
      if (TEMP == 13)
      { // carriage return detection
         FLAG = 1;
         RB0 = !RB0;
         if (UARTBUFFER[5] == 'C')
         { // GPRMC in the buffer
            for (int i = _SERIALINDEX + 1; i < 80; i++)
            {
               UARTBUFFER[i] = ' ';
            }
            for (int i = 0; i < 80; i++)
            {
               GPSRAW[i] = UARTBUFFER[i];
               UARTBUFFER[i] = ' ';
            }
         }

         _SERIALINDEX = 0; // reset the buffer
      }
      RCIF = 0;
   }
}





void GPS_parseData(void)
{

   gps.timestamp = (GPSRAW[7]-48)*100000   +
                  (GPSRAW[8]-48)*10000    +
                  (GPSRAW[9]-48)*1000     +
                  (GPSRAW[10]-48)*100     +
                  (GPSRAW[11]-48)*10      +
                  (GPSRAW[12]-48)*1       +
                  (GPSRAW[14]-48)*0.1     +
                  (GPSRAW[15]-48)*0.01    +
                  (GPSRAW[16]-48)*0.001   ;

   gps.isValid = (GPSRAW[18] == 'A');

   if(1)
   {

      gps.latitude = (GPSRAW[30] == 'N' ? 1 : -1) * 
      (
         (GPSRAW[20]-48)*10 + 
         (GPSRAW[21]-48) + 
         (
            (GPSRAW[22]-48)*10 +
            (GPSRAW[23]-48)*1 +
            (GPSRAW[25]-48)*0.1 +
            (GPSRAW[26]-48)*0.01 +
            (GPSRAW[27]-48)*0.001 
         )/60.00
      );
         
      gps.longitude = (GPSRAW[43] == 'E' ? 1 : -1) * 
      (
         (GPSRAW[32]-48)*100 + 
         (GPSRAW[33]-48)*10 +
         (GPSRAW[34]-48)*1 +
         (
            (GPSRAW[35]-48)*10 +
            (GPSRAW[36]-48)*1 +
            (GPSRAW[38]-48)*0.1 +
            (GPSRAW[39]-48)*0.01 +
            (GPSRAW[40]-48)*0.001 
         )/60.00
      );
   }
   else
   {
      gps.latitude = 0;
      gps.longitude = 0;
   }
}

/**
 * @brief calculate distance between two coordinates using haversines
 * 
 * @param currentLat 
 * @param currentLon 
 * @param targetLat 
 * @param targetLon 
 * @return long distance in feet; 
 */
double GPS_calcDistanceFargo( double currentLat,  double currentLon,  double targetLat,  double targetLon)
{
   double latDist = (targetLat - currentLat)*250000.00;
   double lonDist = (targetLon- currentLon) *363636.00;

   return sqrt(latDist*latDist + lonDist*lonDist);
}