/*-----------------------------------------------------------------------------------*/
/* FILE			: LASERINTERFACE.C      											 */
/* MODULE		: LASERINTERFACE   											         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef SOURCE_LASERINTERFACE
#define SOURCE_LASERINTERFACE

/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "Unit.h"
#include "Function.h"
#include "Common.h"
#include "Declaration.h"
#include "LaserInterface.h"
#include "SharedMem.h"
#include "manage.h"
#pragma comment(lib, "wsock32.lib")


/*-----------------------------------------------------------------------------------*/
/* Laser Interface Main Module                                                       */
/*-----------------------------------------------------------------------------------*/

void  Laser_Interface(void)
{
	if (DataLaser.USE == NOK) return;
	if (!CheckPeriod(DataTime.Ts))  return;

	//printf("gogo\n");
	lsr_ReceiveData();

	//lsrCount++;
};

/*-----------------------------------------------------------------------------------*/
/* Initialize                                                                        */
/*-----------------------------------------------------------------------------------*/

void  lsr_Initialize(void)
{
	if (DataLaser.USE == NOK) return;    // INP���Ͽ��� �ҷ����� ������. NOK�� �� �����.

	DEC_Count = 0;
	DEC_Dist = 0;


	lsr_SocketInit();
	lsr_Opensocket();
	lsr_ConnectSocket();
	lsr_DeviceLogin();
	lsr_ConfScanFreq();

	lsr_OutputData();
};

/*-----------------------------------------------------------------------------------*/
/* Input Data                                                                        */
/*-----------------------------------------------------------------------------------*/

/*
void  lsr_InputData(void)
{
};
*/

/*-----------------------------------------------------------------------------------*/
/* Output Data                                                                       */
/*-----------------------------------------------------------------------------------*/
//LaserInterface.c��⿡�� �ٸ� ���� ������ DATA���� ���� buffer�� �̵�

void  lsr_OutputData(void)
{
	//printf("lsr_OutputData \n");
	COUNT i;
	LOOP(i, MAX_LASER_POINT)
	{
		OUVEC(bfOutLaser, xCoordinate);
		OUVEC(bfOutLaser, yCoordinate);
	}

};

/*-----------------------------------------------------------------------------------*/
/* Data Memory Write                                                                 */
/*-----------------------------------------------------------------------------------*/

void  lsr_MemWrite(void)
{
	register COUNT i, j;  // register ����ϸ� �޸� ��� CPU�� �������͸� ��� -> �Ϲݺ������� ������, �ݺ�Ƚ���� ���� �� ������

	if (DataLaser.USE == NOK) return;

	//VEC_WRT(bufdist, dist);

	// ���� �޸𸮿� ����

	memcpy(laser_smdat->dist, dist, sizeof(dist));

};

/*-----------------------------------------------------------------------------------*/
/* Data Print                                                                        */
/*-----------------------------------------------------------------------------------*/


void lsr_WriteData(void)
{
	//extern REAL bufTflight[BUF_LENGTH];
	char FileName[50];
	char timeName[50];
	register COUNT i, j;
	FILE* pFile;
	FILE* tFILE;

	if (DataLaser.USE == NOK) return;


	sprintf(FileName, "raw_data_210720.txt");
	sprintf(timeName, "realtime_210720.txt");

	fopen_s(&pFile, FileName, "w");
	fopen_s(&tFILE, timeName, "w");

	//---------------------------------------------------------------------------//
	LOOP(i, Time.count)
	{

		VEC_PRT(bufdist, 1.0);
		STR_PRT("\n");	

		fprintf(pFile, "\n\n\n");
	}

	//---------------------------------------------------------------------------//

	LOOP(i, Time.count)
	{
		TIME_PRT(bufTimeLaserIn, 1.0);
		TIME_PRT(bufTimeLaserOut, 1.0);
		TIME_PRT(bufRunTime, 1.0);
	}

	fclose(pFile);
	fclose(tFILE);
};

/*-----------------------------------------------------------------------------------*/
/* Socket Initialize                                                                 */
/*-----------------------------------------------------------------------------------*/

