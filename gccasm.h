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
 * File:   gccasm.h
 * Author: lindholm
 *
 * Created on September 1, 2011, 11:32 PM
 */

#ifndef GCCASM_H
#define	GCCASM_H

#ifdef	__cplusplus
extern "C" {
#endif

#if  defined(__alpha_max__)
#define pixel_light_p(envmap,texture,pixel)  \
__asm__ __volatile__ (  \
"ldl %0,0(%1)\n\t"  \
"ldl $1,0(%2)\n\t" \
"eqv %0,$31,$2\n\t" \
"minub8 $1,$2,$1\n\t"    \
"addq $1, %0,%0\n\t"   \
: "=r" (pixel)   \
: "r" (envmap), "r" (texture) \
: "$1","$2");
#endif

#if defined(__sparc_v9__)
#define pixel_light_p(texture,envmap,pixel)  \
__asm__ __volatile__ (  \
"ld [%1],%%f0\n\t"  \
"ld [%2],%%f1\n\t"  \
"fexpand %%f0,%%f2\n\t" \
"fexpand %%f1,%%f4\n\t" \
"fpadd16 %%f2,%%f4,%%f4\n\t"   \
"fpack16 %%f4,%%f0\n\t" \
"st %%f0, [%0]\n\t"  \
: \
: "r"(&(pixel)), "r" (texture), "r" (envmap) \
: "memory", "%f0","%f1","%f2","%f4");
#endif


#if defined(__sparc_v9__)
#define pixel_light_pb(texture,envmap,phong,pixel)  \
__asm__ __volatile__ (  \
"ld [%1],%%f0\n\t"  \
"ld [%2],%%f1\n\t"  \
"ld [%3],%%f3\n\t"  \
"fmul8x16al %%f0,%%f3,%%f2\n\t" \
"fmul8x16au %%f1,%%f3,%%f4\n\t"  \
"fpadd16 %%f2,%%f4,%%f4\n\t"   \
"fpack16 %%f4,%%f0\n\t" \
"st %%f0, [%0]\n\t"  \
: \
: "r"(&(pixel)), "r" (texture), "r" (envmap), "r" (phong) \
: "memory", "%f0","%f1","%f2","%f3","%f4");
#endif


//"st %%f0, [%0]\n\t"  \

#if (defined(__GNUC__)  && defined(__sparc_v9__))
#define pixel_phong(texture,light,pixel)  \
__asm__ __volatile__ (  \
"ld [%1],%%f0\n\t"  \
"ld [%2],%%f1\n\t"  \
"fmul8x16au %%f0,%%f1,%%f4\n\t"   \
"fpack16 %%f4,%%f0\n\t" \
"st %%f0, [%0]\n\t"  \
: \
: "r"(&(pixel)), "r" (texture), "r" (light) \
: "memory","%f0","%f1","%f4");
#endif

#if  (defined(__GNUC__ ) && defined(__sparc_v9__))
#define set_gsr(dummy) \
__asm__ __volatile__ (  \
"rd %%gsr,%0\n\t"   \
"wr %0,24,%%gsr\n\t"   \
: \
: "r" (dummy) \
: "g0" );
#endif


//"or %0,24,%0\n\t"   \

#ifdef	__cplusplus
}
#endif

#endif	/* GCCASM_H */

