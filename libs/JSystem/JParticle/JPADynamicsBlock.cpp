#include "JSystem/JGeometry.h"
#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "types.h"

// TODO: check against TP Debug

void JPAVolumePoint(JPAEmitterWorkData* workData)
{
    workData->mVolumePos.zero();
    workData->mVelOmni.set(workData->mEmitter->getRandZH(), workData->mEmitter->getRandZH(), workData->mEmitter->getRandZH());
    workData->mVelAxis.set(workData->mVelOmni.x, 0.0f, workData->mVelOmni.z);
}

void JPAVolumeLine(JPAEmitterWorkData* workData)
{
    if (workData->mEmitter->checkDynFlag(JPADYN_FixedInterval)) {
        workData->mVolumePos.set(0.0f, 0.0f,
                                 workData->mVolumeSize * ((workData->mVolumeEmitIdx / (workData->mCreateNumber - 1.0f) - 0.5f)));
        workData->mVolumeEmitIdx++;
    } else {
        workData->mVolumePos.set(0.0f, 0.0f, workData->mVolumeSize * workData->mEmitter->getRandZH());
    }

    workData->mVelOmni.set(0.0f, 0.0f, workData->mVolumePos.z * workData->mGlobalScl.z);
    workData->mVelAxis.set(0.0f, 0.0f, workData->mVolumePos.z);
}

void JPAVolumeCircle(JPAEmitterWorkData* workData)
{
    s16 angleInShort;
    f32 angleInFloat;
    f32 distance;
    f32 circleSizeInXZ;

    if (workData->mEmitter->checkDynFlag(JPADYN_FixedInterval)) {
        angleInFloat = (s16)((workData->mVolumeEmitIdx << 16) / workData->mCreateNumber);
        angleInShort = angleInFloat * workData->mVolumeSweep;
        workData->mVolumeEmitIdx++;
    } else {
        angleInFloat = workData->mVolumeSweep * workData->mEmitter->getRandS16();
        angleInShort = angleInFloat;
    }

    distance = workData->mEmitter->getRandF32();
    if (workData->mEmitter->checkDynFlag(JPADYN_FixedDensity)) {
        distance = 1.0f - (distance * distance);
    }

    circleSizeInXZ = workData->mVolumeSize * (workData->mVolumeMinRad + distance * (1.0f - workData->mVolumeMinRad));
    workData->mVolumePos.set(circleSizeInXZ * JMASSin(angleInShort), 0.0f, circleSizeInXZ * JMASCos(angleInShort));
    workData->mVelOmni.mul(workData->mVolumePos, workData->mGlobalScl);
    workData->mVelAxis.set(workData->mVolumePos.x, 0.0f, workData->mVolumePos.z);
}

void JPAVolumeCube(JPAEmitterWorkData* workData)
{
    workData->mVolumePos.set(workData->mEmitter->getRandZH() * workData->mVolumeSize,
                             workData->mEmitter->getRandZH() * workData->mVolumeSize,
                             workData->mEmitter->getRandZH() * workData->mVolumeSize);
    workData->mVelOmni.mul(workData->mVolumePos, workData->mGlobalScl);
    workData->mVelAxis.set(workData->mVolumePos.x, 0.0f, workData->mVolumePos.z);
}

void JPAVolumeSphere(JPAEmitterWorkData* workData)
{
    s16 phi;   // r0
    s16 theta; // r4
    if (workData->mEmitter->checkDynFlag(JPADYN_FixedInterval)) {

        u16 preTheta = (workData->mVolumeAngleNum << 16) / (workData->mVolumeAngleMax - 1);
        phi          = u16((workData->mVolumeX << 15) / (workData->mDivNumber - 1) + 0x4000);
        theta        = f32(preTheta) * workData->mVolumeSweep + 32768.0f;
        workData->mVolumeAngleNum++;
        if (workData->mVolumeAngleNum == workData->mVolumeAngleMax) {

            workData->mVolumeAngleNum = 0;
            workData->mVolumeX++;
            if (workData->mVolumeX * 2 < workData->mDivNumber) {
                // a ternary makes much more sense here
                int angleMax;
                switch (workData->mVolumeAngleMax) {
                default:
                    angleMax = workData->mVolumeAngleMax + 4;
                    break;
                case 1:
                    angleMax = workData->mVolumeAngleMax + 3;
                }
                workData->mVolumeAngleMax = angleMax;
            } else {
                int angleMax;
                switch (workData->mVolumeAngleMax) {
                default:
                    angleMax = workData->mVolumeAngleMax - 4;
                    break;
                case 4:
                    angleMax = 1;
                }
                workData->mVolumeAngleMax = angleMax;
            }
        }
    } else {
        phi          = workData->mEmitter->getRandS16() >> 1;
        s16 preTheta = workData->mEmitter->getRandS16();
        theta        = workData->mVolumeSweep * f32(preTheta);
    }

    f32 factor = workData->mEmitter->getRandF32();
    if (workData->mEmitter->checkDynFlag(JPADYN_FixedDensity)) {
        factor = 1.0f - factor * factor * factor;
    }

    f32 rad = workData->mVolumeSize * (workData->mVolumeMinRad + (factor * (1.0f - workData->mVolumeMinRad)));

    workData->mVolumePos.set((rad * JMASCos(phi)) * JMASSin(theta), -rad * JMASSin(phi), (rad * JMASCos(phi)) * JMASCos(theta));
    workData->mVelOmni.mul(workData->mVolumePos, workData->mGlobalScl);
    workData->mVelAxis.set(workData->mVolumePos.x, 0.0f, workData->mVolumePos.z);
}

