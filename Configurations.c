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

const uint8 ESC = 27;
const uint8 CR = 13;
static uint32 FormatClock;
static Time_Type Clock;


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

const StatePtrFormat_Type statesFormat[4] =
{
		{stateShowFormat},
		{stateChange},
		{stateSaveFormat},
		{stateFinalFormat}
};

const StatePtrReadHour_Type statesReadHour[2] =
{
		{stateReadTime},
		{stateFinalRH}
};

const StatePtrReadDate_Type statesReadDate[2] = {
		{stateReadCalendar},
		{stateFinalRD}
};

const StatePtrEco_Type statesEco[2] = {
		{stateTransmitEco},
		{stateFinalEco}
};
/********************************************************************/
/********************************************************************/
void setTimeLCD(Time_Type time){
	if(TRUE == time.modifyTime){
		setRTC_sec((uint8)time.hour.seconds);
		setRTC_min((uint8)time.hour.minutes);
		setRTC_hour((uint8)time.hour.hour);

		time.modifyTime= FALSE;
	}
	if(TRUE == time.modifyDate){
		setRTC_day((uint8)time.date.day);
		setRTC_month((uint8)time.date.month);
		setRTC_year((uint8)time.date.year);

		time.modifyDate = FALSE;
	}
}

Time_Type getTime(void){
	Time_Type realTime;

	realTime.hour.hour = readRTC_hour();
	realTime.hour.minutes = readRTC_min();
	realTime.hour.seconds = readRTC_sec();
	realTime.hour.format = Clock.hour.format;
	realTime.hour.period = Clock.hour.period;

	realTime.date.year = readRTC_year();
	realTime.date.month = readRTC_month();
	realTime.date.day = readRTC_day();

	return (realTime);
}

void printTimeLCD(Time_Type time){

	uint32 tmpHour;
	static uint32 lockFormat;

	/******************HOUR****************************/
	if(FORMAT_24H == time.hour.format){

		LCDNokia_gotoXY(15,2);
		LCDNokia_printValue(BCDHDec(readRTC_hour()));
		LCDNokia_printValue(BCDUni(readRTC_hour()));
		LCDNokia_sendChar(ASCII_DOUBLEPOINT);
	}
	if(FORMAT_12H == time.hour.format){
		if(readRTC_hour() > (0x12)){

			tmpHour = readRTC_hour();
			tmpHour -= (0x12);

			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(BCDHDec(tmpHour));
			LCDNokia_printValue(BCDUni(tmpHour));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);
		}
		if(readRTC_hour() == (0x12)){
			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(0);
			LCDNokia_printValue(0);
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);
		}
	}

	LCDNokia_gotoXY(35,2);
	LCDNokia_printValue(BCDDec(readRTC_min()));
	LCDNokia_printValue(BCDUni(readRTC_min()));
	LCDNokia_sendChar(ASCII_DOUBLEPOINT);

	LCDNokia_gotoXY(55,2);
	LCDNokia_printValue(BCDDec(readRTC_sec()));
	LCDNokia_printValue(BCDUni(readRTC_sec()));

	/*******************DATE***************************/
	LCDNokia_gotoXY(15,4);
	LCDNokia_printValue(BCDDayDec(readRTC_day()));
	LCDNokia_printValue(BCDUni(readRTC_day()));
	LCDNokia_sendChar(ASCII_DIAG);

	LCDNokia_gotoXY(35,4);
	LCDNokia_printValue(BCDMonthDec(readRTC_month()));
	LCDNokia_printValue(BCDUni(readRTC_month()));
	LCDNokia_sendChar(ASCII_DIAG);

	LCDNokia_gotoXY(55,4);
	LCDNokia_printValue(BCDYearDec(readRTC_year()));
	LCDNokia_printValue(BCDUni(readRTC_year()));

	if(FORMAT_12H == time.hour.format){
		if(readRTC_hour() > (0x12)){
			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('P');
			LCDNokia_sendChar('M');
		}else{
			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('A');
			LCDNokia_sendChar('M');
		}
	}
}
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
	uint32 counterChar;

	for(counterChar = 0; counterChar < data.realLenght; counterChar++){
		UART_putChar(UART_0,readMemory((data.realAddress + counterChar)));
		E2PROMdelay(65000);
	}

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
		writeMemory((data.realAddress + counterAddress),data.inputData[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
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
	static uint32 counterTime = 0;
	static FIFO_Type fifoTime;
	static FlagHour_Type flagType = HOUR;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/****************Set Hour***************************/
	if(counterTime < 2){
		pushFIFO_0(getUART0_mailBox());
		counterTime++;
		dataSet1.phaseState = 0;
	}
	if(counterTime == 2){
		counterTime = 0;
		dataSet1.phaseState = 0;
	}
	/******************Detector CR********************/
	if(getUART0_mailBox() == CR){
		counterTime = 0;
		fifoTime = popFIFO_0();
		dataSet1.phaseState = 0;

		if(HOUR == flagType){
			dataSet1.time.hour = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DOUBLEPOINT);
		}
		if(MINUTES == flagType){
			dataSet1.time.minutes = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DOUBLEPOINT);
		}
		if((SECONDS == flagType) && (FORMAT_24H == data.time.format)){
			dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
			dataSet1.phaseState = 1;
		}
		if((SECONDS == flagType) && (FORMAT_12H == data.time.format)){
			dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
			dataSet1.phaseState = 0;
		}
		if((PERIOD == flagType) && (FORMAT_12H == data.time.format)){

			if(('A' == fifoTime.data[0]) || ('a' == fifoTime.data[0])){
				dataSet1.time.period = PERIOD_AM;
				dataSet1.phaseState = 1;
			}
			if(('P' == fifoTime.data[0]) || ('p' == fifoTime.data[0])){
				dataSet1.time.period = PERIOD_PM;
				dataSet1.phaseState = 1;
			}
		}
		clearFIFO_0();
		clearUART0_mailbox();
		flagType++;
		if(flagType > 3){flagType = HOUR;}
	}
	dataSet1.time.format = data.time.format;
	dataSet1.stateMain = SET_HOUR;
	return (dataSet1);
}

