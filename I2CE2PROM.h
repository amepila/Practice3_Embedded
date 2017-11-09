/*
 * I2CE2PROM.h
 *
 *  Created on: 08/11/2017
 *      Author: eric_
 */

#ifndef I2CE2PROM_H_
#define I2CE2PROM_H_

void E2PROMdelay(uint32 delay);
void writeMemory(uint16 add,uint8 data);
uint8 readMemory(uint16 add);

#endif /* I2CE2PROM_H_ */
