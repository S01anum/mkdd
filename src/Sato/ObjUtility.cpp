#include "Sato/ObjUtility.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/KartLoader.h"
#include "Kaneshige/RaceMgr.h"
#include "dolphin/mtx.h"

void ObjUtility::executeShakeCamera(const JGeometry::TVec3f &inputVec, f32 f1, f32 f2, f32 f3) {}

void ObjUtility::getKartPos(int kart_index, JGeometry::TVec3f *kartPos) {
    int kart = kart_index;
    #line 134
    JUT_MINMAX_ASSERT(0, kart_index, 8);
    #line 120
    JUT_MINMAX_ASSERT(0, kart, 8);
    KartLoader *kartObj = RaceMgr::sRaceManager->getKartLoader(kart_index);
    kartObj->getExModelBody()->mBaseTRMtx.getTransInline(*kartPos);
}

void ObjUtility::getCamDependLightMtx(u32, const JGeometry::TVec3f &, Mtx) {}
