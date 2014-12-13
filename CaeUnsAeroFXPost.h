/****************************************************************************
 *
 * class CaeUnsAeroFXPost
 *
 * Proprietary software product of Pointwise, Inc.
 * Copyright (c) 1995-2014 Pointwise, Inc.
 * All rights reserved.
 *
 ***************************************************************************/

#ifndef _CAEUNSAEROFXPOST_H_
#define _CAEUNSAEROFXPOST_H_

#include <string>

#include "apiGridModel.h"
#include "apiPWP.h"

#include "CaePlugin.h"
#include "CaeUnsGridModel.h"


//***************************************************************************
//***************************************************************************
//***************************************************************************

class CaeUnsAeroFXPost :
    public CaeUnsPlugin,
    public CaeFaceStreamHandler {

public:
    CaeUnsAeroFXPost(CAEP_RTITEM *pRti, PWGM_HGRIDMODEL model,
        const CAEP_WRITEINFO *pWriteInfo);
    virtual ~CaeUnsAeroFXPost();
    static bool create(CAEP_RTITEM &rti);
    static void destroy(CAEP_RTITEM &rti);

private:

    virtual bool        beginExport();
    virtual PWP_BOOL    write();
    virtual bool        endExport();

    bool    writeNodes();
    bool    writeBlocks();
    bool    writeBlock(const CaeUnsBlock &blk);
    bool    writeBoundaries();
    bool    writeBoundary(const CaeUnsPatch &patch);
    bool    writeElements(const PWGM_CONDDATA &cond, const CaeUnsElementGroup &elemGrp);
    bool    writeCell(PWP_UINT32 id, PWGM_ELEMDATA &eData);

    // face streaming handlers
    virtual PWP_UINT32 streamBegin(const PWGM_BEGINSTREAM_DATA &data);
    virtual PWP_UINT32 streamFace(const PWGM_FACESTREAM_DATA &data);
    virtual PWP_UINT32 streamEnd(const PWGM_ENDSTREAM_DATA &data);

private:
};

#endif // _CAEUNSAEROFXPOST_H_
