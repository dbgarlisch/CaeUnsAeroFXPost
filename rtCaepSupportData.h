/****************************************************************************
 *
 * Pointwise Plugin utility functions
 *
 * Proprietary software product of Pointwise, Inc.
 * Copyright (c) 1995-2014 Pointwise, Inc.
 * All rights reserved.
 *
 ***************************************************************************/

#ifndef _RTCAEPSUPPORTDATA_H_
#define _RTCAEPSUPPORTDATA_H_

/*! \cond */

/*------------------------------------*/
/* CaeUnsAeroFXPost format item setup data */
/*------------------------------------*/
CAEP_BCINFO CaeUnsAeroFXPostBCInfo[] = {
    { "Inflow", 100 },
    { "Outflow", 101 },
    { "Wall", 102 },
};

/*------------------------------------*/
CAEP_VCINFO CaeUnsAeroFXPostVCInfo[] = {
    { "Fluid", 200 },
    { "Solid", 201 },
};

/*------------------------------------*/
const char *CaeUnsAeroFXPostFileExt[] = {
    "XPOST"
};
/*! \endcond */

#endif /* _RTCAEPSUPPORTDATA_H_ */
