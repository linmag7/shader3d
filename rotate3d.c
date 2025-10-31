// Copyright (c) 2025 [Magnus Lindholm]
// 			      [linmag7@gmail.com]
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the Software), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following condition:
//
// THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.





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

#define W0_X_RES 512
#define W0_Y_RES 512

#define SHM_SIZE W0_X_RES*W0_Y_RES*4+1024

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "rotate3d.h"
#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>



#include <math.h>
#include "skull.h"
#include "face2.h"
#include <string.h>
#include "envmap.h"
#include "envbump.h"
#include "texturemap.h"
#include "phongenv.h"
#include "phongtex.h"
//#include "pbenvtex.h"

#if (defined(__GNUC__) && defined(__sparc_v9__))
#include "gccasm.h"
#endif



#define BUMP_SIZE 1.4
#define PHONG_RADIUS 1.95
#define PHONG_SCALE 4.3

XWindow *xw0; 
  int semid_a;
  int semid_b; /* semid of semaphore set */
  struct sembuf* down;
  //struct sembuf* down_1;
  struct sembuf* up;
  pid_t pid;

static const char *rotate3d_defaults [] = {
  ".background: black",
  ".foreground: white",
  "*fpsSolid:	true",
  "*color:		random",
  "*colorcount:	64",
  "*delay:		30000",
  "*size:		1000",
  "*duration:	120",
  "*soften:		1",
  "*invert:		False",
  "*bump_enable:	True",
  "*mapping:            env",
  "*texture:		/usr/lib/xscreensaver/hacks/test8.tga",
  "*envmap:		/usr/lib/xscreensaver/hacks/lens2.tga",
  "*bump:		/usr/lib/xscreensaver/hacks/bumptest7.tga",
#ifdef __sgi    
  "*visualID:	Best",
#endif
#ifdef HAVE_XSHM_EXTENSION
  "*useSHM:		True",
#endif 
  0
};



static void init_object(object3d*,rot3dState*,int);
static void gen_phonglightmap_vis(unsigned int*,float,float);



#if  (defined(__sun) && defined (__sparcv9))
extern float pixel_light_p(unsigned int*,unsigned int*,unsigned int*);
extern void set_gsr(void);
extern float pixel_phong(unsigned int*,unsigned int*);
#endif

//#include "texturemap.c"
#if (defined(__sparcv9) || defined(__sparc_v9__) ||defined(__alpha_max__))
//#include "envmap.c"
//#include "envbump.c"
//#include "phongtex.c"
//#include "phongenv.c"
//#include "pbenvtex.c"
#endif
//#include "polygon.c"



