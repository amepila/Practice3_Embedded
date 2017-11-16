/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Configurations.h"
#include "Menus.h"

/**ASCII table such as reference**/

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

/**ASCII Code to Esc and CR*/
const uint8 ESC = 27;
const uint8 CR = 13;
const uint32 Medium_Hour = (0x12U);
/**Structure with the time*/
static Time_Type Clock;
/**Flag to detect the buttons*/
static uint32 FlagButton = FALSE;
/**Error message to RTC*/
const uint8 MessageError[] = "ERROR!";


/******************SUB STATES********************************************/
/*********************************************************************/
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
/**************************GENERAL FUNCTIONS OF TIME*****************/
/********************************************************************/
void setTimeLCD(Time_Type time){
	/**Set the hour*/
	if(TRUE == time.modifyTime){
		setRTC_sec((uint8)time.hour.seconds);
		setRTC_min((uint8)time.hour.minutes);
		setRTC_hour((uint8)time.hour.hour);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyTime= FALSE;

	}
	/**Set the date*/
	if(TRUE == time.modifyDate){
		setRTC_day((uint8)time.date.day);
		setRTC_month((uint8)time.date.month);
		setRTC_year((uint8)time.date.year);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyDate = FALSE;
	}
}

Time_Type getTime(void){

	Time_Type realTime;
	/**Separate the digits of hour*/
	uint32 decHour;
	uint32 unHour;
	uint32 decMin;
	uint32 unMin;
	uint32 decSec;
	uint32 unSec;
	/**Separate the digits of date*/
	uint32 decYear;
	uint32 unYear;
	uint32 decMonth;
	uint32 unMonth;
	uint32 decDay;
	uint32 unDay;
	/**Converts from BCD*/
	decHour = BCDHDec(readRTC_hour());
	unHour = BCDUni(readRTC_hour());
	decMin = BCDDec(readRTC_min());
	unMin = BCDUni(readRTC_min());
	decSec = BCDDec(readRTC_sec());
	unSec = BCDUni(readRTC_sec());
	/**Converts from BCD*/
	decYear = BCDYearDec(readRTC_year());
	unYear = BCDUni(readRTC_year());
	decMonth = BCDMonthDec(readRTC_month());
	unMonth = BCDUni(readRTC_month());
	decDay = BCDDayDec(readRTC_day());
	unDay = BCDUni(readRTC_day());

	/**The second digit is multiplied by 10*/
	decHour *= 10;
	decMin *= 10;
	decSec *= 10;
	decYear *= 10;
	decMonth *= 10;
	decDay *= 10;
	/**The pair of digits are summed and saved into realTime*/
	realTime.hour.hour = decHour + unHour;
	realTime.hour.minutes = decMin + unMin;
	realTime.hour.seconds = decSec + unSec;
	realTime.hour.format = Clock.hour.format;
	realTime.hour.period = Clock.hour.period;

	realTime.date.year = decYear + unYear;
	realTime.date.month = decMonth + unMonth;
	realTime.date.day = decDay + unDay;

	return (realTime);
}

