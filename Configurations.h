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
#include "I2C.h"
#include "I2CE2PROM.h"
#include "RTC.h"
#include "UART.h"


/**
 * \brief This data type define the states of program
 */
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

/**
 * \brief This data type define the states of hour format
 */
typedef enum{NON_FORMAT,FORMAT_12H, FORMAT_24H}FORMAT_HOUR;

/**
 * \brief This data type define the time periods of the hour
 */
typedef enum{NON_PERIOD,PERIOD_AM, PERIOD_PM}PERIOD_TIME;

/**
 * \brief This data type define the structure of Hour
 */
typedef struct{
	uint32	hour;
	uint32	minutes;
	uint32	seconds;
	FORMAT_HOUR format;
	PERIOD_TIME period;
}Hour_Type;

/**
 * \brief This data type define the structure of date
 */
typedef struct{
	uint32	year;
	uint32	month;
	uint32	day;
}Date_Type;

/**
 * \brief This data type define the Lock Flags to general use
 */
typedef struct{
	uint32 flag1 : 1;
	uint32 flag2 : 1;
	uint32 flag3 : 1;
	uint32 flag4 : 1;
	uint32 flag5 : 1;
	uint32 flag6 : 1;
	uint32 flag7 : 1;
	uint32 flag8 : 1;
}LockFlags_Type;

/**
 * \brief This data type define the structure of Time
 */
typedef struct{
	Hour_Type hour;
	Date_Type date;
	uint32 modifyTime;
	uint32 modifyDate;
}Time_Type;

/*Data type to main functions*/
typedef States_MenuType(*fptrState)(Time_Type);

/**Data type to machine state**/
typedef const struct State{
	States_MenuType (*stateFunction)(Time_Type);
}StateType;

/**
 * \brief This data type define the data IO of system
 */
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

/***********************************************************/
/**
 * \brief This data type define the data type in READ MEMORY
 */
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
/***********************************************************/
/**
 * \brief This data type define the data type in WRITE MEMORY
 */
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
/***********************************************************/
/**
 * \brief This data type define the data type in SET HOUR
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	Hour_Type time;
	FORMAT_HOUR format;
}StateSetHour_Type;

typedef enum{HOUR, MINUTES, SECONDS, PERIOD}FlagHour_Type;

typedef StateSetHour_Type(*fptrStateSetHour)(StateSetHour_Type);

typedef const struct StateSetHour{
	StateSetHour_Type(*StateSetHour)(StateSetHour_Type);
}StatePtrSetHour_Type;
/***********************************************************/
/**
 * \brief This data type define the data type in SET DATE
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	Date_Type time;
	uint32 flagYear : 1;
	uint32 flagMonth  : 1;
	uint32 flagDay  : 1;
}StateSetDate_Type;

typedef enum{DAY, MONTH, YEAR}FlagDate_Type;

typedef StateSetDate_Type(*fptrStateSetDate)(StateSetDate_Type);

typedef const struct StateSetDate{
	StateSetDate_Type(*StateSetDate)(StateSetDate_Type);
}StatePtrSetDate_Type;
/***********************************************************/
/**
 * \brief This data type define the data type in FORMAT HOUR
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	FORMAT_HOUR format;
}StateFormat_Type;

typedef StateFormat_Type(*fptrStateFormat)(StateFormat_Type);

typedef const struct StateFormat{
	StateFormat_Type(*StateFormat)(StateFormat_Type);
}StatePtrFormat_Type;
/***********************************************************/
/**
 * \brief This data type define the data type in READ HOUR
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	uint32 hour;
	uint32 minutes;
	uint32 seconds;
	uint32 flagBlock;
}StateReadHour_Type;

typedef StateReadHour_Type(*fptrStateReadHour)(StateReadHour_Type);

typedef const struct StateReadHour{
	StateReadHour_Type(*StateReadHour)(StateReadHour_Type);
}StatePtrReadHour_Type;
/***********************************************************/
/**
 * \brief This data type define the data type in READ DATE
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	uint32 year;
	uint32 month;
	uint32 day;
	uint32 flagBlock;
}StateReadDate_Type;

typedef StateReadDate_Type(*fptrStateReadDate)(StateReadDate_Type);

typedef const struct StateReadDate{
	StateReadDate_Type(*StateReadDate)(StateReadDate_Type);
}StatePtrReadDate_Type;
/***********************************************************/
/**
 * \brief This data type define the data type in ECO
 */
