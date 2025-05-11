#include "Kaneshige/Course/CrsGround.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Objects/GeoCannon.h"
#include "Kaneshige/Objects/GeoPuller.h"
#include "Kaneshige/Objects/GeoSplash.h"
#include "Kaneshige/Objects/GeoWater.h"
#include "Shiraiwa/MapObjPool.h"

#include "Sato/GeographyObjMgr.h"

#include "mathHelper.h"

#include "JSystem/JAudio//JASFakeMatch2.h"
#include "types.h"

CrsGround::CrsGround(Course *course) {
    mCourse = RaceMgr::getManager()->getCourse();
    reset();
}

void CrsGround::reset() {
    mWorldPos.zero();
    _10.zero();
    mExceptVally = false;
    _1d = 0;

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
    mMaterial = 0xff;
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

void FakeFunction(ExObjColBase& foo) {
    foo.getNormal();
    foo.getVelocity();
    foo.getWallNormal();
    foo.getAttr();
    foo.getAttrIndex();
    foo.getMaterial();
    foo.getHeight();
    foo.getColDepth();
    foo.scaledRad();
    // foo.scaledRadSq();
}

void CrsGround::searchBase(GeographyObj *param_1, const JGeometry::TVec3f &worldPos, const JGeometry::TVec3f &param_3, unsigned long param_4) {
    JGeometry::TVec3f vec;
    // float fVar1;
    // uint uVar2;
    // uint uVar3;
    // CrsData *this_00;
    // char cVar16;
    // GeographyObjManager *pGVar4;
    // GeographyObj *this_01;
    // JGeometry::TVec3<float> uVar5;
    // JGeometry::TVec3<float> uVar6;
    // int *piVar7;
    // ExObjColBase *pEVar8;
    // int iVar10;
    // int iVar11;
    // int iVar12;
    // JGeometry::TVec3<float> *pTVar13;
    // CrsData::SGround *pSVar14;
    // ushort *puVar15;
    // CrsData::Grid *pGVar17;
    // uint uVar18;
    // CrsData::Grid *pGVar19;
    // CrsData::Grid *this_02;
    // CrsData::Ground *pGVar20;
    // double dVar21;
    // double dVar22;
    // double dVar23;
    // double dVar24;
    // uint local_f0;
    // uint local_ec;
    // JGeometry::TVec3f TStack_e8;
    // JGeometry::TVec3<float> TStack_dc;
    // JGeometry::TVec3<float> TStack_d0;
    // JGeometry::TVec3<float> TStack_c4;
    // JGeometry::TVec3<float> TStack_b8;
    // JGeometry::TVec3<float> TStack_ac;
    // stPlaneParam local_a0;
    // undefined4 local_90;
    // uint uStack_8c;
    // undefined4 local_88;
    // uint uStack_84;
    // undefined4 local_80;
    // uint uStack_7c;
    
    // this->field4_0x1d = '\0';
    // this_00 = Course::getCrsData(this->mCourse);
    // local_a0.x = KaneshigeM::@3050;
    // local_a0.y = KaneshigeM::@3051;
    // local_a0.z = KaneshigeM::@3050;
    // local_a0.field3_0xc = KaneshigeM::@3050;
    // cVar16 = CrsData::isAvailableCol(this_00);
    // if (cVar16 != '\0') {
    //   pGVar4 = KaneshigeM::GetGeoObjMgr();
    //   this_01 = (GeographyObj *)GeographyObjManager::search_boundSp(pGVar4,worldPos,param_1);
    //   if (this_01 == (GeographyObj *)0x0) {
    //     pGVar4 = KaneshigeM::GetGeoObjMgr();
    //     this_01 = (GeographyObj *)GeographyObjManager::search_boundEx(pGVar4,worldPos,param_1);
    //     if (this_01 != (GeographyObj *)0x0) {
    //       uVar5 = JGeometry::TVec3<float><float>::TVec3(&TStack_c4,worldPos);
    //       uVar6 = JGeometry::TVec3<float><float>::TVec3(&TStack_b8,param_3);
    //       piVar7 = (int *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01);
    //       (**(code **)(*piVar7 + 0x1c))(piVar7,uVar6,uVar5);
    //     }
    //   }
    //   else {
    //     (**(code **)(*(int *)this_01 + 0x88))(this_01,param_3,worldPos);
    //   }
    //   if (this_01 == (GeographyObj *)0x0) {
    //     pGVar20 = (Ground *)0x0;
    //     cVar16 = Course::isShaking(this->mCourse);
    //     if (((cVar16 == '\0') && (this->mGround != (Ground *)0x0)) &&
    //        (cVar16 = JGeometry::TVec3<float><float>::equals
    //                            ((JGeometry::TVec3<float><float> *)worldPos,&this->field1_0x4), cVar16 != '\0')) {
    //       pGVar20 = this->mGround;
    //     }
    //     if (pGVar20 == (Ground *)0x0) {
    //       uVar18 = 0;
    //       dVar21 = (double)CrsData::getXGridMin(this_00);
    //       dVar22 = (double)CrsData::getZGridMin(this_00);
    //       dVar23 = (double)CrsData::getXGridWidth(this_00);
    //       dVar24 = (double)CrsData::getZGridWidth(this_00);
    //       iVar10 = CrsData::getXGridNum(this_00);
    //       iVar11 = CrsData::getZGridNum(this_00);
    //       pGVar19 = (Grid *)0x0;
    //       do {
    //         searchGridIndex((int *)&local_ec,(int *)&local_f0,this->mCourse,worldPos,(float)dVar21,
    //                         (float)dVar22,(float)dVar23,(float)dVar24,iVar10,iVar11);
    //         uVar3 = local_ec;
    //         uVar2 = local_f0;
    //         if ((((int)local_ec < 0) || ((int)local_f0 < 0)) ||
    //            ((iVar10 <= (int)local_ec || (iVar11 <= (int)local_f0)))) {
    //           pGVar17 = (Grid *)0x0;
    //         }
    //         else {
    //           iVar12 = CrsData::getGridTable(this_00);
    //           pGVar17 = (Grid *)(iVar12 + (uVar3 + uVar2 * iVar10 + uVar18) * 8);
    //         }
    //         this_02 = pGVar17;
    //         if ((pGVar17 != (Grid *)0x0) && (*(ushort *)(pGVar17 + 2) != 0)) {
    //           uStack_8c = local_ec ^ 0x80000000;
    //           local_90 = 0x43300000;
    //           uStack_84 = local_f0 ^ 0x80000000;
    //           iVar10 = 2;
    //           iVar11 = 2;
    //           local_88 = 0x43300000;
    //           dVar21 = (double)(float)(dVar23 * (double)(float)((double)CONCAT44(0x43300000,uStack_8c)
    //                                                            - KaneshigeM::@3212) + dVar21);
    //           dVar23 = (double)(float)(dVar23 * (double)KaneshigeM::@3207);
    //           dVar22 = (double)(float)(dVar24 * (double)(float)((double)CONCAT44(0x43300000,uStack_84)
    //                                                            - KaneshigeM::@3212) + dVar22);
    //           dVar24 = (double)(float)(dVar24 * (double)KaneshigeM::@3207);
    //           uVar18 = (uint)*(ushort *)(pGVar17 + 2);
    //           this_02 = pGVar19;
    //         }
    //         pGVar19 = this_02;
    //       } while (this_02 != pGVar17);
    //       if (this_02 != (Grid *)0x0) {
    //         pTVar13 = (TVec3<float> *)CrsData::getVertexTable(this_00);
    //         pSVar14 = (SGround *)CrsData::getGroundTable(this_00);
    //         puVar15 = (ushort *)CrsData::getIndexTable(this_00);
    //         pGVar20 = (Ground *)
    //                   CrsData::Grid::searchGround
    //                             (this_02,&local_a0,worldPos,param_3,(Ground *)0x0,puVar15,pSVar14,
    //                              pTVar13,param_4,(bool)this->field3_0x1c);
    //       }
    //     }
    //     JGeometry::TVec3<float><float>::operator_=((TVec3<float><float> *)&this->field1_0x4,worldPos);
    //     JGeometry::TVec3<float><float>::operator_=((TVec3<float><float> *)&this->field2_0x10,param_3);
    //     this->mGround = pGVar20;
    //     this->mObject = (GeographyObj *)0x0;
    //     if (this->mGround == (Ground *)0x0) {
    //       JGeometry::TVec3<float><float>::set
    //                 ((TVec3<float><float> *)&this->mNormal,KaneshigeM::@3050,KaneshigeM::@3051,
    //                  KaneshigeM::@3050);
    //       this->mAttr = 0xff;
    //       fVar1 = KaneshigeM::@3050;
    //       this->mAttrIdx = 0;
    //       this->field14_0x54 = 0xff;
    //       this->mHeight = fVar1;
    //       cVar16 = Course::searchAppendGround(this->mCourse,worldPos);
    //       if (cVar16 != '\0') {
    //         this->mAttr = Course::getAppendGroundAttr(this->mCourse);
    //         this->mAttrIdx = Course::getAppendGroundAttrIndex(this->mCourse);
    //         this->mHeight = (float)(double)Course::getAppendGroundHeight(this->mCourse);
    //       }
    //       JGeometry::TVec3<float>::zero(&this->mVel);
    //     }
    //     else {
    //       cVar16 = Course::isShaking(this->mCourse);
    //       if (cVar16 == '\0') {
    //         pGVar20 = this->mGround;
    //         local_88 = 0x43300000;
    //         uStack_84 = (int)pGVar20->xDir ^ 0x80000000;
    //         local_90 = 0x43300000;
    //         local_80 = 0x43300000;
    //         local_a0.x = KaneshigeM::@3208 *
    //                      (float)((double)CONCAT44(0x43300000,uStack_84) - KaneshigeM::@3212);
    //         uStack_8c = (int)pGVar20->yDir ^ 0x80000000;
    //         local_a0.y = KaneshigeM::@3208 *
    //                      (float)((double)CONCAT44(0x43300000,uStack_8c) - KaneshigeM::@3212);
    //         uStack_7c = (int)pGVar20->zDir ^ 0x80000000;
    //         local_a0.z = KaneshigeM::@3208 *
    //                      (float)((double)CONCAT44(0x43300000,uStack_7c) - KaneshigeM::@3212);
    //         local_a0.field3_0xc = pGVar20->d;
    //       }
    //       (this->mNormal).x = local_a0.x;
    //       (this->mNormal).y = local_a0.y;
    //       (this->mNormal).z = local_a0.z;
    //       this->mAttr = (uint)*(byte *)&this->mGround->colFlag;
    //       this->mAttrIdx = (uint)*(byte *)((int)&this->mGround->colFlag + 1);
    //       uVar18 = CrsData::searchMaterial
    //                          (this_00,*(uchar *)&this->mGround->colFlag,
    //                           *(uchar *)((int)&this->mGround->colFlag + 1));
    //       this->field14_0x54 = uVar18 & 0xffff;
    //       this->mHeight =
    //            -(local_a0.field3_0xc +
    //             local_a0.x * (this->field1_0x4).x + local_a0.z * (this->field1_0x4).z) / local_a0.y;
    //       JGeometry::TVec3<float><float>::__ct
    //                 (&TStack_ac,(this->field1_0x4).x,this->mHeight,(this->field1_0x4).z);
    //       Course::getShakeVelocity(this->mCourse,&this->mVel,(TVec3<float> *)&TStack_ac);
    //       addPullerVelocity(this);
    //     }
    //     JGeometry::TVec3<float><float>::set
    //               ((TVec3<float><float> *)&this->mWallNormal,KaneshigeM::@3050,KaneshigeM::@3051,
    //                KaneshigeM::@3050);
    //     this->mDepth = KaneshigeM::@3053;
    //   }
    //   else {
    //     JGeometry::TVec3<float><float>::operator_=((TVec3<float><float> *)&this->field1_0x4,worldPos);
    //     JGeometry::TVec3<float><float>::operator_=((TVec3<float><float> *)&this->field2_0x10,param_3);
    //     this->mGround = (Ground *)0x0;
    //     this->mObject = this_01;
    //     ExObjColBase::getNormal(&TStack_d0,(ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     JGeometry::TVec3<float><float>::set((TVec3<float><float> *)&this->mNormal,&TStack_d0);
    //     ExGeographyObj::getExBounds((ExGeographyObj *)this_01);
    //     ExObjColBase::getVelocity((ExObjColBase *)&TStack_dc);
    //     JGeometry::TVec3<float><float>::set((TVec3<float><float> *)&this->mVel,&TStack_dc);
    //     ExObjColBase::getWallNormal(&TStack_e8,(ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     JGeometry::TVec3<float><float>::set((TVec3<float><float> *)&this->mWallNormal,&TStack_e8);
    //     this->mAttr = ExObjColBase::getAttr((ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     this->mAttrIdx = ExObjColBase::getAttrIndex((ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     this->field14_0x54 = ExObjColBase::getMaterial((ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     this->mHeight = (float)(double)ExObjColBase::getHeight((ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //     this->mDepth = (float)(double)ExObjColBase::getColDepth((ExObjColBase *)ExGeographyObj::getExBounds((ExGeographyObj *)this_01));
    //   }
    // }
    // return;
  }

void CrsGround::search(const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, bool) {}

void CrsGround::roundDepth(const JGeometry::TVec3f &vec) {
    JGeometry::TVec3f v(vec);
    v.normalize();
    JGeometry::TVec3f norm(mWallNormal);
    norm.normalize();
    if (v.dot(norm) > -0.2f)
        mDepth = 0.1f;
}

void CrsGround::checkGridBySphere(float *, const CrsData::SColInfoSphere &, unsigned long, float, float, float) {}

// bool CrsGround::checkPolygonCover(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, const CrsData::Ground *, float *) {}

void CrsGround::checkPolygonCoverMain(const JGeometry::TVec3f &, const CrsData::SColInfoSphere &, unsigned long, float, float, float, const CrsData::Ground *) {}

void CrsGround::isInsideGrid(const JGeometry::TVec3f &, float, float, float, float) {}

void CrsGround::searchGridIndex(int *, int *, Course *, const JGeometry::TVec3f &, float, float, float, float, int, int) {}

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
        TPoolSupervisor *pool = TMapObjPool::getSupervisor();
        JUT_ASSERT_MSG(pool, "NOT FOUND POOL");
        ret = pool->getHeight(mWorldPos.x, mWorldPos.z);
        break;
    }
    default: {
//#line 1603
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
//#line 1603
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

void CrsGround::getWallNormal(JGeometry::TVec3f *, JGeometry::TVec3f *) {}

void CrsGround::addPullerVelocity() {
    if (getAttribute() == Attr_Puller) {
        GeoPuller *puller = GeoPuller::getSupervisor()->search(getAttrIndex());
        JGeometry::TVec3f pullerVel;
        puller->getVelocity(&pullerVel, mWorldPos);
        mVelocity.add(pullerVel);
    }
}