void JPAVolumeCylinder(JPAEmitterWorkData* workData)
{
    s16 angle  = workData->mVolumeSweep * f32(workData->mEmitter->getRandS16());
    f32 factor = workData->mEmitter->getRandF32();
    if (workData->mEmitter->checkDynFlag(JPADYN_FixedDensity)) {
        factor = 1.0f - (factor * factor);
    }

    f32 rad = workData->mVolumeSize * (workData->mVolumeMinRad + (factor * (1.0f - workData->mVolumeMinRad)));

    workData->mVolumePos.set(rad * JMASSin(angle), workData->mVolumeSize * workData->mEmitter->getRandZP(), rad * JMASCos(angle));
    workData->mVelOmni.mul(workData->mVolumePos, workData->mGlobalScl);
    workData->mVelAxis.set(workData->mVolumePos.x, 0.0f, workData->mVolumePos.z);
}

void JPAVolumeTorus(JPAEmitterWorkData* workData)
{
    s16 phi   = workData->mVolumeSweep * f32(workData->mEmitter->getRandS16());
    s16 theta = workData->mEmitter->getRandS16();

    f32 rad = workData->mVolumeSize * workData->mVolumeMinRad;

    workData->mVelAxis.set((rad * JMASSin(phi)) * JMASCos(theta), rad * JMASSin(theta), (rad * JMASCos(phi)) * JMASCos(theta));
    workData->mVolumePos.set(workData->mVelAxis.x + (workData->mVolumeSize * JMASSin(phi)), workData->mVelAxis.y,
                             workData->mVelAxis.z + (workData->mVolumeSize * JMASCos(phi)));
    workData->mVelOmni.mul(workData->mVolumePos, workData->mGlobalScl);
}

JPADynamicsBlock::JPADynamicsBlock(const u8* data)
{
    mData = (const JPADynamicsBlockData*)data;
    init();
}

void JPADynamicsBlock::init_jpa(const u8*, JKRHeap*)
{
    // UNUSED FUNCTION
}

void JPADynamicsBlock::init()
{
    switch (getVolumeType()) {
    case JPAVOL_Cube:
        mVolumeFunction = &JPAVolumeCube;
        break;
    case JPAVOL_Sphere:
        mVolumeFunction = &JPAVolumeSphere;
        break;
    case JPAVOL_Cylinder:
        mVolumeFunction = &JPAVolumeCylinder;
        break;
    case JPAVOL_Torus:
        mVolumeFunction = &JPAVolumeTorus;
        break;
    case JPAVOL_Point:
        mVolumeFunction = &JPAVolumePoint;
        break;
    case JPAVOL_Circle:
        mVolumeFunction = &JPAVolumeCircle;
        break;
    case JPAVOL_Line:
        mVolumeFunction = &JPAVolumeLine;
        break;
    }
}

void JPADynamicsBlock::create(JPAEmitterWorkData* work)
{
    if (work->mEmitter->checkStatus(JPAEMIT_RateStepEmit)) {
        s32 emitCount;
        s32 createCount;

        if (work->mEmitter->checkDynFlag(JPADYN_FixedInterval)) {
            s32 count;
            if (getVolumeType() == JPAVOL_Sphere) {
                count = 4 * getDivNumber() * getDivNumber() + 2;
            } else {
                count = getDivNumber();
            }
            emitCount = count;

            work->mVolumeEmitIdx = 0;
        } else {
            f32 newPtclCount           = work->mEmitter->mRate * (getRateRndm() * work->mEmitter->getRandZP() + 1.0f);
            f32 newEmitCount           = work->mEmitter->mEmitCount + newPtclCount;
            work->mEmitter->mEmitCount = newEmitCount;
            emitCount                  = (s32)newEmitCount;
            work->mEmitter->mEmitCount -= emitCount;

            if (work->mEmitter->checkStatus(JPAEMIT_FirstEmit) && 0.0f < newPtclCount && newPtclCount < 1.0f)
                emitCount = 1;
        }

        work->mCreateNumber = emitCount;
        if (work->mEmitter->checkStatus(JPAEMIT_StopEmitting)) {
            emitCount = 0;
        }

        createCount = emitCount;
        while (createCount > 0) {
            JPABaseParticle* ptcl = work->mEmitter->createParticle();
            if (ptcl == NULL)
                break;
            createCount--;
        }
    }

    if (++work->mEmitter->mRateStepTimer >= (work->mEmitter->mRateStep + 1)) {
        work->mEmitter->mRateStepTimer -= (work->mEmitter->mRateStep + 1);
        work->mEmitter->setFlag(JPAEMIT_RateStepEmit);
    } else {
        work->mEmitter->clearStatus(JPAEMIT_RateStepEmit);
    }

    work->mEmitter->clearStatus(JPAEMIT_FirstEmit);
}