void printTimeLCD(Time_Type time){

	uint32 tmpHour;
	/******************HOUR****************************/
	if(FORMAT_24H == Clock.hour.format){
		/*Print the hour converted from BCD**/
		LCDNokia_gotoXY(15,2);
		delay(6500);
		LCDNokia_printValue(BCDHDec(readRTC_hour()));
		LCDNokia_printValue(BCDUni(readRTC_hour()));
		LCDNokia_sendChar(ASCII_DOUBLEPOINT);

		LCDNokia_gotoXY(35,2);
		LCDNokia_printValue(BCDDec(readRTC_min()));
		LCDNokia_printValue(BCDUni(readRTC_min()));
		LCDNokia_sendChar(ASCII_DOUBLEPOINT);

		LCDNokia_gotoXY(55,2);
		LCDNokia_printValue(BCDDec(readRTC_sec()));
		LCDNokia_printValue(BCDUni(readRTC_sec()));
		/**Clear the space of AM PM*/
		LCDNokia_gotoXY(35,3);
		LCDNokia_sendChar(' ');
		LCDNokia_sendChar(' ');

	}
	if(FORMAT_12H == Clock.hour.format){
		/**If the value is more than 12 then is rested 12*/
		if(readRTC_hour() > (Medium_Hour)){

			tmpHour = readRTC_hour();
			tmpHour -= (Medium_Hour);

			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(BCDHDec(tmpHour));
			LCDNokia_printValue(BCDUni(tmpHour));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('P');
			LCDNokia_sendChar('M');
		}
		/**If the value is 12 then put PM*/
		if(readRTC_hour() == (Medium_Hour)){
			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(1);
			LCDNokia_printValue(2);
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('P');
			LCDNokia_sendChar('M');
		}
		/**If the value of RTC is less than 12 then put AM*/
		if(readRTC_hour() < (Medium_Hour)){
			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(BCDHDec(readRTC_hour()));
			LCDNokia_printValue(BCDUni(readRTC_hour()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('A');
			LCDNokia_sendChar('M');
		}
	}

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
}

void printHourUART(Time_Type time){

	uint32 partDecHour;
	uint32 partUnHour;
	uint32 partDecMinutes;
	uint32 partUnMinutes;
	uint32 partDecSeconds;
	uint32 partUnSeconds;
	static uint8 date[6];

	/**To print the value is separated in two digits*/
	partDecHour = time.hour.hour / 10;
	partUnHour = time.hour.hour % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecHour){
		date[0] = '0';
	}else{
		date[0] = '0' + partDecHour;
	}
	/**The second digit is converted and saved*/
	date[1] = '0' + partUnHour;

	partDecMinutes = time.hour.minutes / 10;
	partUnMinutes = time.hour.minutes % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecMinutes){
		date[2] = '0';
	}else{
		date[2] = '0' + partDecMinutes;
	}
	/**The second digit is converted and saved*/
	date[3] = '0' + partUnMinutes;

	partDecSeconds = time.hour.seconds / 10;
	partUnSeconds = time.hour.seconds % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecSeconds){
		date[4] = '0';
	}else{
		date[4] = '0' + partDecSeconds;
	}
	/**The second digit is converted and saved*/
	date[5] = '0' + partUnSeconds;

	/**Verified if the data is different that zero*/
	if('0' == date[0]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[0]);}

	/**Verified if the data is different that zero*/
	if(0 == date[1]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[1]);}
	UART_putChar(UART_0, ASCII_DOUBLEPOINT);

	/**Verified if the data is different that zero*/
	if(0 == date[2]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[2]);}

	/**Verified if the data is different that zero*/
	if(0 == date[3]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[3]);}
	UART_putChar(UART_0, ASCII_DOUBLEPOINT);

	/**Verified if the data is different that zero*/
	if(0 == date[4]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[4]);}

	/**Verified if the data is different that zero*/
	if(0 == date[5]){UART_putChar(UART_0, ASCII_0);}
	else{UART_putChar(UART_0, date[5]);}

	UART_putString(UART_0,"\033[10;36H");
}

void printDateUART(Time_Type time){

	uint32 partDecDay;
	uint32 partUnDay;
	uint32 partDecMonth;
	uint32 partUnMonth;
	uint32 partDecYear;
	uint32 partUnYear;
	uint8 date[6];

	/**To print the value is separated in two digits*/
	partDecDay = time.date.day / 10;
	partUnDay = time.date.day % 10;
	/**The pair of digits is converted and saved*/
	date[0] = '0' + partDecDay;
	date[1] = '0' + partUnDay;

	partDecMonth = time.date.month / 10;
	partUnMonth = time.date.month % 10;
	/**The pair of digits is converted and saved*/
	date[2] = '0' + partDecMonth;
	date[3] = '0' + partUnMonth;

	if(time.date.year > 1000){	partDecYear = (time.date.year - 2000)/ 10;}
	if(time.date.year < 1000){  partDecYear = (time.date.year / 10);}
	partUnYear = time.date.year % 10;
	/**The pair of digits is converted and saved*/
	date[4] = '0' + partDecYear;
	date[5] = '0' + partUnYear;

	/**Print the date*/
	UART_putChar(UART_0, date[0]);
	UART_putChar(UART_0, date[1]);
	UART_putChar(UART_0, ASCII_DIAG);
	UART_putChar(UART_0, date[2]);
	UART_putChar(UART_0, date[3]);
	UART_putChar(UART_0, ASCII_DIAG);
	UART_putChar(UART_0, date[4]);
	UART_putChar(UART_0, date[5]);
}

