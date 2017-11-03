/*
 * Menus.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Menus.h"

void menu_Main(DataIO_Type data){

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[10;10H");
	UART_putString(UART_0, "MENU PRINCIPAL\r");
	UART_putString(UART_0,"\033[11;10H");
	UART_putString(UART_0, "1) Leer memoria I2C\r");
	UART_putString(UART_0,"\033[12;10H");
	UART_putString(UART_0, "2) Escribir memoria I2C\r");
	UART_putString(UART_0,"\033[13;10H");
	UART_putString(UART_0, "3) Establecer hora\r");
	UART_putString(UART_0,"\033[14;10H");
	UART_putString(UART_0, "4) Establecer fecha\r");
	UART_putString(UART_0,"\033[15;10H");
	UART_putString(UART_0, "5) Formato de hora\r");
	UART_putString(UART_0,"\033[16;10H");
	UART_putString(UART_0, "6) Leer hora\r");
	UART_putString(UART_0,"\033[17;10H");
	UART_putString(UART_0, "7) Leer fecha\r");
	UART_putString(UART_0,"\033[18;10H");
	UART_putString(UART_0, "8) Comunicacion con terminal 2\r");
	UART_putString(UART_0,"\033[19;10H");
	UART_putString(UART_0, "9) Eco en LCD\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[20;10H");
}

void menu_ReadI2C(DataIO_Type data){

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[10;10H");
	UART_putString(UART_0, "MENU PRINCIPAL\r");
	UART_putString(UART_0,"\033[11;10H");
	UART_putString(UART_0, "1) Leer memoria I2C\r");
	UART_putString(UART_0,"\033[12;10H");
	UART_putString(UART_0, "2) Escribir memoria I2C\r");
	UART_putString(UART_0,"\033[13;10H");
	UART_putString(UART_0, "3) Establecer hora\r");
	UART_putString(UART_0,"\033[14;10H");
	UART_putString(UART_0, "4) Establecer fecha\r");
	UART_putString(UART_0,"\033[15;10H");
	UART_putString(UART_0, "5) Formato de hora\r");
	UART_putString(UART_0,"\033[16;10H");
	UART_putString(UART_0, "6) Leer hora\r");
	UART_putString(UART_0,"\033[17;10H");
	UART_putString(UART_0, "7) Leer fecha\r");
	UART_putString(UART_0,"\033[18;10H");
	UART_putString(UART_0, "8) Comunicacion con terminal 2\r");
	UART_putString(UART_0,"\033[19;10H");
	UART_putString(UART_0, "9) Eco en LCD\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[20;10H");
}


void menu_WriteI2C(DataIO_Type data){

}

void menu_SetHour(DataIO_Type data){

}

void menu_SetDate(DataIO_Type data){

}

void menu_FormatHour(DataIO_Type data){

}

void menu_ReadHour(DataIO_Type data){

}

void menu_ReadDate(DataIO_Type data){

}

void menu_CommTerminal2(DataIO_Type data){

}

void menu_Read(DataIO_Type data){

}

void menu_EcoLCD(DataIO_Type data){

}


