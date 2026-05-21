#include "Inagaki/GameAudioMain.h"
#include "JSystem/J3D/J3DJoint.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/Movie/MovieApp.h"
#include "Osako/ResMgr.h"
#include "Sato/AnmController.h"
#include "Sato/J3DAnmObject.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/JPEffectPerformer.h"
#include "Sato/ObjUtility.h"
#include "Shiraiwa/AnmPlayer.h"
#include "Shiraiwa/JugemMain.h"
#include "Shiraiwa/JugemRodItem.h"
#include "Yamamoto/kartCamera.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/mtx.h"
#include "types.h"

J3DAnmTevRegKey *TJugemRodSignal::sJugemRodSignalBrkAnm;
const char *TJugemRodSignal::scRedParticleName = "mk_jg_signalR";
const char *TJugemRodSignal::scGreenParticleName = "mk_jg_signalB";
s16 TJugemRodSignal::sRandomWait;
int TJugemRodSignal::sLeftJointNo;
int TJugemRodSignal::sMiddleJointNo;
int TJugemRodSignal::sRightJointNo;
const u8 TJugemRodSignal::scSignalInterval = 0x3c;


TJugemRodSignal::TJugemRodSignal() : TJugemRodItem(0x107) {
    NewAnmCtrl();
    setObjFlagNorm();
}

TJugemRodSignal::~TJugemRodSignal() {}

void TJugemRodSignal::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    sRandomWait = 0;
    mEmitterRight = nullptr;
    mEmitterMiddle = nullptr;
    mEmitterLeft = nullptr;
    _164.resetFrame();
    _184 = 0;
    _188 = 0;
    if (TJugem::isDemoMode()) {
        mScale.x = 1.0f;
        mScale.y = 1.0f;
        mScale.z = 1.0f;
    } else {
        mScale.x = 1.5f;
        mScale.y = 1.5f;
        mScale.z = 1.5f;
    }
}

void TJugemRodSignal::loadAnimation() {
    void *pvVar1;
    J3DModelData *test = mModel.getModelData();
    static char *fileName = "/Objects/jg_signal.brk";
    if (TJugem::isDemoMode()) {
        pvVar1 = ResMgr::getPtr(ResMgr::mcArcOpening, fileName);
    } else {
        pvVar1 = ObjUtility::getPtrMainArc(fileName);
    }
    J3DAnmObjMaterial::setupTevRegAnmData(
        &sJugemRodSignalBrkAnm, 
        test,
        pvVar1
    );
}

const char *TJugemRodSignal::getShadowBmdFileName() {
    return nullptr;
}

const char *TJugemRodSignal::getBmdFileName() {
    static const char *cBmdName = "/Objects/jg_signal.bmd";
    return cBmdName;
}

void TJugemRodSignal::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x1000000, 0);
    _164.setExModel(&mModel);
    _164.setAnmBase(sJugemRodSignalBrkAnm);
    _164.initFrameCtrl(_164.getAnmBase());

    sLeftJointNo = mModel.getModelData()->getJointName()->getIndex("left");
    #line 147
    JUT_ASSERT_MSG(sLeftJointNo != -1, "joint \"left\" is Not Found.");

    sMiddleJointNo = mModel.getModelData()->getJointName()->getIndex("middle");
    #line 149
    JUT_ASSERT_MSG(sMiddleJointNo != -1, "joint \"middle\" is Not Found.");

    sRightJointNo = mModel.getModelData()->getJointName()->getIndex("right");
    #line 151
    JUT_ASSERT_MSG(sRightJointNo != -1, "joint \"right\" is Not Found.");
}

void TJugemRodSignal::createColModel(J3DModelData *modelData) {}

void TJugemRodSignal::calc() {
    if (_184 != false) {
        bool effectAccepted = isAcceptEffect();
        _188++;
        if (_188 == scSignalInterval) {
            if (effectAccepted) {
                createEmitter(&mEmitterLeft, sLeftJointNo, scRedParticleName);
            }
            show(1);
            GetGameAudioMain()->startRaceSystemSe(0x20006);
        } else if (_188 == 0x78) {
            if (effectAccepted) {
                createEmitter(&mEmitterMiddle, sMiddleJointNo,scRedParticleName);
            }
            show(3);
            GetGameAudioMain()->startRaceSystemSe(0x20006);
        } else if (_188 == 0xb4) {
            if (effectAccepted) {
                createEmitter(&mEmitterRight, sRightJointNo, scRedParticleName);
            }
            show(5);
            GetGameAudioMain()->startRaceSystemSe(0x20006);
        } else if (_188 == sRandomWait + 0xf0) {
            if (effectAccepted) {
                createEmitter(&mEmitterLeft, sLeftJointNo,  scGreenParticleName);
                createEmitter(&mEmitterMiddle, sMiddleJointNo,scGreenParticleName);
                createEmitter(&mEmitterRight, sRightJointNo, scGreenParticleName);
            }
            show(7);
            GetGameAudioMain()->startRaceSystemSe(0x20007);
        } else if (_188 > sRandomWait + 300) {
            _184 = false;
        }
    }
}