void lsr_SocketInit(void)
{
	if (Error.Detection == YOK) return;  

	if (WSAStartup(MAKEWORD(2, 2), &InitWinSock) != 0)  //winsock ���α׷��� - �ʱ�ȭ �ܰ�: ���� �� 0��ȯ, ���� �� 0�� �ƴ� �ڵ� ��ȯ. ||  ���� ���� 2.2
	{
		Error.Detection = YOK;
		Error.CodeLsr[Error.CountLsr++] = LSR_ERR_INIT_SOCKET;
	}

	printf("\nSocketInit Completed\n");
}

/*-----------------------------------------------------------------------------------*/
/* Socket Open                                                                       */
/*-----------------------------------------------------------------------------------*/

void lsr_Opensocket(void)
{
	if (Error.Detection == YOK) return;

	LaserSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (LaserSocket == INVALID_SOCKET)       // ������ ������ �ʾ��� ��� ���� ���
	{
		Error.Detection = YOK;
		Error.CodeLsr[Error.CountLsr++] = LSR_ERR_OPEN_SOCKET;

		WSACleanup();   // �ʱ�ȭ �ϰ� Ż���Ѵ�.
	}
	else
	{
		printf("\n LASER TCP-IP socket is opened...\n");
		//printf("Isr_Opensocket\n");
	}
};

/*-----------------------------------------------------------------------------------*/
/* Connect Socket                                                                    */
/*-----------------------------------------------------------------------------------*/

void lsr_ConnectSocket(void)
{

	if (Error.Detection == YOK) return;

	memset(&LaserClientAddress, 0, sizeof(LaserClientAddress));     // ���� �ʱ�ȭ

	LaserClientAddress.sin_family = AF_INET;					// bidirectional communication	
	LaserClientAddress.sin_port = htons(SERVERPORT);			// LASER to ethernet port number         htons: ��Ʈ��ũ ����Ʈ ����(���ͳ� �����ϱ�)
	LaserClientAddress.sin_addr.s_addr = inet_addr(SERVERIP);	// server IP address                     inet_addr: ���ڿ� ���·� IPv4�ּҸ� �޾� 32��Ʈ ���ڷ� ��ȯ

	//printf("Connect : %d \n", connect(LaserSocket, (struct sockaddr*) & LaserClientAddress, sizeof(LaserClientAddress)));
	if (connect(LaserSocket, (struct sockaddr*)&LaserClientAddress, sizeof(LaserClientAddress)) == -1)       // ���� ���� ���, ���ϴݰ� clear���ش�.
	{
		Error.Detection = YOK;
		Error.CodeLsr[Error.CountLsr++] = LSR_ERR_CONNECT_SOCKET;

		closesocket(LaserSocket);
		WSACleanup();
		LaserSocket = INVALID_SOCKET;
		printf("lsr_ConnectSocket Error \n");
	}
	else
	{
		printf("\t\t LASER client is connected...\n");
	}
};

/*-----------------------------------------------------------------------------------*/
/* Login to Laser Sensor                                                             */
/*-----------------------------------------------------------------------------------*/

void  lsr_DeviceLogin(void)
{


	if (Error.Detection == YOK)  return;

	switch (DataLaser.LoginMode)      // SETTING.INP ���� �α��� ��尡 1������ �Ǿ��ִ�(Authorized client).  LMS511��ǰ�� telegrams_for_configuring_and_operating ������ Ȯ���ϸ� ��.
	{
	case 0:
		strcpy(SendBuffer, "\02sMN SetAccessMode 02 B21ACE26\03");	//Login to device - Maintenance
		break;

	case 1:
		strcpy(SendBuffer, "\02sMN SetAccessMode 03 F4724744\03");	//Login to device - Authorized client
		break;

	case 2:
		strcpy(SendBuffer, "\02sMN SetAccessMode 04 81BE23AA\03");	//Login to device -	Service
		break;
	}

	send(LaserSocket, (char*)SendBuffer, sizeof(SendBuffer) - 1, 0);      // ����� ����(LMS511)�� SendBuffer�� ����� ���ڿ��� �����Ѵ�.
	recv(LaserSocket, (char*)ReceiveBuffer, sizeof(ReceiveBuffer), 0);    // ����� ����(LMS511)�κ��� �޽����� ���۹޴´�.


	//printf("\n - LOG IN \n [���� ������] %s\n", ReceiveBuffer);

};

