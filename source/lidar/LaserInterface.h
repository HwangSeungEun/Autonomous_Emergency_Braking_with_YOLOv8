/*-----------------------------------------------------------------------------------*/
/* FILE			: LASERINTERFACE.H      											 */
/* MODULE		: LASERINTERFACE   											         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_LASERINTERFACE
#define HEADER_LASERINTERFACE

#include "Function.h"
#include "Unit.h"
#include "Declaration.h"

/*-----------------------------------------------------------------------------------*/
/* Memory Write Buffer                                                               */
/*-----------------------------------------------------------------------------------*/

static REAL			bufxCoordinate[BUF_LENGTH][MAX_LASER_POINT];
static REAL			bufyCoordinate[BUF_LENGTH][MAX_LASER_POINT];
static REAL			bufdist[BUF_LENGTH][MAX_LASER_POINT];

static REAL			bufTimeLaserIn[BUF_LENGTH];
static REAL			bufTimeLaserOut[BUF_LENGTH];

/*-----------------------------------------------------------------------------------*/
/* Definitions                                                                       */
/*-----------------------------------------------------------------------------------*/

#define	BUFSIZE		512
#define SERVERIP				(   IP)("192.168.0.1")   // SOPAS 프로그램에서 주소 확인가능
//#define SERVERIP		"192.168.0.1"

//#define SERVERIP		        (   IP)(  "169.254.229.197")
//#define SERVERIP		        (   IP)(  "203.252.117.101")
//#define SERVERIP		        (   IP)(  "169.254.149.248")

#define SERVERPORT		        ( PORT)(              2111)    //LMS511 PORT
//#define SERVERPORT		        ( PORT)(              0002)

#define MAX_LASER_RECEIVE		(NSIZE)(		   16*1024)
#define MAX_LASER_SEND			(NSIZE)(		       100)
#define MAX_SCAN_MODE			(NSIZE)(		         4)

#define RANGE					( DOU )(				18 )
//#define ANGLE					( DOU )(				350)
#define ANGLE					"90_1"
#define VECHICLE				"AVAN"

#define DIRECTORY				"Data_Check"  // "RESOLUTION_0_1"
#define LIDAR_POSITION			"RL"
#define TRIAR					"201"


/*-----------------------------------------------------------------------------------*/
/* Function Declaration                                                              */
/*-----------------------------------------------------------------------------------*/

void  Laser_Interface(void);

void  lsr_Initialize(void);

void  lsr_OutputData(void);

void  lsr_MemWrite(void);
void  lsr_WriteData(void);

void  lsr_SocketInit(void);
void  lsr_Opensocket(void);
void  lsr_ConnectSocket(void);
void  lsr_DeviceLogin(void);
void  lsr_ConfScanFreq(void);

void  lsr_ReceiveData(void);

void  lsr_CloseSocket(void);



/*-----------------------------------------------------------------------------------*/
/* Interface [INPUT]                                                                 */
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/* Interface [OUTPUT]                                                                */
/*-----------------------------------------------------------------------------------*/
//struct.h의 OUT_LASER구조체 안의 변수들과 같은 이름으로 선언

 REAL  xCoordinate[MAX_LASER_POINT];
 REAL  yCoordinate[MAX_LASER_POINT];
 REAL  angle[MAX_LASER_POINT];
 REAL  dist[MAX_LASER_POINT];




/*-----------------------------------------------------------------------------------*/
/* LOCAL                                                                             */
/*-----------------------------------------------------------------------------------*/

static WSADATA		InitWinSock;
static SOCKADDR_IN	LaserClientAddress;
static SOCKET		LaserSocket;


static unsigned char ReceiveBuffer[MAX_LASER_RECEIVE];
static unsigned char SendBuffer[MAX_LASER_SEND];

//static REAL			Ts;

static int	        SkipData[MAX_SCAN_MODE] = { 60, 30, 30, 15 };
static int	        SaveData[MAX_SCAN_MODE] = { 640, 320, 320, 160 };
static REAL	        ScanRes[MAX_SCAN_MODE] = { 0.25, 0.5, 0.5, 1.0 }; // [deg]

static int			DEC_Count;
static int			DEC_Dist;
static char* ASCII_Count;
static char* pToken = NULL;
static char* next_data;
static char* pSeparator = " ";


 REAL			xCoordinate[MAX_LASER_POINT];
 REAL			yCoordinate[MAX_LASER_POINT];


static COUNT	    lsrCount = 0;
/*-----------------------------------------------------------------------------------*/

#endif


