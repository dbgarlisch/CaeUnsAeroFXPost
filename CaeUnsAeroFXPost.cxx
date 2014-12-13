/****************************************************************************
 *
 * class CaeUnsAeroFXPost
 *
 * Proprietary software product of Pointwise, Inc.
 * Copyright (c) 1995-2014 Pointwise, Inc.
 * All rights reserved.
 *
 ***************************************************************************/

#include <algorithm>
#include <limits>
#include <string>

#include "apiCAEP.h"
#include "apiCAEPUtils.h"
#include "apiGridModel.h"
#include "apiPWP.h"
#include "runtimeWrite.h"
#include "pwpPlatform.h"

#include "CaePlugin.h"
#include "CaeUnsGridModel.h"
#include "CaeUnsAeroFXPost.h"


//***************************************************************************
//***************************************************************************
//***************************************************************************

CaeUnsAeroFXPost::CaeUnsAeroFXPost(CAEP_RTITEM *pRti, PWGM_HGRIDMODEL
        model, const CAEP_WRITEINFO *pWriteInfo) :
    CaeUnsPlugin(pRti, model, pWriteInfo)
{
}


CaeUnsAeroFXPost::~CaeUnsAeroFXPost()
{
}


bool
CaeUnsAeroFXPost::beginExport()
{
    //PWP_BOOL doDump;
    //model_.getAttribute("debugDump", doDump);
    //model_.getAttribute("quality", quality_);
    setProgressMajorSteps(3);
    return true;
}


PWP_BOOL
CaeUnsAeroFXPost::write()
{
    return writeNodes() && writeBlocks() && writeBoundaries();
}


bool
CaeUnsAeroFXPost::endExport()
{
    return true;
}


bool
CaeUnsAeroFXPost::writeNodes()
{
    bool ret = progressBeginStep(model_.vertexCount());
    //rtFile_.write("//\n");
    //rtFile_.write("// Created by Pointwise\n");
    //rtFile_.write("//\n");
    //rtFile_.write("\n");
    rtFile_.write("Nodes AllNodes\n");
    CaeUnsVertex v(model_);
    while (ret && v.isValid()) {
        ret = rtFile_.write(v.index() + 1, " ") && rtFile_.write(v.x(), " ") &&
            rtFile_.write(v.y(), " ") && rtFile_.write(v.z(), "\n") &&
            progressIncrement();
        ++v;
    }
    return progressEndStep() && ret;
}


bool
CaeUnsAeroFXPost::writeBlocks()
{
    bool ret = progressBeginStep(model_.elementCount());
    CaeUnsBlock blk(model_);
    while (blk.isValid()) {
        if (!writeBlock(blk)) {
            ret = false;
            break;
        }
        ++blk;
    }
    return progressEndStep() && ret;
}


bool
CaeUnsAeroFXPost::writeBlock(const CaeUnsBlock &blk)
{
    PWGM_CONDDATA vc;
    return blk.condition(vc) && writeElements(vc, blk);
}


bool
CaeUnsAeroFXPost::writeBoundaries()
{
    // count the total number of surface elements
    PWP_UINT32 cnt  = 0;
    CaeUnsPatch patch(model_);
    while (patch.isValid()) {
        cnt += patch.elementCount();
        ++patch;
    }
    bool ret = progressBeginStep(cnt);
    patch.moveFirst(model_);
    while (patch.isValid()) {
        if (!writeBoundary(patch)) {
            ret = false;
            break;
        }
        ++patch;
    }
    return progressEndStep() && ret;
}


bool
CaeUnsAeroFXPost::writeBoundary(const CaeUnsPatch &patch)
{
    PWGM_CONDDATA bc;
    return patch.condition(bc) && writeElements(bc, patch);
}


static const PWP_UINT32 UndefId =
    PWP_UINT32(std::numeric_limits<PWP_INT32>::max());

bool
CaeUnsAeroFXPost::writeElements(const PWGM_CONDDATA &cond,
    const CaeUnsElementGroup &elemGrp)
{
    bool ret = true;
    if (UndefId != cond.id) {
        std::string name(cond.name);
        // replace blanks with underscores.
        std::replace(name.begin(), name.end(), ' ', '_');

        rtFile_.write("Elements ");
        rtFile_.write(name.c_str());
        rtFile_.write(" using AllNodes\n");
        CaeUnsElement e(elemGrp);
        PWGM_ELEMDATA eData;
        while (e.data(eData)) {
            if (!(writeCell(e.index() + 1, eData) && progressIncrement())) {
                ret = false;
                break;
            }
            ++e;
        }
    }
    return ret;
}


