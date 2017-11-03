/*
 * Buttons.c
 *
 *  Created on: Oct 14, 2017
 *      Author: Andres Hernandez
 */

#include "Buttons.h"

/*Flag of buttons*/
static Button_PressedFlag Button_Pressed;

static void Button_clock(Button_PortType port){

	/*Selector of Port*/
	switch(port){
	case PORT_A:
		/**Enable the clock of port A**/
		GPIO_clockGating(GPIO_A);
		break;
	case PORT_B:
		/**Enable the clock of port B**/
		GPIO_clockGating(GPIO_B);
		break;
	case PORT_C:
		/**Enable the clock of port C**/
		GPIO_clockGating(GPIO_C);
		break;
	case PORT_D:
		/**Enable the clock of port D**/
		GPIO_clockGating(GPIO_D);
		break;
	case PORT_E:
		/**Enable the clock of port E**/
		GPIO_clockGating(GPIO_E);
		break;
	default:
		break;
	}
}

static void Button_enable(Button_PortType port, BitsType bit){

	/*The configuration of buttons is Pull Up and Rising edge interrupter*/
	static GPIO_pinControlRegisterType pinControlRegisterPort = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;

	/*Selector of port*/
	switch(port){
	case PORT_A:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_A,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_A,GPIO_INPUT,bit);
		break;
	case PORT_B:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_B,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,bit);
		break;
	case PORT_C:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_C,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,bit);
		break;
	case PORT_D:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_D,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_D,GPIO_INPUT,bit);
		break;
	case PORT_E:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_E,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_E,GPIO_INPUT,bit);
		break;
	default:
		break;
	}
}

void Button_statusFlag(GPIO_portNameType gpio, BitsType bit){

	/*Selector of Port*/
	switch(gpio){
	case GPIO_A:
		if(BIT1 == bit){
			/**Flag Button4 is set**/
			Button_Pressed.flagButton4 = TRUE;
		}
		if(BIT2 == bit){
			/**Flag Button2 is set**/
			Button_Pressed.flagButton2 = TRUE;
		}
		break;
	case GPIO_B:
		if(BIT9 == bit){
			/**Flag Button5 is set**/
			Button_Pressed.flagButton5 = TRUE;
		}
		if(BIT23 == bit){
			/**Flag Button3 is set**/
			Button_Pressed.flagButton3 = TRUE;
		}
		break;
	case GPIO_C:
		if(BIT2 == bit){
			/**Flag Button1 is set**/
			Button_Pressed.flagButton1 = TRUE;
		}
		if(BIT3 == bit){
			/**Flag Button0 is set**/
			Button_Pressed.flagButton0 = TRUE;
		}
		break;
	case GPIO_D:
		break;
	case GPIO_E:
		break;
	default:
		break;
	}
}

uint8 Button_getFlag(Button_Type button){

	/**Selector of buttons**/
	switch(button){
	case BUTTON_0:
		/**Return the status flag Button 0**/
		return (Button_Pressed.flagButton0);
		break;
	case BUTTON_1:
		/**Return the status flag Button 1**/
		return (Button_Pressed.flagButton1);
		break;
	case BUTTON_2:
		/**Return the status flag Button 2**/
		return (Button_Pressed.flagButton2);
		break;
	case BUTTON_3:
		/**Return the status flag Button 3**/
		return (Button_Pressed.flagButton3);
		break;
	case BUTTON_4:
		/**Return the status flag Button 4**/
		return (Button_Pressed.flagButton4);
		break;
	case BUTTON_5:
		/**Return the status flag Button 5**/
		return (Button_Pressed.flagButton5);
		break;
	default:
		return FALSE;
		break;
	}
}

void Button_clearFlag(Button_Type button){

	/*Selector of buttons*/
	switch(button){
	case BUTTON_0:
		/**Clear the flag of button 0**/
		Button_Pressed.flagButton0 = FALSE;
		break;
	case BUTTON_1:
		/**Clear the flag of button 1**/
		Button_Pressed.flagButton1 = FALSE;
		break;
	case BUTTON_2:
		/**Clear the flag of button 2**/
		Button_Pressed.flagButton2 = FALSE;
		break;
	case BUTTON_3:
		/**Clear the flag of button 3**/
		Button_Pressed.flagButton3 = FALSE;
		break;
	case BUTTON_4:
		/**Clear the flag of button 4**/
		Button_Pressed.flagButton4 = FALSE;
		break;
	case BUTTON_5:
		/**Clear the flag of button 5**/
		Button_Pressed.flagButton5 = FALSE;
		break;
	default:
		break;
	}
}

void Buttons_init(const Button_ConfigType* Button_Config){

	/**Number of buttons**/
	uint8 counter;

	/**Loop that set up each button**/
	for(counter = 0; counter < 6; counter++){
		/**Set the clock of button**/
		Button_clock(Button_Config[counter].Button_Port);
		/**Set the button like output**/
		Button_enable(Button_Config[counter].Button_Port, Button_Config[counter].Bit_Type);
	}
}

