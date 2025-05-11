#ifndef OBJCOLLISION_H
#define OBJCOLLISION_H

#include "JSystem/J3D/J3DAnmCluster.h"
#include "JSystem/J3D/J3DSkinDeform.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Sato/stMath.h"

class ObjColCylinder;

class ObjColBase
{ 
public:
    enum CKind
    {
        SPHERE,
        CYLINDER,
        CUBE,
    };

    ObjColBase(J3DModelData *, JGeometry::TVec3f, CKind);
    ObjColBase(f32, f32, CKind);
    void initRadius(J3DModelData *);
    void setScale(const JGeometry::TVec3f &);

    // Vtable
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32) = 0;
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &) = 0;

    // Inline/Unused
    void initialize();
    ObjColBase(J3DModelData *, CKind);
    ObjColBase(J3DModelData *, f32, CKind);
    u8 IsHitBoundPos(JGeometry::TVec3f, JGeometry::TVec3f);

    void setRadius(f32 rad) { mRadius = rad; }
    void setScale(f32 scale) { mScale = scale; }

    f32 getScale() const { return mScale; }
    f32 scaledRad() const {
        return mRadius * mScale;
    }

    // f32 scaledRadSq() {
    //     float mScaledRadius = mRadius * mScale;
    //     float mScaledRadiusSq = mScaledRadius * mScaledRadius;
    //     return mScaledRadiusSq;
    // }

// private:
    // VTBL _00
    f32 mRadius;            // _04
    CKind mKind;            // _08
    f32 mScale;             // _0C
    f32 mBoundDepth;        // _10
    JGeometry::TVec3f mPos; // _14
    u32 _20;                // _20, unused?
};

class ExObjColBase : public ObjColBase
{
public:
    void setWallParam(int);
    void setBottom(f32);

    // Vtable
    virtual void makeParameter(J3DModelData *, Mtx m) = 0;             // 10
    virtual u8 IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);       // 14
    virtual u8 IsHitBoundRadius(JGeometry::TVec3f, f32);               // 18
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f) = 0;     // 1C
    virtual bool IsOnDashPlane(JGeometry::TVec3f);                     // 20
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f) = 0; // 24

    JGeometry::TVec3f getNormal() const {
        return mNormal;
    }
    JGeometry::TVec3f getVelocity() {
        return mVelocity;
    }
    JGeometry::TVec3f getWallNormal() const {
        return mWallNormal;
    }
    u32 getAttr() const {
        return mAttr;
    }
    u32 getAttrIndex() const {
        return mAttrIndex;
    }
    u32 getMaterial() const {
        return mMaterial;
    }
    f32 getHeight() const {
        return mHeight;
    }
    f32 getColDepth() const {
        return mColDepth;
    }
    
    stPlaneParam mPlanes[4];       // _24
    u8 _64[0x84 - 0x64];           // _64
    JGeometry::TVec3f mNormal;     // _84
    JGeometry::TVec3f mWallNormal; // _90
    u8 _9c[0xa8 - 0x9c];           // _9c
    JGeometry::TVec3f mVelocity;   // _a8
    f32 mHeight;                   // _b4
    f32 mBottom;                   // _b8
    u32 mAttr;                     // _bc
    u32 mMaterial;                 // _c0
    u8 _c4[0xc8 - 0xc4];           // _c4
    f32 mColDepth;                 // _c8
    u8 _cc[0xd0 - 0xcc];           // _cc
    u32 mAttrIndex;                // _d0
};

class ObjColSphere : public ObjColBase
{
public:
    ObjColSphere(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, SPHERE) {}
    ObjColSphere(f32 radius, f32 scale) : ObjColBase(radius, scale, SPHERE) {}
    
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColCylinder : public ObjColBase
{
public:
    ObjColCylinder(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, CYLINDER), mCylinderRadius(0.0f), mCylinderHeight(0.0f) {}
    ObjColCylinder(f32 radius, f32 scale) : ObjColBase(radius, scale, CYLINDER), mCylinderRadius(0.0f), mCylinderHeight(0.0f) {}
    
    void setCylinderRadius(f32 radius) { mCylinderRadius = radius; }
    void setCylinderHeight(f32 height) { mCylinderHeight = height; }

    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
    
    f32 mCylinderRadius; // _24
    f32 mCylinderHeight; // _28
};

class ObjColBoard // UNUSED
{
public:
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
};

class ObjColCube : public ObjColBase
{
public:
    ObjColCube(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, CUBE) {}

    void makeParameter(J3DModelData *, Mtx m);
    void updateParameter(Mtx m);
    void chkIsHitQuad(const JGeometry::TVec3f &, const f32 &);

    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);

    u8 _24[0x80 - 0x24];      // _24
    JGeometry::TVec3f _80[5]; // _80
}; // size: 0xbc

class ObjColBlock : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump1 : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump2 // UNUSED
{
public:
    void IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeParameter(J3DModelData *, Mtx m);
    void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeColParam(JGeometry::TVec3f, stPlaneParam, CrsGround::EAttr);
    void IsOnDashPlane(JGeometry::TVec3f);
    void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

#endif // OBJCOLLISION_H