/*********************SUB FUNCTIONS***********************************/
/********************************************************************/
StateReadI2C_Type stateAddress(StateReadI2C_Type data){

	static uint32 counter1 = 0;
	static StateReadI2C_Type dataState1;

	/**Verifies is mailbox is different that CR*/
	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/**Counts starting third element*/
	if((counter1 > 1) && (counter1 < 8)){
		/**Save the address*/
		dataState1.inputAddress[counter1-2] = getUART0_mailBox();
	}
	counter1++;

	dataState1.phaseState = 0;
	/**Finalize the transmission of characters*/
	if(getUART0_mailBox() == CR){
		/**Convert the data*/
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

	/**Verifies is mailbox is different that CR*/
	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/**Save the size*/
	dataState2.inputLenght[counter2] = getUART0_mailBox();
	counter2++;

	dataState2.phaseState = 1;
	/**Finalize the transmission*/
	if(getUART0_mailBox() == CR){
		/**Convert the data*/
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

	/**Print the value of each address of memomry*/
	for(counterChar = 0; counterChar < data.realLenght; counterChar++){
		/**Print in the UART0*/
		UART_putChar(UART_0,readMemory((data.realAddress + counterChar)));
		E2PROMdelay(65000);
	}
	/**Continue*/
	dataState3.phaseState = 3;
	dataState3.stateMain = READ;

	return (dataState3);

}
StateReadI2C_Type stateFinalReadI2C(StateReadI2C_Type data){

	static StateReadI2C_Type dataState4;
	/**Wait for the CR to continue*/
	clearUART0_mailbox();
	dataState4.stateMain = MENU;

	return (dataState4);
}

/*************************************************************/
StateWriteI2C_Type stateAddressWrite(StateWriteI2C_Type data){

	static StateWriteI2C_Type dataWrite1;
	static uint32 counterWrite1 = 0;

	/**Verifies is mailbox is different that CR*/
	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/**Save the address*/
	if((counterWrite1 > 1) && (counterWrite1 < 8)){
		dataWrite1.inputAddress[counterWrite1 - 2] = getUART0_mailBox();
	}
	counterWrite1++;
	dataWrite1.phaseState = 0;
	/**Finalize the transmission*/
	if(getUART0_mailBox() == CR){
		/**Convert the data*/
		dataWrite1.realAddress = Convert_numberASCIItoDATA(dataWrite1.inputAddress);
		/**Clean and continue*/
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

	/**Verifies is mailbox is different that CR*/
	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/**Save the input data*/
	dataWrite2.inputData[counterWrite2] = getUART0_mailBox();
	counterWrite2++;
	dataWrite2.phaseState = 1;

	/**Finalize the transmission*/
	if(getUART0_mailBox() == CR){
		/**Clean and continue*/
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

	/**Assign the input data into Memory*/
	for(counterSave = data.sizeData; counterSave != 0; counterSave--){
		writeMemory((data.realAddress + counterAddress),data.inputData[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
	}
	/**Continue with the next phase*/
	dataWrite3.phaseState = 3;
	dataWrite3.stateMain = WRITE;

	return (dataWrite3);
}

StateWriteI2C_Type stateFinalWriteI2C(StateWriteI2C_Type data){

	static StateWriteI2C_Type dataWrite4;
	/**Wait for CR to continue*/
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
	static uint32 lockSetHour = FALSE;
	static uint32 flagButton = FALSE;
	static Time_Type time;

	/**Get the time into variable only once*/
	if(FALSE == lockSetHour){
		time = getTime();
		lockSetHour = TRUE;
	}
	/**Verifies is mailbox is different that CR*/
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
	/**Verifies that two digits was pressed*/
	if(counterTime == 2){
		counterTime = 0;
		dataSet1.phaseState = 0;
	}
	/****************Push Buttons**********************/
	/**Verifies that button of hour was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_0))){
		time.hour.hour++;
		/**Reset at 1 if reaches the limit*/
		if(FORMAT_12H == Clock.hour.format){
			if(time.hour.hour == 13){
				time.hour.hour = 1;
				time.hour.format = FORMAT_12H;
				/**Save the configurations of format*/
				if(PERIOD_AM == Clock.hour.period){
					time.hour.period = PERIOD_AM;
				}
				/**Save the configurations of format*/
				if(PERIOD_PM == Clock.hour.period){
					time.hour.period = PERIOD_PM;
				}
			}
		}
		/**Reset at 0 if reaches the limit*/
		if(FORMAT_24H == Clock.hour.format){
			time.hour.format = FORMAT_24H;
			if(time.hour.hour == 25){time.hour.hour = 0;}
		}
		/**Clear interrupts**/
		Button_clearFlag(BUTTON_0);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of minutes was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_1))){
		time.hour.minutes++;
		/**Reset at 0 if reaches the limit*/
		if(time.hour.minutes == 60){time.hour.minutes = 0;}

		/**Clear interrupts**/
		Button_clearFlag(BUTTON_1);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of seconds was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_2))){
		time.hour.seconds++;
		/**Reset at 0 if reaches the limit*/
		if(time.hour.seconds == 60){time.hour.seconds = 0;}
		flagButton = TRUE;

		/**Clear interrupts**/
		Button_clearFlag(BUTTON_2);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of set was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_3))){
		/**Set time into RTC*/
		if(flagButton == TRUE){
			time.modifyTime = TRUE;
			setTimeLCD(time);
			flagButton = FALSE;
		}
		/**Clean and continue*/
		time.modifyTime = FALSE;
		lockSetHour = FALSE;
		dataSet1.phaseState = 1;
		FlagButton = TRUE;
		/**Clear interrupts**/
		Button_clearFlag(BUTTON_3);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/******************Detector CR********************/
	/**Finalize the transmission*/
	if(getUART0_mailBox() == CR){
		counterTime = 0;
		fifoTime = popFIFO_0();
		dataSet1.phaseState = 0;
		/**Flag indicates that digits is changing */
		if(HOUR == flagType){
			dataSet1.time.hour = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DOUBLEPOINT);
		}
		/**Flag indicates that digits is changing */
		if(MINUTES == flagType){
			dataSet1.time.minutes = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DOUBLEPOINT);
		}
		/**Flag indicates that digits is changing */
		if((SECONDS == flagType) && (FORMAT_24H == data.time.format)){
			dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
			dataSet1.phaseState = 1;
		}
		/**Flag indicates that digits is changing */
		if((SECONDS == flagType) && (FORMAT_12H == data.time.format)){
			dataSet1.time.seconds = Convert_numberASCIItoDATA(fifoTime.data);
			dataSet1.phaseState = 0;
		}
		/**Change depending of Period */
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
		/**Clean and continue*/
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
	/**Change the time by keyboard*/
	if(FALSE == FlagButton){
		time.hour.period = data.time.period;
		time.hour.hour = data.time.hour;
		time.hour.minutes = data.time.minutes;
		time.hour.seconds = data.time.seconds;
		time.modifyTime = TRUE;
		time.hour.format = data.time.format;

		setTimeLCD(time);
	}
	/**Change the time by buttons*/
	/*Only change the flag*/
	if(TRUE == FlagButton){
		FlagButton = FALSE;
	}
	dataSet2.stateMain = SET_HOUR;
	dataSet2.phaseState = 2;

	return (dataSet2);
}

