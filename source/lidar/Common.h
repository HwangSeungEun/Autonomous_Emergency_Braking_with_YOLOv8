/*-----------------------------------------------------------------------------------*/
/* FILE			: COMMON.H												             */
/* MODULE		: COMMON                                                             */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_COMMON
#define HEADER_COMMON

/*-----------------------------------------------------------------------------------*/
/* Definition          														         */
/*-----------------------------------------------------------------------------------*/

#define MAX_DATA_LENGTH			(NSIZE)(           8640000)
#define MAX_LASER_POINT			(NSIZE)(			   761)
#define MAX_RADAR_TARGET		(NSIZE)(			    96)
#define MAX_RADAR_OBJECT		(NSIZE)(			    40)
#define MAX_ERROR				(NSIZE)(                10)
#define MAX_LASER_RECEIVE		(NSIZE)(		   16*1024)

/*-----------------------------------------------------------------------------------*/
/* Management          														         */
/*-----------------------------------------------------------------------------------*/

enum    Integration { PREV, CURR, N_MEMO };
enum    StopCondi { GO_NEXT, FINAL_TIME, ERR_DETECT };
enum	InterfaceMode { IMPORT, EXPORT };


/*-----------------------------------------------------------------------------------*/
/* Sensor Parameters   														         */
/*-----------------------------------------------------------------------------------*/

enum    LaserErrCode { LSR_ERR_INIT_SOCKET, LSR_ERR_OPEN_SOCKET, LSR_ERR_CONNECT_SOCKET };
enum    RadarErrCode { RDR_ERR_CONF_RANGE_LENGTH, RDR_ERR_OUTPUT_TYPE, RDR_ERR_POWER_REDUCTION, RDR_ERR_ELEVATION };

/*-----------------------------------------------------------------------------------*/

#endif