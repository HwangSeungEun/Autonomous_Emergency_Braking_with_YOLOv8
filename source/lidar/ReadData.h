/*-----------------------------------------------------------------------------------*/
/* FILE			: READDATA.h									                     */
/* MODULE		: READDATA										                     */
/* AUTHOR		: HGU SASLAB 									                     */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_READDATA
#define HEADER_READDATA

/*-----------------------------------------------------------------------------------*/

#include "Include.h"
#include "Declaration.h"

/*-----------------------------------------------------------------------------------*/
/* Function Declaration                                                              */
/*-----------------------------------------------------------------------------------*/

void  ReadData(void);
void  ClearDataBuffer(void);
void  LoadIniFile(void);
void  LoadSettingFile(void);
void  LoadSensorFile(void);

/*-----------------------------------------------------------------------------------*/

LOGIC GetFileName(char *Srcbuffer, FILE_FORMAT *Filename);
LOGIC GetFileNameSingle(char *Srcbuffer, char *Filename);
void  ReadInput(FILE_FORMAT *InputName, REAL* ValueBuffer);
void  PutFileInfo(FILE_FORMAT *InputName, char *buffer);

/*-----------------------------------------------------------------------------------*/

#endif