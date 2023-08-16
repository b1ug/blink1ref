/**
 * Simple WS2812 driver using USART in SPI mode but not using DMA
 * Turns off interrupts during sending to strip.
 * 
 * Uses concept from: https://jeelabs.org/book/1450d/
 *
 * 2018, Tod E. Kurt / https://todbot.com/blog
 *
 **/

#ifndef WS2812_SPI_H
#define WS2812_SPI_H

#include "color_types.h"

#define BOARD_TYPE_BLINK1MK3 1
#define BOARD_TYPE_TOMU 2
#define BOARD_TYPE_EFM32HGDEVKIT 3

#if BOARD_TYPE == BOARD_TYPE_BLINK1MK3
#pragma message("Board Type BLINK1MK3")
// --- blink1mk3
#define USART0_LOCATION USART_ROUTE_LOCATION_LOC4
#define USART0_TXPORT   gpioPortB
#define USART0_TXPIN    7
#define USART0_CLKPORT  gpioPortB // but ununsed here
#define USART0_CLKPIN   13        // but ununsed here

#elif BOARD_TYPE == BOARD_TYPE_TOMU
#pragma message("Board Type TOMU")
// --- tomu board
#define USART0_LOCATION USART_ROUTE_LOCATION_LOC3
#define USART0_TXPORT   gpioPortE
#define USART0_TXPIN    13        // CAP1B on tomu board
#define USART0_CLKPORT  gpioPortC // but ununsed here
#define USART0_CLKPIN   15        // but ununsed here

#elif BOARD_TYPE == BOARD_TYPE_EFM32HGDEVKIT
#pragma message("Board Type EFM32HGDEVKIT")
// --- efm32hg dev board
#define USART0_LOCATION USART_ROUTE_LOCATION_LOC0
#define USART0_TXPORT   gpioPortE
#define USART0_TXPIN    10
#define USART0_CLKPORT  gpioPortE
#define USART0_CLKPIN   12

#else
#error "BOARD_TYPE_... not defined"
#endif


/**********************************************************************
 * @brief  Setup USART0 SPI as Master
 **********************************************************************/
static void ws2812_setupSpi(void)
{
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;  
  
  // Initialize SPI 
  usartInit.databits = usartDatabits12;
  //usartInit.baudrate = 2400000; // 2.4MHz
  usartInit.baudrate = 3000000;  // 3.0 MHz for SK6812mini (works for WS2812 too apparently)
  usartInit.msbf = true;

  USART_InitSync(USART0, &usartInit);
  
  // Enable SPI transmit and receive 
  USART_Enable(USART0, usartEnable);
  
  // Configure GPIO pins for SPI
  //GPIO_PinModeSet(USART0_CLKPORT, USART0_CLOCKPIN  gpioModePushPull, 0); // CLK // don't need clock
  GPIO_PinModeSet(USART0_TXPORT, USART0_TXPIN, gpioModePushPull, 0); // MOSI 
 
  // Route USART clock and USART TX to LOC0 (PortE12, PortE10)
  USART0->ROUTE = USART0_LOCATION |
                  USART_ROUTE_TXPEN;
                  //USART_ROUTE_CLKPEN  // don't need clock pin
}

// Convert nibble to WS2812 bitstream
// @ 2.4MHz, 3 SPI bits for each ws2812 bit
// ws2812 0 bit = 0b100 
// ws2812 1 bit = 0b110
// => 12 bits carry 4 ws2812 bits
// To send one ws2812 byte, send two 12-bit transfers
// concept from: https://jeelabs.org/book/1450d/
static const uint16_t bits[] =
  {
    0b100100100100, // => 0b0000 in ws2812 bits
    0b100100100110, // => 0b0001 in ws2812 bits
    0b100100110100, // => 0b0010 in ws2812 bits
    0b100100110110, // => 0b0011 in ws2812 bits
    0b100110100100, // => 0b0100 in ws2812 bits
    0b100110100110, // => 0b0101 in ws2812 bits
    0b100110110100, // => 0b0110 in ws2812 bits
    0b100110110110, // => 0b0111 in ws2812 bits
    0b110100100100, // => 0b1000 in ws2812 bits
    0b110100100110, // => 0b1001 in ws2812 bits
    0b110100110100, // => 0b1010 in ws2812 bits
    0b110100110110, // => 0b1011 in ws2812 bits
    0b110110100100, // => 0b1100 in ws2812 bits
    0b110110100110, // => 0b1101 in ws2812 bits
    0b110110110100, // => 0b1110 in ws2812 bits
    0b110110110110, // => 0b1111 in ws2812 bits
  };


// note double-wide with TxDouble, sending 12-bit words
#define ws2812_spiSend(x) USART_TxDouble( USART0, x)

/**********************************************************************
 *
 **********************************************************************/
static inline void ws2812_sendByte (int value)
{
    ws2812_spiSend( bits[value >> 4] );
    ws2812_spiSend( bits[value & 0xF] );
}

/**********************************************************************
 * @brief Send LED data out via SPI, disables interrupts
 **********************************************************************/
static void ws2812_sendLEDs(rgb_t* leds, int num)
{
  CORE_irqState_t is = CORE_EnterCritical();
  for( int i=0; i<num; i++ ) {
    // send out GRB data
    ws2812_sendByte( leds[i].g );
    ws2812_sendByte( leds[i].r );
    ws2812_sendByte( leds[i].b );
  }
  CORE_ExitCritical(is);
  // delay at least 50usec before sending again
}

#endif
