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
 *  A small demo of polygon texture mapping and shading. A few differen
 *  light/shadning model. Created by Magnus Lindholm (linmag7@gmail.com)
 *
 *
 *  Modification history:
 *
 *  2010-06-01: Magnus Lindholm     Creation
 */

#include "rotate3d.h"
#include "pbenvtex.h"
#include "envbump.h"
#include "pbenvtex.h"
#include "phongtex.h"
#include "texturemap.h"
#include "envmap.h"
#include "phongenv.h"
#include "polygon.h"




#define ROTATE_X o->rot_x
#define ROTATE_Y o->rot_y
#define ROTATE_Z o->rot_z

#define ROTATE_NX o->rot_nx
#define ROTATE_NY o->rot_ny
#define ROTATE_NZ o->rot_nz



#define VIEW_PORT_X  o->trans_x
#define VIEW_PORT_Y  o->trans_y
#define VIEW_PORT_Z  o->trans_z



#define LIMIT 0

static void poly_texture_lower_clip(polygon* p){}
static void poly_texture_upper_clip(polygon* p){}

static void rotate_x(float* costable,float* x, float* y, float* z,int ang){
/*      X = x
 *      Y = y * cos v - z * sin v
 *      Z = y * sin v + z * cos v
 */
    float yy=(*y * costable[ang] - *z * costable[ang+64]);
          *z=(*y * costable[ang+64] + *z * costable[ang]);
          *y=yy;

}
static void rotate_y(float* costable,float* x, float* y, float* z,int ang){
/*      X = x * cos v - z * sin v
 *      Y = y
 *      Z = x * sin v + z * cos v
 */
    float xx=(*x * costable[ang] - *z * costable[ang+64]);
          *z=(*x * costable[ang+64] + *z * costable[ang]);
          *x=xx;
}
static void rotate_z(float* costable,float* x, float* y, float* z,int ang){
/*      X = x * cos v - y * sin v
 *      Y = x * sin v + y * cos v
 *      Z = z
 */
    float xx=(*x * costable[ang] - *y * costable[ang+64]);
          *y=(*x * costable[ang+64] + *y * costable[ang]);
          *x=xx;
}


