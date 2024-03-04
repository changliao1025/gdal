/******************************************************************************
 *
 * Project:  OpenGIS Simple Features Reference Implementation
 * Purpose:  Implements Open FileGDB OGR driver.
 * Author:   Even Rouault, <even dot rouault at spatialys.com>
 *
 ******************************************************************************
 * Copyright (c) 2024, Even Rouault <even dot rouault at spatialys.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#ifndef FILEGDB_COORDPREC_READ_H
#define FILEGDB_COORDPREC_READ_H

#include "cpl_minixml.h"

#include "ogr_geomcoordinateprecision.h"

/*************************************************************************/
/*                      GDBGridSettingsToOGR()                           */
/*************************************************************************/

/** Fill OGRGeomCoordinatePrecision from XML "psSpatialReference" node.
 */
static OGRGeomCoordinatePrecision
GDBGridSettingsToOGR(const CPLXMLNode *psSpatialReference)
{
    OGRGeomCoordinatePrecision oCoordPrec;
    const char *pszXYScale =
        CPLGetXMLValue(psSpatialReference, "XYScale", nullptr);
    if (pszXYScale && CPLAtof(pszXYScale) > 0)
    {
        oCoordPrec.dfXYResolution = 1.0 / CPLAtof(pszXYScale);
    }
    const char *pszZScale =
        CPLGetXMLValue(psSpatialReference, "ZScale", nullptr);
    if (pszZScale && CPLAtof(pszZScale) > 0)
    {
        oCoordPrec.dfZResolution = 1.0 / CPLAtof(pszZScale);
    }
    const char *pszMScale =
        CPLGetXMLValue(psSpatialReference, "MScale", nullptr);
    if (pszMScale && CPLAtof(pszMScale) > 0)
    {
        oCoordPrec.dfMResolution = 1.0 / CPLAtof(pszMScale);
    }

    CPLStringList aosSpecificOptions;
    for (const CPLXMLNode *psChild = psSpatialReference->psChild; psChild;
         psChild = psChild->psNext)
    {
        if (psChild->eType == CXT_Element)
        {
            const char *pszValue = CPLGetXMLValue(psChild, "", "");
            if (CPLGetValueType(pszValue) == CPL_VALUE_REAL)
                pszValue = CPLSPrintf("%.15g", CPLAtof(pszValue));
            aosSpecificOptions.SetNameValue(psChild->pszValue, pszValue);
        }
    }
    oCoordPrec.oFormatSpecificOptions["FileGeodatabase"] =
        std::move(aosSpecificOptions);
    return oCoordPrec;
}

#endif /* FILEGDB_COORDPREC_READ_H */
