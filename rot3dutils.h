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
#ifndef _BUMPMAP_H
#define	_BUMPMAP_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PI 3.141596
extern void gen_displacement_map(int*, int*,unsigned int*, float);
extern void gen_sincos_table(float*,float*,int);
extern void reflection_map(unsigned int*,unsigned int*,unsigned int*, float,float);
extern void gen_phongmap(unsigned int*,float,float);


#ifdef	__cplusplus
}
#endif

#endif	/* _BUMPMAP_H */