static void poly_texture(polygon* p,rot3dState* sRot){

    object3d* o=sRot->current_object;
    float dy01,dy12,dy02;


    float lenght0, lenght1;

    polygon p_upper,p_lower;
    vertex v3,v4;
    vector va,vb,vx,vy;
    float f;
    vertex* temp;
    delta   delta_values;
    int add_a,add_b;
    int vcx;

    va.x=p->v1->x - p->v0->x;
    if(!p->side)
    	return;

    va.y=p->v1->y - p->v0->y;
    va.z=p->v1->z - p->v0->z;
    va.u=p->v1->u - p->v0->u;
    va.v=p->v1->v - p->v0->v;
    va.nx=p->v1->nx - p->v0->nx;
    va.ny=p->v1->ny - p->v0->ny;

    vb.x=p->v2->x - p->v0->x;
    vcx=va.x-vb.x;

    vb.y=p->v2->y - p->v0->y;
    vb.z=p->v2->z - p->v0->z;
    vb.u=p->v2->u - p->v0->u;
    vb.v=p->v2->v - p->v0->v;
    vb.nx=p->v2->nx - p->v0->nx;
    vb.ny=p->v2->ny - p->v0->ny;



    f=(vb.y*va.x-va.y*vb.x);
    if(f*f<0.001)
        f=0.01;

    vx.z=(vb.y*va.z-va.y*vb.z)/f;
    vx.u=(vb.y*va.u-va.y*vb.u)/f;
    vx.v=(vb.y*va.v-va.y*vb.v)/f;
    vx.nx=(vb.y*va.nx-va.y*vb.nx)/f;
    vx.ny=(vb.y*va.ny-va.y*vb.ny)/f;
    vx.x=1;
    vx.y=0;

    delta_values.dzdx=(TEXTURE_SIZE-1)*vx.z;
    delta_values.dudx=(TEXTURE_SIZE-1)*vx.u;
    delta_values.dvdx=(TEXTURE_SIZE-1)*vx.v;
    delta_values.dnxdx=(TEXTURE_SIZE-1)*vx.nx;
    delta_values.dnydx=(TEXTURE_SIZE-1)*vx.ny;


    if(p->clip!=INVISIBLE){
    lenght0=(p->v0->y)-(p->v1->y);
    lenght1=(p->v1->y)-(p->v2->y);

        if(lenght0==0 && lenght1!=0){
            if (p->v0->x >= p->v1->x){    /*swap x0 and x1 so that x0<=x1 is always true*/
                temp=p->v0;
                p->v0=p->v1;
                p->v1=temp;
            }

            delta_values.dy12=vb.y;
            delta_values.dxdy_ll=(p->v2->x - p->v0->x)/vb.y;
            delta_values.dxdy_lr=(p->v2->x - p->v1->x)/vb.y;
            delta_values.dzdy_l=(p->v2->z - p->v0->z)/vb.y;
            delta_values.dudy_l=(p->v2->u - p->v0->u)/vb.y;
            delta_values.dvdy_l=(p->v2->v - p->v0->v)/vb.y;
            delta_values.dnxdy_l=(p->v2->nx - p->v0->nx)/vb.y;
            delta_values.dnydy_l=(p->v2->ny - p->v0->ny)/vb.y;

           if(p->clip==NO_CLIP){
               if(p->side)
                o->draw_front_face_lower(POLYGON_FILL_PARAM_LOWER);

           }
           else
            poly_texture_lower_clip(p);
        }

        else if(lenght1==0 && lenght0!=0){
            if (p->v1->x >= p->v2->x){    /*swap x1 and x2 so that x1<=x2 is always true*/
                temp=p->v1;
                p->v1=p->v2;
                p->v2=temp;
            }


            delta_values.dy01=va.y;
            delta_values.dxdy_ul=(p->v1->x - p->v0->x)/va.y;
            delta_values.dxdy_ur=(p->v2->x - p->v0->x)/vb.y;
            delta_values.dzdy_u=(p->v1->z - p->v0->z)/va.y;
            delta_values.dudy_u=(p->v1->u - p->v0->u)/va.y;
            delta_values.dvdy_u=(p->v1->v - p->v0->v)/va.y;
            delta_values.dnxdy_u=(p->v1->nx - p->v0->nx)/va.y;
            delta_values.dnydy_u=(p->v1->ny - p->v0->ny)/va.y;

            if(p->clip==NO_CLIP){
                if(p->side)
                    o->draw_front_face_upper(POLYGON_FILL_PARAM);

            }
            else
                poly_texture_upper_clip(p);

        }

        else if (lenght1!=0 && lenght0!=0){

            /*dy01=p->v1->y - p->v0->y;*/
            dy12=p->v2->y - p->v1->y;
            /*dy02=p->v2->y - p->v0->y;*/
            delta_values.dy01=va.y;
            delta_values.dy12=dy12;


            temp=p->v0;
            v3.x= ( (vb.x/vb.y) * (va.y) + p->v0->x);

            /* left side delta values */
            if(p->v1->x<v3.x){
            delta_values.dxdy_ul=(va.x)/va.y;
            delta_values.dxdy_ur=(vb.x)/vb.y;
/*
            delta_values.dzdy_u=(p->v1->z - p->v0->z)/dy01;
            delta_values.dudy_u=(p->v1->u - p->v0->u)/dy01;
            delta_values.dvdy_u=(p->v1->v - p->v0->v)/dy01;
            delta_values.dnxdy_u=(p->v1->nx - p->v0->nx)/dy01;
            delta_values.dnydy_u=(p->v1->ny - p->v0->ny)/dy01;
*/
            delta_values.dzdy_u=(va.z)/va.y;
            delta_values.dudy_u=(va.u)/va.y;
            delta_values.dvdy_u=(va.v)/va.y;
            delta_values.dnxdy_u=(va.nx)/va.y;
            delta_values.dnydy_u=(va.ny)/va.y;

            delta_values.dxdy_ll=(p->v2->x - p->v1->x)/dy12;
            delta_values.dxdy_lr=delta_values.dxdy_ur;
            delta_values.dzdy_l=(p->v2->z - p->v1->z)/dy12;
            delta_values.dudy_l=(p->v2->u - p->v1->u)/dy12;
            delta_values.dvdy_l=(p->v2->v - p->v1->v)/dy12;
            delta_values.dnxdy_l=(p->v2->nx - p->v1->nx)/dy12;
            delta_values.dnydy_l=(p->v2->ny - p->v1->ny)/dy12;

            }
            else {
            delta_values.dxdy_ul=(vb.x)/vb.y;
            delta_values.dxdy_ur=(va.x)/va.y;
/*
            delta_values.dzdy_u=(p->v2->z - p->v0->z)/dy02;
            delta_values.dudy_u=(p->v2->u - p->v0->u)/dy02;
            delta_values.dvdy_u=(p->v2->v - p->v0->v)/dy02;
            delta_values.dnxdy_u=(p->v2->nx - p->v0->nx)/dy02;
            delta_values.dnydy_u=(p->v2->ny - p->v0->ny)/dy02;
*/
            delta_values.dzdy_u=(vb.z)/vb.y;
            delta_values.dudy_u=(vb.u)/vb.y;
            delta_values.dvdy_u=(vb.v)/vb.y;
            delta_values.dnxdy_u=(vb.nx)/vb.y;
            delta_values.dnydy_u=(vb.ny)/vb.y;

            delta_values.dxdy_ll=delta_values.dxdy_ul;
            delta_values.dxdy_lr=(p->v2->x - p->v1->x)/dy12;
            delta_values.dzdy_l=delta_values.dzdy_u;
            delta_values.dudy_l=delta_values.dudy_u;
            delta_values.dvdy_l=delta_values.dvdy_u;
            delta_values.dnxdy_l=delta_values.dnxdy_u;
            delta_values.dnydy_l=delta_values.dnydy_u;

            }

            if(p->clip==NO_CLIP){
                if(p->side)
                   o->draw_front_face(POLYGON_FILL_PARAM);

            }
            else {
                poly_texture_upper_clip(&p_upper);
                poly_texture_lower_clip(&p_lower);
            }

        }
    }

}


