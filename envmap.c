/* 
 *
 *  Description:
 *
 *  A small demo of polygon texture mapping and shading. A few differen
 *  light/shadning model. Created by Magnus Lindholm (linmag7@gmail.com)
 *
 *
 *  Modification history:
 *
 *  2010-06-01: Magnus Lindholm     Creation
 */
#include "rotate3d.h"
#include "envmap.h"
#ifdef PUTPIXEL
 #undef PUTPIXEL
#endif

#ifdef LIGHT_LOOKUP
 #undef LIGHT_LOOKUP
#endif
#define LIGHT_LOOKUP (ny&BIT_MASK)|((nx&BIT_MASK)>>9)
#ifdef PIXEL_LOOKUP
 #undef PIXEL_LOOKUP
#endif
#define PIXEL_LOOKUP (v&BIT_MASK)|((u>>9)&0x1ff)
#ifdef FBPOINTER
 #undef FBPOINTER
#endif
#define FBPOINTER int*

#if  (defined(__GNUC__)  && defined(__alpha_max__))
#define env_pixel(envmap,pixel)  \
__asm__ __volatile__ (  \
"ldl %0,0(%1)\n\t"  \
: "=a" (pixel)   \
: "a" (envmap) \
);
#define PUTPIXEL env_pixel(environmentmap+(LIGHT_LOOKUP),framebuffer[x])
#endif

#if (defined(__sparcv8) || defined(__sparcv9) || defined(__sparc_v9__))
 #define PUTPIXEL framebuffer[x]=environmentmap[LIGHT_LOOKUP]
#endif

#ifndef PUTPIXEL
 #define PUTPIXEL framebuffer[x]=environmentmap[LIGHT_LOOKUP]
#endif

void poly_envmap(POLYGON_FILL_ARGS){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;
    float fpixel;
    int xd,yd,pixel,light;


    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* environmentmap=sRot->current_object->environmentmap;
    //int* xlight_disp=sRot->current_object->xlight_disp;
    //int* ylight_disp=sRot->current_object->ylight_disp;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;
    

    xs=(float) v0->x; xe=(float)0.5+(float) v0->x;
    zs=(v0->z)<<9;
    us=(v0->u)<<9;    vs=(v0->v)<<9;
    nxs=(v0->nx)<<9;  nys=(v0->ny)<<9;


    dx_r=d->dxdy_ur;
    dx_l=d->dxdy_ul;
    dzdy =(int)511*(d->dzdy_u);
    dzdx =(d->dzdx);
    dvdy =(int)511*(d->dvdy_u);
    dvdx =(d->dvdx);
    dudy =(int)511*(d->dudy_u);
    dudx =(d->dudx);
    dnxdy=(int)511*(d->dnxdy_u);
    dnxdx=(d->dnxdx);
    dnydy=(int)511*(d->dnydy_u);
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


    dx_r=d->dxdy_lr; dx_l=d->dxdy_ll; dzdy=(int)511*(d->dzdy_l); dudy=(int)511*(d->dudy_l);
    dvdy=(int)511*(d->dvdy_l); dnxdy=(int)511*(d->dnxdy_l); dnydy=(int)511*(d->dnydy_l);



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

void poly_envmap_upper(POLYGON_FILL_ARGS){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;

    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* environmentmap=sRot->current_object->environmentmap;
    int* xlight_disp=sRot->current_object->xlight_disp;
    int* ylight_disp=sRot->current_object->ylight_disp;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;
    unsigned int light,pixel;
    float phong;
    unsigned int r,g,b;
    int xd,yd;

    xs=(float) v0->x; xe=(float)0.5+(float) v0->x;
    zs=(v0->z)<<9;
    us=(v0->u)<<9;    vs=(v0->v)<<9;
    nxs=(v0->nx)<<9;  nys=(v0->ny)<<9;


    dx_r=d->dxdy_ur;
    dx_l=d->dxdy_ul;
    dzdy =(int)511*(d->dzdy_u);
    dzdx =(d->dzdx);
    dvdy =(int)511*(d->dvdy_u);
    dvdx =(d->dvdx);
    dudy =(int)511*(d->dudy_u);
    dudx =(d->dudx);
    dnxdy=(int)511*(d->dnxdy_u);
    dnxdx=(d->dnxdx);
    dnydy=(int)511*(d->dnydy_u);
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

void poly_envmap_lower(POLYGON_FILL_ARGS_LOWER){
    rot3dState* sRot=(rot3dState*)s;
    float dx_r,dx_l,xs,xe;
    int dzdy,dzdx,zs,z;
    int dvdy,dvdx,vs,v;
    int dudy,dudx,us,u;
    int dnxdy,dnxdx,nxs,nx;
    int dnydy,dnydx,nys,ny;

    FBPOINTER framebuffer=CURR_Y_FRAMEBUFFER;
    unsigned int* environmentmap=sRot->current_object->environmentmap;
    int* xlight_disp=sRot->current_object->xlight_disp;
    int* ylight_disp=sRot->current_object->ylight_disp;
    int* zbuffp=CURR_Y_ZBUFFER;
    unsigned int x,y;
    int xd,yd,pixel,light;

    xs=(float) v0->x; xe=(float)0.5+(float) v1x;
    zs=(v0->z)<<9;
    us=(v0->u)<<9;    vs=(v0->v)<<9;
    nxs=(v0->nx)<<9;  nys=(v0->ny)<<9;


    dx_r=d->dxdy_lr;
    dx_l=d->dxdy_ll;
    dzdy =(int)511*(d->dzdy_l);
    dzdx =(d->dzdx);
    dvdy =(int)511*(d->dvdy_l);
    dvdx =(d->dvdx);
    dudy =(int)511*(d->dudy_l);
    dudx =(d->dudx);
    dnxdy=(int)511*(d->dnxdy_l);
    dnxdx=(d->dnxdx);
    dnydy=(int)511*(d->dnydy_l);
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

