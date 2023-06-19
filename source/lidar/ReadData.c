/*-----------------------------------------------------------------------------------*/
/* FILE			: READDATA.C	     									             */
/* MODULE		: READDATA													         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef SOURCE_READDATA
#define SOURCE_READDATA

/*-----------------------------------------------------------------------------------*/

#include "Include.h"
#include "ReadData.h"


/*-----------------------------------------------------------------------------------*/

static FILE_NAME    InputFileName;
static FILE* pFile;
static REAL         ValueBuffer[100];
static char         msg[MAX_STRING];

/*-----------------------------------------------------------------------------------*/
/* Read Data Main Module                                                             */
/*-----------------------------------------------------------------------------------*/

void    ReadData(void)
{
	ClearDataBuffer();

	LoadIniFile();
	LoadSettingFile();
	LoadSensorFile();

	printf("\n\n\t\t ================ File Loading Success ================ ");
};

/*-----------------------------------------------------------------------------------*/
/* 1. Data Buffer Clear                                                              */
/*-----------------------------------------------------------------------------------*/

void    ClearDataBuffer(void)
{
	INIT_BUF(DataTime); //DataTime의 메모리 초기화 :  memset(numPtr, 0, )

};

/*-----------------------------------------------------------------------------------*/
/* 2. TARGET.INI File Loading                                                        */
/*-----------------------------------------------------------------------------------*/

void    LoadIniFile(void)
{
	char MainExeFile[MAX_STRING];

	char InitName[MAX_STRING];
	char buffer[MAX_STRING];

	FILE* INI_File;

	chdir("../LIDAR");


	strcpy(InitName, "TARGET.INI"); //폴더내에 TARGET.INI 파일이 존재함.              

	INI_File = FileOpen(InitName, "r"); // READ모드로 파일을 OPEN하고 INI_FILE로 변수명을 저장한다.

	puts("\n\n\t\t =========== Target Tracking Filter Program ==========="); // 해당 내용을 출력한다.

	while (!feof(INI_File))  // TARGET.INI 파일 내용의 끝까지 실행한다.
	{
		if (fgets(buffer, MAX_STRING, INI_File) == NULL) break; // 파일이 비어있을 시 탈출한다.

		if (strstr(buffer, "[MAIN PROGRAM PATH & FILE]")) GetFileNameSingle(buffer, MainExeFile);  // strstr: 문자열 내에서 특정 문자 검색해 buffer에 저장, getfilenamesingle에서 
		else if (strstr(buffer, "-SETTING")) GetFileName(buffer, &InputFileName.ProgramSetting);
		else if (strstr(buffer, "-SENSOR")) GetFileName(buffer, &InputFileName.LaserConfig);
		else if (strstr(buffer, "-RADAR")) GetFileName(buffer, &InputFileName.RadarConfig);
	}

	fclose(INI_File);    // 파일 닫기
};

/*-----------------------------------------------------------------------------------*/
/* 3. Setting File Loading                                                           */
/*-----------------------------------------------------------------------------------*/

void    LoadSettingFile(void)
{
	register INDEX i = 0;

	ReadInput(&InputFileName.ProgramSetting, ValueBuffer);    // SETTING.INP파일 

	DataTime.Ts = (ValueBuffer[i++]);
	DataTime.Tf = (ValueBuffer[i++]);
	DataTime.Tstart = (ValueBuffer[i++]);
	DataTime.Tfinal = (ValueBuffer[i++]); // 4

	DataDAQ.USE = (LOGIC)(ValueBuffer[i++]); // 1

	DataLaser.USE = (LOGIC)(ValueBuffer[i++]);
	DataLaser.LoginMode = (MODE)(ValueBuffer[i++]);
	DataLaser.ScanMode = (MODE)(ValueBuffer[i++]); // 3

	DataRadar[0].USE = (LOGIC)(ValueBuffer[i++]);
	DataRadar[0].RangeLength = (CONF)(ValueBuffer[i++]);
	DataRadar[0].OutputType = (CONF)(ValueBuffer[i++]);
	DataRadar[0].PowerReduction = (CONF)(ValueBuffer[i++]);
	DataRadar[0].Elevation = (CONF)(ValueBuffer[i++]);

	//1

	printf("\n Ts: %f, Tf: %f, Tstart: %f, Tfinal: %f \n\n", DataTime.Ts, DataTime.Tf, DataTime.Tstart, DataTime.Tfinal);
	//printf("\n Lidar LoginMode: %d, Lidar ScanMode: %d\n\n", DataLaser.LoginMode, DataLaser.ScanMode);
	DataRadar[1].USE = (LOGIC)(ValueBuffer[i++]); //1

	printf("DAQ USE:%d, Laser USE: %d, Radar USE: %d\n", DataDAQ.USE, DataLaser.USE, DataRadar[0].USE);


	//4

};


/*-----------------------------------------------------------------------------------*/
/* 4. Sensor File Loading                                                           */
/*-----------------------------------------------------------------------------------*/

