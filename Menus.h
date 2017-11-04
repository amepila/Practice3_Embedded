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

uint8 menu_Main(void);

uint8 menu_ReadI2C(uint32 address, uint32 length, sint8 *data, uint8 counter);

uint8 menu_WriteI2C(DataIO_Type*);

uint8 menu_SetHour(DataIO_Type*);

uint8 menu_SetDate(DataIO_Type*);

uint8 menu_FormatHour(DataIO_Type*);

uint8 menu_ReadHour(DataIO_Type*);

uint8 menu_ReadDate(DataIO_Type*);

uint8 menu_CommTerminal2(DataIO_Type*);

uint8 menu_EcoLCD(DataIO_Type*);



#endif /* MENUS_H_ */
