int arr[] =
{128, 64, 32, 16, 8, 4, 2, 1};

#undef move
#define READBIT(byte,pos) byte & (128 >> pos)
#define VER "1.8"

int i, fs = 16, cnY, cnX, ocY, ocX, key, tmp, cnt, hlp = 1, w = 2;
unsigned char ASC = 1, SET = 1, DET = 1, barX = 1, barY = 1;
unsigned char obX = 1, obY = 1, FDE = 1, m = 1, om = 1, fd, COL = 1;
unsigned char cpaste[15], chars[255], font[4096], buf[8192];

char *Asc[8], *files[500];

char *cmds[32] =
{"%C1%W:%c  Save current font",
 "%C2%W:%c  Load a new font",
 "%C3%W:%c  Toggle Wrap mode",
 "%C4%W:%c  Clear character (editor)",
 "%C5%W:%c  Inverse",
 "%C6%W:%c  Flip Y-vertex (Up/Down)",
 "%C7%W:%c  Flip X-vertex (Left/Right)",
 "%CC%W:%c  Copy character",
 "%CP%W:%c  Paste character",
 "%CF1%W:%c  Save font as .pcf",
 "%CTab%W:%c  Switch windows",
 "%CSpace%W:%c  Toggle pixel (On/Off)",
 "%CEsc/Q%W:%c  Quit Fonter (Why??)",
 "%CEnter%W:%c  Return to Editor",
 "%CArrows%W:%c  Move cursor",
 "%CDel%W:%c  Second help screen",
 "%CIns%W:%c  First help screen",
 "%CEnd%W:%c  Same as down, but from editor",
 "%CHome%W:%c  Same as up, but from editor",
 "%CPage Up%W:%c  Go to the previous char",
 "%CPage Down%W:%c  Go to the next char",
 "%CO%W:%c  Restore original font page",
 "%CR%W:%c  Restore original char",
 "%CA%W:%c  Shrink Y-vertex",
 "%CS%W:%c  Shrink X-vertex (buggy)",
 "%CH%W:%c  Scoot char left",
 "%CL%W:%c  Scoot char right",
 "%CK%W:%c  Scoot char up",
 "%CJ%W:%c  Scoot char down",
 "%CI%W:%c  Program information",
 "%C/%W:%c  Jump to character",
 "%C%W%c"};

int main(int argc, char **argv);
void line();
void endprog();
void error(char *er);
void init();
void clr();
void drawbox(int y0, int x0, int dy, int dx, char *str);
void drwscrn();
void printz(char *str,...);
void move_to(int a0, int a1);
void mvprintz(int a0, int a1, char *str);
void charz(int a, char *ch);
void updateboxes();
void setfont();
void getfont();
void setupchars();
void enlarge(int charnum);
void inverse();
void space();
void clearit();
void copychar();
void pastechar();
void flipud();
void fliprl();
void goleft();
void goright();
void goup();
void godown();
void backup();
void status(char *str);
void asciido();
void reschar();
void shrinkud();
void shrinkrl();
int loadfiles();
void info();
void filemenu();
void save();
void savepcf();
