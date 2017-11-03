/*
 *
 *
 */

/***Libraries to the project***/
#include <stdio.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "DatatypeDefinitions.h"
#include "LCDNokia5110.h"
#include "SPI.h"
#include "NVIC.h"
#include "Buttons.h"
#include "MCG.h"
#include "I2C.h"
#include "UART.h"
#include "Menus.h"
#include "Configurations.h"

/**Macros to Clock Frequency**/

/* CLKIN0 frequency */
#define CLK_FREQ_HZ 50000000
/*This is the approximate value for the slow irc*/
#define SLOW_IRC_FREQ 32768
/*This is the approximate value for the fast irc*/
#define FAST_IRC_FREQ 4000000
/*It defines an external clock*/
#define EXTERNAL_CLOCK 0
/**PLL is enabled*/
#define PLL_ENABLE 1
/**PLL is disabled*/
#define PLL_DISABLE 0
/*It defines an crystal oscillator*/
#define CRYSTAL_OSC 1
/* Set the oscillator for low power mode */
#define LOW_POWER 0
/* Set the slow IRC */
#define SLOW_IRC 0
/* Crystal or canned oscillator clock input */
#define CLK0_TYPE 0
/* PLL predivider value */
#define PLL0_PRDIV 25
/* PLL multiplier value*/
#define PLL0_VDIV 30

/** Macros for debugging*/
#define DEBUG
#define PLL_DIRECT_INIT
/*Factor to convert the velocity in float*/
#define CENT			(100U)
/*The modulo of output FTM*/
#define MODULO			(0xFFU)
/*The modulo of input FTM*/
#define MODULO_CAPTURE	(0xFFFFU)
/*Init mode to output FTM*/
#define INIT_MOD		(0.80F)

/**Set of pin of Buttons**/
const Button_ConfigType Buttons_Config[6] = {
							{PORT_C,BIT3},	/**Button 1**/
							{PORT_C,BIT2},	/**Button 2**/
							{PORT_A,BIT2},	/**Button 3**/
							{PORT_B,BIT23},	/**Button 4**/
							{PORT_A,BIT1},	/**Button 5**/
							{PORT_B,BIT9}	/**Button 6**/
};

/**Settings of SPI**/
const SPI_ConfigType SPI_Config={
							SPI_DISABLE_FIFO,	/**Disable fifos of SPI**/
							SPI_LOW_POLARITY,	/**Low Polarity to SPI**/
							SPI_LOW_PHASE,		/**Low Phase to SPI**/
							SPI_MSB,			/**The MSB bit is the first to be transmitted**/
							SPI_0,				/**Channel SPI0**/
							SPI_MASTER,			/**Set like master the SPI**/
							GPIO_MUX2,			/**Set like MUX2 in GPIO**/
							SPI_BAUD_RATE_4,	/**Baudrate to SPI**/
							SPI_FSIZE_8,		/**Size of frame**/
							{GPIO_D,BIT1,BIT2}};/**GPIO for SPI**/

int main(void){

	/**Configuration to enable the clock in 60 MHz**/
	int mcg_clk_hz;
	unsigned char modeMCG = 0;
	mcg_clk_hz = pll_init(CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK, PLL0_PRDIV, PLL0_VDIV, PLL_ENABLE);

	/**First state in the program**/
  	States_MenuType currentState = MENU;
	States_MenuType(*mainFunctions)(uint32);

	/**Configurations of devices**/
	SPI_init(&SPI_Config);
	LCDNokia_init();
	LCDNokia_clear();
	Buttons_init(Buttons_Config);

	/*********************************/
	/***INTERRUPTIONS CONFIGURATION***/
	/*********************************/

	/**Set the reference priority **/
	NVIC_setBASEPRI_threshold(PRIORITY_10);

	/**Set the priority**/
	NVIC_enableInterruptAndPriotity(PORTA_IRQ, PRIORITY_8);
	NVIC_enableInterruptAndPriotity(PORTB_IRQ, PRIORITY_8);
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_8);
	NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(UART1_IRQ, PRIORITY_9);

	/**Configures UART 0 to transmit/receive at 115200 bauds with a 60 MHz of clock core*/
	UART_init (UART_0, 60000000, BD_115200);
	/**Enables the UART 0 interrupt*/
	UART0_interruptEnable(UART_0);
	/**Enable all the interrupts **/

	EnableInterrupts;
    while(1){

    	/**Machine states based on tags**/
    	mainFunctions = StateProgram[currentState].stateFunction;
    	currentState = mainFunctions();
    }
    return 0;
}
