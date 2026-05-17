#include "Kaneshige/Course/CrsGround.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Objects/GeoCannon.h"
#include "Kaneshige/Objects/GeoPuller.h"
#include "Kaneshige/Objects/GeoSplash.h"
#include "Kaneshige/Objects/GeoWater.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/stMath.h"
#include "Shiraiwa/Objects/MapObjPool.h"

#include "mathHelper.h"

#include "JSystem/JAudio//JASFakeMatch2.h"
#include "types.h"

#define GRID_RANGE_ASSERT(min, cur, max, pos) \
  JUT_ASSERT_F((((min) <= (cur)) && ((cur) < (max))), "out range : " #cur " = %d\n pos: (%6.3f, %6.3f, %6.3f)", (cur), (pos.x), (pos.y), (pos.z));

CrsGround::CrsGround(Course *course) {
    mCourse = RaceMgr::getManager()->getCourse();
    reset();
}

void CrsGround::reset() {
    mWorldPos.zero();
    _10.zero();
    mExceptValley = false;
    _1d = false;

    mGround = nullptr;
    mGeoObj = nullptr;
    mNormal.set(0.0f, 1.0f, 0.0f);
    mVelocity.zero();
    mWallNormal.set(0.0f, 1.0f, 0.0f);

    if (mCourse->getCrsData()->isAvailableCol()) {
        mAttribute = Attr_255;
        mHeight = 0.0f;
    }
    else {
        mAttribute = Attr_1;
        mHeight = 1000.0f;
    }

    mAttrIdx = 0;
    mMaterial = Mat_255;
    mDepth = -1.0f;
}

void CrsGround::search(const JGeometry::TVec3f &pos) {
    searchBase(nullptr, pos, pos, 3);
}

void CrsGround::search(const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2) {
    searchBase(nullptr, pos1, pos2, 3);
}

void CrsGround::search(GeographyObj *geoObj, const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2) {
    searchBase(geoObj, pos1, pos2, 3);
}

void CrsGround::searchBase(GeographyObj *geoObj, const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2, u32 p4) {
    stPlaneParam plane;
    _1d = false;

    CrsData *crsData = mCourse->getCrsData();
    plane.x = 0.0;
    plane.y = 1.0;
    plane.z = 0.0;
    plane.d = 0.0;

    if (!crsData->isAvailableCol()) return;

    ExGeographyObj *exGeoObj = GetGeoObjMgr()->search_boundSp(pos1, geoObj);
    if (exGeoObj) {
        exGeoObj->Search(pos2, pos1);
    }
    else {
        exGeoObj = GetGeoObjMgr()->search_boundEx(pos1, geoObj);
        if (exGeoObj) {        
            exGeoObj->getExBounds()->Search(pos2, pos1);
        }
    }

    if (exGeoObj) {
        mWorldPos = pos1;
        _10 = pos2;
        mGround = nullptr;
        mGeoObj = exGeoObj;
        mNormal.set(exGeoObj->getExBounds()->getNormal());
        mVelocity.set(exGeoObj->getExBounds()->getVelocity());
        mWallNormal.set(exGeoObj->getExBounds()->getWallNormal());
        mAttribute = (EAttr)exGeoObj->getExBounds()->getAttr();
        mAttrIdx = exGeoObj->getExBounds()->getAttrIndex();
        mMaterial = (EMat)exGeoObj->getExBounds()->getMaterial();
        mHeight = exGeoObj->getExBounds()->getHeight();
        mDepth = exGeoObj->getExBounds()->getColDepth();
        return;
    }

    CrsData::Ground *ground = nullptr;
    if ((!mCourse->isShaking() && mGround) && pos1.equals(mWorldPos)) {
        ground = mGround;
    }

    if (ground == nullptr) {
        CrsData::Grid *grid1 = nullptr;
        CrsData::Grid *grid2 = nullptr;
        u32 gridIdx = 0;
        f32 xGridMin = crsData->getXGridMin();
        f32 zGridMin = crsData->getZGridMin();
        f32 xGridWidth = crsData->getXGridWidth();
        f32 zGridWidth = crsData->getZGridWidth();
        int xGridNum = crsData->getXGridNum();
        int zGridNum = crsData->getZGridNum();
        
        do {
            int xGridIdx, zGridIdx;
            searchGridIndex(&xGridIdx, &zGridIdx, mCourse, pos1, xGridMin, zGridMin, xGridWidth, zGridWidth, xGridNum, zGridNum);
            
            if (((xGridIdx >= 0) && (zGridIdx >= 0)) && ((xGridIdx < xGridNum && (zGridIdx < zGridNum)))) {
                grid1 = crsData->getGridTable() + (xGridIdx + zGridIdx * xGridNum + gridIdx);
            }
            else {
                grid1 = nullptr;
            }

            if (grid1 && (grid1->_2 != 0)) {
                xGridNum = 2;
                zGridNum = 2;
                xGridMin += (xGridWidth * xGridIdx);
                zGridMin += (zGridWidth * zGridIdx);
                xGridWidth *= 0.5f;
                zGridWidth *= 0.5f;
                gridIdx = grid1->_2;
            }
            else {
                grid2 = grid1;
            }
        } while (grid2 != grid1);
        
        if (grid2) {
            ground = grid2->searchGround(
                &plane, pos1, pos2, ground, 
                crsData->getIndexTable(), crsData->getGroundTable(), crsData->getVertexTable(),
                p4, mExceptValley
            );
        }
    }

    mWorldPos = pos1;
    _10 = pos2;
    mGround = ground;
    mGeoObj = nullptr;
    
    if (mGround) {
        if (!mCourse->isShaking()) {
            plane.x = mGround->normDir.x * 0.0001f;
            plane.y = mGround->normDir.y * 0.0001f;
            plane.z = mGround->normDir.z * 0.0001f;
            plane.d = mGround->dir;
        }
        mNormal.x = plane.x;
        mNormal.y = plane.y;
        mNormal.z = plane.z;
        mAttribute = (EAttr)mGround->attribute;
        mAttrIdx = mGround->attrIdx;
        mMaterial = (EMat)crsData->searchMaterial(mGround->attribute, mGround->attrIdx);
        mHeight = -(plane.x * (mWorldPos).x + plane.z * (mWorldPos).z + plane.d) / plane.y;
        JGeometry::TVec3f TStack_ac(mWorldPos.x, mHeight, mWorldPos.z);
        mCourse->getShakeVelocity(&mVelocity, TStack_ac);
        addPullerVelocity();
    }
    else {
        mNormal.set(0.0f, 1.0f ,0.0f);
        mAttribute = Attr_255;
        mAttrIdx = 0;
        mMaterial = Mat_255;
        mHeight = 0.0f;
        if (mCourse->searchAppendGround(pos1)) {
            mAttribute = (EAttr)mCourse->getAppendGroundAttr();
            mAttrIdx = mCourse->getAppendGroundAttrIndex();
            mHeight = mCourse->getAppendGroundHeight();;
        }
        mVelocity.zero();
    }
    mWallNormal.set(0.0f, 1.0f, 0.0f);
    mDepth = -1.0f;
}

void CrsGround::search(const JGeometry::TVec3f &pos, const JGeometry::TVec3f &prevPos, f32 r, bool checkObjects) {
    _1d = true;

    CrsData* crsData = mCourse->getCrsData();

    if (!crsData->isAvailableCol()) {
        return;
    }

    mDepth = -1.0f;

    //
    // Check dynamic geography objects first
    //

    ExGeographyObj* geoObj = GetGeoObjMgr()->search_boundSp(pos, r);

    mGeoObj = geoObj;
    mGround = nullptr;

    if (!checkObjects) {
        geoObj = nullptr;
    }

    if (geoObj != nullptr) {

        // Perform object collision test
        geoObj->Search_Wall(pos, r);

        f32 colDepth = geoObj->getExBounds()->getColDepth();
        mDepth = colDepth;

        if (colDepth >= 0.0f) {

            mWallNormal.set(geoObj->getExBounds()->getWallNormal());

            mAttribute = (EAttr)geoObj->getExBounds()->getAttr();
            mAttrIdx = geoObj->getExBounds()->getAttrIndex();
            mMaterial = (EMat)geoObj->getExBounds()->getMaterial();

            mGround = nullptr;
        }
    }

    //
    // Static course collision
    //
    if (mDepth < 0.0f) {

        mGeoObj = geoObj;

        CrsData::SColInfoSphere sphere;

        sphere._0.set(pos);
        sphere._c.set(prevPos);
        sphere.d = r;

        //
        // Compute sphere AABB
        //
        JGeometry::TVec3f minPos(pos);
        minPos.x -= r;
        minPos.z -= r;

        JGeometry::TVec3f maxPos(pos);
        maxPos.x += r;
        maxPos.z += r;

        //
        // Grid parameters
        //
        f32 xGridMin   = crsData->getXGridMin();
        f32 zGridMin   = crsData->getZGridMin();

        f32 xGridSize  = crsData->getXGridWidth();
        f32 zGridSize  = crsData->getZGridWidth();

        int xGridCount   = crsData->getXGridNum();
        int zGridCount   = crsData->getZGridNum();

        //
        // Determine grid search range
        //
        int minGridX;
        int minGridZ;

        searchGridIndex(
            &minGridX,
            &minGridZ,
            mCourse,
            minPos,
            xGridMin,
            zGridMin,
            xGridSize,
            zGridSize,
            xGridCount,
            zGridCount
        );

        int maxGridX;
        int maxGridZ;

        searchGridIndex(
            &maxGridX,
            &maxGridZ,
            mCourse,
            maxPos,
            xGridMin,
            zGridMin,
            xGridSize,
            zGridSize,
            xGridCount,
            zGridCount
        );

        //
        // Bounds validation
        //
        if (minGridX < 0 || maxGridX > xGridCount ||
            minGridZ < 0 || maxGridZ > zGridCount)
        {
            mDepth = -1.0f;
            return;
        }

        mDepth = -1.0f;
        mWallNormal.zero();

        bool foundGround = false;

        //
        // Traverse overlapping grids
        //
        for (int z = minGridZ; z <= maxGridZ; ++z) {

            for (int x = minGridX; x <= maxGridX; ++x) {
                bool hit = checkGridBySphere(
                    &mDepth,
                    sphere,
                    x + (z * xGridCount),
                    xGridMin + (xGridSize * x),
                    zGridMin + (zGridSize * z),
                    xGridSize
                );

                foundGround |= hit;
            }
        }

        //
        // Normalize accumulated wall normal
        //
        mWallNormal.normalize();

        //
        // Collision response
        //
        if (mDepth > 0.0f) {

            JGeometry::TVec3f movement;
            movement.sub(pos, prevPos);

            if (!foundGround &&
                movement.squared() > 0.1f)
            {
                movement.normalize();

                roundDepth(movement);
            }
        }

        //
        // Apply ground material attributes
        //
        if (mGround != nullptr && mDepth >= 0.0f) {

            mAttribute = (EAttr)mGround->attribute;
            mAttrIdx = mGround->attrIdx;

            mMaterial =
                (EMat)crsData->searchMaterial(
                    mGround->attribute,
                    mGround->attrIdx
                );
        }

        //
        // Clear temporary object pointer
        //
        mGeoObj = nullptr;
    }
}

void CrsGround::roundDepth(const JGeometry::TVec3f &vec) {
    JGeometry::TVec3f v(vec);
    v.normalize();
    JGeometry::TVec3f norm(mWallNormal);
    norm.normalize();
    if (v.dot(norm) > -0.2f)
        mDepth = 0.1f;
}

bool CrsGround::checkGridBySphere(
    f32 *maxDistance, 
    const CrsData::SColInfoSphere &sphere, 
    u32 gridIndex, 
    f32 gridX,
    f32 gridZ,
    f32 gridSize
) {
    bool hit = false;

    CrsData* crsData = mCourse->getCrsData();
    CrsData::Grid* gridTable = crsData->getGridTable();

    CrsData::Grid* grid = &gridTable[gridIndex];

    // Leaf node
    if (grid->_2 != 0) {
        const f32 halfSize = gridSize * 0.5f;

        for (u32 z = 0; z < 2; ++z) {

            f32 childZ = gridZ + (z == 0 ? 0.0f : halfSize);

            for (u32 x = 0; x < 2; ++x) {

                f32 childX = gridX + (x == 0 ? 0.0f : halfSize);

                u32 childIndex = (z * 2) + x + grid->_2;

                bool childHit = checkGridBySphere(
                    maxDistance,
                    sphere,
                    childIndex,
                    childX,
                    childZ,
                    halfSize
                );

                hit |= childHit;
            }
        }

    } 
    else {
        if (!isInsideGrid(
                sphere,
                gridX,
                gridZ,
                gridSize))
        {
            return false;
        }

        JGeometry::TVec3f wallNormal;
        f32 collisionDistance = -1.0f;
        f32 floorHeight = -1.0f;

        CrsData::Ground* ground = grid->checkPolygonBySphere(
            &wallNormal,
            &collisionDistance,
            crsData,
            sphere,
            &floorHeight
        );

        if (ground != nullptr) {

            bool valid = true;

            // Optional object clipping check
            if (mGeoObj != nullptr) {

                f32 clipHeight = mGeoObj->getMaxHeight();

                if (floorHeight < (clipHeight + sphere.d)) {
                    valid = false;
                }
            }

            if (valid) {

                mWallNormal.add(wallNormal);

                if (*maxDistance < collisionDistance) {
                    *maxDistance = collisionDistance;
                }

            } else {
                ground = nullptr;
            }
        }

        if (ground != nullptr) {

            if (floorHeight == -1.0f ||
                floorHeight >= sphere._0.y)
            {
                hit = true;
            }

            mGround = ground;
        }        
    }

    return hit;
}

bool CrsGround::checkPolygonCover(
    const JGeometry::TVec3f &startPos, 
    const JGeometry::TVec3f &endPos, 
    const JGeometry::TVec3f &sphereCenter, 
    f32 r, 
    const CrsData::Ground *ground, 
    f32 *outHeight) 
{
    CrsData *crsData = mCourse->getCrsData();
    f32 xGridMin = crsData->getXGridMin();
    f32 zGridMin = crsData->getZGridMin();
    f32 xGridWidth = crsData->getXGridWidth();
    f32 zGridWidth = crsData->getZGridWidth();
    int xGridNum = crsData->getXGridNum();
    int zGridNum = crsData->getZGridNum();

    CrsData::SColInfoSphere sphere;
    sphere._0.set(sphereCenter);
    sphere.d = r;

    //
    // Search start position grid
    //
    int gridX;
    int gridZ;

    searchGridIndex(
        &gridX,
        &gridZ,
        mCourse,
        startPos,
        xGridMin,
        zGridMin,
        xGridWidth,
        zGridWidth,
        xGridNum,
        zGridNum
    );

#line 1164
    GRID_RANGE_ASSERT(0, gridX, xGridNum, startPos);
    GRID_RANGE_ASSERT(0, gridZ, zGridNum, startPos);

    // Query polygon cover at start position
    int startIndex = gridX;    
    startIndex += (gridZ * xGridNum);

    f32 startHeight = checkPolygonCoverMain(startPos, sphere, startIndex,
        xGridMin + (xGridWidth * gridX), zGridMin + (zGridWidth * gridZ), xGridWidth, ground
    );

    *outHeight = startHeight;

    // No polygon found
    f32 yDiff = (sphereCenter.y - r);
    if (startHeight >= 0.0f) {
        if (startHeight < yDiff) {
            return true;
        }
    }
    else {
        return false;
    }

    // Search end position grid
    searchGridIndex(&gridX, &gridZ,mCourse,endPos,
        xGridMin, zGridMin, xGridWidth, zGridWidth, xGridNum, zGridNum
    );

#line 1191
    GRID_RANGE_ASSERT(0, gridX, xGridNum, endPos);
    GRID_RANGE_ASSERT(0, gridZ, zGridNum, endPos);

    int startIndex2 = gridX;
    startIndex2 += (gridZ * xGridNum);
    f32 endHeight = checkPolygonCoverMain(
        endPos,
        sphere,
        startIndex2,
        xGridMin + (xGridWidth * gridX),
        zGridMin + (zGridWidth * gridZ),
        xGridWidth,
        nullptr
    );

    //
    // Reject if cover height difference is too large
    //
    if (startHeight == -1.0f ||
        (startHeight - endHeight) > getOverLevel())
    {
        return false;
    }

    //
    // Sphere is already above cover
    //
    return true;
}

f32 CrsGround::checkPolygonCoverMain(const JGeometry::TVec3f &pos, const CrsData::SColInfoSphere &colInfo, u32 gridIndex, f32 gridX, f32 gridZ, f32 gridSize, const CrsData::Ground *ground) {
    CrsData::Grid* gridTable = mCourse->getCrsData()->getGridTable();

    CrsData::Grid* grid = &gridTable[gridIndex];

    if (!isInsideGrid(pos, 0.01f, gridX, gridZ, gridSize)) {
        return -1.0;
    }

    f32 ret = -1.0f;
    if (grid->_2 != 0) {
        const f32 halfSize = gridSize * 0.5f;

        for (u32 z = 0; z < 2; z++) {
            f32 subGridZ = gridZ + (z == 0 ? 0.0f : halfSize);

            for (u32 x = 0; x < 2; x++) {
                f32 subGridX = gridX + (x == 0 ? 0.0f : halfSize);

                u32 childGridIndex = (z * 2) + x + grid->_2;

                ret = checkPolygonCoverMain(
                    pos,
                    colInfo,
                    childGridIndex,
                    subGridX,
                    subGridZ,
                    halfSize,
                    ground
                );

                if (ret != -1.0f) {
                    return ret;
                }
            }
        }
    }
    else {
        ret = grid->checkPolygonCoverWall(pos, colInfo, ground);
    }

    return ret;
}

bool CrsGround::isInsideGrid(const JGeometry::TVec3f &pos, f32 padding, f32 gridX, f32 gridZ, f32 cellSize) {
    bool ret = true;
    JGeometry::TVec3f corner[4]; // nooo not the precious stack!

    corner[0].set(gridX, 0.0f, gridZ);
    const f32 maxX = gridX + cellSize;
    corner[1].set(maxX, 0.0f, gridZ);

    const f32 maxZ = gridZ + cellSize;
    corner[2].set(maxX, 0.0f, maxZ);
    corner[3].set(gridX, 0.0f, maxZ);

    if ((gridX - padding) > pos.x || (maxX + padding) < pos.x 
        || (gridZ - padding) > pos.z || (maxZ + padding) < pos.z)
    {
        ret = false;
    }

    return ret;
}

void CrsGround::searchGridIndex(
    int *pX, int *pZ, Course *course, const JGeometry::TVec3f &pos, 
    f32 xGridMin, f32 zGridMin, f32 xGridWidth, f32 zGridWidth, 
    int xGridNum, int zGridNum) 
{
    JGeometry::TVec3f gridCoord;
    JGeometry::TVec3f worldPos(pos);
    course->convWorldPosToGridCoord(&gridCoord, worldPos);
    *pX = -1;
    *pZ = -1;

    f32 xOffset = xGridWidth * xGridNum + xGridMin;
    f32 zOffset = zGridWidth * zGridNum + zGridMin;

    if ((gridCoord.x - xGridMin < -0.001f)) return;
    if (((gridCoord.x - xOffset) > 0.001f)) return;
    if (((gridCoord.z - zGridMin) < -0.001f)) return;
    if (((gridCoord.z - zOffset) > 0.001f)) return;
    
    *pX = (gridCoord.x - xGridMin) / xGridWidth;
    *pZ = (gridCoord.z - zGridMin) / zGridWidth;
}

f32 CrsGround::getHeight() const { return mHeight; }

void CrsGround::getNormal(JGeometry::TVec3f *out) const { out->set(mNormal); }

u8 CrsGround::getCameraCode() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->camCode : ret;
    }
    return ret;    
}

