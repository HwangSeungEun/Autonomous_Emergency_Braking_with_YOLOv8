/*-----------------------------------------------------------------------------------*/
/* FILE			: RADARINTERFACE.H      											 */
/* MODULE		: RADARINTERFACE   											         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 3. 3.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_RADARINTERFACE
#define HEADER_RADARINTERFACE

#include "Function.h"
#include "Unit.h"
#include "Declaration.h"
#include "nixnet.h"

/*-----------------------------------------------------------------------------------*/
/* Memory Write Buffer                                                               */
/*-----------------------------------------------------------------------------------*/

static REAL			bufxCoordinate[BUF_LENGTH][MAX_LASER_POINT];
static REAL			bufyCoordinate[BUF_LENGTH][MAX_LASER_POINT];
static REAL			bufTimeRadarIn[BUF_LENGTH];
static REAL			bufTimeRadarOut[BUF_LENGTH];

/*-----------------------------------------------------------------------------------*/
/* Definitions                                                                       */
/*-----------------------------------------------------------------------------------*/

#define	BUFSIZE		40000

//#define SERVERIP		(   IP)("192.168.0.1")
//#define SERVERIP	"192.168.0.1"

//#define SERVERIP		        (   IP)(  "192.168.0.1")
//#define SERVERPORT		        ( PORT)(              2111)

#define MAX_LASER_RECEIVE		(NSIZE)(		   16*1024)
#define MAX_LASER_SEND			(NSIZE)(		       100)
#define MAX_SCAN_MODE			(NSIZE)(		         4)


#define ID_RADAR_CONFIGURATION	(   ID)(             0x200)	
#define ID_RADAR_STATE			(   ID)(             0x201)	
#define ID_SPEED_INFORMATION    (   ID)(             0x300)
#define ID_YAW_RATE_INFORMATION	(   ID)(             0x301)





#define ID_TARGET_STATUS		(   ID)(             0x600)	
#define ID_TARGET_INFO_1		(   ID)(             0x701)	
#define ID_TARGET_INFO_2		(   ID)(             0x702)	

#define ID_OBJ_STATE			(   ID)(             0x60A)	
#define ID_OBJ_INFO_1			(   ID)(             0x60B)
#define ID_OBJ_INFO_2			(   ID)(             0x60C)

#define MIN_RANGE_LENGTH        ( CONF)(                50)
#define MAX_RANGE_LENGTH        ( CONF)(               200)

#define MAX_ELEVATION           ( CONF)(                32)

#define BAUD_RATE               ( BAUD)(            500000)


#define NUM_FRAMES 2
/*-----------------------------------------------------------------------------------*/

enum ReceiveMode { MODE_TARGET, MODE_OBJECT, MODE_CONF };

/*-----------------------------------------------------------------------------------*/
/* Structures                                                                        */
/*-----------------------------------------------------------------------------------*/

typedef struct 
{
	u8		row[8];
} MESSAGE;

/*-----------------------------------------------------------------------------------*/

typedef struct
{
	char Timestamp[20];

	LOGIC    NVMwriteStatus;
	LOGIC    NVMreadStatus;
	REAL     currRangeLengthCal; //50~200[m]
	REAL     currElevationCal;   //0~32[deg]
	LOGIC    RadarPowerReduction;
	LOGIC    CurrentRadarPower;
	LOGIC    SupVolt_L;
	MODE     RxInvalid;
	LOGIC    SensDef;
	LOGIC    SensTempErr;
	
} CONFIG_MESSAGE;

/*-----------------------------------------------------------------------------------*/

typedef struct _TARGET_MESSAGE
{
	char	 Timestamp[20];

	int		NoOfTargetsNear;
	int		NoOfTargetsFar;

	//	int		NumOfTarget_1;
	REAL	Tar_Dist_ms[96];
	REAL	Tar_Ang_ms[96];
	REAL	Tar_Vrel_ms[96];
	REAL	Tar_Vrel[96];
	REAL	Tar_Dist[96];

	//	int		NumOfTarget_2;
	int		Tar_PdH0[96];
	REAL	Tar_Length[96];
	REAL	Tar_Width[96];
	int		Tar_Ang_stat[96];
	int		Tar_Type[96];
	REAL	Tar_Ang[96];
	REAL	Tar_RCSValue[96];

	struct _TARGET_MESSAGE* pNext;
	

} TARGET_MESSAGE;

/*-----------------------------------------------------------------------------------*/