StateSetHour_Type stateSaveTime(StateSetHour_Type data){

	static StateSetHour_Type dataSet2;
	Time_Type time;

	time.hour.period = data.time.period;
	time.hour.hour = data.time.hour;
	time.hour.minutes = data.time.minutes;
	time.hour.seconds = data.time.seconds;
	time.modifyTime = TRUE;
	time.hour.format = data.time.format;

	setTimeLCD(time);

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
	static uint32 counterDate = 0;
	static FIFO_Type fifoTime;
	static FlagDate_Type flagType= YEAR;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/****************Set Date***************************/
	if(counterDate < 2){
		pushFIFO_0(getUART0_mailBox());
		counterDate++;
		dataSetDate1.phaseState = 0;
	}
	if(counterDate== 2){
		counterDate = 0;
		dataSetDate1.phaseState = 0;
	}
	/******************Detector CR********************/
	if(getUART0_mailBox() == CR){
		counterDate = 0;
		fifoTime = popFIFO_0();
		dataSetDate1.phaseState = 0;

		if(YEAR == flagType){
			dataSetDate1.time.year = Convert_numberASCIItoDATA(fifoTime.data);
		}
		if(MONTH == flagType){
			dataSetDate1.time.month = Convert_numberASCIItoDATA(fifoTime.data);
		}
		if(DAY == flagType){
			dataSetDate1.time.day = Convert_numberASCIItoDATA(fifoTime.data);
			dataSetDate1.phaseState = 1;
		}
		clearFIFO_0();
		clearUART0_mailbox();
		UART_putChar(UART_0, ASCII_DIAG);

		flagType++;
		if(flagType > 2){flagType = YEAR;}
	}

	dataSetDate1.stateMain = SET_DATE;
	return (dataSetDate1);
}

