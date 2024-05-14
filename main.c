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
//definting macros
#define UP RB0
#define Down RB1
#define Send RB2
// Functions Declarations
void SPI_Master_Init();
void SPI_Write(uint8_t);

//main function
void main(void)
{
  SPI_Master_Init(); // Initialize The SPI in Master Mode @ Fosc/64 SCK
  uint8_t Data = 0; // The Data Byte
  TRISB = 0x07; // RB0, RB1 & RB2 Are Input Pins (Push Buttons)
  TRISD = 0x00; // Output Port (4-Pins)
  PORTD = 0x00; // Initially OFF
  while(1)
  {
    if (UP) // Increment The Data Value
    {
      Data++;
      __delay_ms(250);
    }
    if (Down) // Decrement The Data Value
    {
      Data--;
      __delay_ms(250);
    }
    if (Send) // Send The Current Data Value Via SPI
    {
      SPI_Write(Data);
      __delay_ms(250);
    }
    PORTD = Data; // Display The Current Data Value at PORTD
  }
  return;
}

void SPI_Master_Init()
{
  // Set Spi Mode To Master + Set SCK Rate To Fosc/64
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 0;
  SSPM3 = 0;
  SSPEN = 1;                      // Enable The Synchronous Serial Port
  CKP = 0;                        // Configure The Clock Polarity & Phase (SPI Mode Num. 1)
  CKE = 0;
  SMP = 0;                        // Configure The Sampling Time (Let's make it at middle
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 0; // SCK -> Output
}
 
void SPI_Write(uint8_t Data)
{
  SSPBUF = Data; // Transfer The Data To The Buffer Register
}