bool CrsGround::isItemInvalGround() const {
    bool ret = false;
    if (!isObject()) {
        ret = (mGround) ? mGround->itemType != 0 : false;
    }
    return ret;
}

u8 CrsGround::getAddThickness() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->getAddThickness() : ret;
    }
    return ret;
}

u8 CrsGround::getStaggerCode() const {
    u8 ret = 0;
    if (!isObject()) {
        if (mGround) {
            ret = (u8)mGround->staggerCode;
        }
        else if (mCourse->getCrsData()->isAvailableCol()) {
            ret = 1;
        }
        else {
            ret = 0;
        }
    }
    return ret;    
}

u8 CrsGround::getSpiralCode() const {
    u8 ret = 0;
    if (!isObject() && mGround) {
        ret = (u8)mGround->spiralCode;
    }
    return ret;
}

u8 CrsGround::getSplashCode() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->splashCode : ret;
    }
    return ret;
}

f32 CrsGround::getSplashHeight() {
    f32 ret = -1000.0f;
    u8 splashCode = getSplashCode();
    switch(splashCode) {
    case 0:
        break;
    case 0xfe: {
        ret = GeoWater::getSupervisor()->searchWater(0)->getSurfaceHeight();
        break;
    }
    case 0xff: {
#line 1560
        TPoolSupervisor *pool = TMapObjPool::getSupervisor();
        JUT_ASSERT_MSG(pool, "NOT FOUND POOL");
        ret = pool->getHeight(mWorldPos.x, mWorldPos.z);
        break;
    }
    default: {
#line 1566
        GeoSplash *splash = GeoSplash::getSupervisor()->search(splashCode);
        JUT_ASSERT_MSG(splash, "NOT FOUND SPLASH");
        ret = splash->getSurfaceHeight();
    }
    }
    return ret; 
}

