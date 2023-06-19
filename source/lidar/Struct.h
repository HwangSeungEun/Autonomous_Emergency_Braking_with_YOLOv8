/*-----------------------------------------------------------------------------------*/
/* FILE	        : STRUCT.H                                                           */
/* MODULE       : STRUCT                                                             */
/* AUTHOR       : HGU SASLAB                                                         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_DATA_STRUCT
#define HEADER_DATA_STRUCT

/*-----------------------------------------------------------------------------------*/

#include "Function.h"
#include "Common.h"

/*************************************************************************************/
/*                                 GLOBAL BUFFER                                     */
/*************************************************************************************/


/*-----------------------------------------------------------------------------------*/
/* Program Control    														         */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	REAL    Run;    REAL    Process;
	REAL    prev;    REAL    curr;	  COUNT   count;

}TIME; /* Time */

/*-----------------------------------------------------------------------------------*/

typedef struct
{
	MODE    Laser;
	MODE    Radar;

}PHASE; /* Phase */

/*-----------------------------------------------------------------------------------*/
/* Program Management          														 */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	COUNT   RunCount;
	LOGIC   StopCond;

}MANAGE; /* Manage */


/*-----------------------------------------------------------------------------------*/
/* Program Management          														 */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	LOGIC   Detection;

	COUNT   CountLsr;
	CODE    CodeLsr[MAX_ERROR];

	COUNT   CountRdr;
	CODE    CodeRdr[MAX_ERROR];

}ERRORS; /* Error */


/*-----------------------------------------------------------------------------------*/
/* File Format		      											             */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	char  InpFile[MAX_STRING];
	char  DataFile[6][MAX_STRING];

}FILE_FORMAT;

/*-----------------------------------------------------------------------------------*/

typedef struct
{
	FILE_FORMAT  ProgramSetting;
	FILE_FORMAT  RadarConfig;
	FILE_FORMAT  LaserConfig;

}FILE_NAME;


/*-----------------------------------------------------------------------------------*/
/* Laser Interface Global Buffer													 */
/*-----------------------------------------------------------------------------------*/

//typedef struct
//{
//	REAL  xCoordinate[MAX_LASER_POINT];
//	REAL  yCoordinate[MAX_LASER_POINT];
//
//}OUT_LASER;

typedef struct
{
	//COUNT       count[1];
	REAL    xCoordinate[MAX_LASER_POINT];
	REAL    yCoordinate[MAX_LASER_POINT];
	REAL           dist[MAX_LASER_POINT];
	REAL          angle[MAX_LASER_POINT];
	CHAR   stringDATA[MAX_LASER_RECEIVE];

}OUT_LASER;


typedef struct
{
	REAL  angle[MAX_LASER_POINT];
	REAL  distance[MAX_LASER_POINT];

}BUF_TEMP;

/*-----------------------------------------------------------------------------------*/
/* Radar Interface Global Buffer													 */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	/* Target Data */
	REAL  TarAng[MAX_RADAR_TARGET];
	REAL  TarDist[MAX_RADAR_TARGET];
	REAL  TarVrel[MAX_RADAR_TARGET];

	REAL  TarLength[MAX_RADAR_TARGET];
	REAL  TarWidth[MAX_RADAR_TARGET];

	int   TarType[MAX_RADAR_TARGET];

	/* Object Data */
	REAL  ObjLongDisp[MAX_RADAR_OBJECT];
	REAL  ObjLongVrel[MAX_RADAR_OBJECT];
	REAL  ObjLongAcce[MAX_RADAR_OBJECT];

	REAL  ObjLatDisp[MAX_RADAR_OBJECT];
	REAL  ObjLatVrel[MAX_RADAR_OBJECT];

	REAL  ObjLength[MAX_RADAR_OBJECT];
	REAL  ObjWidth[MAX_RADAR_OBJECT];

	int   ObjProbOfExist[MAX_RADAR_OBJECT];
	int   ObjDyprop[MAX_RADAR_OBJECT];


}OUT_RADAR;



/*-----------------------------------------------------------------------------------*/




/*************************************************************************************/
/*                             INPUT DATA STRUCTURE                                  */
/*************************************************************************************/

/*-----------------------------------------------------------------------------------*/
/* Input Data : Time                                                                 */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	REAL	Ts;
	REAL	Tf;
	REAL	Tstart;
	REAL    Tfinal;
} DATA_TIME;

/*-----------------------------------------------------------------------------------*/
/* NI-DAQ Configuration Input Data                                                   */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	LOGIC	USE;

} DATA_DAQ;

/*-----------------------------------------------------------------------------------*/
/* Laser Sensor Configuration Input Data                                             */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	LOGIC	USE;
	MODE	LoginMode;
	MODE	ScanMode;

} DATA_LASER;


/*-----------------------------------------------------------------------------------*/
/* Radar Sensor Configuration Input Data                                             */
/*-----------------------------------------------------------------------------------*/

typedef struct
{
	LOGIC	USE;
	CONF    RangeLength;
	CONF    OutputType;
	CONF    PowerReduction;
	CONF    Elevation;


	REAL	ScanningFrequency;

} DATA_RADAR;


#endif