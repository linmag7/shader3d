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
#include "rotate3d.h"
#include "gccasm.h"
#include "phongtex.h"

#ifdef LIGHT_LOOKUP
 #undef LIGHT_LOOKUP
#endif
#define LIGHT_LOOKUP (ny&BIT_MASK)|((nx>>SHIFT_SCALE)&BIT_MASK_L)
#ifdef PIXEL_LOOKUP
 #undef PIXEL_LOOKUP
#endif
#define PIXEL_LOOKUP (v&BIT_MASK)|((u>>SHIFT_SCALE)&BIT_MASK_L)
#ifdef FBPOINTER
 #undef FBPOINTER
#endif
#define FBPOINTER float*

#ifdef PUTPIXEL
 #undef PUTPIXEL
#endif


#if (defined(__sun) && defined(__sparcv9))
#define PUTPIXEL framebuffer[x]=pixel_phong(texturebuffer+(PIXEL_LOOKUP),phonglightmap_vis+(LIGHT_LOOKUP))
#endif

#if  (defined(__GNUC__)  && defined(__alpha_max__))
#define PUTPIXEL  pixel_light_p(texturebuffer+(PIXEL_LOOKUP),phonglightmap_vis+(LIGHT_LOOKUP),framebuffer[x])
#endif
#if  (defined(__GNUC__)  && defined(__sparc_v9__))
#define PUTPIXEL pixel_phong(texturebuffer+(PIXEL_LOOKUP),phonglightmap_vis+(LIGHT_LOOKUP),framebuffer[x])
#endif

#ifndef PUTPIXEL
#define PUTPIXEL  framebuffer[x]=texturebuffer[PIXEL_LOOKUP]
#endif

