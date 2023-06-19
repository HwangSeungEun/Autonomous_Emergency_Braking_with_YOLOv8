/*-----------------------------------------------------------------------------------*/
/* FILE			: MANAGE.C				                                             */
/* MODULE		: MANAGE										                     */
/* AUTHOR		: HGU SASLAB 								                         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef SOURCE_MANAGEMENT
#define SOURCE_MANAGEMENT

/*-----------------------------------------------------------------------------------*/

#include <time.h>
#include <Windows.h>
#include "Function.h"
#include "Unit.h"
#include "Common.h"
#include "Declaration.h"
#include "Manage.h"

/*-----------------------------------------------------------------------------------*/
/* 1. Check Period                                                                   */
/*-----------------------------------------------------------------------------------*/

LOGIC CheckPeriod(REAL Ts)
{

	if (!((int)(Time.Run / DataTime.Tf + SIGN_VAL(0.5, Time.Run))
		% (int)((Ts / DataTime.Tf + 0.5)))) return YOK;
	else                                                               return NOK;

	//    return (ROUND(Time.Flight*10000) % ROUND(Ts*10000)) ? NOK : YOK ;
}

/*-----------------------------------------------------------------------------------*/
/* 2. Check Mission & Flight Time                                                    */
/*-----------------------------------------------------------------------------------*/

void CheckTime(void)
{
	Time.curr = GetWindowTime() * UNIT_Ms2Sec;
	if (Time.count) Time.Process = Time.prev - Time.curr;

	/* wait for sampling time */
	while (Time.curr - Time.prev < DataTime.Tf)
		Time.curr = GetWindowTime() * UNIT_Ms2Sec;     // 샘플링타임까지 남은 시간을 기다린다.
	Time.prev = Time.curr;                             // 현재시점을 이전시점으로 업데이트

	Time.Run = (REAL)(Time.count++) * DataTime.Tf + DataTime.Tstart;  // 실행시간 체크
	if (fmod(Time.Run, 1.0) == 0)
	{
		printf("\n\t%0.4f\t", Time.Run);
	}
	WRT(bufRunTime, Time.Run);



}

/*-----------------------------------------------------------------------------------*/
/* 3. Check Missile Phase                                                            */
/*-----------------------------------------------------------------------------------*/

void  CheckPhase(void)
{
	register COUNT  j = 0;

	if (Time.Run < -3.0)
	{

	}
	else if (Time.Run < -2.0)
	{

	}
	else if (Time.Run < 0.0)
	{

	}
	else
	{

	}
}


/*-----------------------------------------------------------------------------------*/
/* 4. Check Stop Condition                                                           */
/*-----------------------------------------------------------------------------------*/

LOGIC  CheckStop(void)
{
	char* MsgBuf[] = { "\n\n\t\t Go Next Loop...\n",
		"\n\n\t\t Terminated by 'Final Time' condition...\n",
		//"\n\n\t\t Terminated by 'Error' condition...\n"
		"\n\n\t\t Error : "
	};

	char* ErrBufLsr[] = { "LSR_ERR_INIT_SOCKET\n",
						 "LSR_ERR_OPEN_SOCKET\n",
						 "LSR_ERR_CONNECT_SOCKET\n"
	};

	COUNT  j = 0;
	LOGIC  StopCondition;

	if (Time.Run == 0.0)
		j = 0;
	//printf("DataTime.Tfinal : %f\n", DataTime.Tfinal);
	if (Time.Run > DataTime.Tfinal)		StopCondition = FINAL_TIME;
	else if (Error.Detection == YOK)	StopCondition = ERR_DETECT;
	else								StopCondition = GO_NEXT;

	/* stop message */
	if (StopCondition != GO_NEXT)
	{
		//STR_MSG("\n\n\t\t ================= Simulation Result ================= \n");
		STR_MSG(MsgBuf[StopCondition]);
		LOOP(j, Error.CountLsr)	STR_MSG(ErrBufLsr[Error.CodeLsr[j]]);

		//printf("\n\n STOP!! \n\n");
	}

	return StopCondition;
}

/*-----------------------------------------------------------------------------------*/

#endif
