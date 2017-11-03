/*
 * Buttons.h
 *
 *  Created on: Oct 14, 2017
 *      Author: Andres Hernandez
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"

/*Macro to define the value of system Clock*/
#define SYSTEM_CLOCK	(60000000U)

/*Data type to assign the buttons*/
typedef enum{
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5
}Button_Type;

/*Data type to assign the flag of the buttons*/
typedef struct{
	uint8 flagButton0 : 1;
	uint8 flagButton1 : 1;
	uint8 flagButton2 : 1;
	uint8 flagButton3 : 1;
	uint8 flagButton4 : 1;
	uint8 flagButton5 : 1;
} Button_PressedFlag;

/*Data type to assign the port of buttons to use*/
typedef enum{PORT_A,PORT_B,PORT_C,PORT_D,PORT_E}Button_PortType;

/*Data type to set up the buttons*/
typedef struct{
	Button_PortType Button_Port;
	BitsType Bit_Type;
}Button_ConfigType;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This activate the clock of the button port
 	\param[in] port Selected port
 	\return void
 */
//static void Button_clock(Button_PortType port);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This enable the button
 	\param[in] port Selected port
 	\param[in] bit	Selected bit
 	\return void
 */
//static void Button_enable(Button_PortType port,BitsType bit);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This set the flag of Button
 	\param[in] port Selected port
 	\param[in] bit Selected bit
 	\return void
 */
void Button_statusFlag(GPIO_portNameType gpio, BitsType bit);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This send the value of the flag of a button
 	\param[in] button Selected button
 	\return status of the button flag
 */
uint8 Button_getFlag(Button_Type button);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This clear the flag of a button
 	\param[in] button Selected button
 	\return void
 */
void Button_clearFlag(Button_Type button);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This set up of the buttons
 	\param[in] Button_Config Configurations of buttons
 	\return void
 */
void Buttons_init(const Button_ConfigType*);


#endif /* BUTTONS_H_ */
