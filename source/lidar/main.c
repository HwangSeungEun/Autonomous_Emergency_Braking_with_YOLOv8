
#include "Include.h"
#include "Declaration.h"

/*-----------------------------------------------------------------------------------*/


void main(void)
{

	//readSettingData();
	//initConsole();

	int count = 0;
	ReadData();       // ���Ͽ��� �⺻ ������ ���� ������ �ҷ��´�.
	Initialize();    // shared memory create

	while (!Manage.StopCond)      // ������ ���̸� �����. => CheckStop���� Ȯ�ΰ���
	{
		
		Laser_Interface();

		MemWrite();

		Manage.StopCond = CheckStop();

		count++;

		CheckTime();

	}

	WriteData();    //������ ����

	printf("\n\tTask Finished\n");

	system("pause");

	return 0;
};
