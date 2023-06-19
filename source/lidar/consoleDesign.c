#include "consoleDesign.h"



void initConsole(void)
{


    // Console Title
    SetConsoleTitleA(CON_NAME);

    // Console Font
    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = CON_FONTSIZE;
    info.dwFontSize.X = CON_FONTSCALE * CON_FONTSIZE;
    wcscpy(info.FaceName, CON_FONT);
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);

    // Console Window Position
    MoveWindow(GetConsoleWindow(), CON_OX, CON_OY, CON_W, CON_H, FALSE);

    // Console Color
    char colorcommand[20] = { '\0', };
    sprintf(colorcommand, "color %x%x", CON_BACKCOLOR, CON_TEXTCOLOR);
    system(colorcommand);


    // Console Mouse Lock
    DWORD consoleModePrev;
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(handle, &consoleModePrev);
    SetConsoleMode(handle, consoleModePrev & ~ENABLE_QUICK_EDIT_MODE);


}
 
void readSettingData(void)
{
    extern int cport_nr;

    #define MAX_STRING 100
    FILE* pFile = fopen("../../../fileIn/parameter/PARM_CONSOLE.csv", "r");

    if (!pFile) exit(0);


    char 		line[MAX_STRING], name[MAX_STRING / 2], val[MAX_STRING / 2];

    while (!feof(pFile))
    {
        if (!fgets(line, MAX_STRING, pFile) || (line[0] == '\n')) continue;
        sscanf(line, "%[^,] , %[^,]", name, val);

        if (!strcmp(name, "CON_FONTSIZE")) 	CON_FONTSIZE = atoi(val);
    }
    // File Close
    fclose(pFile);
}


