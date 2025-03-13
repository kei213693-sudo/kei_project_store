/*
 * BtaPos.h
 *
 *  Created on: 2024年7月3日
 *      Author: F704012276_
 */

#ifndef BTAPOS_H_
#define BTAPOS_H_

typedef enum{
	External_Rear = 0,
	External_Front,
}BtaPos_t;

BtaPos_t Get_BTAPos(void);
#endif /* BTAPOS_H_ */
