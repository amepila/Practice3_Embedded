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

typedef void(*fptrMenu)(DataIO_Type data);



void menu_Main(DataIO_Type data);

void menu_ReadI2C(DataIO_Type data);

void menu_WriteI2C(DataIO_Type data);

void menu_SetHour(DataIO_Type data);

void menu_SetDate(DataIO_Type data);

void menu_FormatHour(DataIO_Type data);

void menu_ReadHour(DataIO_Type data);

void menu_ReadDate(DataIO_Type data);

void menu_CommTerminal2(DataIO_Type data);

void menu_Read(DataIO_Type data);

void menu_EcoLCD(DataIO_Type data);



#endif /* MENUS_H_ */
