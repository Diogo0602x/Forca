#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

typedef enum {
   BLACK,
   BLUE,
   GREEN,
   CYAN,
   RED,
   MAGENTA,
   BROWN,
   LIGHTGRAY,
   DARKGRAY,
   LIGHTBLUE,
   LIGHTGREEN,
   LIGHTCYAN,
   LIGHTRED,
   LIGHTMAGENTA,
   YELLOW,
   WHITE
} COLORS2;

enum text_modes2 {  
   LASTMODE=-1,
   BW40=0,
   C40,
   BW80,
   C80,
   MONO=7,
   C4350=64 
};

typedef
struct text_info2 {
   unsigned char winleft;        /* left window coordinate */
   unsigned char wintop;         /* top window coordinate */
   unsigned char winright;       /* right window coordinate */
   unsigned char winbottom;      /* bottom window coordinate */
   unsigned char attribute;      /* text attribute */
   unsigned char normattr;       /* normal attribute */
   unsigned char currmode;       /* BW40, BW80, C40, C80, or C4350 */
   unsigned char screenheight;   /* text screen's height */
   unsigned char screenwidth;    /* text screen's width */
   unsigned char curx;           /* x-coordinate in current window */
   unsigned char cury;           /* y-coordinate in current window */
} text_info2;
text_info2 vActual2 = {0, 0, 79, 24, WHITE, WHITE, C80, 25, 80, 1, 1};

/* Funcoes Borland: */
void clrscr(void);
void gotoxy(int, int);
int kbhit(void);
void _setcursortype(int cur_t);
void textbackground(int newcolor);
void textcolor(int newcolor);

void clrscr () {
   DWORD escrito;
   int i;

   for( i = 0; i < vActual2.screenheight; i++) {
      FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
         vActual2.attribute, vActual2.screenwidth,
         (COORD) {vActual2.winleft, vActual2.wintop+i}, &escrito);
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
         vActual2.screenwidth,
         (COORD) {vActual2.winleft, vActual2.wintop+i}, &escrito);
   }
   gotoxy(vActual2.curx, vActual2.cury);
}

void gotoxy(int x, int y) {
   COORD c;

   if(x < 1 || x > vActual2.screenwidth || 
      y < 1 || y > vActual2.screenheight) return;
   vActual2.curx = x;
   vActual2.cury = y;
   c.X = vActual2.winleft + x - 1;
   c.Y = vActual2.wintop + y - 1;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int kbhit(void) {
   DWORD nEventos;
   INPUT_RECORD *eventos;
   DWORD leidos;
   int retval = 0;
   int i;
   
   GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE),
      &nEventos);
   // eventos = new INPUT_RECORD[nEventos];
   eventos = (INPUT_RECORD *)malloc(nEventos*sizeof(INPUT_RECORD));
   PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE),
      eventos, nEventos, &leidos);
   for(i = 0;  i < nEventos; i++)
      if(eventos[i].EventType == KEY_EVENT && eventos[i].Event.KeyEvent.bKeyDown) retval = 1;
   // delete[] eventos;
   free( eventos );
   return retval;
}

// _NOCURSOR	Turns off the cursor
// _NORMALCURSOR	Normal underscore cursor
// _SOLIDCURSOR	Solid block cursor
void _setcursortype(int cur_t) {
   CONSOLE_CURSOR_INFO cci;

   switch(cur_t) {
      case _NOCURSOR:
         cci.dwSize = 100;
         cci.bVisible = FALSE;
         break;
      case _SOLIDCURSOR:
         cci.dwSize = 100;
         cci.bVisible = TRUE;
         break;
      case _NORMALCURSOR:
         cci.dwSize = 10;
         cci.bVisible = TRUE;
         break;
   }
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void textbackground(int newcolor) {
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
      (csbi.wAttributes & 0x0f) | (newcolor << 4));
   vActual2.attribute = (csbi.wAttributes & 0x0f) | (newcolor << 4);
}

void textcolor(int newcolor) {
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
      (csbi.wAttributes & 0xf0) | newcolor);
   vActual2.attribute = (csbi.wAttributes & 0xf0) | newcolor;
}

#ifdef __cplusplus
}
#endif