StateSetDate_Type stateSaveDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate2;
	Time_Type time;

	time.date.day = data.time.day;
	time.date.month = data.time.month;
	time.date.year = data.time.year;
	time.modifyDate = TRUE;

	setTimeLCD(time);

	dataSetDate2.stateMain = SET_DATE;
	dataSetDate2.phaseState = 2;

	return (dataSetDate2);
}

StateSetDate_Type stateFinalSetDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate3;

	dataSetDate3.stateMain = MENU;
	return (dataSetDate3);
}
/*****************************************************************/
StateFormat_Type stateShowFormat(StateFormat_Type data){

	static StateFormat_Type dataFormat1;

	if(FORMAT_12H == data.format){
		UART_putString(UART_0,"12h");
		dataFormat1.format = FORMAT_12H;
	}
	if(FORMAT_24H == data.format){
		UART_putString(UART_0,"24h");
		dataFormat1.format = FORMAT_24H;
	}

	dataFormat1.stateMain = FORMAT;
	dataFormat1.phaseState = 1;

	return (dataFormat1);
}

StateFormat_Type stateChange(StateFormat_Type data){

	static StateFormat_Type dataFormat2;

	if(FORMAT_12H == data.format){
		UART_putString(UART_0,"24h ");
	}
	if(FORMAT_24H == data.format){
		UART_putString(UART_0,"12h ");
	}

	dataFormat2.format = data.format;
	dataFormat2.stateMain = FORMAT;
	dataFormat2.phaseState = 2;

	return (dataFormat2);
}

StateFormat_Type stateSaveFormat(StateFormat_Type data){

	static StateFormat_Type dataFormat3;
	static FIFO_Type fifo;
	static uint32 counterYes = 0;
	static uint32 counterNo = 0;

	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}

	pushFIFO_0(getUART0_mailBox());
	dataFormat3.phaseState = 2;

	if(getUART0_mailBox() == CR){
		fifo =  popFIFO_0();
		if(('S' == fifo.data[0]) || ('s' == fifo.data[0])){  counterYes++;}
		if(('I' == fifo.data[1]) || ('i' == fifo.data[1])){  counterYes++;}
		if(('N' == fifo.data[0]) || ('n' == fifo.data[0])){  counterNo++;}
		if(('O' == fifo.data[1]) || ('o' == fifo.data[1])){  counterNo++;}
		dataFormat3.phaseState = 3;
	}

	if(counterYes == 2){
		counterYes = 0;
		counterNo = 0;
		clearFIFO_0();

		if(FORMAT_12H == data.format){
			dataFormat3.format = FORMAT_24H;
		}
		if(FORMAT_24H == data.format){
			dataFormat3.format = FORMAT_12H;
		}
	}
	if(counterNo == 2){
		counterYes = 0;
		counterNo = 0;
		clearFIFO_0();

		if(FORMAT_12H == data.format){
			dataFormat3.format = FORMAT_12H;
		}
		if(FORMAT_24H == data.format){
			dataFormat3.format = FORMAT_24H;
		}
	}
	dataFormat3.stateMain = FORMAT;
	return (dataFormat3);
}

StateFormat_Type stateFinalFormat(StateFormat_Type data){

	static StateFormat_Type dataFormat4;

	dataFormat4.stateMain = MENU;

	return (dataFormat4);
}

