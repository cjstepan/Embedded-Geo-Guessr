// --------- LCD_PortD.C ---------------------
//
// This routine has LCD driver routines
//
//  LCD_INST:   send an instruction to the LCD
//  LCD_DATA:   send data to the LCD
//  LCD_INIT:   initialize the LCD to 16x4 mode
//
//--- Revision History -----------------
//     5/20/00   JSG
//     9/27/00   Modify LCD_HELLO for Jump messages
//    11/05/00   Clean up LCD routine to use less RAM
//    10/14/14   Modified for new PIC boards

void Wait_ms(unsigned int X)
{
   unsigned int i, j;

   for (i=0; i<X; i++)
      for (j=0; j<617; j++);
}


void LCD_Pause(void)
{
   unsigned char x;
   for (x=0; x<20; x++);
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
   RD2 = 0;        // send an instruction
	PORTD = (PORTD & 0x0F) |  (c & 0xF0);
	LCD_Strobe();
	PORTD = (PORTD & 0x0F) |  ((c<<4) & 0xF0);
	LCD_Strobe();
	Wait_ms(10);
}

void LCD_Move(unsigned char Row, unsigned char Col)
{
   if (Row == 0) LCD_Inst(0x80 + Col);
   if (Row == 1) LCD_Inst(0xC0 + Col);
   if (Row == 2) LCD_Inst(0x94 + Col);
   if (Row == 3) LCD_Inst(0xD4 + Col);
   }
   
void LCD_Write(unsigned char c)
{
   RD2 = 1;        // send data
	PORTD = (PORTD & 0x0F) |  (c & 0xF0);
	LCD_Strobe();
	PORTD = (PORTD & 0x0F) |  ((c<<4)  & 0xF0);
	LCD_Strobe();
	
}

void LCD_writeLine(unsigned char *line, unsigned char row)
{
   unsigned char i;
   LCD_Move(row, 0);
   for (i=0; i<16; i++) 
   {
      LCD_Write(line[i]);
   }
}

void LCD_writeChar(unsigned char c, unsigned char row, unsigned char col)
{
   LCD_Move(row, col);
   LCD_Write(c);
}

void LCD_Clear(void)
{
   LCD_Inst(0x01);
   Wait_ms(10);
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
   
   if(DATA < 0) {
      LCD_Write('-');
      DATA = -DATA;
      }
   else LCD_Write(' ');
   for (i=0; i<10; i++) {
      A[i] = DATA % 10;
      DATA = DATA / 10;
   }
   for (i=D; i>0; i--) {
      if (i == N) LCD_Write('.');
      LCD_Write(A[i-1] + '0');
   }
}

void SCI_Out(long int DATA, unsigned char D, unsigned char N)
{
   unsigned char A[10], i;

   while(!TRMT);   
   if(DATA < 0) {
      TXREG = '-';
      DATA = -DATA;
      }
   else TXREG = ' ';
   for (i=0; i<10; i++) {
      A[i] = DATA % 10;
      DATA = DATA / 10;
      }
   for (i=D; i>0; i--) {
      if (i == N) { while(!TRMT); TXREG = '.'; }
      while(!TRMT);  TXREG = A[i-1] + 48;
      }
}


void SCI_CRLF(void)
{
   while(!TRMT);  TXREG = 13;
   while(!TRMT);  TXREG = 10;
}