static void* rotate3d_init(int width, int height, char *screendata){


  XWindowAttributes XWinAttribs;
  rot3dState* sRot;
  unsigned int* bumpmap;

  char texture_name[] = "test8.tga";
  char bump_name[] = "bumptest7.tga";
  char env_name[] =  "lens2.tga";


  unsigned int* reflectionmap;
#ifdef __GNUC__
  unsigned long dummy;
#endif
  sRot=(rot3dState *)malloc(sizeof(rot3dState));
  //sRot->dpy=xw->display;
  //sRot->Win=xw->window;
  //XGetWindowAttributes(xw->display, xw->window, &XWinAttribs );
  sRot->width=width;
  sRot->height=height;
  
  //sRot->GraphicsContext=XCreateGC(xw->display,xw->window,0,NULL);


  //sRot->delay=get_integer_resource(sRot->dpy, "delay"   , "Integer");
  //sRot->scale=get_integer_resource(sRot->dpy, "size"   , "Integer");
  //texture_name = get_string_resource (dpy, "texture", "Bitmap");
  //bump_name = get_string_resource (dpy, "bump", "Bitmap");
  //env_name = get_string_resource (dpy, "envmap", "Bitmap");
   sRot->scale=1000;
   sRot->delay=30000;     

  //sRot->x_res=max((XWinAttribs.width)/2,(XWinAttribs.height)/2);
  //sRot->y_res=max((XWinAttribs.width)/2,(XWinAttribs.height)/2);
  sRot->x_res=width;
  sRot->y_res=height;    
  sRot->scale=sRot->x_res*sRot->scale/300;
  //sRot->pXImage=create_xshm_image( sRot->dpy, XWinAttribs.visual, XWinAttribs.depth,
//			ZPixmap, NULL, &sRot->XShmInfo, sRot->x_res, sRot->x_res );


  //sRot->pXImage=xw->ximage;
  
  sRot->rotate=30;
  sRot->byte_pp=4;
  sRot->size=sRot->x_res*sRot->y_res*4;
  sRot->framebuffer=(unsigned char*) screendata;


  if ((bumpmap = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of bumpmap buffer failed\n");
      exit(-1);
   }


    if ((sRot->zbuffer = malloc(sRot->x_res*sRot->y_res*sizeof(unsigned int))) == NULL) {
      fprintf(stderr,"malloc of zbuffer failed\n");
      exit(-1);
   }

    if ((sRot->phongmap = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of phongmap buffer failed\n");
      exit(-1);
  }
    if ((sRot->current_object = malloc(sizeof(object3d))) == NULL) {
      fprintf(stderr,"malloc of current_object buffer failed\n");
      exit(-1);
  }
    if ((sRot->sintable = malloc(1024*sizeof(unsigned int))) == NULL) {
      fprintf(stderr,"malloc of sintable failed\n");
      exit(-1);
   }
    if ((sRot->costable = malloc(1024*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of costable buffer failed\n");
      exit(-1);
  }
    if ((sRot->current_object->environmentmap = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of environment buffer failed\n");
      exit(-1);
   }
      if ((reflectionmap = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of environment buffer failed\n");
      exit(-1);
   }
    if ((sRot->current_object->xlight_disp = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of light displacement map failed\n");
      exit(-1);
   }
    if ((sRot->current_object->ylight_disp = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of  light displacement map failed\n");
      exit(-1);
   }
    if ((sRot->current_object->texturebuffer = malloc(TEXTURE_SIZE*TEXTURE_SIZE*sizeof(int))) == NULL) {
      fprintf(stderr,"malloc of texture buffer failed\n");
      exit(-1);
  }
  load_tga(texture_name,sRot->current_object->texturebuffer);
  load_tga(env_name,sRot->current_object->environmentmap);
  load_tga(bump_name,bumpmap);
  gen_displacement_map(sRot->current_object->xlight_disp,sRot->current_object->ylight_disp,bumpmap,BUMP_SIZE);
  free(bumpmap);
  gen_sincos_table(sRot->sintable,sRot->costable,512);

  
  init_object(sRot->current_object,sRot,sRot->x_res/3.2+620);
  #if (defined(__sun) && defined(__sparcv9))
  gen_phonglightmap_vis(sRot->phongmap,PHONG_RADIUS,PHONG_SCALE);
  #endif
  #if (defined(__GNUC__)  && defined(__sparc_v9__))
  gen_phongmap(sRot->phongmap,PHONG_RADIUS,20*PHONG_SCALE);
  #endif
  #if (defined(__GNUC__)  && defined(__alpha_max__))
  gen_phongmap(sRot->phongmap,PHONG_RADIUS,50*PHONG_SCALE);
  #endif
 
  //mapping_str = get_string_resource (dpy, "mapping", "Mapping");
  sRot->current_object->mapping = PHONGTEX; 
  if(sRot->current_object->mapping==PHONGENV){
    reflection_map(sRot->current_object->texturebuffer,sRot->current_object->environmentmap,reflectionmap,0.65,0.6);
    free(sRot->current_object->environmentmap);
    sRot->current_object->environmentmap=reflectionmap;
  }
  
#if (defined(__GNUC__)  && defined(__sparc_v9__))
  set_gsr(dummy);
#endif

#if (defined(__sun) && defined(__sparcv9))
  set_gsr();
#endif
 
  return sRot;
}

static void init_object(object3d* o, rot3dState* s,int size){
  
  o->facelist=skull_Face;
  o->fnormal=skull_NFace;
  o->texture_uv=skull_TriTex;
  o->vertex=skull_Vertex;
  o->vnormal=skull_NVertex7;
  o->nfaces=1243;
/*
  o->facelist=face2_facelist;
  o->fnormal=face2_Nface;
  o->texture_uv=face2_TriTex;
  o->vertex=face2_vertex;
  o->vnormal=face2_NVertex7;
  o->nfaces=718;
*/
  o->rot_x=0;
  o->rot_y=128;
  o->rot_z=0;
  o->rot_nx=0;
  o->rot_ny=0;
  o->rot_nz=0;
  o->trans_x=s->x_res/2;
  o->trans_y=s->y_res/2.2;
  o->trans_z=size;
  o->mapping=REFLECTION;
}

static void rotate3d_free(Display *dpy, Window window,rot3dState* sRot){
    free(sRot->zbuffer);
    free(sRot->current_object->ylight_disp);
    free(sRot->current_object->xlight_disp);
    
    free(sRot->current_object->environmentmap);
    free(sRot->current_object->texturebuffer);
    free(sRot->sintable);
    free(sRot->costable);
    free(sRot->current_object);
    free(sRot->phongmap);
    //destroy_xshm_image(dpy, sRot->pXImage,&sRot->XShmInfo);
    //XFreeGC(dpy,sRot->GraphicsContext);

}

static void rotate3d_reshape(Display *dpy, Window window, void *closure,
                 unsigned int w, unsigned int h){

}

static unsigned long
rotate3d_draw(rot3dState *sRot){
    object3d* current_object=sRot->current_object;
      memset(sRot->framebuffer,0,sRot->size);
      memset(sRot->zbuffer,0x77,sRot->size);
#if (defined(__sun) && defined(__sparcv9))
  set_gsr();
#endif
 

      current_object->roto_x=sRot->rotate;
      current_object->roto_y=sRot->rotate;
      current_object->roto_z=sRot->rotate;
      draw_object(sRot);
     // XShmPutImage(dpy, window, sRot->GraphicsContext, sRot->pXImage,
	//	   0, 0, sRot->width/2-sRot->x_res/2, sRot->height/2-sRot->y_res/2, sRot->x_res, sRot->y_res, True);
      
      PutXImage(xw0);
      sRot->rotate+=1;
      sRot->rotate&=0xff;
      
      return sRot->delay;
}

static void rotate3d_event(Display *dpy, Window window, void *closure, XEvent *event) {


}

rot3dState* sRot;


int
main(int argc,char *argv[])
{
  int InfiniteLoop=TRUE;
  rot3dState* sRot;
  int mapping = PHONGTEX;
  
  xw0=(XWindow *)malloc(sizeof(XWindow));
  if(OpenXWindow(xw0,W0_X_RES,W0_Y_RES,24,"Polygon 3D",NULL,TRUE,TRUE,TRUE,TRUE))
    {
      printf("\nClosing application.\n");
      return 1;
    }

  
  sRot=rotate3d_init(W0_X_RES,W0_Y_RES,xw0->virtualscreen);
    
  
  while(InfiniteLoop)
    {
      
      XEvent ev;
      long ks;
      rotate3d_draw(sRot);
      XNextEvent(xw0->display,&ev);
	  switch(ev.type)
	    {
	    case KeyPress:
	      ks=XLookupKeysym((XKeyEvent *)&ev,0);
	      //ClearKBuf();
              if (ks == XK_Escape) {
                printf("Escape pressed!\n");
		InfiniteLoop=FALSE;
              } else {
		mapping++;
		sRot->current_object->mapping = mapping;
		if (mapping > 8)
			mapping = 0;
              }
	      break;
	    case ButtonPress:
	      InfiniteLoop=FALSE;
	      break;

	    }

    }
 
}
 


