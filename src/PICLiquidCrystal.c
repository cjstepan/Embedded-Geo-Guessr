#include <pic18.h>


void Wait_ms(unsigned int ms)
{
	for (unsigned int i = 0; i < ms; i++)
		for (unsigned int j = 0; j < 617; j++)
			;
}

void LCD_Pause(void)
{
	for (int x = 0; x < 20; x++)
		;
}

void LCD_Strobe(void)
{
	RD3 = 0;
	LCD_Pause();
	RD3 = 1;
	LCD_Pause();
	RD3 = 0;
	LCD_Pause();
}
//  write a byte to the LCD in 4 bit mode

void LCD_Inst(unsigned char c)
{
	RD2 = 0; // send an instruction
	PORTD = (PORTD & 0x0F) | (c & 0xF0);
	LCD_Strobe();
	PORTD = (PORTD & 0x0F) | ((c << 4) & 0xF0);
	LCD_Strobe();
	Wait_ms(10);
}

void LCD_Move(unsigned char Row, unsigned char Col)
{
	if (Row == 0)
		LCD_Inst(0x80 + Col);
	if (Row == 1)
		LCD_Inst(0xC0 + Col);
	if (Row == 2)
		LCD_Inst(0x94 + Col);
	if (Row == 3)
		LCD_Inst(0xD4 + Col);
}

void LCD_Write(unsigned char c)
{
	RD2 = 1; // send data
	PORTD = (PORTD & 0x0F) | (c & 0xF0);
	LCD_Strobe();
	PORTD = (PORTD & 0x0F) | ((c << 4) & 0xF0);
	LCD_Strobe();
}

void LCD_writeLine(unsigned char *lineBuffer, unsigned char row)
{
	unsigned char i;
	LCD_Move(row, 0);
	for (i = 0; i < 16; i++)
	{
		LCD_Write(lineBuffer[i]);
	}
}

void LCD_Init(void)
{
	TRISD = 0;
	RD1 = 0;
	LCD_Inst(0x33);
	LCD_Inst(0x32);
	LCD_Inst(0x28);
	LCD_Inst(0x0E);
	LCD_Inst(0x01);
	LCD_Inst(0x06);
	Wait_ms(100);
}

void LCD_Out(long int DATA, unsigned char D, unsigned char N)
{
	unsigned char A[10], i;

	if (DATA < 0)
	{
		LCD_Write('-');
		DATA = -DATA;
	}
	else
		LCD_Write(' ');
	for (i = 0; i < 10; i++)
	{
		A[i] = DATA % 10;
		DATA = DATA / 10;
	}
	for (i = D; i > 0; i--)
	{
		if (i == N)
			LCD_Write('.');
		LCD_Write(A[i - 1] + '0');
	}
}

LCD_Create_Char(int address, int data[])
{
	address = address % 8;

	// send CGRAM address instruction
	LCD_Inst(0x40 | (address << 3));

	for (int i = 0; i < 8; i++)
	{
		LCD_Write(data[i]);
	}
}