StateSetHour_Type stateFinalSetHour(StateSetHour_Type data){

	static StateSetHour_Type dataSet3;
	/**Wait for CR and continue*/
	dataSet3.stateMain = MENU;
	return (dataSet3);
}
/**************************************************************/
StateSetDate_Type stateSetCalendar(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate1;
	static uint32 counterDate = 0;
	static FIFO_Type fifoTime;
	static FlagDate_Type flagType= DAY;
	static uint32 lockSetDate = FALSE;
	static uint32 flagButton = FALSE;
	static Time_Type time;

	/**Get the time into variable only once*/
	if(FALSE == lockSetDate){
		time = getTime();
		lockSetDate = TRUE;
	}
	/**Verifies is mailbox is different that CR*/
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
	/**Verifies that two digits was pressed*/
	if(counterDate== 2){
		counterDate = 0;
		dataSetDate1.phaseState = 0;
	}
	/****************Push buttons*********************/
	/**Verifies that button of day was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_0))){
		time.date.day++;
		/**Change depending of the month and its number days*/
		if((time.date.month == 1) || (time.date.month == 3) || (time.date.month == 5)
			|| (time.date.month == 7) || (time.date.month == 8) || (time.date.month == 10) || (time.date.month == 12))
		{
			if(time.date.day == 32){ time.date.day = 0;}
		}
		if((time.date.month == 4) || (time.date.month == 6) || (time.date.month == 9) || (time.date.month == 11))
		{
			if(time.date.day == 31){ time.date.day = 0;}
		}
		if(time.date.month == 2)
		{
			if(time.date.day == 29){ time.date.day = 0;}
		}

		/**Clear interrupts**/
		Button_clearFlag(BUTTON_0);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of month was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_1))){
		time.date.month++;
		if(time.date.month == 13){time.date.month = 0;}
		/**Clear interrupts**/
		Button_clearFlag(BUTTON_1);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of year was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_2))){
		time.date.year++;
		if(time.date.year == 99){time.date.year = 0;}
		flagButton = TRUE;
		/**Clear interrupts**/
		Button_clearFlag(BUTTON_2);
		GPIO_clearIRQStatus(GPIO_C);
	}
	/**Verifies that button of set was pressed*/
	if((TRUE == GPIO_getIRQStatus(GPIO_C)) && (TRUE == Button_getFlag(BUTTON_3))){

		/**Clean and continue*/
		if(flagButton == TRUE){
			time.modifyDate = TRUE;
			setTimeLCD(time);
			flagButton = FALSE;
		}
		time.modifyDate = FALSE;
		lockSetDate = FALSE;
		dataSetDate1.phaseState = 1;
		FlagButton = TRUE;

		/**Clear interrupts**/
		Button_clearFlag(BUTTON_3);
		GPIO_clearIRQStatus(GPIO_C);
	}

	/******************Detector CR********************/
	/**Finalize the transmission*/
	if(getUART0_mailBox() == CR){
		counterDate = 0;
		fifoTime = popFIFO_0();
		dataSetDate1.phaseState = 0;

		/**Flag indicates that digits is changing */
		if(DAY == flagType){
			dataSetDate1.time.day = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DIAG);
		}
		/**Flag indicates that digits is changing */
		if(MONTH == flagType){
			dataSetDate1.time.month = Convert_numberASCIItoDATA(fifoTime.data);
			UART_putChar(UART_0, ASCII_DIAG);

		}
		/**Flag indicates that digits is changing */
		if(YEAR == flagType){
			dataSetDate1.time.year = Convert_numberASCIItoDATA(fifoTime.data);
			dataSetDate1.phaseState = 1;
		}
		/**Clean and continue*/
		clearFIFO_0();
		clearUART0_mailbox();

		flagType++;
		if(flagType > 2){flagType = DAY;}
	}

	dataSetDate1.stateMain = SET_DATE;
	return (dataSetDate1);
}

