// main.c
// A starting point for this project

#include <pic18.h>
#include "picliquidcrystal.h"
#include "lcd_portd.c"
#include "SerialGPS.h"

// Variable names
double ndsu_ece[] = {46.894494, -96.801560};
double churchill_field[] = {46.893185, -96.798746};
double newman_outdoor_field[] = {46.898577, -96.803808};
double fargodome[] = {46.902992, -96.799206};
double hector_international_airport[] = {46.918720, -96.816482};
unsigned int T0, selection, TIME, RUN, REACT, temp;
double targetLat, targetLong;

// High-priority service
void interrupt IntServe(void)
{
   if (TMR2IF) 
	{
		RC0 = !RC0;
		if(!RUN)
		{
			if(RB0) 
			{
				RUN = 1;
				REACT = 0;
			}              
			temp = 5000;
		}
		if(RUN) 
		{
			TIME += 1;
			if((temp < TIME))
			{
				REACT += 1;
				if(RB0) 
				{
					RUN = 0;
				}
			}
		}
		TMR2IF = 0;
	}

   serialISR();
}

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

void menuInit(void)
{
   LCD_writeLine("Welcome to the  ", 0);
   LCD_writeLine("ECE376 GeoGuessr", 1);
   Wait_ms(2000);

   LCD_writeLine("Choose a Spot   ", 0);
   LCD_writeLine("With the Buttons", 1);
   Wait_ms(2000);

   LCD_writeLine("1) ECE Building ", 0);
   LCD_writeLine("2)ChurchillField", 1);
   Wait_ms(2000);

   LCD_writeLine("3) Newman Field ", 0);
   LCD_writeLine("4) Fargodome    ", 1);
   Wait_ms(2000);

   LCD_Clear();
   LCD_writeLine("5) Fargo Airport", 0);

   while(selection == 0)
   {
      if(RB1) selection = 1;
      if(RB2) selection = 2;
      if(RB3) selection = 3;
      if(RB4) selection = 4;
      if(RB5) selection = 5;

   }

   // Wait_ms(1000);

   LCD_writeLine("Your Selection  ", 0);
   switch (selection)
   {
   case 1:
      LCD_writeLine("  ECE Building  ", 1);
      targetLat = ndsu_ece[0];
      targetLong = ndsu_ece[1];
      break;
   case 2:
      LCD_writeLine("Churchill Field ", 1);
      targetLat = churchill_field[0];
      targetLong = churchill_field[1];
      break;
   case 3:
      LCD_writeLine("  Newman Field  ", 1);
      targetLat = newman_outdoor_field[0];
      targetLong = newman_outdoor_field[1];
      break;
   case 4:
      LCD_writeLine("    Fargodome   ", 1);
      targetLat = fargodome[0];
      targetLong = fargodome[1];
      break;
   case 5: 
      LCD_writeLine(" Fargo Airport  ", 1);
      targetLat = hector_international_airport[0];
      targetLong = hector_international_airport[1];
      break;
   default:
      LCD_writeLine("Invalid Choice  ", 1);
      break;
   }

   Wait_ms(1000);
   LCD_writeLine("Distance:       ", 0);

   Wait_ms(1000);
   LCD_writeLine("Time:           ", 1);
}

void main(void)
{

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init(); // initialize the LCD
   Serial_Init();
   menuInit();

   // Timer2 Initialize
   TMR2ON = 1;
   TMR2IE = 1;
   PEIE = 1;
   T2CON = 0x4D;
   PR2 = 249;
   GIE = 1;

   TIME = 0;
   RUN = 0;
   REACT = 0;

   while (1)
   {
      // TIME = TIME + 1;

      LCD_Move(0, 9); // Move to row #1, column #10
      LCD_Out(GPS_calcDistanceFargo(gps.latitude * 100000.0, gps.longitude * 100000.0, targetLat, targetLong), 5, 1); // 1 decimal place, 4 digits

      LCD_Move(1, 7); // Move to row #2, column #10
      LCD_Out(TIME, 7, 3); // 1 decimal place, 4 digits

      Wait_ms(100); // Wait 100ms (approx)
   }
}