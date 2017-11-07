/*
 * Menus.h
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#ifndef MENUS_H_
#define MENUS_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "UART.h"
#include "Configurations.h"


typedef struct{
	uint8 flag1 : 1;
	uint8 flag2 : 1;
	uint8 flag3 : 1;
	uint8 flag4 : 1;
	uint8 flag5 : 1;
}Flags_Type;

uint8 menu_Main(void);

uint8 menu_ReadI2C(uint8 phase);

uint8 menu_WriteI2C(uint8 phase);

uint8 menu_SetHour(uint8 phase);

uint8 menu_SetDate(uint8 phase);

uint8 menu_FormatHour(uint8 phase);

uint8 menu_ReadHour(uint8 phase);

uint8 menu_ReadDate(uint8 phase);

uint8 menu_CommTerminal2(uint8 phase);

uint8 menu_EcoLCD(uint8 phase);



#endif /* MENUS_H_ */
