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
/******************States********************************************/
/********************************************************************/
const StatePtrRead_Type statesReadI2C[4] =
{
		{stateAddress},
		{stateLenght},
		{stateData},
		{stateFinal}
};

const StatePtrWrite_Type statesWriteI2C[3] =
{
		{stateAddressWrite},
		{stateDataWrite},
		{stateFinalWrite}
};

/********************************************************************/
/********************************************************************/
StateReadI2C_Type stateAddress(StateReadI2C_Type data){

	static uint32 counter1 = 0;
	static StateReadI2C_Type dataState1;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	if((counter1 > 1) && (counter1 < 8)){
		dataState1.inputAddress[counter1-2] = getUART0_mailBox();
	}
	counter1++;

	if(getUART0_mailBox() == CR){

		dataState1.realAddress = Convert_numberASCIItoDATA(dataState1.inputAddress);

		dataState1.phaseState = 1;
		counter1 = 0;
		clearUART0_mailbox();
	}
	dataState1.stateMain = READ;
	return (dataState1);
}
StateReadI2C_Type stateLenght(StateReadI2C_Type data){

	static uint32 counter2 = 0;
	static StateReadI2C_Type dataState2;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}

	dataState2.inputLenght[counter2] = getUART0_mailBox();
	counter2++;

	if(getUART0_mailBox() == CR){

		dataState2.realLenght = Convert_numberASCIItoDATA(dataState2.inputLenght);

		dataState2.phaseState = 2;
		counter2 = 0;
		clearUART0_mailbox();
	}
	dataState2.stateMain = READ;
	return (dataState2);
}

StateReadI2C_Type stateData(StateReadI2C_Type data){

	static StateReadI2C_Type dataState3;
	//uint8 data[5] = {'h', 'o', 'l', 'a'};

	/**IMPORTANT PART**/
	//Function to extract from memory
	//Print the data
	//Put the function to allocate with address and lenght
	//data.AddressRead and data.lenght
	//Function to decode the data

	//UART_putString(UART_0, &data);

	dataState3.phaseState = 3;
	dataState3.stateMain = READ;

	return (dataState3);

}
StateReadI2C_Type stateFinal(StateReadI2C_Type data){
	static StateReadI2C_Type dataState4;

	clearUART0_mailbox();
	dataState4.stateMain = MENU;

	return (dataState4);
}

/*************************************************************/
StateWriteI2C_Type stateAddressWrite(){

	static StateWriteI2C_Type dataWrite1;
	static uint32 counterWrite1 = 0;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	if((counterWrite1 > 1) && (counterWrite1 < 8)){
		dataWrite1.inputAddress[counterWrite1 - 2] = getUART0_mailBox();
	}
	counterWrite1++;

	if(getUART0_mailBox() == CR){

		dataWrite1.realAddress = Convert_numberASCIItoDATA(dataWrite1.inputAddress);

		dataWrite1.phaseState = 1;
		counterWrite1 = 0;
		clearUART0_mailbox();
	}
	dataWrite1.stateMain = WRITE;
	return (dataWrite1);
}

StateWriteI2C_Type stateDataWrite(){

	static StateWriteI2C_Type dataWrite2;
	static uint32 counterWrite2 = 0;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}

	dataWrite2.inputData[counterWrite2] = getUART0_mailBox();
	counterWrite2++;

	if(getUART0_mailBox() == CR){

		dataWrite2.phaseState = 2;
		counterWrite2 = 0;
		clearUART0_mailbox();
	}
	dataWrite2.stateMain = WRITE;
	return (dataWrite2);
}

StateWriteI2C_Type stateFinalWrite(){

}


/********************************************************/
/********************************************************/
States_MenuType stateMenu(){

	States_MenuType state = MENU;
	static uint32 flagUART0 = FALSE;
	static uint32 counter = 0;
	uint8 optionMenu[2];

	if(FALSE == flagUART0){flagUART0 = menu_Main();}

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
		/**Saves the input data in an array of 2 spaces**/
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

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateReadI2C_Type stateRead;
	static StateReadI2C_Type dataMemory;
	StateReadI2C_Type(*readFunctions)(StateReadI2C_Type);
	stateRead.stateMain = READ;

	if(FALSE == flagUART0){
		flagUART0 = menu_ReadI2C(phase);
	}

	if(phase == 1){ dataMemory.realAddress = stateRead.realAddress;}
	if(phase == 2){
		dataMemory.realLenght = stateRead.realLenght;
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(dataMemory);
	}

	if(getUART0_flag()){
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(stateRead);

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateRead.phaseState;

	return (stateRead.stateMain);
}

States_MenuType stateWrite(){

	static uint32 phase = 0;
	static flagUART0 = FALSE;
	static StateWriteI2C_Type stateWrite;
	StateWriteI2C_Type(*writeFunctions)(void);
	stateWrite.stateMain = WRITE;

	if(FALSE == flagUART0){
		flagUART0 = menu_WriteI2C(phase);
	}

	if(getUART0_flag()){
		writeFunctions = statesWriteI2C[phase].StateWriteI2C;
		stateWrite = writeFunctions();

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	phase = stateWrite.phaseState;

	return (stateWrite.stateMain);
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