void draw_object(rot3dState* sRot){
    object3d* o=sRot->current_object;
    polygon p;
    vertex v0,v1,v2;
    int i;

    p.v0=&v0; p.v1=&v1; p.v2=&v2;

    switch(o->mapping){

    case  FLAT:
      break;
    case  TEXTURE:
        o->draw_front_face=poly_texmap;
        o->draw_front_face_upper=poly_texmap_upper;
        o->draw_front_face_lower=poly_texmap_lower;
        break;
    case  PHONGTEX:
       o->draw_front_face=poly_phongtex;
       o->draw_front_face_upper=poly_phongtex_upper;
       o->draw_front_face_lower=poly_phongtex_lower;
      break;
    case  PBTEXTURE:
        o->draw_front_face=poly_texmap;
        o->draw_front_face_upper=poly_texmap_upper;
        o->draw_front_face_lower=poly_texmap_lower;
      break;
    case  REFLECTION:
       o->draw_front_face=poly_envmap;
       o->draw_front_face_upper=poly_envmap_upper;
       o->draw_front_face_lower=poly_envmap_lower;
      break;
    case  ENVBUMP:
       o->draw_front_face=poly_envbump;
       o->draw_front_face_upper=poly_envbump_upper;
       o->draw_front_face_lower=poly_envbump_lower;
      break;
    case  PHONGENV:
      o->draw_front_face=poly_phongenv;
      o->draw_front_face_upper=poly_phongenv_upper;
      o->draw_front_face_lower=poly_phongenv_lower;
      break;
    case  PHONGBUMPTEX:
        o->draw_front_face=poly_texmap;
        o->draw_front_face_upper=poly_texmap_upper;
        o->draw_front_face_lower=poly_texmap_lower;
       break;
    case PENVBTEX:
       o->draw_front_face=poly_phongbumptex;
       o->draw_front_face_upper=poly_phongbumptex_upper;
       o->draw_front_face_lower=poly_phongbumptex_lower;
       break;
    default:
       o->draw_front_face=poly_texmap;
       o->draw_front_face_upper=poly_texmap_upper;
       o->draw_front_face_lower=poly_texmap_lower;
       break;
    }

    for(i=0;i<o->nfaces;i++){
        poly3dto2d(sRot,&p,3*i);
        poly_texture(&p,sRot);
    }
}