void TJugemRodSignal::createEmitter(JPABaseEmitter **emitter, s32 index, const char *effectName) {
    Mtx &mtx = mModel.getModel()->getAnmMtx(index);
    JGeometry::TVec3f pos(mtx[0][3], mtx[1][3], mtx[2][3]);
    *emitter = JPEffectMgr::getEffectMgr()->createEmtCam(effectName, pos, mKartCamIndex);
    
    JGeometry::TVec3f stackScale = mScale;
    (*emitter)->setGlobalScale(stackScale);
}

void TJugemRodSignal::startCountDown() {
    _188 = 0;
    _184 = 1;
}

void TJugemRodSignal::show(u8 param_1) {
    _164.setFrame(param_1);
    clrObjFlagHidding();
}


// FIX: Try and remove GOTOs.
void TJugemRodSignal::update() {
    _164.anmFrameProc();
    setModelMatrixAndScale();
    mModel.update(0);

    if (!_184) return;
    if (!isAcceptEffect()) return;

    bool bVar1;

    // Left Emitter
    JPABaseEmitter* emitter = mEmitterLeft;
    int jointNumber = sLeftJointNo;
    if (emitter != nullptr) {
        bVar1 = false;
        if (emitter->checkStatus(0x8) != 0 &&
            (emitter->mAlivePtclBase.getNum() + emitter->mAlivePtclChld.getNum() == 0)) {
            bVar1 = true;
        }

        if (!bVar1) {
            JPASetRMtxTVecfromMtx(mModel.getModel()->getAnmMtx(jointNumber), emitter->mGlobalRot, &emitter->mGlobalTrs);
            goto end_left; // Match assembly jump behavior
        }
    }
    mEmitterLeft = nullptr;
    end_left:;

    // Middle Emitter
    emitter = mEmitterMiddle;
    jointNumber = sMiddleJointNo;
    if (emitter != nullptr) {
        bVar1 = false;
        if (emitter->checkStatus(0x8) != 0 &&
            (emitter->mAlivePtclBase.getNum() + emitter->mAlivePtclChld.getNum() == 0)) {
            bVar1 = true;
        }

        if (!bVar1) {
            JPASetRMtxTVecfromMtx(mModel.getModel()->getAnmMtx(jointNumber), emitter->mGlobalRot, &emitter->mGlobalTrs);
            goto end_mid;
        }
    }
    mEmitterMiddle = nullptr;
    end_mid:;

    // Right Emitter
    emitter = mEmitterRight;
    jointNumber = sRightJointNo;
    if (emitter != nullptr) {
        bVar1 = false;
        if (emitter->checkStatus(0x8) != 0 &&
            (emitter->mAlivePtclBase.getNum() + emitter->mAlivePtclChld.getNum() == 0)) {
            bVar1 = true;
        }

        if (!bVar1) {
            JPASetRMtxTVecfromMtx(mModel.getModel()->getAnmMtx(jointNumber), emitter->mGlobalRot, &emitter->mGlobalTrs);
            goto end_right;
        }
    }
    mEmitterRight = nullptr;
    end_right:;
}

bool TJugemRodSignal::isAcceptEffect() {
    switch (GetKartCtrl()->getKartCam(mKartCamIndex)->GetCameraMode()) {
        case 2:
            return false;
        default:
            return true;
    }
}




TAnmInfo TJugemRodPukuPuku::sAnmInfos_Puku_Demo2[3] = {
    {"/Objects/jg_puku_in_b.bck", nullptr, nullptr, 0, 0, 1, 1, 0},
    {"/Objects/jg_puku_wait_b.bck", nullptr, nullptr, 2, 0, 0, 2, 0},
    {"/Objects/jg_puku_out_b.bck", nullptr, nullptr, 0, 0, 1, 0xFF, 0},
};

TAnmInfo TJugemRodPukuPuku::sAnmInfos_Puku_Demo3[1] = {
    { "/Objects/jg_puku_in_c.bck", nullptr, nullptr, 0, 0, 1, 0xFF, 0 }
};

TJugemAnmTableEntry TJugemRodPukuPuku::sDemoAnmStateTable[3] = {
    { nullptr, 0 },
    { sAnmInfos_Puku_Demo2, 3 },
    { sAnmInfos_Puku_Demo3, 1 }
};


TJugemRodPukuPuku::TJugemRodPukuPuku() : TJugemRodItem(0x108) {
    for (int i = 0; i < 3; i++) {
        mAnmController[i] = new AnmController();
        if (sDemoAnmStateTable[i].tAnmInfo != nullptr) {
            TAnmPlayer::resetAnimations(sDemoAnmStateTable[i].tAnmInfo, sDemoAnmStateTable[i].arraySize);
        }
    }
}