/**********************************************************/
StateReadHour_Type stateReadTime(StateReadHour_Type data){

	static StateReadHour_Type dataReadTime1;
	static uint32 hourPartD;
	static uint32 minPartD;
	static uint32 secPartD;
	static uint32 hourPartU;
	static uint32 minPartU;
	static uint32 secPartU;
	static uint8 time[6];

	hourPartD = BCDHDec(readRTC_hour());
	minPartD = BCDHDec(readRTC_min());
	secPartD = BCDHDec(readRTC_sec());
	hourPartU = BCDUni(readRTC_hour());
	minPartU = BCDUni(readRTC_min());
	secPartU = BCDUni(readRTC_sec());

	if(getUART0_mailBox() != CR){
/*
		hourPartU %= 10;
		hourPartD /= 10;

		minPartU %= 10;
		minPartD /= 10;

		secPartU %= 10;
		secPartD /= 10;
*/
		time[0] = hourPartU + 48;
		time[1] = hourPartD + 48;
		time[2] = minPartU + 48;
		time[3] = minPartD + 48;
		time[4] = secPartU + 48;
		time[5] = secPartD + 48;

		if(FALSE == data.flagBlock){

			UART_putString(UART_0,"\033[14;10H");
			UART_putChar(UART_0, time[0]);
			UART_putChar(UART_0, time[1]);

			UART_putChar(UART_0,ASCII_DOUBLEPOINT);
			UART_putChar(UART_0, time[2]);
			UART_putChar(UART_0, time[3]);

			UART_putChar(UART_0,ASCII_DOUBLEPOINT);
			UART_putChar(UART_0, time[4]);
			UART_putChar(UART_0, time[5]);

			data.flagBlock = TRUE;
		}
		dataReadTime1.phaseState = 0;
	}
	if(getUART0_mailBox() == CR){
		dataReadTime1.phaseState = 0;
	}

	dataReadTime1.stateMain = READ_HOUR;
	return dataReadTime1;
}

StateReadHour_Type stateFinalRH(StateReadHour_Type data){

	static StateReadHour_Type dataReadTime2;

	dataReadTime2.stateMain = MENU;
	return dataReadTime2;
}
/**********************************************************/
StateReadDate_Type stateReadCalendar(StateReadDate_Type data){

	static StateReadDate_Type dataReadCal1;
	static uint32 dayrPartD;
	static uint32 monthPartD;
	static uint32 yearPartD;
	static uint32 dayPartU;
	static uint32 monthPartU;
	static uint32 yearPartU;
	static uint8 date[6];

	//dayPartD =

	if(getUART0_mailBox() != CR){



		dataReadCal1.phaseState = 0;
	}
	if(getUART0_mailBox() == CR){
		dataReadCal1.phaseState = 0;
	}

	dataReadCal1.stateMain = READ_HOUR;
	return dataReadCal1;
}

StateReadDate_Type stateFinalRD(StateReadDate_Type data){

	static StateReadDate_Type dataReadCal2;

	dataReadCal2.stateMain = MENU;
	return dataReadCal2;

}

/*************************************************************/
StateEco_Type stateTransmitEco(StateEco_Type data){

	static StateEco_Type dataEco1;

	if(getUART0_mailBox() != ESC){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
		LCDNokia_sendChar(getUART0_mailBox());
	}

	//LCDNokia_gotoXY(15,2);
	pushFIFO_0(getUART0_mailBox());
	dataEco1.phaseState = 0;

	if(getUART0_mailBox() == ESC){
		dataEco1.phaseState = 1;
		clearFIFO_0();
	}

	dataEco1.stateMain = ECO;
	return (dataEco1);
}

StateEco_Type stateFinalEco(StateEco_Type data){

	static StateEco_Type dataEco2;

	dataEco2.stateMain = MENU;
	return (dataEco2);
}

