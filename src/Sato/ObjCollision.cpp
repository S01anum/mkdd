#include "Sato/ObjCollision.h"

ObjColBase::ObjColBase(J3DModelData *modelData, JGeometry::TVec3f scale, CKind kind) : mKind(kind)
{
    if (!modelData) {
        return;
    }

    initRadius(modelData);
    setScale(scale);
    mPos.zero();
    mBoundDepth = 0.0f;
}

ObjColBase::ObjColBase(f32 radius, f32 scale, CKind kind)
{
    mRadius = radius;
    mKind = kind;
    mScale = scale;
    mPos.zero();
    mBoundDepth = 0.0f;
}

void ObjColBase::initRadius(J3DModelData *model)
{
    f32 maxRadius = 0.0f;

    int nJoints = 3;
    if (nJoints >= model->mJointTree.mJointCnt) {
        nJoints = model->mJointTree.mJointCnt;
    }

    for (u8 i = 0; i < nJoints; i++) {
        if (maxRadius < model->mJointTree.mJoints[i]->mRadius) {
            maxRadius = model->mJointTree.mJoints[i]->mRadius;
        }
    }
    this->mRadius = maxRadius;
    mPos.zero();
}

void ObjColBase::setScale(const JGeometry::TVec3f &scale)
{
    float s = scale.x;
    if (scale.y > s) {
        s = scale.y;
    }
    if (scale.z > s) {
        s = scale.z;
    }
    mScale = s;
}

bool ObjColSphere::IsHitSphere(JGeometry::TVec3f objPos, JGeometry::TVec3f spherePos, f32 sphereRadius)
{
    mPos.sub(spherePos, objPos);

    f32 minSeparation = mRadius * mScale + sphereRadius;
    f32 minSeparationSq = minSeparation * minSeparation;

    f32 distanceSq = mPos.squared();

    bool isHit = false;
    if (minSeparationSq - distanceSq > 0.0f) {
        // minSeparation was right there, but ok
        f32 minSeparationAgain = JMAFastSqrt(minSeparationSq);
        f32 distance = JMAFastSqrt(distanceSq);
        mBoundDepth = minSeparationAgain - distance;
        stVecNormalize(mPos);
        isHit = true;
    }
    return isHit;
}

bool ObjColSphere::IsHitCylinder(JGeometry::TVec3f objPos, JGeometry::TVec3f cylinderPos,
        const ObjColCylinder &cylinderCol)
{
    f32 scaledRadius = mRadius * mScale;
    f32 minSeparation = scaledRadius + cylinderCol.mCylinderRadius * cylinderCol.mScale;

    f32 deltaX = cylinderPos.x - objPos.x;
    f32 deltaZ = cylinderPos.z - objPos.z;

    f32 distanceSq = deltaX * deltaX + deltaZ * deltaZ;
    f32 minSeparationSq = minSeparation * minSeparation;

    bool isHit = false;
    if (distanceSq < minSeparationSq) {
        f32 scaledCylinderHeight = cylinderCol.getScale() * cylinderCol.mCylinderHeight;
        if ((objPos.y - scaledRadius < cylinderPos.y + scaledCylinderHeight) && (cylinderPos.y > objPos.y + scaledRadius)) {
            // AGAIN, really?
            f32 minSeparationAgain = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparationAgain - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

bool ObjColCylinder::IsHitSphere(JGeometry::TVec3f objPos, JGeometry::TVec3f spherePos, f32 sphereRadius)
{
    f32 minSeparation = mCylinderRadius * mScale + sphereRadius;
    float fVar3;
    float fVar4;

    
    f32 deltaX = spherePos.x - objPos.x;
    f32 deltaZ = spherePos.z - objPos.z;
    
    f32 distanceSq = deltaX * deltaX + deltaZ * deltaZ;
    f32 minSeparationSq = minSeparation * minSeparation;

    bool isHit = false;
    if (distanceSq < minSeparation) {
        f32 scaledCylinderHeight = getScale() * mCylinderHeight;
        if ((objPos.y < spherePos.y + sphereRadius) && (spherePos.y - sphereRadius < scaledCylinderHeight + objPos.y)) {
            f32 minSeparationAgain = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparationAgain - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

bool ObjColCylinder::IsHitCylinder(JGeometry::TVec3f objPos, JGeometry::TVec3f cylinderPos,
        const ObjColCylinder &cylinderCol)
{
    f32 radius = mRadius * mScale;
    f32 minSeparation = radius + cylinderCol.mCylinderRadius * cylinderCol.mScale;
    f32 scaledRadius = radius;

    f32 deltaX = cylinderPos.x - objPos.x;
    f32 deltaZ = cylinderPos.z - objPos.z;

    f32 distanceSq = deltaX * deltaX + deltaZ * deltaZ;
    f32 minSeparationSq = minSeparation * minSeparation;

    bool isHit = false;
    if (distanceSq < minSeparationSq) {
        f32 scaledCylinderHeight = cylinderCol.getScale() * cylinderCol.mCylinderHeight;
        if ((objPos.y - radius < cylinderPos.y + scaledCylinderHeight) && (cylinderPos.y > objPos.y + scaledRadius)) {
            // AGAIN, really?
            f32 minSeparationAgain = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparationAgain - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

u8 ObjColBase::IsHitBoundPos(JGeometry::TVec3f objPos, JGeometry::TVec3f boundPos)
{
    JGeometry::TVec3f deltaPos;
    deltaPos.sub(objPos, boundPos);

    u8 isHit = 0;
    f32 minSeparation = mRadius * mScale;
    if (deltaPos.squared() < minSeparation * minSeparation) {
        isHit = 1;
    }
    return isHit;
}

u8 ExObjColBase::IsHitBound(JGeometry::TVec3f objPos, JGeometry::TVec3f boundPos)
{
    u8 isHit = 0;
    if (IsHitBoundPos(objPos, boundPos)) {
        u8 nHits = 0;
        if (boundPos.y < this->mBottom) {
            return 0;
        }
        for (u8 i = 0; i < 4; i++) {
            nHits += stSearchInSurface(boundPos, this->mPlanes[i]);
        }
        if (nHits == 4) {
            isHit = 1;
        }
    }
    return isHit;
}

u8 ExObjColBase::IsHitBoundRadius(JGeometry::TVec3f pos, f32 radiusSq)
{
    u8 isHit = 0;
    for (u8 i = 0; i < 4; i++) {
        if (stCollideSurfaceAndSphere(pos, radiusSq, mPlanes[i], mColDepth)) {
            isHit = 1;
            break;
        }
    }
    return isHit;
}
