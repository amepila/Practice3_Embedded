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

/*Data type to main functions*/
typedef States_MenuType(*fptrState)(uint32);

/**Data type to machine state**/
typedef const struct State{
	States_MenuType (*stateFunction)(void);
}StateType;

typedef enum{FORMAT_12H, FORMAT_24H}FORMAT_HOUR;

typedef struct{
	uint32	hour;
	uint32	minutes;
	uint32	seconds;
}Hour_Type;

typedef struct{
	uint32	year;
	uint32	month;
	uint32	day;
}Date_Type;

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
	uint32 flagHour : 1;
	uint32 flagMin  : 1;
	uint32 flagSec  : 1;
}StateSetHour_Type;

typedef StateSetHour_Type(*fptrStateSetHour)(StateSetHour_Type);

typedef const struct StateSetHour{
	StateSetHour_Type(*StateSetHour)(StateSetHour_Type);
}StatePtrSetHour_Type;
////////////////////////////////////////////////////

StateReadI2C_Type stateAddress(StateReadI2C_Type data);
StateReadI2C_Type stateLenght(StateReadI2C_Type data);
StateReadI2C_Type stateData(StateReadI2C_Type data);
StateReadI2C_Type stateFinal(StateReadI2C_Type data);

StateWriteI2C_Type stateAddressWrite(StateWriteI2C_Type data);
StateWriteI2C_Type stateDataWrite(StateWriteI2C_Type data);
StateWriteI2C_Type stateFinalWrite(StateWriteI2C_Type data);

StateSetHour_Type stateSetTime(StateSetHour_Type data);
StateSetHour_Type stateSave(StateSetHour_Type data);

States_MenuType stateMenu();
States_MenuType stateRead();
States_MenuType stateWrite();
States_MenuType stateSetHour();
States_MenuType stateSetDate();
States_MenuType stateFormat();
States_MenuType stateReadHour();
States_MenuType stateReadDate();
States_MenuType stateTerminal2();
States_MenuType stateEco();



#endif /* CONFIGURATIONS_H_ */
