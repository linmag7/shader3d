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

