
#include "Include.h"
#include "Declaration.h"

/*-----------------------------------------------------------------------------------*/


void main(void)
{

	//readSettingData();
	//initConsole();

	int count = 0;
	ReadData();       // 파일에서 기본 세팅을 위한 정보를 불러온다.
	Initialize();    // shared memory create

	while (!Manage.StopCond)      // 논리값이 참이면 멈춘다. => CheckStop에서 확인가능
	{
		
		Laser_Interface();

		MemWrite();

		Manage.StopCond = CheckStop();

		count++;

		CheckTime();

	}

	WriteData();    //데이터 저장

	printf("\n\tTask Finished\n");

	system("pause");

	return 0;
};
