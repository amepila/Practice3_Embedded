/*
 * I2C.c
 *
 *  Created on: Oct 31, 2017
 *      Author: Andres Hernandez
 */

#include "I2C.h"
#include "GPIO.h"


void I2C_init(I2C_ChannelType channel, uint32 systemClock, uint8 baudRate){

	uint32 valueSCL;
	uint32 valueMULT = 4;
	static GPIO_pinControlRegisterType pinControlRegisterPortTx = GPIO_MUX1;
	static GPIO_pinControlRegisterType pinControlRegisterPortRx = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;

	GPIO_clockGating(GPIO_B);

	GPIO_pinControlRegister(GPIO_B,BIT2,&pinControlRegisterPortTx);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_OUTPUT,BIT2);

	GPIO_pinControlRegister(GPIO_B,BIT3,&pinControlRegisterPortRx);
	GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,BIT3);

	valueSCL = systemClock/(baudRate*valueMULT);

	switch(channel){
	case I2C_0:

		I2C0->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C0->F |= I2C_F_MULT(2);//MULT = 4
		I2C0->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module

		break;
	case I2C_1:

		I2C1->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C1->F |= I2C_F_MULT(2);//MULT = 4
		I2C1->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module
		break;
	case I2C_2:

		I2C2->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C2->F |= I2C_F_MULT(2);//MULT = 4
		I2C2->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module
		break;
	default:
		break;
	}
}

uint8 I2C_busy(){
	if(FALSE == (I2C0->S & I2C_S_BUSY_MASK)){
		return TRUE;//I2C is idle
	}else{
		return FALSE;//I2C is busy
	}
}

void I2C_MST_OrSLV_Mode(uint8 mode){

	if(FALSE == mode){
		I2C0->C1 &= ~(I2C_C1_MST_MASK);//Slave mode
	}else{
		I2C0->C1 = I2C_C1_MST_MASK; //Master mode
	}
}

void I2C_TX_RX_Mode(uint8 mode){

	if(FALSE == mode){
		I2C0->C1 &= ~(I2C_C1_TX_MASK); //Receive mode
	}else{
		I2C0->C1 = I2C_C1_TX_MASK; //Master mode
	}
}

void I2C_NACK(void){

	I2C0->C1 |= I2C_C1_TXAK_MASK;
}

void I2C_repeted_Start(void){

	I2C0->C1 |= I2C_C1_RSTA_MASK;

}

void I2C_write_Byte(uint8 data){

	I2C0->D = data;

}

uint8  I2C_read_Byte(void){
	uint8 data;

	data = I2C0->D;
	return data;
}

void I2C_wait(void){
	if(TRUE == (I2C0->S & I2C_S_IICIF_MASK)){//Verifies if there is an interruption
		while(TRUE == I2C_busy()){}//If the bus is busy then wait
	}
}

uint16 I2C_get_ACK(void){

	if(FALSE == (I2C0->S & I2C_S_RXAK_MASK)){
		return FALSE; //The ack was received
	}else{
		return TRUE; //The ack wasn't received
	}
}

void I2C_start(void){

	I2C_MST_OrSLV_Mode(I2C_MASTER_MODE);
	I2C_TX_RX_Mode(I2C_TX_MODE);
}


void I2C_stop(void){

	I2C_MST_OrSLV_Mode(I2C_SLAVE_MODE);
	I2C_TX_RX_Mode(I2C_RX_MODE);
}
/*
secondsType PCF8583_getSeconds(){

	uint8 dataFromPCDF8583;
	I2C_start();//it configures the I2C in transmitting mode and generates the start signal

	I2C_write_Byte(0xA0);//Writing RTC address in the data register
	I2C_wait(); //checking if the I2C module is busy by checking the busy flag
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
					//if an acknowledge was received by checking
	I2C_write_Byte(0x02);//writing register address
	I2C_wait();//Checking if the I2C module is busy
	I2C_get_ACK();//Waiting for acknowledge, this function is able to detect
					//if an acknowledge was received by checking
	I2C_repeated_Start();//Generating a new start
	I2C_write_Byte(0xA1);//Writing slave in order the previous register
	I2C_wait();//Checking if the I2C module is busy
	I2C_get_ACK();//Waiting for acknowledge, this function is able to detect
					//if an acknowledge was received by checking
	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

	I2C_NACK();//Generating Not acknowledge
	dataFromPCF8583 = I2C_read_Byte();//Dummy read
	I2C_wait();// Checking if the I2C module is busy

	I2C_stop();// Generating stop signal
	dataFromPCF8583 = I2C_read_Byte();// Reading the true value

	return (dataFromPCF8583);

}
*/
