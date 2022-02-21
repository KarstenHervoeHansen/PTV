#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <string.h>
#include <alloc.h>
#include <bios.h>
#include <dir.h>
#include <ctype.h>
#include <math.h>
#include "pm5639.h"

#pragma hdrstop

extern char* SoftwareType;

WINDOW MenuWindow;
RECT MenuRect;
MENU* Menu[MENUCOUNT];

WININFO Win[MAXWINDOW];
WININFO* WinPtr[MAXWINDOW];

int CIECrossSize[MAXDISPLAY+1];
void* CIECrossImage[MAXDISPLAY+1];

int NoWin, ActiveMenu, MenuMode;

int COMopened[2];
int FASTmode;

const char* COMText[] = {
  "COM1", "COM2"
};
const char* ButtonText[] = {
  "OK", "Restore", "Save", "Cancel", "Yes", "No", "Print", "Retry"
};

static int menuindex = 0;

/* Graphics initialization aborted - write message and exit */

void GraphAbort( char* Msg, int Err) {

  printf( "%s%s%s\n", Msg, ': ', grapherrormsg( Err));
  exit( EXIT_FAILURE);
}


/* Turn on graphics */

int initGraphics( void) {

  int GrDriver = EGA;
  int GrMode = EGAHI;
  int ErrCode;

  if ( registerbgidriver( EGAVGA_driver) < 0)
    GraphAbort( "EGA/VGA", graphresult());

  if ( registerbgifont( small_font) < 0 )
    GraphAbort( "SmallFont", graphresult());

  if ( registerbgifont( sansserif_font) < 0)
    GraphAbort( "SansSerifFont", graphresult());

  initgraph( &GrDriver, &GrMode, "");

  if (( ErrCode = graphresult()) != grOk) {
    restorecrtmode();
    GraphAbort( "Init graphics error", ErrCode);
  }

  if ( !strcmp( SoftwareType, "INDU"))
    StartUpScreen( "PM 5639 Industrial CRT Color Analyzer",
            "Philips TV Test Equipment A/S, Denmark", "1992");
  else
    StartUpScreen( "PM 5639 CRT Color Analyzer, PC",
            "Philips TV Test Equipment A/S, Denmark", "1992");

  return( -1);
}


/* initialize the window manager */

