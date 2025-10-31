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
#ifndef _PHONGENV_H
#define	_PHONGENV_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void poly_phongenv(POLYGON_FILL_ARGS);
extern void poly_phongenv_upper(POLYGON_FILL_ARGS);
extern void poly_phongenv_lower(POLYGON_FILL_ARGS_LOWER);


#ifdef	__cplusplus
}
#endif

#endif	/* _PHONGENV_H */