/********************************************************/
/********************************************************/
States_MenuType stateMenu(Time_Type realTime){

	States_MenuType state = MENU;
	static uint32 flagUART0 = FALSE;
	static uint32 lockRTC = FALSE;
	FIFO_Type fifoMenu;

	if(FALSE == lockRTC){
		Clock = realTime;
		lockRTC = TRUE;
	}
	printTimeLCD(Clock);

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

	printTimeLCD(Clock);

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

	printTimeLCD(Clock);

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

	printTimeLCD(Clock);

	if(FALSE == flagUART0){
		flagUART0 = menu_SetHour(phase);
	}
	if(phase == 0){
		if(FALSE == flagInit_Format){
			clearUART0_mailbox();
			state_SetHour.time.format = Clock.hour.format;
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
	static uint32 flagInit_Format;
	static StateSetDate_Type state_SetDate;
	static StateSetDate_Type dataMemory_SetDate;
	StateSetDate_Type(*setDateFunctions)(StateSetDate_Type);
	state_SetDate.stateMain = SET_DATE;

	printTimeLCD(Clock);

	if(FALSE == flagUART0){
		flagUART0 = menu_SetDate(phase);
	}
	if(phase == 0){
		if(FALSE == flagInit_Format){
			clearUART0_mailbox();
			flagInit_Format = TRUE;
		}
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
			flagInit_Format = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = state_SetDate.phaseState;
	return (state_SetDate.stateMain);
}

States_MenuType stateFormat(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static uint32 flagPrint = FALSE;
	static StateFormat_Type stateFormat;
	static StateFormat_Type dataMemory;
	StateFormat_Type(*formatFunctions)(StateFormat_Type);
	stateFormat.stateMain = FORMAT;

	printTimeLCD(Clock);

	if(FALSE == flagUART0){
		flagUART0 = menu_FormatHour(phase);
	}

	if(phase == 0){
		clearUART0_mailbox();
		dataMemory.format = Clock.hour.format;
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);
	}
	if(phase == 1){
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);
	}
	if(getUART0_flag()){
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);

		if(stateFormat.phaseState == 3){
			Clock.hour.format = stateFormat.format;
			printTimeLCD(Clock);
		}
		if(phase == 4){
			stateFormat.phaseState = 0;
			flagUART0 = FALSE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateFormat.phaseState;
	return (stateFormat.stateMain);
}

States_MenuType stateReadHour(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateReadHour_Type stateReadHour;
	StateReadHour_Type(*readHourFunctions)(StateReadHour_Type);
	stateReadHour.stateMain = READ_HOUR;

	printTimeLCD(Clock);
	stateReadHour.hour = realTime.hour.hour;
	stateReadHour.minutes = realTime.hour.minutes;
	stateReadHour.seconds = realTime.hour.seconds;

	if(FALSE == flagUART0){
		flagUART0 = menu_ReadHour(phase);
	}
	if(phase == 0){
		readHourFunctions = statesReadHour[phase].StateReadHour;
		stateReadHour = readHourFunctions(stateReadHour);
	}
	if(phase == 1){
		stateReadHour.phaseState = 0;
		flagUART0 = FALSE;
	}
	if(getUART0_flag()){
		readHourFunctions = statesReadHour[phase].StateReadHour;
		stateReadHour = readHourFunctions(stateReadHour);

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	phase = stateReadHour.phaseState;
	return (stateReadHour.stateMain);
}

States_MenuType stateReadDate(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateReadDate_Type stateReadDate;
	StateReadDate_Type(*readDateFunctions)(StateReadDate_Type);
	stateReadDate.stateMain = READ_DATE;

	printTimeLCD(Clock);
	stateReadDate.day = realTime.date.day;
	stateReadDate.month = realTime.date.month;
	stateReadDate.year = realTime.date.year;

	if(FALSE == flagUART0){
		flagUART0 = menu_ReadDate(phase);
	}
	if(phase == 0){
		readDateFunctions = statesReadDate[phase].StateReadDate;
		stateReadDate = readDateFunctions(stateReadDate);
	}
	if(phase == 1){
		stateReadDate.phaseState = 0;
		flagUART0 = FALSE;
	}
	if(getUART0_flag()){
		readDateFunctions = statesReadDate[phase].StateReadDate;
		stateReadDate = readDateFunctions(stateReadDate);

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	phase = stateReadDate.phaseState;
	return (stateReadDate.stateMain);
}

States_MenuType stateTerminal2(Time_Type realTime){

	States_MenuType state = TERMINAL2;

	printTimeLCD(Clock);

	if(getUART0_flag()){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return state;
}

States_MenuType stateEco(Time_Type realTime){

	static uint32 phase = 0;
	static uint32 flagUART0 = FALSE;
	static StateEco_Type stateEco;
	StateEco_Type(*ecoFunctions)(StateEco_Type);
	stateEco.stateMain = ECO;

	if(FALSE == flagUART0){
		flagUART0 = menu_EcoLCD(phase);
	}

	if(getUART0_flag()){
		ecoFunctions = statesEco[phase].StateEco;
		stateEco = ecoFunctions(stateEco);

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateEco.phaseState;
	return (stateEco.stateMain);
}

