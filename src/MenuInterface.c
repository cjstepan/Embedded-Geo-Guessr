#include <pic18.h>
#include "../include/picliquidcrystal.h"
#include "../include/MenuInterface.h"

void menuInit(void)
{
    LCD_writeLine("Welcome to the  ", 0);
    LCD_writeLine("ECE376 GeoGuessr", 1);
    Wait_ms(1000);

    LCD_writeLine("Choose a Spot   ", 0);
    LCD_writeLine("With the Buttons", 1);
    Wait_ms(1000);

    LCD_writeLine("1) ECE Building ", 0);
    LCD_writeLine("2)ChurchillField", 1);
    Wait_ms(1000);

    LCD_writeLine("3) Newman Field ", 0);
    LCD_writeLine("4) Fargodome    ", 1);
    Wait_ms(1000);

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

    LCD_writeLine("Distance:       ", 0);
    Wait_ms(1000);
    LCD_writeLine("Time:           ", 1);
}

void timer2Init(void)
{
    TMR2ON = 1;
    TMR2IE = 1;
    PEIE = 1;
    T2CON = 0x4D;
    PR2 = 249;
    GIE = 1;
}

void timer2ISR(void)
{
    if (TMR2IF) 
	{
		if(!RUN)
		{
			if(RB0) 
			{
				RUN = 1;
			}
			temp = 10;
		}
		if(RUN) 
		{
			TIME += 1;
            if(RB0)
            {
                RUN = 0;
            }
		}
		TMR2IF = 0;
	}
}