bool
CaeUnsAeroFXPost::writeCell(PWP_UINT32 id, PWGM_ELEMDATA &eData)
{
    const PWP_UINT32 xpTypes[PWGM_ELEMTYPE_SIZE] = {
        1, // PWGM_ELEMTYPE_BAR
        3, // PWGM_ELEMTYPE_HEX
        2, // PWGM_ELEMTYPE_QUAD
        4, // PWGM_ELEMTYPE_TRI
        5, // PWGM_ELEMTYPE_TET
        10, // PWGM_ELEMTYPE_WEDGE
        0, // PWGM_ELEMTYPE_PYRAMID
        0, // PWGM_ELEMTYPE_POINT
    };
    PWP_UINT32 cellType = xpTypes[eData.type];
    bool ret = (0 != eData.vertCnt) && (0 != cellType);
    if (ret) {
        ret = rtFile_.write(id, " ") && rtFile_.write(cellType, " ");
        for (PWP_UINT32 ndx = 0; ret && ndx < eData.vertCnt - 1; ++ndx) {
            ret = rtFile_.write(eData.index[ndx] + 1, " ");
        }
        ret = ret && rtFile_.write(eData.index[eData.vertCnt - 1] + 1, "\n");
    }
    return ret;
}


//===========================================================================
// face streaming handlers
//===========================================================================

PWP_UINT32
CaeUnsAeroFXPost::streamBegin(const PWGM_BEGINSTREAM_DATA &data)
{
    char msg[128];
    sprintf(msg, "STREAM BEGIN: %lu", (unsigned long)data.totalNumFaces);
    sendInfoMsg(msg);
    return 1;
}

PWP_UINT32
CaeUnsAeroFXPost::streamFace(const PWGM_FACESTREAM_DATA &data)
{
    char msg[128];
    sprintf(msg, "  STREAM FACE: %lu %lu", (unsigned long)data.owner.cellIndex,
        (unsigned long)data.face);
    sendInfoMsg(msg);
    return 1;
}

PWP_UINT32
CaeUnsAeroFXPost::streamEnd(const PWGM_ENDSTREAM_DATA &data)
{
    char msg[128];
    sprintf(msg, "STREAM END: %s", (data.ok ? "true" : "false"));
    sendInfoMsg(msg);
    return 1;
}


//===========================================================================
// called ONCE when plugin first loaded into memory
//===========================================================================

bool
CaeUnsAeroFXPost::create(CAEP_RTITEM &rti)
{
    (void)rti.BCCnt; // silence unused arg warning
    bool ret = true;

    // Uncomment this INFO attribute if your solver supports both big and
    // little endian byte orderings.
    //ret = ret && caeuAssignInfoValue("AllowedFileByteOrders",
    //                "BigEndian|LittleEndian", true);

    // Uncomment one of these INFO attributes if your solver requires a
    // particular byte ordering.
    //ret = ret && caeuAssignInfoValue("AllowedFileByteOrders", "BigEndian",
    //                true);
    //ret = ret && caeuAssignInfoValue("AllowedFileByteOrders", "LittleEndian",
    //                true);

    // These attributes are for example only. You can publish any attribute
    // needed for your solver.
    // ret = ret &&
    //     publishUIntValueDef(rti, "iterations", 5, "Number of iterations", 0,
    //          2000) &&
    //     publishIntValueDef(rti, "magnitude", -5, "Signed int magnitude",
    //          -100, 100) &&
    //     publishRealValueDef(rti, "mach", 0.3, "Incoming flow velocity", 0.0,
    //          1000.0, 0.0, 50.0) &&
    //     publishRealValueDef(rti, "temperature", 77.5, "Ambient temperature",
    //          -5000.0, 5000.0, -100.0, 3000.0) &&
    //     publishEnumValueDef(rti, "temperature.units", "Fahrenheit",
    //          "Grid temperature units", "Fahrenheit|Celsius") &&
    //     publishEnumValueDef(rti, "units", "Inches", "Grid dimensional units",
    //          "Yards|Inches|Meters|Millimeters") &&
    //     publishStringValueDef(rti, "description", "", "Grid description") &&
    //     publishBoolValueDef(rti, "linear", false, "Grid is linear",
    //          "reject|accept");

    return ret;
}


//===========================================================================
// called ONCE just before plugin unloaded from memory
//===========================================================================

void
CaeUnsAeroFXPost::destroy(CAEP_RTITEM &rti)
{
    (void)rti.BCCnt; // silence unused arg warning
}
