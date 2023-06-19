
#include "Include.h"

/*-----------------------------------------------------------------------------------*/
/* Shared Memory for Naviagtion                                                              */
/*-----------------------------------------------------------------------------------*/

/*=============== Transmitter ===============*/
void SharedMem_CREATE(void)
{

	TCHAR SERV_NAME[] = TEXT("Lidar_smdat_ReadData");

	hMampF_LASER = NULL;

	hMampF_LASER = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(OUT_LASER),
		SERV_NAME);

	if (hMampF_LASER == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}

	laser_smdat = (OUT_LASER*)MapViewOfFile(hMampF_LASER, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(OUT_LASER));

	printf("\n SM created_\n");

	//getchar();
}

/*=============== Receiver ===============*/

void SharedMem_OPEN(void)
{
	TCHAR SERV_NAME[] = TEXT("SERV_smdat");

	hMampF_LASER = OpenFileMapping(
		FILE_MAP_READ,   // read/write access
		FALSE,                 // do not inherit the name
		SERV_NAME);               // name of mapping object RP_SMDAT

	if (hMampF_LASER == NULL)
	{
		_tprintf(TEXT("1. Could not open file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}

	laser_smdat = (OUT_LASER*)MapViewOfFile(hMampF_LASER, FILE_MAP_READ, 0, 0, sizeof(OUT_LASER));

	if (laser_smdat == NULL)
	{
		_tprintf(TEXT("2. Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMampF_LASER);
		return 1;
	}

	printf("sm_connect\n");

}

void SharedMem_RECV(void)
{
	memcpy(laser_smdat->dist, dist, sizeof(dist));
	//memcpy(laser_smdat->angle, angle, sizeof(angle));

	//printf("shared distance: ");

	//for(int i = 0; i< 641; i++)
	//printf(" %f", dist[i]);

	//printf("/n");
}

/*-----------------------------------------------------------------------------------*/