typedef struct{
	uint32 phaseState;
	States_MenuType stateMain;
	uint32 flagBlock;
}StateEco_Type;

typedef StateEco_Type(*fptrStateEco)(StateEco_Type);

typedef const struct StateEco{
	StateEco_Type(*StateEco)(StateEco_Type);
}StatePtrEco_Type;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in LCD
 	 \param[in]  time Data of time
 	 \return void
 */
void printTimeLCD(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Set the time into RTC
 	 \param[in]  time Data of time
 	 \return void
 */
void setTimeLCD(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Get the current time of RTC
 	 \param[in]  void
 	 \return The value of Time
 */
Time_Type getTime();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in UART
 	 \param[in]  time Data of time
 	 \return void
 */
void printHourUART(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in UART
 	 \param[in]  time Data of time
 	 \return void
 */
void printDateUART(Time_Type time);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ MEMORY where obtain the address
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadI2C_Type stateAddress(StateReadI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ MEMORY where obtain the size
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadI2C_Type stateLenght(StateReadI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ MEMORY where set the data
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadI2C_Type stateData(StateReadI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ MEMORY where finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadI2C_Type stateFinalReadI2C(StateReadI2C_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of WRITE MEMORY where obtain the address
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateWriteI2C_Type stateAddressWrite(StateWriteI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of WRITE MEMORY where obtain the data
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateWriteI2C_Type stateDataWrite(StateWriteI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of WRITE MEMORY to set the data
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateWriteI2C_Type stateFinalWrite(StateWriteI2C_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of WRITE MEMORY to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateWriteI2C_Type stateFinalWriteI2C(StateWriteI2C_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET HOUR to obtain the time
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetHour_Type stateSetTime(StateSetHour_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET HOUR to save the info
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetHour_Type stateSaveTime(StateSetHour_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET HOUR to finalize and save the time
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetHour_Type stateFinalSetHour(StateSetHour_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET CALENDAR to obtain the time
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetDate_Type stateSetCalendar(StateSetDate_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET CALENDAR to save the info
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetDate_Type stateSaveDate(StateSetDate_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of SET CALENDAR to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateSetDate_Type stateFinalSetDate(StateSetDate_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of FORMAT HOUR to show the current format
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateFormat_Type stateShowFormat(StateFormat_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of FORMAT HOUR to ask the change
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateFormat_Type stateChange(StateFormat_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of FORMAT HOUR to save the info
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateFormat_Type stateSaveFormat(StateFormat_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of FORMAT HOUR to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateFormat_Type stateFinalFormat(StateFormat_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ HOUR to read
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadHour_Type stateReadTime(StateReadHour_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ HOUR to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadHour_Type stateFinalRH(StateReadHour_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ CALENDAR to read
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadDate_Type stateReadCalendar(StateReadDate_Type data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of READ CALENDAR to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateReadDate_Type stateFinalRD(StateReadDate_Type data);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of ECO to transmit
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateEco_Type stateTransmitEco(StateEco_Type);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Sub state of ECO to finalize
 	 \param[in]  data The structure with the variables
 	 \return The structure with new data
 */
StateEco_Type stateFinalEco(StateEco_Type);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Menu
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateMenu(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Read
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateRead(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Write
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateWrite(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Hour
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateSetHour(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Set Date
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateSetDate(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Format
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateFormat(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Read Hour
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateReadHour(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Read Date
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateReadDate(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Terminal 2
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateTerminal2(Time_Type realTime);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Eco
 	 \param[in]  realTime The value of current time
 	 \return The next state
 */
States_MenuType stateEco(Time_Type realTime);

#endif /* CONFIGURATIONS_H_ */
