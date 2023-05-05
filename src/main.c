// main.c
// A starting point for this project

#include <pic18.h>
#include "PICLiquidCrystal.h"
#include <SerialGPS.h>

const unsigned char MSG0[16] = "Hello world!    ";

const int bellChar[] =
    {
        0B00000,
        0B00100,
        0B01110,
        0B01110,
        0B01110,
        0B11111,
        0B11111,
        0B00000};

// High-priority service
void interrupt IntServe(void)
{
   serialISR();
}

void main(void)
{
   unsigned int i, j;
   unsigned int TIME;

   TRISA = 0;
   TRISB = 0;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();    // initialize the LCD
   Serial_Init(); // initialize the serial port @9600 baud

   LCD_Create_Char(0, bellChar);

   LCD_writeLine(MSG0, 0);

   LCD_Move(0, 14);
   LCD_Write(0); // write custom char at CGRAM address 0

   TIME = 0;

   while (1)
   {
      if (FLAG)
      {
         GPS_parseData();

         for (int i = 0; i < 80; i++)
         {
            while (!TRMT)
            {
               ;
            }
            TXREG = GPSRAW[i];
         }

         SCI_CRLF();
         FLAG = 0;
      }

      LCD_Move(0, 0);
      LCD_Out((long)(gps.latitude * 100000.0), 8, 5);
      LCD_Move(1, 0);
      LCD_Out((long)(gps.longitude * 100000.0), 8, 5);
   }
}