void poly3dto2d(rot3dState* sRot, polygon* p, int index){
   object3d* o=sRot->current_object;
   float* costable=sRot->costable;
   float* sintable=sRot->sintable;
   float x,y,z,u,v,nx,ny,nz;
   int vtx;
   vertex* temp;
   float* vertexlist=o->vertex;
   float* normallist=o->fnormal;
   int* facelist=o->facelist;
   float* vertex_texture=o->texture_uv;
   float* vertex_normal=o->vnormal;
   int roto_x=o->roto_x;
   int roto_y=o->roto_y;
   int roto_z=o->roto_z;

    vtx=facelist[index]*3;    
    x=vertexlist[vtx];
    y=vertexlist[vtx+1];
    z=vertexlist[vtx+2];
    nx=vertex_normal[vtx];
    ny=vertex_normal[vtx+1];
    nz=vertex_normal[vtx+2];
    vtx=facelist[index]<<1;
    u=vertex_texture[vtx];
    v=vertex_texture[vtx+1];

    rotate_x(costable,&x,&y,&z,roto_x);
    rotate_y(costable,&x,&y,&z,roto_y);
    rotate_z(costable,&x,&y,&z,roto_z);
    rotate_x(costable,&nx,&ny,&nz,roto_x);
    rotate_y(costable,&nx,&ny,&nz,roto_y);
    rotate_z(costable,&nx,&ny,&nz,roto_z);

    rotate_x(costable,&x,&y,&z,ROTATE_X);
    rotate_y(costable,&x,&y,&z,ROTATE_Y);
    rotate_z(costable,&x,&y,&z,ROTATE_Z);
    rotate_x(costable,&nx,&ny,&nz,ROTATE_X+ROTATE_NX);
    rotate_y(costable,&nx,&ny,&nz,ROTATE_Y+ROTATE_NY);
    rotate_z(costable,&nx,&ny,&nz,ROTATE_Z+ROTATE_NZ);




    p->v0->x=SCALE*x/(z+VIEW_PORT_Z)+VIEW_PORT_X;
    p->v0->y=SCALE*y/(z+VIEW_PORT_Z)+VIEW_PORT_Y;
    p->v0->z=SCALE*z/(z+VIEW_PORT_Z)+VIEW_PORT_Z;
    p->v0->u=u*TEXTURE_LIGHTMAP_SCALE;
    p->v0->v=v*TEXTURE_LIGHTMAP_SCALE;
    p->v0->nx=(nx+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v0->ny=(ny+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v0->nz=(nz+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;

    vtx=facelist[index+1]*3;
    x=vertexlist[vtx];
    y=vertexlist[vtx+1];
    z=vertexlist[vtx+2];
    nx=vertex_normal[vtx];
    ny=vertex_normal[vtx+1];
    nz=vertex_normal[vtx+2];
    vtx=facelist[index+1]<<1;
    u=vertex_texture[vtx];
    v=vertex_texture[vtx+1];

    rotate_x(costable,&x,&y,&z,roto_x);
    rotate_y(costable,&x,&y,&z,roto_y);
    rotate_z(costable,&x,&y,&z,roto_z);
    rotate_x(costable,&nx,&ny,&nz,roto_x);
    rotate_y(costable,&nx,&ny,&nz,roto_y);
    rotate_z(costable,&nx,&ny,&nz,roto_z);

    rotate_x(costable,&x,&y,&z,ROTATE_X);
    rotate_y(costable,&x,&y,&z,ROTATE_Y);
    rotate_z(costable,&x,&y,&z,ROTATE_Z);
    rotate_x(costable,&nx,&ny,&nz,ROTATE_X+ROTATE_NX);
    rotate_y(costable,&nx,&ny,&nz,ROTATE_Y+ROTATE_NY);
    rotate_z(costable,&nx,&ny,&nz,ROTATE_Z+ROTATE_NZ);
    



    p->v1->x=SCALE*x/(z+VIEW_PORT_Z)+VIEW_PORT_X;
    p->v1->y=SCALE*y/(z+VIEW_PORT_Z)+VIEW_PORT_Y;
    p->v1->z=SCALE*z/(z+VIEW_PORT_Z)+VIEW_PORT_Z;
    p->v1->u=u*TEXTURE_LIGHTMAP_SCALE;
    p->v1->v=v*TEXTURE_LIGHTMAP_SCALE;
    p->v1->nx=(nx+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v1->ny=(ny+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v1->nz=(nz+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;

  
    vtx=facelist[index+2]*3;
    x=vertexlist[vtx];
    y=vertexlist[vtx+1];
    z=vertexlist[vtx+2];
    nx=vertex_normal[vtx];
    ny=vertex_normal[vtx+1];
    nz=vertex_normal[vtx+2];
    vtx=facelist[index+2]<<1;
    u=vertex_texture[vtx];
    v=vertex_texture[vtx+1];

    rotate_x(costable,&x,&y,&z,roto_x);
    rotate_y(costable,&x,&y,&z,roto_y);
    rotate_z(costable,&x,&y,&z,roto_z);
    rotate_x(costable,&nx,&ny,&nz,roto_x);
    rotate_y(costable,&nx,&ny,&nz,roto_y);
    rotate_z(costable,&nx,&ny,&nz,roto_z);

    rotate_x(costable,&x,&y,&z,ROTATE_X);
    rotate_y(costable,&x,&y,&z,ROTATE_Y);
    rotate_z(costable,&x,&y,&z,ROTATE_Z);
    rotate_x(costable,&nx,&ny,&nz,ROTATE_X+ROTATE_NX);
    rotate_y(costable,&nx,&ny,&nz,ROTATE_Y+ROTATE_NY);
    rotate_z(costable,&nx,&ny,&nz,ROTATE_Z+ROTATE_NZ);



    p->v2->x=SCALE*x/(z+VIEW_PORT_Z)+VIEW_PORT_X;
    p->v2->y=SCALE*y/(z+VIEW_PORT_Z)+VIEW_PORT_Y;
    p->v2->z=SCALE*z/(z+VIEW_PORT_Z)+VIEW_PORT_Z;
    p->v2->u=u*TEXTURE_LIGHTMAP_SCALE;
    p->v2->v=v*TEXTURE_LIGHTMAP_SCALE;
    p->v2->nx=(nx+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v2->ny=(ny+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;
    p->v2->nz=(nz+OBJECT_UV_SCALE/2)*TEXTURE_LIGHTMAP_SCALE;


    x=normallist[index+0];
    y=normallist[index+1];
    z=normallist[index+2];
    rotate_x(costable,&x,&y,&z,roto_x);
    rotate_y(costable,&x,&y,&z,roto_y);
    rotate_z(costable,&x,&y,&z,roto_z);
    rotate_x(costable,&x,&y,&z,ROTATE_X);
    rotate_y(costable,&x,&y,&z,ROTATE_Y);
    rotate_z(costable,&x,&y,&z,ROTATE_Z);
   

    /*rotate_z(&x,&y,&z,rotate+ROTATE_Y);
    p->nx=x;
    p->ny=y;
    p->nz=z;
     if we need normal to face, uncomment above
    */
    if(z<=0)
       p->side=0;

    else
       p->side=1;

    /* sort polygon */

        if(p->v0->y>p->v1->y){
            temp=p->v0;
            p->v0=p->v1;
            p->v1=temp;
        }
    /* now y1 is greater than y0 */

         if(p->v0->y>p->v2->y){
             temp=p->v0;
             p->v0=p->v2;
             p->v2=temp;
        }
     /* now y1 and y2 is greater than y0 */
        if(p->v1->y>p->v2->y){
             temp=p->v1;
             p->v1=p->v2;
             p->v2=temp;
        }
    /* now the order is y2, y1, y0 */

    /* don't draw lines completely outside screen */
    p->clip=CLIP;
    if(p->v0->y>0 && p->v2->y<Y_RES && max(max(p->v0->x,p->v1->x),p->v2->x)<X_RES && min(min(p->v0->x,p->v1->x),p->v2->x)>0)
        p->clip=NO_CLIP;
    else if(p->v0->y>Y_RES || p->v2->y<0 || max(max(p->v0->x,p->v1->x),p->v2->x)<0 || min(min(p->v0->x,p->v1->x),p->v2->x)>X_RES)
          p->clip=INVISIBLE;

}
