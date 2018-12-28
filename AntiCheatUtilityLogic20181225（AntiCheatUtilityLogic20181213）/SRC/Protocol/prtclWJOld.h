#ifndef __PRTCLWJOLD_H
#define __PRTCLWJOLD_H
				
#include "config.h"

#ifdef  __PRTCLWJOLD_C
#define	PRTCLWJOLD_EXT	 
#else
#define	PRTCLWJOLD_EXT	extern
#endif

//PRTCLWJOLD_EXT void FVehTransForSend(Vehicle_Struct	c_vehicleInfo);
PRTCLWJOLD_EXT void ToBuff1( Vehicle_Struct *pdata );
PRTCLWJOLD_EXT uint32 VehicleType_judge(uint32 Axisnum,int32 Axisdis[]);
#endif
