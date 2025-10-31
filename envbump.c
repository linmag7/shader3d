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
#include "envbump.h"

#ifdef LIGHT_LOOKUP
 #undef LIGHT_LOOKUP
#endif
#define LIGHT_LOOKUP (yd&BIT_MASK)|((xd&BIT_MASK)>>9)
#ifdef PIXEL_LOOKUP
 #undef PIXEL_LOOKUP
#endif
#define PIXEL_LOOKUP (v&BIT_MASK)|((u>>9)&0x1ff)
#ifdef FBPOINTER
 #undef FBPOINTER
#endif
#define FBPOINTER int*

void poly_envbump(POLYGON_FILL_ARGS){
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
    int* xlight_disp=sRot->current_object->xlight_disp;
    int* ylight_disp=sRot->current_object->ylight_disp;
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
               pixel=PIXEL_LOOKUP;
               xd=xlight_disp[pixel]+nx;
               yd=ylight_disp[pixel]+ny;
               framebuffer[x]=environmentmap[LIGHT_LOOKUP];
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
                pixel=PIXEL_LOOKUP;
                xd=xlight_disp[pixel]+nx;
                yd=ylight_disp[pixel]+ny;
                framebuffer[x]=environmentmap[LIGHT_LOOKUP];
            }

            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;

        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;

    }

}

void poly_envbump_upper(POLYGON_FILL_ARGS){
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
               pixel=PIXEL_LOOKUP;
               xd=xlight_disp[pixel]+nx;
               yd=ylight_disp[pixel]+ny;
               framebuffer[x]=environmentmap[LIGHT_LOOKUP];
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;

        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;
    }
}

void poly_envbump_lower(POLYGON_FILL_ARGS_LOWER){
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
                pixel=PIXEL_LOOKUP;
                xd=xlight_disp[pixel]+nx;
                yd=ylight_disp[pixel]+ny;
                framebuffer[x]=environmentmap[LIGHT_LOOKUP];
            }
            z+=dzdx; u+=dudx; v+=dvdx; nx+=dnxdx; ny+=dnydx;
        }
        xs+=dx_l; xe+=dx_r; zs+=dzdy; us+=dudy; vs+=dvdy; nxs+=dnxdy; nys+=dnydy;
    }

}


