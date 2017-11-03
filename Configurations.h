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
#include "Menus.h"
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
	uint8	hour;
	uint8	minutes;
	uint8	seconds;
}Hour_Type;

typedef struct{
	uint8	year;
	uint8	month;
	uint8	day;
}Date_Type;

typedef struct{
	uint32	address;
	uint32	lenght;
	uint8	dataIn[30];
	uint8	dataOut[30];
	Hour_Type Hour;
	Date_Type Date;
	FORMAT_HOUR	formatHour;
	uint8	error : 1;
}DataIO_Type;


#endif /* CONFIGURATIONS_H_ */