StateSetDate_Type stateSaveDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate2;
	Time_Type time;

	/**Change the time by keyboard*/
	if(FALSE == FlagButton){
		time.date.day = data.time.day;
		time.date.month = data.time.month;
		time.date.year = data.time.year;
		time.modifyDate = TRUE;
		setTimeLCD(time);
	}
	/**Change the time by bottons*/
	if(TRUE == FlagButton){
		FlagButton = FALSE;
	}
	dataSetDate2.stateMain = SET_DATE;
	dataSetDate2.phaseState = 2;

	return (dataSetDate2);
}

StateSetDate_Type stateFinalSetDate(StateSetDate_Type data){

	static StateSetDate_Type dataSetDate3;
	/**Wait for CR to continue*/
	dataSetDate3.stateMain = MENU;
	return (dataSetDate3);
}
/*****************************************************************/
StateFormat_Type stateShowFormat(StateFormat_Type data){

	static StateFormat_Type dataFormat1;
	/**Verifies the current format*/
	if(FORMAT_12H == data.format){
		UART_putString(UART_0,"12h");
		dataFormat1.format = FORMAT_12H;
	}
	/**Verifies the current format*/
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

	/**Change to other form*/
	if(FORMAT_12H == data.format){
		UART_putString(UART_0,"24h ");
	}
	/**Change to other form*/
	if(FORMAT_24H == data.format){
		UART_putString(UART_0,"12h ");
	}
	/*Save and continue*/
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

	/**Verifies the character is different that CR*/
	if(getUART0_mailBox() != CR){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, getUART0_mailBox());
	}
	/**Save the input into mailbox*/
	pushFIFO_0(getUART0_mailBox());
	dataFormat3.phaseState = 2;

	/**Finalize the transmission and compares*/
	if(getUART0_mailBox() == CR){
		fifo =  popFIFO_0();
		if(('S' == fifo.data[0]) || ('s' == fifo.data[0])){  counterYes++;}
		if(('I' == fifo.data[1]) || ('i' == fifo.data[1])){  counterYes++;}
		if(('N' == fifo.data[0]) || ('n' == fifo.data[0])){  counterNo++;}
		if(('O' == fifo.data[1]) || ('o' == fifo.data[1])){  counterNo++;}
		dataFormat3.phaseState = 3;
	}
	/**Change the format*/
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
	/**Mantein the format*/
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
	/**Wait for the CR to continue*/
	dataFormat4.stateMain = MENU;

	return (dataFormat4);
}