TJugemRodPukuPuku::~TJugemRodPukuPuku() {
    for (s32 i = 0; i < 3; i++) {
        delete mAnmController[i];
    }
}

void TJugemRodPukuPuku::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    //_164.getFrameCtrl()->setAttribute(0);
    _170 = 0;
    //_164.getFrameCtrl()->mState = 0;
    _171 = 0;
}

void TJugemRodPukuPuku::loadAnimation() {
    J3DModelData *modelData = mModel.getModelData();
    for (int i = 0; i < 3; i++) {
        if (sDemoAnmStateTable[i].tAnmInfo != nullptr) {
            TAnmPlayer::loadAnimations(sDemoAnmStateTable[i].tAnmInfo,
                sDemoAnmStateTable[i].arraySize, modelData, ResMgr::mcArcOpening
            );
        }
    }

    for (u16 i = 0; i < modelData->getShapeNum(); i++) {
        modelData->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
    }
}

void TJugemRodPukuPuku::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x200, 1);

    u16 idx = mModel.getModelData()->getJointName()->getIndex("puku_root");
    J3DJoint *joint = mModel.getModelData()->getJointNodePointer(idx);
    joint->setCallBack(nodeCallBack);
    mModel._14 = nullptr;

    for (int i = 0; i < 3; i++) {
        if (sDemoAnmStateTable[i].tAnmInfo != nullptr) {
            TAnmPlayer::registAnimations(
                mAnmController[i],
                &mModel,
                sDemoAnmStateTable[i].tAnmInfo, sDemoAnmStateTable[i].arraySize
            );
        }
    }
}

void TJugemRodPukuPuku::show(u8 idx) {
    #line 450
    JUT_MINMAX_ASSERT(0, idx, 3);
    if (idx != 0) {
        mAnmCtrl = mAnmController[idx];
        mAnmPlayer.init(mAnmController[idx],
            sDemoAnmStateTable[idx].tAnmInfo,
            sDemoAnmStateTable[idx].arraySize
        );
    }
    _171 = idx;
    clrObjFlagHidding();
}

void TJugemRodPukuPuku::update() {
    setModelMatrixAndScale();
    mModel.update(0);
}

void TJugemRodPukuPuku::calc() {
    AnmControlTrans *trans;
    
    mAnmPlayer.update();
    if (mAnmPlayer._10 == 0) {
        setObjFlagHidding();
    }
    switch (_171) {
        case 2:
            u8 anm_no = mAnmPlayer.getCurAnmNumber();
            if (mAnmPlayer.mController->getFrameCtrl(anm_no)->getFrame() < 151.0f) {
                GetGameAudioMain()->startSystemSe(0x20068);
            } else if (_170 == 0) {
                GetGameAudioMain()->startSystemSe(0x20069);
                _170 = 1;
            }
        break;
    }
}

void TJugemRodPukuPuku::setPosition(const JGeometry::TVec3f &newPos) {
    switch (_171) {
        case 2:
            if (_170 != 0) {
                return;
            }
            mPos.set(newPos);
            return;
    }
    
    mPos.set(newPos);
}

void TJugemRodPukuPuku::setRMtx(const JGeometry::TPos3f &newRotMtx) {
    switch (_171) {
        case 2:
            if (_170 != 0) {
                return;
            }
            mRotMtx.set(newRotMtx.mMtx);
            return;
    }
    
    mRotMtx.set(newRotMtx.mMtx);
}

void TJugemRodPukuPuku::setCurrentViewNo(u32 viewNo) {
    mModel.setCurrentViewNo(viewNo);
    
    MtxPtr effectMtx = MovieApp::getMovieApp()->getMdlViewer()->getChaseLight()->getEffectMtx();
    mModel.setEffectMtx(effectMtx, 1);
}

bool TJugemRodPukuPuku::nodeCallBack(J3DJoint* joint, int timing) {
    if (timing == 0) {
        int idx = joint->getJntNo();

        J3DModel *model = j3dSys._38;
        // FIX: This is almost certainly wrong. Need to work out correct casting.
        TJugemRodItem *tJugemRod = (TJugemRodItem *)((ExModel*)model->getUserArea())->_14;
        
        if (tJugemRod != nullptr) {
            MtxPtr calc_mtx = model->getAnmMtx(idx);

            // JGeometry::TVec3f test;
            // test.set(calc_mtx[0][0], calc_mtx[0][1], calc_mtx[0][2]);

            calc_mtx[1][3] = calc_mtx[0][1];
            calc_mtx[2][3] = calc_mtx[0][2];
            calc_mtx[3][3] = calc_mtx[0][3];

            model->setAnmMtx(idx, calc_mtx);
            PSMTXCopy(calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return true;
}
