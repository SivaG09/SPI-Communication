#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 20000000
 
void SPI_Slave_Init();
//uint8_t SPI_Read();
 
unsigned char Data; // Global Received Data Variable 
 
void main(void)
{
  SPI_Slave_Init();
  TRISB = 0x00;
  while(1)
  {
    // Dumping Received Data To PORTB To Validate Communication Success!
    PORTB = Data;
  }
  return;
}
 
void SPI_Slave_Init()
{
  SSPM0 = 0;                                   // Set Spi Mode To Slave + SS Enabled
  SSPM1 = 0;
  SSPM2 = 1;
  SSPM3 = 0;
  SSPEN = 1;                                   // Enable The Synchronous Serial Port
  CKP = 0;                                     // Configure The Clock Polarity & Phase (SPI Mode Num. 1)
  CKE = 0;
  SMP = 0;                                     // Clear The SMP Bit
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 1; // SCK -> Intput
  PCFG3 = 0; // Set SS (RA5/AN4) To Be Digital IO
  PCFG2 = 1;
  PCFG1 = 0;
  PCFG0 = 0;
  TRISA5 = 1; // SS -> Input
  // Enable Interrupts. 
  SSPIE = 1;
  PEIE = 1;
  GIE = 1;
}
 
void __interrupt() ISR(void)
{
  if(SSPIF)
  {
    Data = SSPBUF; // Read The Buffer
    SSPIF = 0; // Clear The Interrupt Flag
  }
}


//uint8_t SPI_Read() // Not Recommended Method To Read SPI Data
//{
//  uint8_t Data;
//  if(BF) // Check If Any New Data Is Received
//  {
//    Data = SSPBUF; // Read The Buffer
//    BF = 0; // Clear The Buffer-Filled Indicator Bit
//    SSPIF = 0; // Clear The Interrupt Flag Bit
//    SSPOV = 0; // Clear The Overflow Indicator Bit
//    return Data;
//  }
//}