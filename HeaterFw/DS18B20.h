/*
 * DS18B20.h
 *
 *  Created on: Oct 23, 2016
 *      Author: ujagaga
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "hw_cfg.h"

#define DS_MIN		(0)
#define DS_MAX		(78)
#define DS_ERR		(0xFF)

uint8_t DS_readTemp( void );

#endif /* DS18B20_H_ */
