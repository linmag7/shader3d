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

