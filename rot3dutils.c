/* xscreensaver, Copyright (c) 1992-2010 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rotate3d.h"

#define BUMPSTEP 2

void gen_sincos_table(float* sintable,float* costable,int res){
    int i;
    for(i=0;i<res;i++){
        sintable[i]=sin(4*PI*i/res);
        costable[i]=cos(4*PI*i/res);
    }
}

void gen_displacement_map(int* nxdisp, int* nydisp,unsigned int* bumpmap, float bumpsize) {
    int xdisp = 0;
    int ydisp = 0;
    int y, x;
    for (y = 0; y < TEXTURE_SIZE; y++) {
        for (x = 0; x < TEXTURE_SIZE; x++) {
            if ((x < BUMPSTEP) | x > (TEXTURE_SIZE - BUMPSTEP))
                xdisp = 0;
            else
                xdisp = (bumpmap[y * TEXTURE_SIZE | (x - BUMPSTEP)]&0xff)-(bumpmap[y * TEXTURE_SIZE | (x + BUMPSTEP)]&0xff);
            if ((y < BUMPSTEP) | y > (TEXTURE_SIZE - BUMPSTEP))
                ydisp = 0;
            else
                ydisp = (bumpmap[(y - BUMPSTEP) * TEXTURE_SIZE | x]&0xff)-(bumpmap[(y + BUMPSTEP) * TEXTURE_SIZE | x]&0xff);

            nxdisp[y * TEXTURE_SIZE | x] = (int)(xdisp*bumpsize)<<SHIFT_SCALE;
            nydisp[y * TEXTURE_SIZE | x] = (int)(ydisp*bumpsize)<<SHIFT_SCALE;
        }

    }

}

void gen_saturation_table(int* table, int shift) {

    int i;
    float scale=1.0;
    for (i = 0; i < 1024; i++) {
        if (scale*i <0xff)
            table[i] = (int)(scale*i) << shift;
        else
            table[i] = 0xff << shift;
    }


}

void reflection_map(unsigned int *texture,unsigned int *envmap,unsigned int *reflection, float s,float base) {
    unsigned int r, g, b, pixel;
    float w, w_r, w_g, w_b;
    float base_r,base_g,base_b;
    
    int x, y;
    r=0;g=0;b=0;
    for (y = 0; y < TEXTURE_SIZE; y++) {
        for (x = 0; x < TEXTURE_SIZE; x++) {

            r += texture[y * TEXTURE_SIZE + x]&0x000000ff;
            g += (texture[y * TEXTURE_SIZE + x]&0x0000ff00) >> 8;
            b += (texture[y * TEXTURE_SIZE + x]&0x00ff0000) >> 16;
            
            }
        }
    w_r=r/TEXTURE_SIZE*TEXTURE_SIZE;
    w_g=g/TEXTURE_SIZE*TEXTURE_SIZE;
    w_b=b/TEXTURE_SIZE*TEXTURE_SIZE;
    w = sqrt(w_r * w_r + w_g * w_g + w_b * w_b) / sqrt(3);
    if (w<1)
       w=1;
    base_r = base*w_r / w;
    base_g = base*w_g / w;
    base_b = base*w_b / w;


    for (y = 0; y < TEXTURE_SIZE; y++) {
        for (x = 0; x < TEXTURE_SIZE; x++) {

            r = texture[y * TEXTURE_SIZE + x]&0x000000ff;
            g = (texture[y * TEXTURE_SIZE + x]&0x0000ff00) >> 8;
            b = (texture[y * TEXTURE_SIZE + x]&0x00ff0000) >> 16;
            w = sqrt(r * r + g * g + b * b) / sqrt(3);
            if (w<1)
              w=1;
            w_r = r / w+base_r;
            w_g = g / w+base_g;
            w_b = b / w+base_b;

            r = envmap[y * TEXTURE_SIZE + x]&0x000000ff;
            g = (envmap[y * TEXTURE_SIZE + x]&0x0000ff00) >> 8;
            b = (envmap[y * TEXTURE_SIZE + x]&0x00ff0000) >> 16;

            r=(s * r * (w_r*0.25 + 0.5));
            if(r>0xff)
                r=0xff;
            g=(s * g * (w_g*0.25 + 0.5));
            if(g>0xff)
                g=0xff;
            b=(s * b * (w_b*0.25 + 0.5));
            if(b>0xff)
                b=0xff;

            pixel = r |(g << 8) | (b << 16);
            reflection[y * TEXTURE_SIZE + x] = pixel;
        }

    }
}


#define SCALE_1 150
#define SCALE_2 120

#define FACT SCALE_2/SCALE_1
#define PFACT 1.25*SCALE_2/SCALE_1
#define TEXTURE_SIZE_HALF (TEXTURE_SIZE/2)

void gen_phongmap(unsigned int* pmap,float r,float scale){
    float n,p,xx,yy;
    unsigned int light;
    unsigned int light_int;
   
    int y,x;
    for(y=TEXTURE_SIZE-1;y>=0;y--){
        for(x=TEXTURE_SIZE-1;x>=0;x--){
          xx=((float)x-TEXTURE_SIZE_HALF)/TEXTURE_SIZE_HALF;
          yy=((float)y-TEXTURE_SIZE_HALF)/TEXTURE_SIZE_HALF;

          n=1-r*sqrt(xx*xx+yy*yy);
          p=scale*(n+FACT*n*n*n*n*n);

          if(p<0)
              p=0;
          if(p>0xff)
              p=0xff;
          light=(((unsigned int) p) <<24)|(((unsigned int) p) <<16)|(((unsigned int) p) <<8)|((unsigned int) p) ;
          
          pmap[y*TEXTURE_SIZE|x]=light;
        }
    }
}

void gen_phonglightmap(float* pmap,float r,float scale){
    float n,p,xx,yy;
    unsigned char light;
   
    int y,x;
    for(y=0;y<TEXTURE_SIZE;y++){
        for(x=0;x<TEXTURE_SIZE;x++){
          xx=2*((float)x-TEXTURE_SIZE/2)/TEXTURE_SIZE;
          yy=2*((float)y-TEXTURE_SIZE/2)/TEXTURE_SIZE;

          n=1-r*sqrt(xx*xx+yy*yy);
          p=(scale*n+PFACT*scale*n*n*n*n*n);

          if(p<0.4)
              p=0.4;
          if(p>3.2)
              p=3.2;
          
          pmap[y*TEXTURE_SIZE|x]=p;
        }
    }
}
void gen_phonglightmap_vis(unsigned int* pmap,float r,float scale){
    float n,p,xx,yy;
    unsigned char light;
   
    int y,x;
    for(y=0;y<TEXTURE_SIZE;y++){
        for(x=0;x<TEXTURE_SIZE;x++){
          xx=2*((float)x-TEXTURE_SIZE/2)/TEXTURE_SIZE;
          yy=2*((float)y-TEXTURE_SIZE/2)/TEXTURE_SIZE;

          n=1-r*sqrt(xx*xx+yy*yy);
          p=(scale*n+PFACT*scale*n*n*n*n*n);

          if(p<0.1)
              p=0.1;
          if(p>3.5)
              p=3.5;
          
          pmap[y*TEXTURE_SIZE|x]=((unsigned int)(0xaff*p)<<16)|(unsigned int)(0xfff*p);
        }
    }
}

void load_tga(char* filename,unsigned int* buffer){
tgaHEADER header;
FILE *fptr;
int skipover=0,bpp;
unsigned char p[4];

int i;



	if ((fptr = fopen(filename,"r")) == NULL) {
		printf("File open failed [%s]\n",filename);
     		exit(-1); 
   	}
	header.idlength = fgetc(fptr);
   	header.colourmaptype = fgetc(fptr);
   	header.datatypecode = fgetc(fptr);
   
   	if (!fread(&header.colourmaporigin,2,1,fptr)) {
   		printf("Error reading from [%s]\n",filename);
		exit(-1);
   	}
   
	if (!fread(&header.colourmaplength,2,1,fptr)) {
   		printf("Error reading from [%s]\n",filename);
		exit(-1);
	}

  
	header.colourmapdepth = fgetc(fptr);
 
	if(!fread(&header.x_origin,2,1,fptr)) {
		printf("Error reading from [%s]\n",filename);
		exit(-1);
	}
  
	if(!fread(&header.y_origin,2,1,fptr)) {
		printf("Error reading from [%s]\n",filename);
		exit(-1);
  	}
	if(!fread(&header.width,2,1,fptr)) {
		printf("Error reading from [%s]\n",filename);
		exit(-1);
	}
	if(!fread(&header.height,2,1,fptr)) {
		printf("Error reading from [%s]\n",filename);
		exit(-1);
	}
  	header.bitsperpixel = fgetc(fptr);
 	header.imagedescriptor = fgetc(fptr);
	
	skipover += header.idlength;
	skipover += header.colourmaptype * header.colourmaplength;
  
	fseek(fptr,skipover,SEEK_CUR);

	bpp = header.bitsperpixel / 8;
	for(i=0;i<TEXTURE_SIZE*TEXTURE_SIZE;i++){
		if (fread(p,1,bpp,fptr) != bpp) {
			fprintf(stderr,"Unexpected end of file at pixel %d\n",i);
               		exit(-1);
            }

        buffer[i]=((int)p[2])<<16 | ((int)p[1])<<8 | ((int)p[0]);
   	}
   	fclose(fptr);
   
}

