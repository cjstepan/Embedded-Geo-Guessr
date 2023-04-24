// main.c
// A starting point for this project

#include <pic18.h>

#include "lcd_portd.c"

const unsigned char MSG0[16] = "Hello world!    ";

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

   LCD_Init(); // initialize the LCD

   LCD_Move(0, 0);
   for (i = 0; i < 16; i++)
   {
      LCD_Write(MSG0[i]);
   }

   TIME = 0;

   while (1)
   {
      TIME = TIME + 1;

      LCD_Move(1, 0); // Move to row #1, column #1

      LCD_Out(TIME, 5, 1); // 1 decimal place, 5 digits

      Wait_ms(100); // Wait 100ms (approx)
   }
}