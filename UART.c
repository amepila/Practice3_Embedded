/*
 * UART.c
 *
 *  Created on: Oct 10, 2017
 *      Author: Andres Hernandez
 */
#include "UART.h"
#include "GPIO.h"
#define S 65000

/*Global variable that saves the info*/
UART_MailBoxType UART0_MailBox;

/*Table of BRFD. Range 0:32 */
const float BRFD_Type[32] = {
		(0),	(0.03125),	(0.0625),	(0.09375),	(0.125),	(0.15625),	(0.1875),	(0.21875),
		(0.25), (0.28125),	(0.3125),	(0.34375),	(0.375),	(0.40625),	(0.4375),	(0.46875),
		(0.5),	(0.53125),	(0.5625),	(0.59375),	(0.625),	(0.65625),	(0.6875),	(0.71875),
		(0.75),	(0.78125),	(0.8125),	(0.84375),	(0.875),	(0.90625),	(0.9375),	(0.96875)
};

void UART0_RX_TX_IRQHandler (void){
	/*First is verified if the serial port finished to transmit*/
	if((UART0->S1 & UART_S1_RDRF_MASK)){
		/*The info is saved in Data Register*/
		UART0_MailBox.mailBox = UART0->D;
		/*There are new data*/
		UART0_MailBox.flag = 1;
	}
}

uint8 getUART0_mailBox(){
	/*Return the value of mailbox*/
	return (UART0_MailBox.mailBox);
}

uint8 getUART0_flag(){
	/*Return the value of the flag of mailbox*/
	return (UART0_MailBox.flag);
}

void setUART0_mailBox(uint8 character){
	/*Assigns the character into the mailbox*/
	UART0_MailBox.mailBox = character;
}

void setUART0_flag(uint8 status){
	/*Changes the value of the flag of mailbox*/
	UART0_MailBox.flag = status;
}


void delay(uint32 delay){
	volatile uint32 counter;

	for(counter=delay;counter>0;counter--){
	}
}

void UART_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate){

	/*Variable saves the original value of Baud Rate*/
	float sBR;
	/*Variables saves the Baud Rate by parts*/
	uint32 sbr_HIGH;
	uint32 sbr_LOW;
	/*Variable saves the complete Baud Rate*/
	uint32 sbr_TEMP;
	/*Variables saves the Baud Rate Fine Adjust*/
	uint8 BRFA;
	/*Variables saves the difference of BaudRate expected and the original*/
	float diff;
	/*Counter that advances between BRFD values*/
	uint8 counter;
	/*Variable saves the BRFD value
	 *its default value is 1 for practicality
	 */
	float BRFD = 1;


	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	PORTB->PCR[16] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	PORTB->PCR[17] = PORT_PCR_MUX(3);

	/*Isolate the baudRate variable
	 * 115200 = 21000000/((SBR+BRFD)*16)
	 * sBR+BRFD = 11.3932
	 * Assume BRFD = 0
	 * sBR = 11.3932 - BRFD
	 * sBR = 11.3932 - 0.375
	 * BRFD = 0.375 = 12/32 = 01100
	 * 115384 BR
	 */

	/*Isolate the SBR value assuming that BRFD = 0*/
	sBR = ((float)systemClk/(baudRate*16));
	/*Obtains the value of the float difference of SBR*/
	diff = (sBR - (uint32)sBR);

	/*Loop that verifies with the BRFD values*/
	for(counter = 0; counter < 32; counter++){

		/*Verifies that the difference is between some value of BRFA*/
		if((diff > BRFD_Type[counter-1]) && (diff < BRFD_Type[counter+1])){
			/*Verifies that the BRFD is the value with smaller difference*/
			if(BRFD > BRFD_Type[counter]){
				/*Saves the values of BRFD and BRFA*/
				BRFD = BRFD_Type[counter];
				BRFA = counter;
			}
		}
		/*Special case with the initial value of BRFA*/
		if(counter == 0){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff >= BRFD_Type[counter]) && (diff < BRFD_Type[counter+1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
		/*Special case with the final value of BRFA*/
		if(counter == 31){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff <= BRFD_Type[counter]) && (diff > BRFD_Type[counter-1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
	}

	/*Assigns the indicated BRFA*/
	//BRFA = 0x0C;

	/*Considers only the integer part*/
	sbr_TEMP = (uint32)sBR;
	/*Saves the High part of Baud Rate*/
	sbr_HIGH = sbr_TEMP & UART_SBR_MASK_HIGH;
	/*Saves the Low part of Baud Rate*/
	sbr_LOW = sbr_TEMP & UART_SBR_MASK_LOW;


	/*Enable the clock of UART 0 */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	/*Disable the Rx of UART*/
	UART0->C2 &= ~(UART_C2_RE_MASK);
	/*Disable the Tx of UART*/
	UART0->C2 &= ~(UART_C2_TE_MASK);


	/*Clear the High part of SBR*/
	UART0->BDH &= ~(UART_CLEAR_BDH);
	/*Send the High part of SBR*/
	UART0->BDH |= sbr_HIGH;
	/*Clear the Low part of SBR*/
	UART0->BDL &= ~(UART_CLEAR_BDL);
	/*Send the Low part of SBR*/
	UART0->BDL |= sbr_LOW;


	/*Send the Baud Rate Fine Adjust to register*/
	UART0->C4 |= BRFA;
	/*Enable Rx of UART*/
	UART0->C2 |= (UART_C2_RE_MASK);
	/*Enable Tx of UART*/
	UART0->C2 |= (UART_C2_TE_MASK);

}

void UART0_interruptEnable(UART_ChannelType uartChannel){
	/*Verifies if the data is complete*/
	if(!(UART0->S1 & UART_S1_RDRF_MASK)){
		/*Enable the interrupter of reception*/
		UART0->C2 |= UART_C2_RIE_MASK;
	}
}

void UART_putChar (UART_ChannelType uartChannel, uint8 character){
	/*Check if there isn't data transmission*/
	if((UART0->S1 & UART_S1_TC_MASK)){
		/*Send character to Data Register*/
		UART0->D |= character;
		delay(S);
	}
}

void UART_putString(UART_ChannelType uartChannel, sint8* string){
	/*Counter that verifies each position of the array*/
	uint8 counter = 0;
	/*Check if there isn't data transmission*/
	if((UART0->S1 & UART_S1_TC_MASK)){
		/*Transmit the data until find the NULL value*/
		while(string[counter] != '\0'){
			/*Each character of string is send to Data Register*/
			UART0->D |= string[counter];
			/*Move to next position in the array*/
			counter++;
			delay(S);
		}
	}
}
