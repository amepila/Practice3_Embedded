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

void menu_ReadI2C(DataIO_Type*);

void menu_WriteI2C(DataIO_Type*);

void menu_SetHour(DataIO_Type*);

void menu_SetDate(DataIO_Type*);

void menu_FormatHour(DataIO_Type*);

void menu_ReadHour(DataIO_Type*);

void menu_ReadDate(DataIO_Type*);

void menu_CommTerminal2(DataIO_Type*);

void menu_EcoLCD(DataIO_Type*);



#endif /* MENUS_H_ */