/*-----------------------------------------------------------------------------------*/
/* Configure Laser Sensor Scan Frequency & Resolution                                */
/*-----------------------------------------------------------------------------------*/

void  lsr_ConfScanFreq(void)
{
	//printf("Select Scan Freq: 0, 1, 2, 3 \n");
	if (Error.Detection == YOK) return;

	switch (DataLaser.ScanMode - 1)
		//switch (DataLaser.ScanMode)
	{
	case 0:
		//Ts = 1.0 / 25.0;
		strcpy(SendBuffer, "\02sMN mLMPsetscancfg +2500 1 +2500 -50000 +1850000\03");									//	25Hz / 0.25'
		break;

	case 1:
		//Ts = 1.0 / 35.0;
		strcpy(SendBuffer, "\02sMN mLMPsetscancfg +3500 1 +5000 -50000 +1850000\03");									//	35Hz / 0.5'
		break;

	case 2:
		//Ts = 1.0 / 50.0;
		strcpy(SendBuffer, "\02sMN mLMPsetscancfg +5000 1 +5000 -50000 +1850000\03");									//	50Hz / 0.5'
		break;

	case 3:
		//Ts = 1.0 / 75.0;
		strcpy(SendBuffer, "\02sMN mLMPsetscancfg +7500 1 +10000 -50000 +1850000\03");									//	75Hz / 1'
		break;
	}

	send(LaserSocket, (char*)SendBuffer, sizeof(SendBuffer) - 1, 0);
	recv(LaserSocket, (char*)ReceiveBuffer, sizeof(ReceiveBuffer), 0);


	//printf(" -\n - Config Scan Freq \n [���� ������] %s\n", ReceiveBuffer);


	//Measurement output telegram
	strcpy(SendBuffer, "\02sWN LMDscandatacfg 01 00 0 0 0 00 00 0 0 0 0 +1\03");
	send(LaserSocket, (char*)SendBuffer, sizeof(SendBuffer) - 1, 0);
	recv(LaserSocket, (char*)ReceiveBuffer, sizeof(ReceiveBuffer), 0);


	//printf(" - MEASUREMENT OUTPUT TELEGRAM \n [���� ������] %s\n", ReceiveBuffer);

};


/*-----------------------------------------------------------------------------------*/
/* Request Laser Sensor Scan                                                         */
/*-----------------------------------------------------------------------------------*/

void  lsr_ReceiveData(void)
{

	int i, m;

	strcpy(SendBuffer, "\02sRN LMDscandata\03");     // Page.22 Ȯ��
	send(LaserSocket, (char*)SendBuffer, sizeof(SendBuffer) - 1, 0);
	recv(LaserSocket, (char*)ReceiveBuffer, sizeof(ReceiveBuffer), 0);

	pToken = strtok((char *)ReceiveBuffer, pSeparator); // ������ �������� ���ڿ��� �ڸ���.
	

	// ���� ���� ������ (25��), 0~15�� ������ (0.25��: 60��, 0.5��: 30��) skip 
	//for (int i = 1; i<(25 + 60); ++i)	
	//for (i = 1; i < (25 + SkipData[DataLaser.ScanMode-1]); ++i)           //-> 84��
	for (i = 1; i < 25 ; ++i)     
	//for ( i = 1; i<(25 + SkipData[DataLaser.ScanMode]); ++i)
	{
		pToken = strtok(NULL, pSeparator);
	}	

	for (int m = 0; m<761; m++)
	//for (m = 0; m < SaveData[DataLaser.ScanMode - 1]; ++m)
	//for (m = 0; m < SaveData[DataLaser.ScanMode - 1]; ++m)
	//for ( m = 0; m<SaveData[DataLaser.ScanMode]; ++m)	//0.5���ϰ�� m<320, 0.25�� �ϰ�� m<640
	{
		pToken = strtok(NULL, pSeparator);
		DEC_Dist = strtoul(pToken, NULL, 16);  // 16���� ���ڿ��� ������ ��ȯ

		dist[m] = DEC_Dist;
		//printf(" %f \n", dist[m]);
	}
}

/*-----------------------------------------------------------------------------------*/
/* Close Socket                                                                      */
/*-----------------------------------------------------------------------------------*/




void  lsr_CloseSocket(void)
{
	closesocket(LaserSocket);
};

/*-----------------------------------------------------------------------------------*/

#endif