#ifndef _CONSOLEDESIGN
#define _CONSOLEDESIGN

#define _CRT_SECURE_NO_WARNINGS


#include <windows.h>
#include <conio.h>
#include <process.h>

#include <stdio.h>

	// =================================  //
	//              Variables             //
	// =================================  //
	enum CON_COLOR { CON_BLACK, CON_BLUE, CON_GREEN, CON_JADE, CON_RED, CON_PURPLE, CON_YELLOW, CON_WHITE, CON_GRAY, CON_LIGHT_BLUE, CON_LIGHT_GREEN, CON_LIGHT_JADE, CON_LIGHT_RED, CON_LIGHT_PURPLE, CON_LIGHT_YELLOW, CON_LIGHT_WHITE };
#define     CON_NAME           "Heung-gi's LiDAR"
#define     CON_FONT           L"D2Coding" // "Times New Roman" "D2Coding" 
#define     CON_TEXTCOLOR           CON_LIGHT_YELLOW
#define     CON_BACKCOLOR           CON_BLACK
#define     CON_FONTSCALE           0.49
#define     CON_OX                  (int) ( 0.70 * GetSystemMetrics(SM_CXSCREEN) )
#define     CON_OY                  (int) ( 0.66 * GetSystemMetrics(SM_CYSCREEN) )
#define     CON_W                   (int) ( 0.30 * GetSystemMetrics(SM_CXSCREEN) )
#define     CON_H                   (int) ( 0.29 * GetSystemMetrics(SM_CYSCREEN) )


static int CON_FONTSIZE;



// =================================  //
//              function              //
// =================================  //

void initConsole(void);
void readSettingData(void);


#endif