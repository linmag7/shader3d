/*
 *
 *  Description:
 *
 *  A small demo of polygon texture mapping and shading. A few different
 *  light/shadning model. Created by Magnus Lindholm (linmag7@gmail.com)
 *
 *
 *  Modification history:
 *
 *  2010-06-01: Magnus Lindholm     Creation
 */
#ifndef _ROTATE3D_H
#define	_ROTATE3D_H

#ifdef	__cplusplus
extern "C" {
#endif

/* .inline set_gsr,0

    rd %gsr,%o0
    or %o0,24,%o0
    wr %o0,%gsr

.end
*/


#include <stdio.h>
#include <stdlib.h>
#include "xinit.h"    
//#include "utils.h"
//#include "xshm.h"

#define PI 3.141596
extern void gen_displacement_map(int*, int*,unsigned int*, float);
extern void gen_sincos_table(float*,float*,int);
extern void reflection_map(unsigned int*,unsigned int*,unsigned int*, float,float);
extern void gen_phongmap(unsigned int*,float,float);



#define X_RES sRot->x_res
#define Y_RES sRot->y_res
#define BYTE_PP sRot->byte_pp

#define CURR_Y_FRAMEBUFFER sRot->framebuffer+((int)(v0->y))*(sRot->x_res)*(sRot->byte_pp)
#define CURR_Y_ZBUFFER &sRot->zbuffer[((int)(v0->y))*(sRot->x_res)]
#define POLYGON_FILL_ARGS vertex* v0,delta* d,void* s
#define POLYGON_FILL_ARGS_LOWER vertex* v0,int v1x,delta* d,void* s

#define POLYGON_FILL_PARAM p->v0,&delta_values,(void*)sRot
#define POLYGON_FILL_PARAM_LOWER p->v0,p->v1->x,&delta_values,(void*)sRot

#define ROUND
#define X_CHECK x<(unsigned int)(xe)
#define X_DELTA_CHECK (xe-xs>2)
#define R_PIXEL sattable_r[(int)(((pixel&0x000000ff)+((light&0x000000ff)))*phong)]
#define G_PIXEL sattable_r[(int)((((pixel&0x0000ff00)+((light&0x0000ff00)))>>8)*phong)]
#define B_PIXEL sattable_r[(int)((((pixel&0x00ff0000)+((light&0x00ff0000)))>>16)*phong)]
#define RGB_PIXEL r|(g<<8)|(b<<16)
//#define BIT_MASK 0x3fe00

//#define PI 3.141596

#define CLIP    0
#define NO_CLIP 1
#define INVISIBLE -1

#define SCALE sRot->scale


#define OBJECT_UV_SCALE 256


#define TEXTURE_SIZE    512

#if (TEXTURE_SIZE  == 256)
#define TEXTURE_LIGHTMAP_SCALE (TEXTURE_SIZE/OBJECT_UV_SCALE)
#define SHIFT_SCALE 8
#define BIT_MASK 0xff00
#define BIT_MASK_L 0xff
#endif

#if (TEXTURE_SIZE  == 512)
#define TEXTURE_LIGHTMAP_SCALE (TEXTURE_SIZE/OBJECT_UV_SCALE)
#define SHIFT_SCALE 9
#define BIT_MASK 0x3fe00
#define BIT_MASK_L 0x1ff
#endif

#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))

#define FLAT              0
#define TEXTURE           1
#define PHONGTEX          2
#define PBTEXTURE         3
#define REFLECTION        4
#define PHONGENV          5
#define PHONGBUMPTEX      6
#define ENVBUMP           7
#define PENVBTEX          8


typedef struct {
   char  idlength;
   char  colourmaptype;
   char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short width;
   short height;
   char  bitsperpixel;
   char  imagedescriptor;
} tgaHEADER;

extern void load_tga(char*,unsigned int*);

typedef struct {

    int x;
    int y;
    int z;
    int u;
    int v;
    int nx;
    int ny;
    int nz;

} vertex;

typedef struct {
    float x;
    float y;
    float z;
    float u;
    float v;
    float nx;
    float ny;
} vector;

typedef struct {
    float dy01;
    float dy12;
    float dxdy_ul;
    float dxdy_ur;
    float dxdy_ll;
    float dxdy_lr;
    float dzdy_u;
    float dzdy_l;
    float dudy_u;
    float dudy_l;
    float dvdy_u;
    float dvdy_l;
    float dnxdy_u;
    float dnxdy_l;
    float dnydy_u;
    float dnydy_l;
    int dzdx;
    int dudx;
    int dvdx;
    int dnxdx;
    int dnydx;
} delta;



typedef struct {
    vertex* v0;
    vertex* v1;
    vertex* v2;
    float nx;
    float ny;
    float nz;
    int side;
    int clip;
} polygon;


typedef struct {

    float* vertex;
    float* texture_uv;
    int* facelist;
    float* vnormal;
    float* fnormal;
    int nfaces;
    int roto_x;
    int roto_y;
    int roto_z;
    int rot_x;
    int rot_y;
    int rot_z;
    int rot_nx;
    int rot_ny;
    int rot_nz;
    int trans_x;
    int trans_y;
    int trans_z;
    int scale_x;
    int scale_y;
    int scale_z;
    unsigned int* texturebuffer;
    unsigned int* environmentmap;
    int* xlight_disp;
    int* ylight_disp;
    int mapping;
    void (*draw_front_face_upper)(POLYGON_FILL_ARGS);
    void (*draw_back_face_upper)(POLYGON_FILL_ARGS);
    void (*draw_front_face_lower)(POLYGON_FILL_ARGS_LOWER);
    void (*draw_back_face_lower)(POLYGON_FILL_ARGS_LOWER);
    void (*draw_front_face)(POLYGON_FILL_ARGS);
    void (*draw_back_face)(POLYGON_FILL_ARGS);
} object3d;



typedef struct
{
	/* XWindows specific variables. */
	Display *dpy;
        int height;
        int width;
	Window Win;
	Screen *screen;
        Pixmap source;
	GC GraphicsContext;
	XColor *xColors;
	XImage *pXImage;
#ifdef HAVE_XSHM_EXTENSION
	XShmSegmentInfo XShmInfo;
	Bool	bUseShm;
#endif /* HAVE_XSHM_EXTENSION */
        unsigned char* framebuffer;
        unsigned int* phongmap;
        int* zbuffer;
        int x_res;
        int y_res;
        int byte_pp;
        int size;
        int rotate;
        object3d* current_object;
        float* costable;
        float* sintable;
        int delay;
        int scale;
} rot3dState;



#ifdef	__cplusplus
}
#endif

#endif	/* _ROTATE3D_H */

