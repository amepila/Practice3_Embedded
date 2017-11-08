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
		{stateFinalReadI2C}
};

const StatePtrWrite_Type statesWriteI2C[4] =
{
		{stateAddressWrite},
		{stateDataWrite},
		{stateFinalWrite},
		{stateFinalWriteI2C}
};

const StatePtrSetHour_Type statesSetHour[3] =
{
		{stateSetTime},
		{stateSaveTime},
		{stateFinalSetHour}
};

const StatePtrSetDate_Type statesSetDate[3] =
{
		{stateSetCalendar},
		{stateSaveDate},
		{stateFinalSetDate}
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

	dataState1.phaseState = 0;
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

	dataState2.phaseState = 1;
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
	UART_putString(UART_0, "Hola mundo\t");

	dataState3.phaseState = 3;
	dataState3.stateMain = READ;

	return (dataState3);

}
StateReadI2C_Type stateFinalReadI2C(StateReadI2C_Type data){

	static StateReadI2C_Type dataState4;

	clearUART0_mailbox();
	dataState4.stateMain = MENU;

	return (dataState4);
}

/*************************************************************/
StateWriteI2C_Type stateAddressWrite(StateWriteI2C_Type data){

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
	dataWrite1.phaseState = 0;
	if(getUART0_mailBox() == CR){

		dataWrite1.realAddress = Convert_numberASCIItoDATA(dataWrite1.inputAddress);

		dataWrite1.phaseState = 1;
		counterWrite1 = 0;
		clearUART0_mailbox();
	}
	dataWrite1.stateMain = WRITE;
	return (dataWrite1);
}

StateWriteI2C_Type stateDataWrite(StateWriteI2C_Type data){

	static StateWriteI2C_Type dataWrite2;
	static uint32 counterWrite2 = 0;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}

	dataWrite2.inputData[counterWrite2] = getUART0_mailBox();
	counterWrite2++;
	dataWrite2.phaseState = 1;

	if(getUART0_mailBox() == CR){

		dataWrite2.sizeData = counterWrite2;
		dataWrite2.phaseState = 2;
		counterWrite2 = 0;
		clearUART0_mailbox();
	}
	dataWrite2.stateMain = WRITE;
	return (dataWrite2);
}

StateWriteI2C_Type stateFinalWrite(StateWriteI2C_Type data){

	static StateWriteI2C_Type dataWrite3;
	uint32 counterSave;
	uint32 counterAddress = 0;


	for(counterSave = data.sizeData; counterSave != 0; counterSave--){
		//Function to write in memory in I2C
		counterAddress++;
	}
	dataWrite3.phaseState = 3;
	dataWrite3.stateMain = WRITE;

	return (dataWrite3);
}

StateWriteI2C_Type stateFinalWriteI2C(StateWriteI2C_Type data){

	static StateWriteI2C_Type dataWrite4;

	clearUART0_mailbox();
	dataWrite4.stateMain = MENU;

	return (dataWrite4);
}


