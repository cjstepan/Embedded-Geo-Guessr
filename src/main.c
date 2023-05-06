/**
 * @file main.c
 * @brief Main file for the project
 * @details This file contains the main function for the project. It initializes
 * the LCD, serial port, and timer2. It also contains the high-priority interrupt
 * service routine.
 *
 * @date 2023-05-05
 *
 * @authors Simon K., Calvin S.
 */
#include <pic18.h>
#include "../include/picliquidcrystal.h"
#include "../include/SerialGPS.h"
#include "../include/MenuInterface.h"

// High-priority service
void interrupt IntServe(void)
{
   timer2ISR();
   serialISR();
}

void main(void)
{

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   LCD_Init();
   Serial_Init();
   menuInit(); // Initialize the menu
   timer2Init();

   while (1)
   {
      GPS_parseData();
      LCD_Move(0, 9);
      long distance = GPS_calcDistanceFargo(gps.latitude, gps.longitude, targetLat, targetLong);
      LCD_Out(distance, 5, 0);

      LCD_Move(1, 7);
      LCD_Out(TIME, 7, 3);

      if (distance < 10 && gps.isValid)
      {
         RUN = 0; // stop timer

         LCD_Move(0, 9);
         LCD_Out(0, 5, 0);

         LCD_Move(1, 7);
         LCD_Out(TIME, 7, 3);

         while(1);
      }

      Wait_ms(100);
   }
}