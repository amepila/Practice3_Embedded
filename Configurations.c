/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Configurations.h"
#include "Menus.h"

typedef enum{
		ASCII_SPACE = 32,	ASCII_EXCLAMATION,	ASCII_QUOTATION,	ASCII_SHARP,		ASCII_DOLLAR,
		ASCII_PERCENT,		ASCII_AMPERSON,		ASCII_APOS,			ASCII_PAROPEN,		ASCII_PARCLOSE,
		ASCII_AST,			ASCII_PLUS,			ASCII_COMMA,		ASCII_MINUS,		ASCII_POINT,
		ASCII_DIAG,			ASCII_0,			ASCII_1,			ASCII_2,			ASCII_3,
		ASCII_4,			ASCII_5,			ASCII_6,			ASCII_7,			ASCII_8,
		ASCII_9,			ASCII_DOUBLEPOINT,	ASCII_COMMAPOINT,	ASCII_MINOR,		ASCII_EQUAL,
		ASCII_MAYOR,		ASCII_QUESTION,		ASCII_COMMAT,		ASCII_A,			ASCII_B,
		ASCII_C,			ASCII_D,			ASCII_E,			ASCII_F,			ASCII_G,
		ASCII_H,			ASCII_I,			ASCII_J,			ASCII_K,			ASCII_L,
		ASCII_M,			ASCII_N,			ASCII_O,			ASCII_P,			ASCII_Q,
		ASCII_R,			ASCII_S,			ASCII_T,			ASCII_U,			ASCII_V,
		ASCII_W,			ASCII_X,			ASCII_Y,			ASCII_Z,			ASCII_CORCHOPEN,
		ASCII_DIAG2,		ASCII_CORCHCLOSE,	ASCII_HAT,			ASCII_LOWBAR,		ASCII_GRAVE,
		ASCII_a,			ASCII_b,			ASCII_c,			ASCII_d,			ASCII_e,
		ASCII_f,			ASCII_g,			ASCII_h,			ASCII_i,			ASCII_j,
		ASCII_k,			ASCII_l,			ASCII_m,			ASCII_n,			ASCII_o,
		ASCII_p,			ASCII_q,			ASCII_r,			ASCII_s,			ASCII_t,
		ASCII_u,			ASCII_v,			ASCII_w,			ASCII_x,			ASCII_y,
		ASCII_z,			ASCII_BRACEOPEN,	ASCII_VERT,			ASCII_BRACECLOSE,	ASCII_TILDE
}ASCII_code;

const uint8 CR = 13;

States_MenuType stateMenu(){

	States_MenuType state = MENU;
	static uint8 flagUART0 = FALSE;
	uint8 optionMenu[2];
	static uint8 counter = 0;

	if(FALSE == flagUART0){flagUART0 = menu_Main();}

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		optionMenu[counter] = getUART0_mailBox();

		if((optionMenu[0] == ASCII_1) && (optionMenu[1] == CR)){state = READ;}
		if((optionMenu[0] == ASCII_2) && (optionMenu[1] == CR)){state = WRITE;}
		if((optionMenu[0] == ASCII_3) && (optionMenu[1] == CR)){state = SET_HOUR;}
		if((optionMenu[0] == ASCII_4) && (optionMenu[1] == CR)){state = SET_DATE;}
		if((optionMenu[0] == ASCII_5) && (optionMenu[1] == CR)){state = FORMAT;}
		if((optionMenu[0] == ASCII_6) && (optionMenu[1] == CR)){state = READ_HOUR;}
		if((optionMenu[0] == ASCII_7) && (optionMenu[1] == CR)){state = READ_DATE;}
		if((optionMenu[0] == ASCII_8) && (optionMenu[1] == CR)){state = TERMINAL2;}
		if((optionMenu[0] == ASCII_9) && (optionMenu[1] == CR)){state = ECO;}

		counter++;
		if(counter > 1){counter = 0;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	return (state);
}

States_MenuType stateRead(){

	States_MenuType state = READ;
	DataIO_Type data;
	static uint8 flagUART0 = FALSE;
	static phaseState = 0;
	uint8 optionMenu[6];


	data.addressRead = 0;
	data.lenght = 0;
	data.dataOut = "DSPs";
	data.error = FALSE;

	if(FALSE == flagUART0){

		flagUART0 = menu_ReadI2C(data.addressRead, data.lenght, data.dataOut, phaseState);
	}

	if(getUART0_flag()){

		if(phaseState == 0){
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, getUART0_mailBox());
			optionMenu[phaseState] = getUART0_mailBox();
/*
			if((optionMenu[0] == ){

			}*/

		}

		phaseState++;
		if(phaseState > 1){phaseState = 0;}

		state =  MENU;
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
