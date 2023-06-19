/*-----------------------------------------------------------------------------------*/
/* FILE			: WRITEDATA.C                                                        */
/* MODULE		: WRITEDATA                                                          */
/* AUTHOR		: HGU SASLAB                                                         */
/* DATE			: 2015.01.02.                                                        */
/*-----------------------------------------------------------------------------------*/

#ifndef SOURCE_WRITEDATA
#define SOURCE_WRITEDATA

/*-----------------------------------------------------------------------------------*/

#include <direct.h> 
#include <time.h>
#include "WriteData.h"
#include "Subsystems.h"
#include "Declaration.h"
#include "LaserInterface.h"

/*-----------------------------------------------------------------------------------*/
/* Main Algorithm    														         */
/*-----------------------------------------------------------------------------------*/

void  WriteData(void)
{

	mkdir("Data");  chdir("Data");          //폴더 생성  나교수님 스타일 아님
	mkdir("Output");  chdir("Output");
	mkdir(DIRECTORY);  chdir(DIRECTORY);

	lsr_WriteData();
}

/*-----------------------------------------------------------------------------------*/

#endif
