/*
 * Menu.h
 *
 *  Created on: Oct 15, 2017
 *      Author: Andres Hernandez
 */

#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "Buttons.h"
#include "SPI.h"
#include "GPIO.h"
#include "UART.h"

/*Data type to States of the program*/
typedef enum{
	MENU,
	READ,
	WRITE,
	SET_HOUR,
	SET_DATE,
	FORMAT,
	READ_HOUR,
	READ_DATE,
	TERMINAL2,
	ECO
}States_MenuType;

typedef enum{FORMAT_12H, FORMAT_24H}FORMAT_HOUR;

typedef struct{
	uint32	hour;
	uint32	minutes;
	uint32	seconds;
	FORMAT_HOUR format;
}Hour_Type;

typedef struct{
	uint32	year;
	uint32	month;
	uint32	day;
}Date_Type;

typedef struct{
	Hour_Type hour;
	Date_Type date;
}Time_Type;

/*Data type to main functions*/
typedef States_MenuType(*fptrState)(Time_Type);

/**Data type to machine state**/
typedef const struct State{
	States_MenuType (*stateFunction)(Time_Type);
}StateType;


typedef struct{
	uint32	addressWrite;
	uint32	addressRead;
	uint32	lenght;
	sint8	*dataIn;
	sint8	*dataOut;
	Hour_Type Hour;
	Date_Type Date;
	FORMAT_HOUR	formatHour;
	uint32	errorFlag : 1;
}DataIO_Type;

////////////////////////////////////////////////////
typedef struct{
	uint8 inputAddress[5];
	uint8 inputLenght[4];
	uint32 phaseState;
	States_MenuType stateMain;
	uint32 realAddress;
	uint32 realLenght;
}StateReadI2C_Type;

typedef StateReadI2C_Type(*fptrStateReadI2C)(StateReadI2C_Type);

typedef const struct StateReadI2C{
	StateReadI2C_Type(*StateReadI2C)(StateReadI2C_Type);
}StatePtrRead_Type;
/////////////////////////////////////////////////////
typedef struct{
	uint8 inputAddress[5];
	uint8 inputData[30];
	uint32 phaseState;
	States_MenuType stateMain;
	uint32 realAddress;
	uint32 sizeData;
}StateWriteI2C_Type;

typedef StateWriteI2C_Type(*fptrStateWriteI2C)(StateWriteI2C_Type);

typedef const struct StateWriteI2C{
	StateWriteI2C_Type(*StateWriteI2C)(StateWriteI2C_Type);
}StatePtrWrite_Type;
/////////////////////////////////////////////////////
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	Hour_Type time;
	FORMAT_HOUR format;
}StateSetHour_Type;

typedef enum{HOUR, MINUTES, SECONDS}FlagHour_Type;

typedef StateSetHour_Type(*fptrStateSetHour)(StateSetHour_Type);

typedef const struct StateSetHour{
	StateSetHour_Type(*StateSetHour)(StateSetHour_Type);
}StatePtrSetHour_Type;
////////////////////////////////////////////////////
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	Date_Type time;
	uint32 flagYear : 1;
	uint32 flagMonth  : 1;
	uint32 flagDay  : 1;
}StateSetDate_Type;

typedef enum{YEAR, MONTH, DAY}FlagDate_Type;

typedef StateSetDate_Type(*fptrStateSetDate)(StateSetDate_Type);

typedef const struct StateSetDate{
	StateSetDate_Type(*StateSetDate)(StateSetDate_Type);
}StatePtrSetDate_Type;
///////////////////////////////////////////////////////
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	FORMAT_HOUR format;
}StateFormat_Type;

typedef StateFormat_Type(*fptrStateFormat)(StateFormat_Type);

typedef const struct StateFormat{
	StateFormat_Type(*StateFormat)(StateFormat_Type);
}StatePtrFormat_Type;
////////////////////////////////////////////////////////////
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
}StateReadHour_Type;

typedef StateReadHour_Type(*fptrStateReadHour)(StateReadHour_Type);

typedef const struct StateReadHour{
	StateReadHour_Type(*StateReadHour)(StateReadHour_Type);
}StatePtrReadHour_Type;
//////////////////////////////////////////////////////////////
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
}StateReadDate_Type;

typedef StateReadDate_Type(*fptrStateReadDate)(StateReadDate_Type);

typedef const struct StateReadDate{
	StateReadDate_Type(*StateReadDate)(StateReadDate_Type);
}StatePtrReadDate_Type;
////////////////////////////////////////////////////////////


StateReadI2C_Type stateAddress(StateReadI2C_Type data);
StateReadI2C_Type stateLenght(StateReadI2C_Type data);
StateReadI2C_Type stateData(StateReadI2C_Type data);
StateReadI2C_Type stateFinalReadI2C(StateReadI2C_Type data);

StateWriteI2C_Type stateAddressWrite(StateWriteI2C_Type data);
StateWriteI2C_Type stateDataWrite(StateWriteI2C_Type data);
StateWriteI2C_Type stateFinalWrite(StateWriteI2C_Type data);
StateWriteI2C_Type stateFinalWriteI2C(StateWriteI2C_Type data);

StateSetHour_Type stateSetTime(StateSetHour_Type data);
StateSetHour_Type stateSaveTime(StateSetHour_Type data);
StateSetHour_Type stateFinalSetHour(StateSetHour_Type data);

StateSetDate_Type stateSetCalendar(StateSetDate_Type data);
StateSetDate_Type stateSaveDate(StateSetDate_Type data);
StateSetDate_Type stateFinalSetDate(StateSetDate_Type data);

StateFormat_Type stateShowFormat(StateFormat_Type data);
StateFormat_Type stateChange(StateFormat_Type data);
StateFormat_Type stateSaveFormat(StateFormat_Type data);
StateFormat_Type stateFinalFormat(StateFormat_Type data);

StateReadHour_Type stateReadTime(StateReadHour_Type data);
StateReadHour_Type stateFinalRH(StateReadHour_Type data);

StateReadDate_Type stateReadCalendar(StateReadDate_Type data);
StateReadDate_Type stateFinalRD(StateReadDate_Type data);

States_MenuType stateMenu(Time_Type realTime);
States_MenuType stateRead(Time_Type realTime);
States_MenuType stateWrite(Time_Type realTime);
States_MenuType stateSetHour(Time_Type realTime);
States_MenuType stateSetDate(Time_Type realTime);
States_MenuType stateFormat(Time_Type realTime);
States_MenuType stateReadHour(Time_Type realTime);
States_MenuType stateReadDate(Time_Type realTime);
States_MenuType stateTerminal2(Time_Type realTime);
States_MenuType stateEco(Time_Type realTime);



#endif /* CONFIGURATIONS_H_ */