typedef struct _OBJECT_MESSAGE
{
	char	 Timestamp[20];

	int		NoOfObjects;
	int		MeasCounter;
	int		InterfaceVersionNumber;

	int		Obj_RolCount[40];
	int		Obj_ID[40];
	double	Obj_LongDispl[40];
	double	Obj_VrelLong[40];
	double	Obj_AccelLong[40];
	int		Obj_ProbOfExist[40];
	int		Obj_DynProp[40];
	double	Obj_LatDispl[40];
	int		Obj_Length[40];
	int		Obj_Width[40];
	int		Obj_MeasStat[40];

	double	Obj_RCSValue[40];
	double	Obj_LatSpeed[40];
	int		Obj_ObstacleProbability[40];

	int		MajorReleaseNumber;
	int		MinorReleaseNumber;
	int		PatchLevel;
	
	struct _OBJECT_MESSAGE* pNext;
	

} OBJECT_MESSAGE;

/*-----------------------------------------------------------------------------------*/

typedef struct _RADAR
{
	/*---------------------General----------------------*/

	nxSessionRef_t  SessionRef;
	nxStatus_t		Status;
	nxFrameCAN_t	*FramePtr;
	
	char *pSelectedInterface;
	char *pFrameArray;
	char *pSelectedDatabase;
	char *pSelectedCluster;
	char *pSelectedFrame;
		
	int NumBytes;
	char Timestamp[20];

	u8 Flags;
	u8 Type;
	u32 NumberOfBytesForFrames;
	u8 Buffer[BUFSIZE];
	
	u32 Baud;
	u8 Config[4];
	
	COUNT  idxTarget1;	
	COUNT  idxTarget2;

	COUNT  idxObject1;
	COUNT  idxObject2;

	/*---------------------Config-----------------------*/

	CONFIG_MESSAGE ConfigMessage;

	/*---------------------Target-----------------------*/

	//RAW_TARGET_MESSAGE RawTarget;
	TARGET_MESSAGE TargetBuff[2];
	TARGET_MESSAGE* pTargetBuff;
	TARGET_MESSAGE* pTargetMessage;

	/*---------------------Object-----------------------*/

	//RAW_OBJECT_MESSAGE RawObjectMessage;
	OBJECT_MESSAGE ObjectBuff[2];
	OBJECT_MESSAGE* pObjectBuff;
	OBJECT_MESSAGE* pObjectMessage;
	
}RADAR;

typedef struct _RDR
{
	/*---------------------General----------------------*/

	nxSessionRef_t  SessionRef;
	nxStatus_t		Status;
	nxFrameCAN_t	*FramePtr;

	char *l_pSelectedInterface ;
	char *l_pSelectedFrameList ;
	char *l_pSelectedDatabase ;
	char *l_pSelectedCluster ;

	u32 l_NumberOfBytesForFrames ;
	u8 l_Buffer[NUM_FRAMES * sizeof(nxFrameCAN_t)];

	nxFrameVar_t *l_pFrame ;
	nxStatus_t l_Status;
	nxSessionRef_t m_SessionRef;

}RDR;

/*-----------------------------------------------------------------------------------*/
/* Function Declaration                                                              */
/*-----------------------------------------------------------------------------------*/


void  Radar_Interface(void);
//void  Rdar_Interface(void);

char* PrintTimestamp(nxTimestamp_t * time);
void PrintStat(nxStatus_t l_status, char *source, int idx);

void rdr_Initialize(void);

void rdr_ConfigParamConv(void);
void rdr_Configuration(void);
void rdr_CheckConfigResult(void);
void rdr_SaveConfResult(RADAR* Radar);


void rdr_ReceiveData(void);
//void rdr_SaveConfResult(nxFrameCAN_t* FramePtr);



void rdr_OutputData(void);




void CloseCANStream();

void CANInit();


/*-----------------------------------------------------------------------------------*/
/* Interface [INPUT]                                                                 */
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/* Interface [OUTPUT]                                                                */
/*-----------------------------------------------------------------------------------*/
//struct.h의 OUT_RADAR구조체 안의 변수들과 같은 이름으로 선언

 REAL  xCoordinate[MAX_LASER_POINT];
 REAL  yCoordinate[MAX_LASER_POINT];


/*-----------------------------------------------------------------------------------*/
/* LOCAL                                                                             */
/*-----------------------------------------------------------------------------------*/

RADAR Radar[2];	// Radar 1개만 사용함.
RADAR Radar1;	

RDR	  Rdr;
/*-----------------------------------------------------------------------------------*/

static COUNT	    rdrCount = 0;

/*-----------------------------------------------------------------------------------*/

#endif


