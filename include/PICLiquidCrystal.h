#ifndef PICLIQUIDCRYSTAL_H
#define PICLIQUIDCRYSTAL_H


void Wait_ms(unsigned int ms);

void LCD_Pause(void);

void LCD_Strobe(void);

void LCD_Inst(unsigned char c);

void LCD_Move(unsigned char Row, unsigned char Col);

void LCD_Write(unsigned char c);

void LCD_writeLine(unsigned char *lineBuffer, unsigned char row);

void LCD_writeChar(unsigned char c, unsigned char row, unsigned char col);

void LCD_Clear(void);

void LCD_Init(void);

void LCD_Out(long int DATA, unsigned char D, unsigned char N);

LCD_Create_Char(int address, int data[]);

void SCI_Out(long int DATA, unsigned char D, unsigned char N);

void SCI_CRLF(void);




#endif /* PICLIQUIDCRYSTAL_H */