int CrsGround::getSplashID() {
    int ret = 0;
    u8 splashCode = getSplashCode();
    switch(splashCode) {
    case 0:
        break;
    case 0xfe: {
        ret = 2;
        break;
    }
    case 0xff: {
        ret = 1;
        break;
    }
    default: {
#line 1603
        GeoSplash *splash = GeoSplash::getSupervisor()->search(splashCode);
        JUT_ASSERT_MSG(splash, "NOT FOUND SPLASH");
        ret = splash->getSplashID();
    }
    }
    return ret; 
}

JugemPoint *CrsGround::getJugemPoint() {
    JugemPoint *ret = nullptr;
    switch (getAttribute()) {
    case Attr_5:
    case Attr_10:
    case Attr_14:
    case Attr_15:
    case Attr_Water:
    case Attr_55:
    case Attr_71: {
        ret = mCourse->searchJugemPoint(getAttrIndex());
        break;
    }
    case Attr_7: {
        if (getAttrIndex() != 0) {
            ret = mCourse->searchJugemPoint(getAttrIndex());
        }
        break;
    }
    }

    return ret;
}

JugemPoint *CrsGround::getCannonPoint() {
    JugemPoint *ret = nullptr;
    if (getAttribute() == Attr_Cannon) {
       ret = mCourse->searchJugemPoint(getAttrIndex()); 
    }
    return ret;
}