void    LoadSensorFile(void)
{
	register INDEX i = 0;

	ReadInput(&InputFileName.LaserConfig, ValueBuffer);    // SENSOR.INP파일

	DataDAQ.USE = (LOGIC)(ValueBuffer[i++]); // 1

	DataLaser.USE = (LOGIC)(ValueBuffer[i++]);
	DataLaser.LoginMode = (MODE)(ValueBuffer[i++]);
	DataLaser.ScanMode = (MODE)(ValueBuffer[i++]); // 3

	DataRadar[0].USE = (LOGIC)(ValueBuffer[i++]); //1

	DataRadar[1].USE = (LOGIC)(ValueBuffer[i++]); //1

	//3

};

/*-----------------------------------------------------------------------------------*/

LOGIC   GetFileName(char* Srcbuffer, FILE_FORMAT* Filename)
{
	char sep[] = "\"";
	char* temp;

	temp = strtok(Srcbuffer, sep);
	temp = strtok(NULL, sep);
	strcpy(Filename->InpFile, temp);

	if (strlen(temp) < 1)  return NOK;
	else                   return YOK;
}

/*-----------------------------------------------------------------------------------*/

LOGIC   GetFileNameSingle(char* Srcbuffer, char* Filename)
{
	char sep[] = "\"";
	char* temp;

	temp = strtok(Srcbuffer, sep);  //  TARGET.INI파일에서 검색한 특정 문자열을 큰따옴표(")를 기준으로 자른다.
	temp = strtok(NULL, sep);       //  문자열을 자른뒤에는 NULL을 넣어줘야함! (코딩도장 참고)
	strcpy(Filename, temp);         //  Filename에 저장

	if (strlen(temp) < 1)  return NOK;   // 만약 저장되지 않았을 때 0을 반환
	else                   return YOK;   // 저장 됐을 시 1 반환
}
/*-----------------------------------------------------------------------------------*/

void  ReadInput(FILE_FORMAT* InputName, REAL* ValueBuffer)
{   //이해가 필요합니다..
	char buffer[MAX_STRING] = { (char)NULL, };
	char StrBuffer[20] = { (char)NULL, };
	char* pStr;

	NSIZE NumberOfData = 0;
	NSIZE Value_Index = 0;
	NSIZE i, j, k;

	FILE* INPname = FileOpen(InputName->InpFile, "r");    // SETTING.INP 파일 읽기

	LOOP(i, 100) ValueBuffer[i] = 0.0;      // 개쩐다 ㅎㄷㄷ....... 

	LineFeed(INPname, 4);       // 조금더 이해가 필요..
	/*
	if(fgets(buffer, MAX_STRING, INPname) == NULL) Message("\n.. ReadInput Error");
	PutFileInfo( InputName, buffer );
	*/
	for (i = 0; !feof(INPname); i++)
	{
		fgets(buffer, MAX_STRING, INPname);

		if (buffer[0] == '|')
		{
			LineFeed(INPname, 2);
			fgets(buffer, MAX_STRING, INPname);

			for (j = 0; j < 4; j++)
			{
				memcpy(StrBuffer, &buffer[j * 20 + 1], 19);

				if (strstr(StrBuffer, "                   ") == NULL)
					ValueBuffer[Value_Index++] = atof(StrBuffer);
			}
		}

		else if (buffer[0] == '[')
		{
			if (strstr(buffer, "[NUM_DATA]"))
			{
				pStr = strstr(buffer, "=");
				NumberOfData = (NSIZE)atof(++pStr);
				LineFeed(INPname, 1);
			}
			else if (strstr(buffer, "DATA FILE"))
			{
				pStr = strtok(buffer, "\"");
				pStr = strtok(NULL, "\"");
				strcpy(InputName->DataFile[0], pStr);

				LOOP(k, 5)
				{
					if (strstr(fgets(buffer, MAX_STRING, INPname), "DATA FILE"))
					{
						pStr = strtok(buffer, "\"");
						pStr = strtok(NULL, "\"");
						strcpy(InputName->DataFile[k + 1], pStr);
					}
					else
					{
						Message("\n\t Check the Number of Data File...");
					}
				}
			}
		}
	}
};

/*-----------------------------------------------------------------------------------*/

void  PutFileInfo(FILE_FORMAT* InputName, char* buffer)
{
	char* token;
	char bufVer[20] = { (char)NULL, }, bufName[20] = { (char)NULL, }, bufDate[20] = { (char)NULL, };
	char seps[500];
	char msg[MAX_STRING];

	strcpy(seps, " |");

	token = strtok(buffer, seps);
	memcpy(bufVer, token, sizeof(bufVer));

	token = strtok(NULL, seps);
	memcpy(bufName, token, sizeof(bufName));

	token = strtok(NULL, seps);
	memcpy(bufDate, token, sizeof(bufDate));

	sprintf(msg, "\n\t\t Loading -> [ %14s | % 14s | %5s ]", FileToken(InputName->InpFile), bufName, bufVer);
	printf(msg);
}

/*-----------------------------------------------------------------------------------*/

#endif