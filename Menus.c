/*
 * Menus.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Menus.h"

uint8 menu_Main(void){

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

	return TRUE;
}

uint8 menu_ReadI2C(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "Direccion de la lectura:\t");

			flagContinue.flag1 = TRUE;
		}
		flagContinue.flag4 = FALSE;
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[11;10H");
			UART_putString(UART_0, "Longitud en bytes: \t");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}

	if(phase == 2){
		if(FALSE == flagContinue.flag3){
			UART_putString(UART_0,"\033[12;10H");
			UART_putString(UART_0, "Contenido: \r");
			UART_putString(UART_0,"\033[13;10H");

			flagContinue.flag3 = TRUE;
		}
		return FALSE;
	}


	if(phase == 3){
		if(FALSE  == flagContinue.flag4){
			UART_putString(UART_0,"\033[16;10H");
			UART_putString(UART_0, "Presione una tecla para continuar....\r");

			flagContinue.flag4 = TRUE;
		}
		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		flagContinue.flag3 = FALSE;

		return FALSE;
	}
}


uint8 menu_WriteI2C(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "Direccion de la escritura:\t");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[11;10H");
			UART_putString(UART_0, "Texto a guardar: \r");
			UART_putString(UART_0,"\033[12;10H");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}
	if(phase ==  2){
		if(FALSE == flagContinue.flag3){
			UART_putString(UART_0,"\033[13;10H");
			UART_putString(UART_0, "Su texto ha sido guardado...\r");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[14;10H");

			flagContinue.flag3 = TRUE;
		}


		return FALSE;
	}
	if(phase == 3){
		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		flagContinue.flag3 = FALSE;
	}
}

uint8 menu_SetHour(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){

			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "Escribir hora en hh/mm/ss:\t");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[11;10H");
			UART_putString(UART_0, "La hora ha sido cambiada...: \r");

			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[20;10H");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}
	if(phase == 2){

		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		return FALSE;
	}
}

uint8 menu_SetDate(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "Escribir fecha: en dd/mm/aa:\t");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[11;10H");
			UART_putString(UART_0, "La hora ha sido cambiada: \r");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[20;10H");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}

	if(phase ==  2){

		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		return FALSE;
	}
}

uint8 menu_FormatHour(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "El formato de hora actual es  ");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[11;10H");
			UART_putString(UART_0, "Desea cambiar el formato a  ");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}

	if(phase == 2){
		if(FALSE == flagContinue.flag3){
			UART_putString(UART_0,"si/no?\t");

			flagContinue.flag3 = TRUE;
		}
		return FALSE;
	}

	if(phase == 3){
		if(FALSE == flagContinue.flag4){
			UART_putString(UART_0,"\033[12;10H");
			UART_putString(UART_0, "El formato de la hora ha sido cambiado\r");

			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[20;10H");

			flagContinue.flag4 = TRUE;
		}
		return FALSE;
	}

	if(phase == 4){

		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		flagContinue.flag3 = FALSE;
		flagContinue.flag4 = FALSE;
		return FALSE;
	}

}

uint8 menu_ReadHour(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "La hora actual es: ");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}
	if(phase == 1){
		flagContinue.flag1 = FALSE;
		return FALSE;
	}
}

uint8 menu_ReadDate(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "La fecha actual es: ");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		flagContinue.flag1 = FALSE;
		return FALSE;
	}
}

uint8 menu_CommTerminal2(uint8 phase){
	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[10;10H");
	UART_putString(UART_0, "Terminal 1: \r");
	//UART_putString(UART_0, &data.addressRead);
	UART_putString(UART_0,"\r");


	UART_putString(UART_0,"\033[11;10H");
	UART_putString(UART_0, "Terminal 2: \r");
	//UART_putString(UART_0, &data.addressRead);
	UART_putString(UART_0,"[ESC]\r");

	UART_putString(UART_0,"\033[12;10H");
	UART_putString(UART_0, "Terminal 2 se desconecto \r");

	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[13;10H");

	return TRUE;
}

uint8 menu_EcoLCD(uint8 phase){

	static Flags_Type flagContinue;

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0){
		if(FALSE == flagContinue.flag1){
			/*VT100 command for clearing the screen*/
			UART_putString(UART_0,"\033[2J");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_putString(UART_0,"\033[10;10H");
			UART_putString(UART_0, "Escribir texto 1: \r");
			UART_putString(UART_0,"\033[11;10H");

			flagContinue.flag1 = TRUE;
		}
		return FALSE;
	}

	if(phase == 1){
		if(FALSE == flagContinue.flag2){
			UART_putString(UART_0,"\033[13;10H");
			UART_putString(UART_0,"Termino la conexion...\r");

			flagContinue.flag2 = TRUE;
		}
		return FALSE;
	}
	if(phase == 2){
		flagContinue.flag1 = FALSE;
		flagContinue.flag2 = FALSE;
		return FALSE;
	}
}