void poly_phongtex(POLYGON_FILL_ARGS){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;
    float fpixel;



    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* texturebuffer=sRot->current_object->texturebuffer;
    unsigned int* phonglightmap_vis=sRot->phongmap;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;
     //int xd,yd;

    xs=(float) v0->x; xe=(float)0.5+(float) v0->x;//(float)0.5+
    zs=(v0->z)<<SHIFT_SCALE;
    us=(v0->u)<<SHIFT_SCALE;    vs=(v0->v)<<SHIFT_SCALE;
    nxs=(v0->nx)<<SHIFT_SCALE;  nys=(v0->ny)<<SHIFT_SCALE;


    dx_r=d->dxdy_ur;
    dx_l=d->dxdy_ul;
    dzdy =(int)(TEXTURE_SIZE-1)*(d->dzdy_u);
    dzdx =(d->dzdx);
    dvdy =(int)(TEXTURE_SIZE-1)*(d->dvdy_u);
    dvdx =(d->dvdx);
    dudy =(int)(TEXTURE_SIZE-1)*(d->dudy_u);
    dudx =(d->dudx);
    dnxdy=(int)(TEXTURE_SIZE-1)*(d->dnxdy_u);
    dnxdx=(d->dnxdx);
    dnydy=(int)(TEXTURE_SIZE-1)*(d->dnydy_u);
    dnydx=(d->dnydx);


    for(y=0;y<d->dy01;y++,framebuffer+=X_RES,zbuffp+=X_RES){
        z=zs; u=us; v=vs; nx=nxs; ny=nys;
        for(x=(unsigned int)(xs ROUND);X_CHECK;x++){
            if (zbuffp[x]>z){
               zbuffp[x]=z;
               PUTPIXEL;
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;
        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;
    }

    dx_r=d->dxdy_lr; dx_l=d->dxdy_ll; dzdy=(int)(TEXTURE_SIZE-1)*(d->dzdy_l); dudy=(int)(TEXTURE_SIZE-1)*(d->dudy_l);
    dvdy=(int)(TEXTURE_SIZE-1)*(d->dvdy_l); dnxdy=(int)(TEXTURE_SIZE-1)*(d->dnxdy_l); dnydy=(int)(TEXTURE_SIZE-1)*(d->dnydy_l);



    for(y=0;y<=d->dy12;y++,framebuffer+=X_RES,zbuffp+=X_RES){
        z=zs; u=us; v=vs; nx=nxs; ny=nys;
        for(x=(unsigned int)(xs ROUND);X_CHECK;x++){
            if (zbuffp[x]>z){
                zbuffp[x]=z;
                PUTPIXEL;
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;
        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;

    }

}

void poly_phongtex_upper(POLYGON_FILL_ARGS){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;

    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* texturebuffer=sRot->current_object->texturebuffer;
    unsigned int* phonglightmap_vis=sRot->phongmap;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;
    unsigned int light,pixel;
    float phong;
    unsigned int r,g,b;


    xs=(float) v0->x; xe=(float)0.5+(float) v0->x;//
    zs=(v0->z)<<SHIFT_SCALE;
    us=(v0->u)<<SHIFT_SCALE;    vs=(v0->v)<<SHIFT_SCALE;
    nxs=(v0->nx)<<SHIFT_SCALE;  nys=(v0->ny)<<SHIFT_SCALE;


    dx_r=d->dxdy_ur;
    dx_l=d->dxdy_ul;
    dzdy =(int)(TEXTURE_SIZE-1)*(d->dzdy_u);
    dzdx =(d->dzdx);
    dvdy =(int)(TEXTURE_SIZE-1)*(d->dvdy_u);
    dvdx =(d->dvdx);
    dudy =(int)(TEXTURE_SIZE-1)*(d->dudy_u);
    dudx =(d->dudx);
    dnxdy=(int)(TEXTURE_SIZE-1)*(d->dnxdy_u);
    dnxdx=(d->dnxdx);
    dnydy=(int)(TEXTURE_SIZE-1)*(d->dnydy_u);
    dnydx=(d->dnydx);



    for(y=0;y<=d->dy01;y++,framebuffer+=X_RES,zbuffp+=X_RES){
        z=zs; u=us; v=vs; nx=nxs; ny=nys;
        for(x=(unsigned int)(xs ROUND);X_CHECK;x++){
            if (zbuffp[x]>z){
               zbuffp[x]=z;
               PUTPIXEL;
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;
        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;
    }
}

void poly_phongtex_lower(POLYGON_FILL_ARGS_LOWER){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;

    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* texturebuffer=sRot->current_object->texturebuffer;
    unsigned int* phonglightmap_vis=sRot->phongmap;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;


    xs=(float) v0->x; xe=(float)0.5+(float) v1x;//(
    zs=(v0->z)<<SHIFT_SCALE;
    us=(v0->u)<<SHIFT_SCALE;    vs=(v0->v)<<SHIFT_SCALE;
    nxs=(v0->nx)<<SHIFT_SCALE;  nys=(v0->ny)<<SHIFT_SCALE;


    dx_r=d->dxdy_lr;
    dx_l=d->dxdy_ll;
    dzdy =(int)(TEXTURE_SIZE-1)*(d->dzdy_l);
    dzdx =(d->dzdx);
    dvdy =(int)(TEXTURE_SIZE-1)*(d->dvdy_l);
    dvdx =(d->dvdx);
    dudy =(int)(TEXTURE_SIZE-1)*(d->dudy_l);
    dudx =(d->dudx);
    dnxdy=(int)(TEXTURE_SIZE-1)*(d->dnxdy_l);
    dnxdx=(d->dnxdx);
    dnydy=(int)(TEXTURE_SIZE-1)*(d->dnydy_l);
    dnydx=(d->dnydx);

        for(y=0;y<=d->dy12;y++,framebuffer+=X_RES,zbuffp+=X_RES){
        z=zs; u=us; v=vs; nx=nxs; ny=nys;
        for(x=(unsigned int)(xs ROUND);X_CHECK;x++){
            if (zbuffp[x]>z){
                zbuffp[x]=z;
                PUTPIXEL;
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;
        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;
    }

}