/**********************************************************/
StateReadHour_Type stateReadTime(StateReadHour_Type data){

	static StateReadHour_Type dataReadTime1;
	static uint32 flagLock = FALSE;

	/**Print the hour once*/
	if(FALSE == flagLock){
		printHourUART(getTime());
		flagLock = TRUE;
	}
	dataReadTime1.phaseState = 0;

	/**Finalize the state and continue*/
	if(getUART0_mailBox() == CR){
		dataReadTime1.phaseState = 1;
		flagLock = FALSE;
	}

	dataReadTime1.stateMain = READ_HOUR;
	return dataReadTime1;
}

StateReadHour_Type stateFinalRH(StateReadHour_Type data){

	static StateReadHour_Type dataReadTime2;
	/**Wait for the key final to continue*/
	dataReadTime2.stateMain = MENU;
	return dataReadTime2;
}
/**********************************************************/
StateReadDate_Type stateReadCalendar(StateReadDate_Type data){

	static StateReadDate_Type dataReadCal1;
	static uint32 flagLock = FALSE;

	/**Printhe the date once*/
	if(FALSE == flagLock){
		printDateUART(Clock);
		flagLock = TRUE;
	}
	dataReadCal1.phaseState = 0;

	/**Finalize the state and continue*/
	if(getUART0_mailBox() == CR){
		dataReadCal1.phaseState = 1;
		flagLock = FALSE;
	}

	dataReadCal1.stateMain = READ_DATE;
	return dataReadCal1;
}

StateReadDate_Type stateFinalRD(StateReadDate_Type data){

	static StateReadDate_Type dataReadCal2;
	/**Wait for the last key to continue*/
	dataReadCal2.stateMain = MENU;
	return dataReadCal2;
}

/*************************************************************/
StateEco_Type stateTransmitEco(StateEco_Type data){

	static StateEco_Type dataEco1;
	static FIFO_Type fifo;
	static uint32 counter = 0;

	/**Save the transmission into FIFO*/
	pushFIFO_0(getUART0_mailBox());
	fifo = popFIFO_0();

	/**While the fifo isn't full continue*/
	if((fifo.data[counter] != ESC) && (counter < 50)){
		/**Sends to the PCA the received data in the mailbox*/
		UART_putChar(UART_0, fifo.data[counter]);
		LCDNokia_sendChar(fifo.data[counter]);
	}
	dataEco1.phaseState = 0;

	/**If the fifo is full, the overwrite in the first position*/
	if(counter == 50){
		clearFIFO_0();
		counter = 0;
	}
	/**Exit when ESC key is detected*/
	if(fifo.data[counter] == ESC){
		/**Clean and continue*/
		dataEco1.phaseState = 1;
		counter = 0;
		clearFIFO_0();
	}
	counter++;
	dataEco1.stateMain = ECO;
	return (dataEco1);
}

