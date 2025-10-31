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
 * File:   envbump.h
 * Author: lindholm
 *
 * Created on May 30, 2010, 1:39 AM
 */

#ifndef _ENVBUMP_H
#define	_ENVBUMP_H

#ifdef	__cplusplus
extern "C" {
#endif



extern void poly_envbump(POLYGON_FILL_ARGS);
extern void poly_envbump_upper(POLYGON_FILL_ARGS);
extern void poly_envbump_lower(POLYGON_FILL_ARGS_LOWER);


#ifdef	__cplusplus
}
#endif

#endif	/* _ENVBUMP_H */

