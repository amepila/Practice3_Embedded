/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Configurations.h"
#include "Menus.h"


States_MenuType stateMenu(){//Complete

	States_MenuType state = MENU;
	static uint8 flagUART0 = FALSE;

	if(FALSE == flagUART0){
		flagUART0 = menu_Main();
	}

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
		/**clear the reception flag*/
		setUART0_flag(FALSE);

		if(getUART0_mailBox() == 49){state = READ;}
		if(getUART0_mailBox() == 50){state = WRITE;}
		if(getUART0_mailBox() == 51){state = SET_HOUR;}
		if(getUART0_mailBox() == 52){state = SET_DATE;}
		if(getUART0_mailBox() == 53){state = FORMAT;}
		if(getUART0_mailBox() == 54){state = READ_HOUR;}
		if(getUART0_mailBox() == 55){state = READ_DATE;}
		if(getUART0_mailBox() == 56){state = TERMINAL2;}
		if(getUART0_mailBox() == 57){state = ECO;}
	}
	return (state);
}

States_MenuType stateRead(){

	States_MenuType state = READ;
	DataIO_Type data;
	static uint8 flagUART0 = FALSE;

	data.adressRead = 123;
	data.lenght = 8;
	data.dataOut = "DSPs";

	if(FALSE == flagUART0){
		flagUART0 = menu_ReadI2C(&data);
	}

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateWrite(){

	States_MenuType state = WRITE;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateSetHour(){

	States_MenuType state = SET_HOUR;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateSetDate(){

	States_MenuType state = SET_DATE;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateFormat(){

	States_MenuType state = FORMAT;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateReadHour(){

	States_MenuType state = READ_HOUR;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateReadDate(){

	States_MenuType state = READ_DATE;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateTerminal2(){

	States_MenuType state = TERMINAL2;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateEco(){

	States_MenuType state = ECO;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}
