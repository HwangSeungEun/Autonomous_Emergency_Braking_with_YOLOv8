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
	INIT_BUF(DataTime); //DataTime�� �޸� �ʱ�ȭ :  memset(numPtr, 0, )

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


	strcpy(InitName, "TARGET.INI"); //�������� TARGET.INI ������ ������.              

	INI_File = FileOpen(InitName, "r"); // READ���� ������ OPEN�ϰ� INI_FILE�� �������� �����Ѵ�.

	puts("\n\n\t\t =========== Target Tracking Filter Program ==========="); // �ش� ������ ����Ѵ�.

	while (!feof(INI_File))  // TARGET.INI ���� ������ ������ �����Ѵ�.
	{
		if (fgets(buffer, MAX_STRING, INI_File) == NULL) break; // ������ ������� �� Ż���Ѵ�.

		if (strstr(buffer, "[MAIN PROGRAM PATH & FILE]")) GetFileNameSingle(buffer, MainExeFile);  // strstr: ���ڿ� ������ Ư�� ���� �˻��� buffer�� ����, getfilenamesingle���� 
		else if (strstr(buffer, "-SETTING")) GetFileName(buffer, &InputFileName.ProgramSetting);
		else if (strstr(buffer, "-SENSOR")) GetFileName(buffer, &InputFileName.LaserConfig);
		else if (strstr(buffer, "-RADAR")) GetFileName(buffer, &InputFileName.RadarConfig);
	}

	fclose(INI_File);    // ���� �ݱ�
};

/*-----------------------------------------------------------------------------------*/
/* 3. Setting File Loading                                                           */
/*-----------------------------------------------------------------------------------*/

void    LoadSettingFile(void)
{
	register INDEX i = 0;

	ReadInput(&InputFileName.ProgramSetting, ValueBuffer);    // SETTING.INP���� 

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

	ReadInput(&InputFileName.LaserConfig, ValueBuffer);    // SENSOR.INP����

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

	temp = strtok(Srcbuffer, sep);  //  TARGET.INI���Ͽ��� �˻��� Ư�� ���ڿ��� ū����ǥ(")�� �������� �ڸ���.
	temp = strtok(NULL, sep);       //  ���ڿ��� �ڸ��ڿ��� NULL�� �־������! (�ڵ����� ����)
	strcpy(Filename, temp);         //  Filename�� ����

	if (strlen(temp) < 1)  return NOK;   // ���� ������� �ʾ��� �� 0�� ��ȯ
	else                   return YOK;   // ���� ���� �� 1 ��ȯ
}
/*-----------------------------------------------------------------------------------*/

void  ReadInput(FILE_FORMAT* InputName, REAL* ValueBuffer)
{   //���ذ� �ʿ��մϴ�..
	char buffer[MAX_STRING] = { (char)NULL, };
	char StrBuffer[20] = { (char)NULL, };
	char* pStr;

	NSIZE NumberOfData = 0;
	NSIZE Value_Index = 0;
	NSIZE i, j, k;

	FILE* INPname = FileOpen(InputName->InpFile, "r");    // SETTING.INP ���� �б�

	LOOP(i, 100) ValueBuffer[i] = 0.0;      // ��¾�� ������....... 

	LineFeed(INPname, 4);       // ���ݴ� ���ذ� �ʿ�..
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