StateEco_Type stateFinalEco(StateEco_Type data){

	static StateEco_Type dataEco2;
	/**Continue to MENU state*/
	LCDNokia_clear();
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

	/**Verifies the RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Pass the info of clock into variable Clock*/
	if(FALSE == lockRTC){
		Clock = realTime;
		lockRTC = TRUE;
	}
	/**Print the time in LCD*/
	printTimeLCD(Clock);

	/**Print the UART for phases*/
	if(FALSE == flagUART0){flagUART0 = menu_Main();}

	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		if(getUART0_mailBox() != CR){
			/**Sends to the PCA the received data in the mailbox*/
			UART_putChar(UART_0, getUART0_mailBox());
		}

		/**Saves the input data in an array of 2 spaces**/
		pushFIFO_0(getUART0_mailBox());

		/**Finalize with the new state*/
		if(getUART0_mailBox() == CR){
			/**Pop the fifo into other variable*/
			fifoMenu = popFIFO_0();
			/**Elects the new state*/
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
	clearUART0_mailbox();
	/**Verifie the state and fifo state*/
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

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the time in the LCD*/
	printTimeLCD(Clock);
	/**Print the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_ReadI2C(phase);
	}
	/**Save the address*/
	if(phase == 1){
		if(FALSE == flagAddress){
			dataMemory.realAddress = stateRead.realAddress;
			flagAddress = TRUE;
		}
	}
	if(phase == 2){
		/**Save lenght*/
		dataMemory.realLenght = stateRead.realLenght;
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(dataMemory);
	}
	/**Detect the pressed keys*/
	if(getUART0_flag()){
		readFunctions = statesReadI2C[phase].StateReadI2C;
		stateRead = readFunctions(stateRead);
		/**Finalize and clean*/
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

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the LCD*/
	printTimeLCD(Clock);
	/**Print in UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_WriteI2C(phase);
	}
	/**Save the address*/
	if(phase == 1){
		if(FALSE == flagAddress){
			dataMemoryWrite.realAddress = stateWrite.realAddress;}
			flagAddress = TRUE;
		}
	/**Save the size of data*/
	if(phase == 2){
		dataMemoryWrite.sizeData = stateWrite.sizeData;
		for(counterChar = 0; counterChar < dataMemoryWrite.sizeData; counterChar++){
			dataMemoryWrite.inputData[counterChar] = stateWrite.inputData[counterChar];
		}
		writeFunctions = statesWriteI2C[phase].StateWriteI2C;
		stateWrite = writeFunctions(dataMemoryWrite);
	}
	/**Detect the pressed keys*/
	if(getUART0_flag()){
		writeFunctions = statesWriteI2C[phase].StateWriteI2C;
		stateWrite = writeFunctions(stateWrite);
		/**Clean and continue*/
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

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the time in the LCD*/
	printTimeLCD(Clock);
	/**Print in the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_SetHour(phase);
	}
	/**Save the format of hour*/
	if(phase == 0){
		if(FALSE == flagInit_Format){
			clearUART0_mailbox();
			state_SetHour.time.format = Clock.hour.format;
			flagInit_Format = TRUE;
		}
	}
	/**Save the info of time*/
	if(phase == 1){
		dataMemory_SetHour.time = state_SetHour.time;
		setHourFunctions = statesSetHour[phase].StateSetHour;
		state_SetHour = setHourFunctions(dataMemory_SetHour);
	}
	/**Detect pressed keys and pressed bottons*/
	if((getUART0_flag()) || (TRUE == GPIO_getIRQStatus(GPIO_C))){
		setHourFunctions = statesSetHour[phase].StateSetHour;
		state_SetHour = setHourFunctions(state_SetHour);
		/**Clean and continue*/
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

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the time in LCD*/
	printTimeLCD(Clock);
	/**Print the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_SetDate(phase);
	}
	/**Clear the mailbox before to continue*/
	if(phase == 0){
		if(FALSE == flagInit_Format){
			clearUART0_mailbox();
			flagInit_Format = TRUE;
		}
	}
	/**Save the info of time*/
	if(phase == 1){
		dataMemory_SetDate.time = state_SetDate.time;
		setDateFunctions = statesSetDate[phase].StateSetDate;
		state_SetDate = setDateFunctions(dataMemory_SetDate);
	}
	/**Detect the pressed keys and pressed bottons*/
	if((getUART0_flag()) || (TRUE == GPIO_getIRQStatus(GPIO_C))){
		setDateFunctions = statesSetDate[phase].StateSetDate;
		state_SetDate = setDateFunctions(state_SetDate);
		/**Clean and continue*/
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
	static StateFormat_Type stateFormat;
	static StateFormat_Type dataMemory;
	StateFormat_Type(*formatFunctions)(StateFormat_Type);
	stateFormat.stateMain = FORMAT;

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the time in LCD*/
	printTimeLCD(Clock);
	/**Print the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_FormatHour(phase);
	}
	/**Save the current format of hour*/
	if(phase == 0){
		clearUART0_mailbox();
		dataMemory.format = Clock.hour.format;
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);
	}
	/**Continue with the phases 1*/
	if(phase == 1){
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);
	}
	/**Detect the pressed keys*/
	if(getUART0_flag()){
		formatFunctions = statesFormat[phase].StateFormat;
		stateFormat = formatFunctions(dataMemory);
		/**Save the resultant format*/
		if(stateFormat.phaseState == 3){
			Clock.hour.format = stateFormat.format;
			printTimeLCD(Clock);
		}
		/**Clean and continue*/
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
	static uint32 flagLock = FALSE;
	static StateReadHour_Type stateReadHour;
	StateReadHour_Type(*readHourFunctions)(StateReadHour_Type);
	stateReadHour.stateMain = READ_HOUR;
	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_ReadHour(phase);
	}
	/**Pass the state once*/
	if(phase == 0){
		if(FALSE == flagLock){
			readHourFunctions = statesReadHour[phase].StateReadHour;
			stateReadHour = readHourFunctions(stateReadHour);
			flagLock = TRUE;
		}
	}
	/**Pass the state once*/
	if(phase == 1){
		readHourFunctions = statesReadHour[phase].StateReadHour;
		stateReadHour = readHourFunctions(stateReadHour);
		stateReadHour.phaseState = 0;
		flagLock = FALSE;
		flagUART0 = FALSE;
	}
	/**Detect the pressed key*/
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
	static uint32 flagLock = FALSE;
	static StateReadDate_Type stateReadDate;
	StateReadDate_Type(*readDateFunctions)(StateReadDate_Type);
	stateReadDate.stateMain = READ_DATE;

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the time in LCD*/
	if(FALSE == flagUART0){
		flagUART0 = menu_ReadDate(phase);
	}
	/**Pass the state once*/
	if(phase == 0){
		if(FALSE == flagLock){
			readDateFunctions = statesReadDate[phase].StateReadDate;
			stateReadDate = readDateFunctions(stateReadDate);
			flagLock = TRUE;
		}
	}
	/**Pass the state once*/
	if(phase == 1){
		readDateFunctions = statesReadDate[phase].StateReadDate;
		stateReadDate = readDateFunctions(stateReadDate);
		stateReadDate.phaseState = 0;
		flagLock = FALSE;
		flagUART0 = FALSE;
	}
	/**Detect the pressed key*/
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
	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the LCD*/
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
	static uint32 flagClear = FALSE;
	static StateEco_Type stateEco;
	StateEco_Type(*ecoFunctions)(StateEco_Type);
	stateEco.stateMain = ECO;

	/**Verifies the state of RTC*/
	if(MEMORYERROR == (getRTCError())){
		LCDNokia_sendString((uint8*)MessageError);
	}
	/**Print the UART for phases*/
	if(FALSE == flagUART0){
		flagUART0 = menu_EcoLCD(phase);
	}
	/**Clear the LCD to transmit*/
	if(FALSE == flagClear){
		LCDNokia_clear();
		flagClear = TRUE;
	}
	/**Detect the pressed key*/
	if(getUART0_flag()){
		ecoFunctions = statesEco[phase].StateEco;
		stateEco = ecoFunctions(stateEco);

		if(phase == 1){ flagClear = FALSE;}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	phase = stateEco.phaseState;
	return (stateEco.stateMain);
}