void initWindowManager( void) {

  int i, j;
  char menukey[2];
  extern char* BottomBarText;

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  settextstyle( DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify( LEFT_TEXT, TOP_TEXT);
  setusercharsize( 1, 1, 1, 1);

  setfillstyle( ManagerBkFill, ManagerBkColor);
  bar( 0, 0, getmaxx(), getmaxy());

  setfillstyle( ManagerBarFill, ManagerBarColor);
  bar( 0, 0, getmaxx(), MANAGERBARDEEP);
  bar( 0, getmaxy()-MANAGERBARDEEP, getmaxx(), getmaxy());

  setcolor( EGA_BLACK);
  line( 0, MANAGERBARDEEP, getmaxx(), MANAGERBARDEEP);
  line( 0, getmaxy()-MANAGERBARDEEP-2,
                    getmaxx(), getmaxy()-MANAGERBARDEEP-2);
  setcolor( ManagerBarText);
  outtextxy( 0, getmaxy()-MANAGERBARDEEP+1,
                "  Alt+X: Exit Program   F1: Help   F10: Menu  ");
  menukey[1] = 0;

  for ( i=0; i < menuindex; ++i) {
    setcolor( EGA_BLACK);
    outtextxy( Menu[i]->titlestart, 2, Menu[i]->title);

    setcolor( MenuKeyColor);
    menukey[0] = Menu[i]->title[1];

    outtextxy( Menu[i]->titlestart + 8, 2, &menukey);
  }
}


void clearWindowManager( void) {

  setfillstyle( ManagerBkFill, ManagerBkColor);
  bar( 0, MANAGERBARDEEP+1, SCREENWIDE, SCREENDEEP-MANAGERBARDEEP-3);
}


/* open a window */

void openWindow( WININFO* wptr, char* headline) {

  int left = wptr->rect.left;
  int top = wptr->rect.top;
  int right = wptr->rect.right;
  int bottom = wptr->rect.bottom;
  int mode = wptr->mode;

  int fillcolor = WindowBkColor;

  if ( wptr->running == INACTIVE)
    if ( COMopened[COM1] || COMopened[COM2])
      if (( mode == CIEMode) || ( mode == RGBMode))
        if (( WinPtr[NoWin]->mode == CIEMode) ||
           ( WinPtr[NoWin]->mode == RGBMode))
          fillcolor = EGA_LIGHTGRAY;

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  setfillstyle( WindowBkFill, fillcolor);

  bar( left, top, right, bottom);

  if (( mode == CIEMeasMode) || ( mode == RGBMeasMode))
    fillcolor = WindowBarRestore;
  else
    if ( wptr == WinPtr[NoWin])
      fillcolor = WindowBarColor;
  else
    fillcolor = WindowInvBarColor;

  setfillstyle( WindowBarFill, fillcolor);

  bar( left+20, top, right, top+WINDOWBARDEEP);

  rectangle( left, top, right, bottom++);
  rectangle( left+7, top+3, left+13, top+7);

  top += WINDOWBARDEEP;
  left += 20;

  line( left-20, top, right++, top);
  line( left, top-WINDOWBARDEEP, left, top);

  right++;
  left -= 20;
  top -= WINDOWBARDEEP - 3;

  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  line( right, top, right, bottom+1);
  line( left+DROPSHADOW, bottom, right, bottom);

  setcolor( EGA_WHITE);

  outtextxy((( left + right)/2)-( 4*strlen( headline)), top, headline);

  if (( mode == CIEMode) || ( mode == RGBMode))
    outtextxy( right-42, top, COMText[wptr->sensor]);
}


/* Close a window */

void closeWindow( void) {

  if ( NoWin--)
    drawWindows();
}


/* add a menu to the menu array */

void addMenu( MENU* m) {

  int i, strstart;

  Menu[menuindex] = m;
  Menu[menuindex]->lastitem = strstart = 0;

  for ( i=0; i < menuindex; i++)
    strstart += strlen( Menu[i]->title) << 3;

  Menu[menuindex++]->titlestart = strstart;
};


/* open a menu */

int openMenu( void) {

  int i, j, left, top, right, bottom;

  if ( MenuWindow.back != NULL)
    closeMenu();

  left = Menu[ActiveMenu]->titlestart + 8;
  top = MANAGERBARDEEP;
  right = left + ( Menu[ActiveMenu]->menulength << 3);
  bottom = top + 11 + Menu[ActiveMenu]->count * MENULINEDEEP;

  if ( right >= SCREENWIDE) {
    i = abs(( right - SCREENWIDE)) + 8 ;
    left -= i;
    right -= i;
  }

  MenuWindow.frame.left = MenuRect.left = left;
  MenuWindow.frame.top  = MenuRect.top = top;

  MenuRect.right = right;
  MenuRect.bottom = bottom;

  MenuWindow.frame.right = right += DROPSHADOW;
  MenuWindow.frame.bottom = bottom += DROPSHADOW;

  if (( i = imagesize( left, top, right, bottom)) == -1)
    return( 0);

  if (( MenuWindow.back = malloc( i)) == NULL)
    return( 0);

  getimage( left, top, right, bottom, MenuWindow.back);

  right -= DROPSHADOW;
  bottom -= DROPSHADOW;

  setcolor( BLACK);
  setfillstyle( MenuBkFill, MenuBkColor);
  bar( left, top, right, bottom);

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  rectangle( left, top, right, bottom++);

  right += 2;

  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  line( right, top+DROPSHADOW, right, bottom+1);
  line( left+DROPSHADOW, bottom, right, bottom);

  top += 7;
  left += 8;

  for ( j=i=0; i < Menu[ActiveMenu]->count; i++, j += MENULINEDEEP)
    outtextxy( left, top + j, Menu[ActiveMenu]->item[i].name);

  InvMenuText( Menu[ActiveMenu]->lastitem, ACTIVE);

  return( 1);
}


/* Close the menu */

void closeMenu( void) {

  WINDOW* w = &MenuWindow;

  if ( w->back != NULL) {
    putimage( w->frame.left, w->frame.top, w->back, COPY_PUT);
    free( w->back);
    w->back = NULL;
  }
}


void closeMenuBar( void) {

  InvMenuTitle( ActiveMenu, INACTIVE);

  if ( MenuMode == 2)
    closeMenu();
  MenuMode = 0;
}


/* Invert a menu-title in the menubar */

void InvMenuTitle( int index, int active) {

  char menukey[2];

  int strstart = Menu[index]->titlestart;
  int strend = strstart + ( strlen( Menu[index]->title) << 3);

  if ( active) {
    setcolor( ManagerInvBarText);
    setfillstyle( ManagerInvBarFill, ManagerInvBarColor);
  }
  else {
    setcolor( ManagerBarText);
    setfillstyle( ManagerBarFill, ManagerBarColor);
  }

  bar( strstart, 0, strend, MANAGERBARDEEP-1);

  outtextxy( strstart, 2, Menu[index]->title);

  setcolor( MenuKeyColor);

  menukey[0] = Menu[index]->title[1];
  menukey[1] = 0;
  outtextxy( strstart+8, 2, &menukey);
}


/* Invert the next menu-title */

void NextMenu( int kbValue) {

  InvMenuTitle( ActiveMenu, INACTIVE);

  if (( kbValue == kbRight) || ( kbValue == kbTab)) {
    if ( ++ActiveMenu >= menuindex)
      ActiveMenu = 0;
  }
  else {
    if ( --ActiveMenu < 0 )
      ActiveMenu = menuindex-1;
  }

  InvMenuTitle( ActiveMenu, ACTIVE);
}


/* Invert the topic in the active menu */

void InvMenuText( int index, int active) {

  int i = index*MENULINEDEEP;

  if ( active) {
    setcolor( MenuInvBarText);
    setfillstyle( MenuInvBarFill, MenuInvBarColor);
  }
  else {
    setcolor( MenuBarText);
    setfillstyle( MenuBarFill, MenuBarColor);
  }

  bar( MenuRect.left+8, i + MANAGERBARDEEP+6,
      MenuRect.right-8, i + MANAGERBARDEEP + MENULINEDEEP+5);

  outtextxy( MenuRect.left+8, i + MANAGERBARDEEP + 7 ,
            Menu[ActiveMenu]->item[index].name);
}


/* Invert the next topic in menu */

void NextMenuText( int kbValue) {

  int* lastitem = &Menu[ActiveMenu]->lastitem;

  InvMenuText( *lastitem, INACTIVE);

  if ( kbValue == kbDown) {
    if ( ++*lastitem >= Menu[ActiveMenu]->count)
      *lastitem = 0;

    if ( Menu[ActiveMenu]->item[*lastitem].name[1] == 'Ä')
      ++*lastitem;
   }
   else {
    if ( --*lastitem < 0 )
      *lastitem = Menu[ActiveMenu]->count-1;

    if ( Menu[ActiveMenu]->item[*lastitem].name[1] == 'Ä')
      --*lastitem;
  }

  InvMenuText( *lastitem, ACTIVE);
}


/* Set a rectangle */

void SetRect( RECT* r, int left, int top, int right, int bottom) {

  r->left = left;
  r->top = top;
  r->right = right;
  r->bottom = bottom;
}


void drawSetupRect( int left, int top, int rows, char* str, int length) {

  int bottom = top + 12*rows + 18;
  int right = left + length;

  setcolor( BLACK);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  line( left, top, left+16, top);
  outtextxy( left+24, top-3, str);
  line( left + 24 + 9*strlen( str), top, right, top);

  line( left, top, left, bottom);
  line( left, bottom, right, bottom);
  line( right, bottom, right, top);
}


/* Add a checkbox object to the window */

int addCheckbox( CHECKBOX* cb, int x, int y, char* text) {

  cb->x = x;
  cb->y = y;
  cb->text = text;
  cb->frame.left = x-1;
  cb->frame.top = y-1;
  cb->frame.right = x+10;
  cb->frame.bottom = y+8;
  cb->select = 0x00;

  return( 0);
}


/* Draw a checkbox */

int drawCheckbox( CHECKBOX* cb, int active) {

  if ( active ==ACTIVE) {
    setcolor( ChkboxBarColor);
    setfillstyle( SOLID_FILL, ChkboxBarColor);
  }
  else {
    setcolor( ChkboxRectangle);
    setfillstyle( SOLID_FILL, WindowBkColor);
  }

  bar( cb->frame.right + 14, cb->frame.top,
    cb->frame.right + 16 + ( strlen( cb->text) << 3), cb->frame.top + 10);

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  rectangle( cb->frame.left, cb->frame.top,
          cb->frame.right, cb->frame.bottom);

  if ( active)
    setcolor( ChkboxBarText);
  else {
    setcolor( ChkboxText);

    if ( cb->select)
      setfillstyle( SOLID_FILL, ChkboxOnColor);
    else
      setfillstyle( SOLID_FILL, ChkboxOffColor);
  }

  outtextxy( cb->frame.right+16, cb->frame.top+2, cb->text);
  bar( cb->frame.left+3, cb->frame.top+2,
        cb->frame.right-3, cb->frame.bottom-2);

  return( 0);
}


/* Add a button to the window */

int addButton( BUTTON* b, int left, int top, int right, int bottom,
                 char* tx, int color) {
  b->frame.left = left;
  b->frame.top = top;
  b->frame.right = right;
  b->frame.bottom = bottom;
  b->color = color;

  b->text = tx;
  b->textx = (int)(( right + left - ( strlen( tx) << 3))/2.);

  return( 0);
}


/* Draw a button */

int drawButton( BUTTON* b, int active) {

  int oldcolor;
  struct fillsettingstype oldfillinfo;
  struct linesettingstype oldlineinfo;

  oldcolor = getcolor();
  getfillsettings( &oldfillinfo);
  getlinesettings( &oldlineinfo);

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  setfillstyle( ButFill, b->color);

  bar( b->frame.left, b->frame.top, b->frame.right, b->frame.bottom);

  if ( !active)
    setcolor( WindowBkColor);
  else
    setcolor( EGA_BLACK);

  line( b->frame.left+2, b->frame.bottom+1,
        b->frame.right, b->frame.bottom+1);

  line( b->frame.right+1, b->frame.top+2,
        b->frame.right+1, b->frame.bottom+2);

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  rectangle( b->frame.left, b->frame.top,
                    b->frame.right, b->frame.bottom);
  if ( active)
    setcolor( ButOnText);
  else
    setcolor( ButOffText);

  outtextxy( b->textx,  b->frame.top+4, b->text);

  setcolor( oldcolor);
  setfillstyle( oldfillinfo.pattern, oldfillinfo.color);
  setlinestyle( oldlineinfo.linestyle, 0, oldlineinfo.thickness);

  return( 0);
}


/* Add three standard buttons in window */

int addButtons( BUTTON* b0, BUTTON* b1, BUTTON* b2, RECT* r, int b0text) {

  int len = ( r->right - r->left) / 3.0;
  int start = r->left + ( len - 60) / 2.0;

  addButton( b0, start, r->bottom-25,
        start+60, r->bottom-10, ButtonText[b0text], ButColor);

  addButton( b1, len+start, r->bottom-25,
        len+start+60, r->bottom-10, "Cancel", ButColor);

  addButton( b2, 2*len+start, r->bottom-25,
        2*len+start+60, r->bottom-10, "Help", ButColor);

  return( 0);
}


/* Add an edit field to the window object w */

int addEditfield( EDITFIELD* t, int x, int y, int l, char* p, int (*proc)()) {

  t->x = x;
  t->y = y;
  t->text = p;
  t->length = l;
  t->curpos = 0;
  t->proc = proc;
  t->frame.left = x-3;
  t->frame.top = y-3;
  t->frame.right = x+( l << 3)+2;
  t->frame.bottom = y + 10;

  return( 0);
}


/* Draw an edit field */

int drawEditfield( EDITFIELD* t, int active) {

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  rectangle( t->frame.left, t->frame.top, t->frame.right, t->frame.bottom);

  if ( active) {
    setcolor( EGA_WHITE);
    setfillstyle( WindowBkFill, EGA_GREEN);
  }
  else {
    setcolor( EGA_BLACK);
    setfillstyle( WindowBkFill, WindowBkColor);
  }

  bar( t->frame.left+1, t->frame.top+1, t->frame.right-1, t->frame.bottom-1);
  outtextxy( t->x, t->y, t->text);

  setcolor( EGA_WHITE);

  if ( active)
    line( t->frame.left+( t->curpos << 3)+3, t->frame.top+11,
        t->frame.left+( t->curpos << 3)+9, t->frame.top+11);

  setcolor( EGA_BLACK);
  return( 0);
}


/* Edit an edit field */

int editEditfield( EDITFIELD* f, int c) {

  int i;
  int len = strlen( f->text);

  switch ( c) {

    case kbDel:
      if ( f->curpos < len)
        for ( i = f->curpos; i <= len; i++)
          f->text[i] = f->text[i+1];
      break;

    case kbHome:
      f->curpos = 0;
      break;

    case kbEnd:
      f->curpos = len;
      break;

    case kbLeft:
      if ( f->curpos)
        f->curpos--;
      break;

    case kbRight:
      if ( f->curpos < len)
        f->curpos++;
      break;

    case kbCtrlBack:
      f->curpos = f->text[0] = 0;
      break;

    case kbBack:
      if ( f->curpos && len) {
        if ( f->curpos < len)
          for ( i = f->curpos; i <= len; i++)
            f->text[i-1] = f->text[i];
        else
          f->text[len-1] = 0;
        f->curpos--;
      }
      break;

    default:
      c &= 0xFF;
      if ( f->proc)
        c = ( f->proc)( c);
      else {
        if ( !isprint( c))
          c = 0;
      }

      if ( c)
        c = toupper( c);

      if ( c && (( len+1) < f->length)) {
        for ( i = len+1; i > f->curpos; i--)
          f->text[i] = f->text[i-1];
        f->text[f->curpos++] = c;
      }
      break;
  }

  drawEditfield( f, ACTIVE);

  return( 0);
}


/* Add a list  */

int addList( LIST* l, int x, int y, int width, int depth,
            int count, int top, char* base) {
  l->x = x;
  l->y = y;
  l->length = width;
  l->depth = depth;
  l->count = count;
  l->top = l->ptr = top;
  l->base = base;

  l->frame.left = x-1;
  l->frame.top = y-1;
  l->frame.right = x + 17 + ( width << 3);
  l->frame.bottom = y + 1 + depth*MENULINEDEEP;

  return( 0);
}


/* Draw a list ( of files ) */

int drawList( LIST* l, int active) {

  register int i;
  register int dbase;
  register int dy=0;
  int x, y;

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  setfillstyle( WindowBkFill, WindowBkColor);

  rectangle( l->frame.left-1, l->frame.top-1,
                l->frame.right+1, l->frame.bottom+1);

  if ( l->base != NULL) {

    x = l->x + 8;
    y = l->y + 2;
    dbase = l->top;

    for ( i=0; i < l->depth; ++i) {

      if ( dbase >= l->count) {
        for ( i=i; i<l->depth; i++) {
          bar( x-8, y-1, l->frame.right-2, y+7);
          dbase++;
          dy += MENULINEDEEP;
          y = l->y + 2 + dy;
        }
        break;
      }

      bar( x-8, y-1, l->frame.right-2, y+7);
      outtextxy( x, y, l->base + dbase*( l->length + 1));

      if ( i == l->ptr)
        if ( active) {
          setcolor( EGA_WHITE);
          setfillstyle( SOLID_FILL, EGA_GREEN);

          bar( x-8, y-1, l->frame.right-2, y+7);
          outtextxy( l->x+8, l->y+2 + dy, l->base +
                                dbase*( l->length + 1));
          setcolor( EGA_BLACK);
          setfillstyle( SOLID_FILL, WindowBkColor);
        }

      dbase++;
      dy += MENULINEDEEP;
      y = l->y + 2 + dy;
    }
  }
  return( 0);
}


/* Update pointer in list */

int updateList( LIST* l, int kbKey) {

  int top = l->top;
  int ptr = l->ptr;

  switch ( kbKey) {
    case kbDown:
      if ( ++l->ptr >= min( l->depth, l->count)) {
        l->ptr = min( l->count, l->depth) - 1;

        if ( ++l->top >= ( l->count - l->depth))
          l->top = max( 0, l->count - l->depth);
      }
      break;

    case kbUp:
      if ( --l->ptr < 0) {
        l->ptr = 0;

        if ( --l->top < 0)
          l->top = 0;
      }
      break;

    case kbPgDn:
      if ( l->count < l->depth)
        l->ptr = l->count - 1;
      else
        if ( l->top == ( l->count - l->depth))
          l->ptr  = l->depth - 1;
      else
        if (( l->top + 2*l->depth - 1) >= l->count) {
          l->top += l->count - l->top - l->depth;
          l->ptr = 0;
        }
      else {
        l->top += l->depth - 1;
        l->ptr = 0;
      }
      break;

    case kbPgUp:
      if ( l->top <= 0)
        l->ptr = 0;
      else
        l->ptr = l->depth - 1;

      l->top = max( 0, ( l->top - l->depth - 1));
      break;

    case kbHome:
      l->top = l->ptr = 0;
      break;

    case kbEnd:
      l->top = max( 0, l->count - l->depth);
      l->ptr = min( l->count, l->depth) - 1;
      break;
  }
  if (( top - l->top) || ( ptr - l->ptr))
    return( 1);

  return( 0);
}


/* Draw a list of White Refs or Phosphors - used by ... */

int drawFileList( LIST* l, FILENODE* p, int active) {

  FILENODE* tmp = p;

  register int i;
  register int dbase;
  register int dy=0;
  int x, y;

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  setfillstyle( WindowBkFill, WindowBkColor);

  rectangle( l->frame.left-1, l->frame.top-1,
                l->frame.right+1, l->frame.bottom+1);
  x = l->x + 8;
  y = l->y + 2;
  dbase = l->top;

  if ( l->count) {
    for ( i=0; i < l->depth; ++i) {

      if ( !tmp) {
        for ( i=i; i<l->depth; i++) {
          bar( x-8, y-1, l->frame.right-2, y+7);
          dbase++;
          dy += MENULINEDEEP;
          y = l->y + 2 + dy;
        }
        break;
      }

      bar( x-8, y-1, l->frame.right-2, y+7);
      outtextxy( x, y, tmp->name);

      if ( i == l->ptr)
        if ( active) {
          setcolor( EGA_WHITE);
          setfillstyle( SOLID_FILL, EGA_GREEN);

          bar( x-8, y-1, l->frame.right-2, y+7);
          outtextxy( l->x+8, l->y+2 + dy, tmp->name);

          setcolor( EGA_BLACK);
          setfillstyle( SOLID_FILL, WindowBkColor);
        }

      tmp = tmp->next;
      dbase++;
      dy += MENULINEDEEP;
      y = l->y + 2 + dy;
    }
  }
  return( 0);
}


/* Draw an active window */

void drawDisplay( int n) {

  switch ( WinPtr[n]->mode) {
    case CIEMode:
      drawCIE( n);
      break;

    case RGBMode:
      drawRGB( n);
      break;

    case CIEMeasMode:
      drawCIE( n);
      updateCIE( n);
      break;

    case RGBMeasMode:
      drawRGB( n);
      updateRGB( n);
      break;
  }
}


/* Draw ALL active windows */

void drawWindows( void) {

  register int i;

  clearWindowManager();

  findDisplayOverlap();

  for ( i=1; i <= NoWin; i++) {
    WinPtr[i]->IOError = 0;
    drawDisplay( i);
  }
}