JugemPoint *CrsGround::getPipePoint() {
    JugemPoint *ret = nullptr;
    if (getAttribute() == Attr_Pipe) {
       ret = mCourse->searchJugemPoint(getAttrIndex()); 
    }
    return ret;
}

GeoCannon *CrsGround::getGeoCannon() {
    GeoCannon *ret = nullptr;
    if (getAttribute() == Attr_Cannon && GeoCannon::getSupervisor()) {
        ret = GeoCannon::getSupervisor()->searchCannon(getAttrIndex());
    }
    return ret;
}

f32 CrsGround::getWaterHeight() {
    f32 ret = -1000.0f;
    if (getAttribute() == Attr_Water && GeoWater::getSupervisor()) {
        GeoWater *water = GeoWater::getSupervisor()->searchWater(0);
        ret = water->getSurfaceHeight();
    }
    return ret;
}

bool CrsGround::isShaking() const {
    return mVelocity.squared() > 0.01f || mCourse->getCrsData()->getShaking() == 2;
}

bool CrsGround::isAttributeCollision() {
    bool ret = false;
    switch (getAttribute()) {
    case Attr_2:
    case Attr_18:
        ret = true;
        break;
    }
    return ret;
}

f32 CrsGround::getWallNormal(JGeometry::TVec3f *inVec1, JGeometry::TVec3f *inVec2) {
    inVec1->set(mWallNormal);

    if (inVec2) {
        inVec2->set(mWorldPos);
    }

    if (_1d) {
        return mDepth;
    }

    if (isObject()) {
        return mDepth;
    }

    if (!mCourse->getCrsData()->isAvailableCol()) {
        return mDepth;
    }

    if (!isAttributeCollision()) {
        return mDepth;
    }

    JGeometry::TVec3f posDiff;
    posDiff.sub(mWorldPos, _10);

    if (posDiff.isZero()) {
        return mDepth;
    }

    f32 ret = mDepth;
    const JGeometry::TVec3f *vertexTable = mCourse->getCrsData()->getVertexTable();
    CrsData::Ground *groundTable = mCourse->getCrsData()->getGroundTable();
    CrsData::Ground *ground = mGround;

    int i = 1;
    
    while (ground) {
        JGeometry::TVec3f upDir(0.0f, 1.0f, 0.0f);
        JGeometry::TVec3f stack_c0[3];
        stPlaneParam stack_f0[3];
        JGeometry::TVec3f stack_114[3];
        f32 stack_138[3];

        for (int j = 0; j < 3; j++) {
            int vertexIdx = ground->pointIndices[(j+1) % 3];
            const JGeometry::TVec3f &vertex = vertexTable[ground->pointIndices[j]];
            const JGeometry::TVec3f &vertex2 = vertexTable[vertexIdx];
            stack_114[j].set(vertex);
            stack_c0[j].sub(vertex2, vertex);
            stMakePlaneParam(stack_f0[j], stack_c0[j], upDir, vertex);
            stack_138[j] = stCollideLineToPlaneIn(_10, mWorldPos, stack_f0[j]);

            if (stAbs(stack_138[j]) < 0.001f) {
                stack_138[j] = 0.0f;
            }
        }

        f32 nearestDist = -1000.0f;
        int crossindex = -1;

        for (int j = 0; j < 3; j++) {
            if (stack_138[j] < 0.0f) continue;

            JGeometry::TVec3f collidePos;
            collidePos = stGetCollidePosFromT(_10, mWorldPos, stack_138[j]);
            bool b = true;
            for (int k = 0; k < 2; k++) {
                int idx = (j + k + 1) % 3;

                if ((collidePos.x * stack_f0[idx].x + collidePos.y * stack_f0[idx].y + collidePos.z * stack_f0[idx].z + stack_f0[idx].d ) > 0.0f) {
                    b = false;
                    break;
                }
            }

            if (!b) continue;
            
            bool b2 = false;
            if (stack_138[j] > nearestDist) {
                b2 = true;
            }
            else {
                if (nearestDist == stack_138[j]) {
                    const u16 nearTrigIdx = ground->nearTrigs[j];
                    if (nearTrigIdx == 0xffff || groundTable[nearTrigIdx].attribute != (u8)getAttribute()) {
                        b2 = true;
                    }
                }
            }
            if (b2) {
                crossindex = j;
                nearestDist = stack_138[j];
            }
        }

        if (crossindex == -1) {
            inVec1->set(mWallNormal);
            if (inVec2) {
                inVec2->set(mWorldPos);
            }
            return mDepth;
        }
#line 1949
        JUT_MINMAX_ASSERT(0, crossindex, 3);

        const u16 crossTrig = ground->nearTrigs[crossindex];

        if (crossTrig == 0xffff || groundTable[crossTrig].attribute != (u8)getAttribute()) {
            ret = stGetCollideDepthFromT(_10, mWorldPos, stack_138[crossindex]);
            stPlaneParam &plane = stack_f0[crossindex]; 
            inVec1->set(plane.x, plane.y, plane.z);
            inVec1->normalize();
            
            if (inVec2) {
                JGeometry::TVec3f collidePos;
                collidePos = stGetCollidePosFromT(_10, mWorldPos, stack_138[crossindex]);
                inVec2->set(collidePos);
            }
            ground = nullptr;
            continue;
        }

        i++;
        ground = groundTable + crossTrig;

        if (i > 100) {
            if (inVec2) {
                inVec2->set(mWorldPos);
            }
            return mDepth;
        } 
    }    
    return ret;
}

void CrsGround::addPullerVelocity() {
    if (getAttribute() == Attr_Puller) {
        GeoPuller *puller = GeoPuller::getSupervisor()->search(getAttrIndex());
        JGeometry::TVec3f pullerVel;
        puller->getVelocity(&pullerVel, mWorldPos);
        mVelocity.add(pullerVel);
    }
}
