/* BIT_MATH.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohamed Ismail
 *      version: 1.0
 */


#ifndef LIB_BIT_MATH_H_
#define LIB_BIT_MATH_H_

#define SET_BIT(REG,BIT)   ((REG) |=  (1U << (BIT)))
#define CLR_BIT(REG,BIT)   ((REG) &= ~(1U << (BIT)))
#define TOG_BIT(REG,BIT)   ((REG) ^=  (1U << (BIT)))
#define GET_BIT(REG,BIT)   (((REG) >> (BIT)) & 0x1U)

#endif /* LIB_BIT_MATH_H_ */

