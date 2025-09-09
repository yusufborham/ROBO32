/*
 * SYSCFG_prg.c
 *
 *  Created on: Aug 20, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.0
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SYSCFG_int.h"
#include "SYSCFG_prv.h"
#include "SYSCFG_cfg.h"


void MSYSCFG_vSetEXTIPort(u8 A_u8LineNo, u8 A_u8PortNo)
{
	u8 A_u8RegisterNo = A_u8LineNo/4;
	u8 A_u8ShiftAmount = (A_u8LineNo%4)*4;
	SYSCFG->EXTICRx[A_u8RegisterNo]&=  ~((0b1111)<<A_u8ShiftAmount);
	SYSCFG->EXTICRx[A_u8RegisterNo]|=   (A_u8PortNo) << (A_u8ShiftAmount);
}

