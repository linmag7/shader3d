#ifndef __XINIT_H__
#define __XINIT_H__

#define __USE_X_SHAREDMEMORY__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "types.h"

#ifdef __USE_X_SHAREDMEMORY__
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#define ERR_XI_FAILURE        0xFF
#define ERR_XI_OK             0x00
#define ERR_XI_NOSHAREDMEMORY 0x01
#define ERR_XI_DISPLAY        0x02
#define ERR_XI_BADDEPTH       0x03
#define ERR_XI_WINDOW         0x04
#define ERR_XI_VIRTALLOC      0x05
#define ERR_XI_XIMAGE         0x06
#define ERR_XI_SHMALLOC       0x07
#define ERR_XI_SHMXIMAGE      0x08
#define ERR_XI_SHMSEGINFO     0x09
#define ERR_XI_SHMVIRTALLOC   0x0A
#define ERR_XI_SHMATTACH      0x0B

#define VIDEO_XI_STANDARD     0x00 /* Use standard Xlib calls */
#define VIDEO_XI_SHMSTD       0X01 /* Use Xlib shared memory extension */
#define VIDEO_XI_SHMPIXMAP    0x02 /* Use shared memory pixmap */

#define PIXEL unsigned long

typedef struct {

  Display *display;
  Window window;
  Screen *screenptr;
  int screennum;
  Visual *visual;
  GC gc;
  XImage *ximage;

  Colormap colormap;
  PIXEL *palette;
  int colorcells;
  
#ifdef __USE_X_SHAREDMEMORY__
  Pixmap pixmap;
  XShmSegmentInfo *shmseginfo;
  byte *videomemory;	
#endif
  
  byte *virtualscreen;
  int videoaccesstype;
  
  int width;
  int height;
  int depth;
  int pixelsize;
  int screensize;

} XWindow;

extern int OpenXWindow(XWindow *XWnd, int width, int height, int bpp,
		       char *title, int *Palette, BOOL createbuffer,
		       BOOL usesharedmemory, BOOL shmpixmap, BOOL verbose);
extern int InitXWindow(XWindow *XWnd, int width, int height, int bpp,
		       char *title, int *Palette, BOOL createbuffer,
		       BOOL usesharedmemory,BOOL shmpixmap);
extern void TrashXWindow(XWindow *XWnd);
extern void PutXImage(XWindow *XWnd);

#endif /* __XINIT_H__ */