/**********************************************************/
StateSetHour_Type stateSetTime(StateSetHour_Type data){

	static StateSetHour_Type dataSet1;
	static uint32 counterHour = 0;
	static uint32 counterMin = 0;
	static uint32 counterSec = 0;
	static FIFO_Type fifoTime;
	static FlagHour_Type flagType = HOUR;

	if(FORMAT_12H == data.format){

		if(getUART0_mailBox() != CR){
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, getUART0_mailBox());
		}
		/****************Set Hour***************************/
		if((counterHour < 2) && (HOUR == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterHour++;
			dataSet1.phaseState = 0;
		}
		if((counterHour == 2) && (HOUR == flagType)){
			counterHour = 0;
			dataSet1.phaseState = 0;
		}
		/****************Set Minutes***********************/
		if((counterMin < 2) && (MINUTES == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterMin++;
			dataSet1.phaseState = 0;
		}
		if((counterMin == 2) && (MINUTES == flagType)){
			counterMin = 0;
			dataSet1.phaseState = 0;
		}
		/******************Set Seconds*********************/
		if((counterSec < 2) && (SECONDS == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterSec++;
			dataSet1.phaseState = 0;
		}
		if((counterSec == 2) && (SECONDS == flagType)){
			counterSec = 0;
			dataSet1.phaseState = 0;
		}
		/******************Detector CR********************/
		if(getUART0_mailBox() == CR){

			if(HOUR == flagType){
				counterHour = 0;
				fifoTime = popFIFO_0();
				dataSet1.phaseState = 0;
				dataSet1.time.hour = Convert_numberASCIItoDATA(fifoTime.data);
				clearFIFO_0();
				clearUART0_mailbox();
				UART_putChar(UART_0, ASCII_DOUBLEPOINT);
			}
			if(MINUTES == flagType){
				counterMin = 0;
				fifoTime = popFIFO_0();
				dataSet1.phaseState = 0;
				dataSet1.time.minutes = Convert_numberASCIItoDATA(fifoTime.data);
				clearFIFO_0();
				clearUART0_mailbox();
				UART_putChar(UART_0, ASCII_DOUBLEPOINT);
			}
			if(SECONDS == flagType){
				counterSec = 0;
				fifoTime = popFIFO_0();
				dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
				dataSet1.phaseState = 1;
				clearFIFO_0();
				clearUART0_mailbox();
			}
			flagType++;
			if(flagType > 2){flagType = HOUR;}
		}
	}
	if(FORMAT_24H == data.format){

		if(getUART0_mailBox() != CR){
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, getUART0_mailBox());
		}
		/****************Set Hour***************************/
		if((counterHour < 2) && (HOUR == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterHour++;
		}
		if((counterHour == 2) && (HOUR == flagType)){
			counterHour = 0;
		}
		/****************Set Minutes***********************/
		if((counterMin < 2) && (MINUTES == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterMin++;
		}
		if((counterMin == 2) && (MINUTES == flagType)){
			counterMin = 0;
		}
		/******************Set Seconds*********************/
		if((counterSec < 2) && (SECONDS == flagType)){
			pushFIFO_0(getUART0_mailBox());
			counterSec++;
		}
		if((counterSec == 2) && (SECONDS == flagType)){
			counterSec = 0;
		}
		/******************Detector CR********************/
		if(getUART0_mailBox() == CR){

			if(HOUR == flagType){
				counterHour = 0;
				fifoTime = popFIFO_0();
				dataSet1.phaseState = 0;
				dataSet1.time.hour = Convert_numberASCIItoDATA(fifoTime.data);
				clearFIFO_0();
				clearUART0_mailbox();
				UART_putChar(UART_0, ASCII_DOUBLEPOINT);
			}
			if(MINUTES == flagType){
				counterMin = 0;
				fifoTime = popFIFO_0();
				dataSet1.phaseState = 0;
				dataSet1.time.minutes = Convert_numberASCIItoDATA(fifoTime.data);
				clearFIFO_0();
				clearUART0_mailbox();
				UART_putChar(UART_0, ASCII_DOUBLEPOINT);
			}
			if(SECONDS == flagType){
				counterSec = 0;
				fifoTime = popFIFO_0();
				dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
				dataSet1.phaseState = 1;
				clearFIFO_0();
				clearUART0_mailbox();
			}
			flagType++;
			if(flagType > 2){flagType = HOUR;}
		}
	}

	dataSet1.stateMain = SET_HOUR;
	return (dataSet1);
}

StateSetHour_Type stateSaveTime(StateSetHour_Type data){

	static StateSetHour_Type dataSet2;

	//Function to pass the data to the RTC

	dataSet2.stateMain = SET_HOUR;
	dataSet2.phaseState = 2;

	return (dataSet2);
}

StateSetHour_Type stateFinalSetHour(StateSetHour_Type data){

	static StateSetHour_Type dataSet3;

	dataSet3.stateMain = MENU;
	return (dataSet3);
}
/**************************************************************/
StateSetDate_Type stateSetCalendar(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate1;
	static uint32 counterYear = 0;
	static uint32 counterMonth = 0;
	static uint32 counterDay = 0;
	static FIFO_Type fifoTime;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/****************Set Year***************************/
	if((counterYear < 2) && (TRUE == data.flagYear)){
		pushFIFO_0(getUART0_mailBox());
		counterYear++;
	}
	if((counterYear == 2) && (TRUE == data.flagYear)){
		counterYear = 0;
	}
	/****************Set Month***********************/
	if((counterMonth < 2) && (TRUE == data.flagMonth)){
		pushFIFO_0(getUART0_mailBox());
		counterMonth++;
	}
	if((counterMonth == 2) && (TRUE == data.flagMonth)){
		counterMonth = 0;
	}
	/******************Set Days*********************/
	if((counterDay < 2) && (TRUE == data.flagDay)){
		pushFIFO_0(getUART0_mailBox());
		counterDay++;
	}
	if((counterDay == 2) && (TRUE == data.flagDay)){
		counterDay = 0;
	}
	/******************Detector CR********************/
	if(getUART0_mailBox() == CR){
		if(data.flagYear){
			fifoTime = popFIFO_0();
			dataSetDate1.time.year = Convert_numberASCIItoDATA(fifoTime.data);
			clearFIFO_0();
			clearUART0_mailbox();
			UART_putChar(UART_0, ASCII_DIAG);
		}
		if(data.flagMonth){
			fifoTime = popFIFO_0();
			dataSetDate1.time.month = Convert_numberASCIItoDATA(fifoTime.data);
			clearFIFO_0();
			clearUART0_mailbox();
			UART_putChar(UART_0, ASCII_DIAG);
		}
		if(data.flagDay){
			fifoTime = popFIFO_0();
			dataSetDate1.time.day = Convert_numberASCIItoDATA(fifoTime.data);
			dataSetDate1.phaseState = 1;
			clearFIFO_0();
			clearUART0_mailbox();
		}
	}

	dataSetDate1.stateMain = SET_DATE;
	return (dataSetDate1);
}

StateSetDate_Type stateSaveDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate2;

	//Function to pass the data to the RTC

	dataSetDate2.stateMain = SET_DATE;
	dataSetDate2.phaseState = 2;

	return (dataSetDate2);
}

StateSetDate_Type stateFinalSetDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate3;

	dataSetDate3.stateMain = MENU;
	return (dataSetDate3);
}


/********************************************************/
/********************************************************/
States_MenuType stateMenu(Time_Type realTime){

	States_MenuType state = MENU;
	static uint32 flagUART0 = FALSE;
	FIFO_Type fifoMenu;

	if(FALSE == flagUART0){flagUART0 = menu_Main();}

	if(getUART0_flag()){
		if(getUART0_mailBox() != CR){
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, getUART0_mailBox());
		}

		/**Saves the input data in an array of 2 spaces**/
		pushFIFO_0(getUART0_mailBox());

		if(getUART0_mailBox() == CR){

			fifoMenu = popFIFO_0();

			if(fifoMenu.data[0] == ASCII_1){state = READ;}
			if(fifoMenu.data[0] == ASCII_2){state = WRITE;}
			if(fifoMenu.data[0] == ASCII_3){state = SET_HOUR;}
			if(fifoMenu.data[0] == ASCII_4){state = SET_DATE;}
			if(fifoMenu.data[0] == ASCII_5){state = FORMAT;}
			if(fifoMenu.data[0] == ASCII_6){state = READ_HOUR;}
			if(fifoMenu.data[0] == ASCII_7){state = READ_DATE;}
			if(fifoMenu.data[0] == ASCII_8){state = TERMINAL2;}
			if(fifoMenu.data[0] == ASCII_9){state = ECO;}

			fifoMenu.stateFIFO = clearFIFO_0();
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	if((state != MENU) && (EMPTY == fifoMenu.stateFIFO)){
		clearUART0_mailbox();
		flagUART0 = FALSE;
	}
	return (state);
}

States_MenuType stateRead(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateReadI2C_Type stateRead;
	static StateReadI2C_Type dataMemory;
	static uint32 flagAddress;
	StateReadI2C_Type(*readFunctions)(StateReadI2C_Type);
	stateRead.stateMain = READ;

	if(FALSE == flagUART0){
		flagUART0 = menu_ReadI2C(phase);
	}

	if(phase == 1){
		if(FALSE == flagAddress){
			dataMemory.realAddress = stateRead.realAddress;
			flagAddress = TRUE;
		}
	}
	if(phase == 2){
		dataMemory.realLenght = stateRead.realLenght;
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(dataMemory);
	}
	if(getUART0_flag()){
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(stateRead);

		if(phase == 3){
			stateRead.phaseState = 0;
			flagUART0 = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateRead.phaseState;

	return (stateRead.stateMain);
}

States_MenuType stateWrite(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateWriteI2C_Type stateWrite;
	static StateWriteI2C_Type dataMemoryWrite;
	static uint32 flagAddress;
	StateWriteI2C_Type(*writeFunctions)(StateWriteI2C_Type);
	stateWrite.stateMain = WRITE;
	uint32 counterChar;

	if(FALSE == flagUART0){
		flagUART0 = menu_WriteI2C(phase);
	}

	if(phase == 1){
		if(FALSE == flagAddress){
			dataMemoryWrite.realAddress = stateWrite.realAddress;}
			flagAddress = TRUE;
		}
	if(phase == 2){
		dataMemoryWrite.sizeData = stateWrite.sizeData;
		for(counterChar = 0; counterChar < dataMemoryWrite.sizeData; counterChar++){
			dataMemoryWrite.inputData[counterChar] = stateWrite.inputData[counterChar];
		}
		writeFunctions = statesWriteI2C[phase].StateWriteI2C;
		stateWrite = writeFunctions(dataMemoryWrite);
	}

	if(getUART0_flag()){
		writeFunctions = statesWriteI2C[phase].StateWriteI2C;
		stateWrite = writeFunctions(stateWrite);

		if(phase == 3){
			stateWrite.phaseState = 0;
			flagUART0 = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateWrite.phaseState;

	return (stateWrite.stateMain);
}

States_MenuType stateSetHour(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static uint32 flagInit_Format;
	static StateSetHour_Type state_SetHour;
	static StateSetHour_Type dataMemory_SetHour;
	StateSetHour_Type(*setHourFunctions)(StateSetHour_Type);
	state_SetHour.stateMain = SET_HOUR;


	if(FALSE == flagUART0){
		flagUART0 = menu_SetHour(phase);
	}
	if(phase == 0){
		if(FALSE == flagInit_Format){
			clearUART0_mailbox();
			state_SetHour.format = realTime.hour.format;
			flagInit_Format = TRUE;
		}
	}
	if(phase == 1){
		dataMemory_SetHour.time = state_SetHour.time;
		setHourFunctions = statesSetHour[phase].StateSetHour;
		state_SetHour = setHourFunctions(dataMemory_SetHour);
	}

	if(getUART0_flag()){
		setHourFunctions = statesSetHour[phase].StateSetHour;
		state_SetHour = setHourFunctions(state_SetHour);

		if(phase == 2){
			state_SetHour.phaseState = 0;
			flagUART0 = FALSE;
			flagInit_Format = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = state_SetHour.phaseState;
	return (state_SetHour.stateMain);
}

States_MenuType stateSetDate(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateSetDate_Type state_SetDate;
	static StateSetDate_Type dataMemory_SetDate;
	StateSetDate_Type(*setDateFunctions)(StateSetDate_Type);
	state_SetDate.stateMain = SET_DATE;

	if(FALSE == flagUART0){
		flagUART0 = menu_SetDate(phase);
	}
	if(phase == 1){
		dataMemory_SetDate.time = state_SetDate.time;
		setDateFunctions = statesSetDate[phase].StateSetDate;
		state_SetDate = setDateFunctions(dataMemory_SetDate);
	}

	if(getUART0_flag()){
		setDateFunctions = statesSetDate[phase].StateSetDate;
		state_SetDate = setDateFunctions(state_SetDate);

		if(phase == 2){
			state_SetDate.phaseState = 0;
			flagUART0 = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = state_SetDate.phaseState;
	return (state_SetDate.stateMain);
}

States_MenuType stateFormat(Time_Type realTime){

	States_MenuType state = FORMAT;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateReadHour(Time_Type realTime){

	States_MenuType state = READ_HOUR;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateReadDate(Time_Type realTime){

	States_MenuType state = READ_DATE;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateTerminal2(Time_Type realTime){

	States_MenuType state = TERMINAL2;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateEco(Time_Type realTime){

	States_MenuType state = ECO;

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}
