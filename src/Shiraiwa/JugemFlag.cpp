#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/ObjUtility.h"
#include "Shiraiwa/JugemItem.h"


J3DAnmTransform *TJugemFlag::sJugemFlagBckAnmTrans;
J3DMtxCalc *TJugemFlag::sJugemFlagBckMtxCalc;

TJugemFlag::TJugemFlag() : TJugemItem(0x102) {
    NewAnmCtrl();
    reset();
}

TJugemFlag::~TJugemFlag() {}

void TJugemFlag::reset() {
    resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
}

void TJugemFlag::loadAnimation() {
    J3DAnmObjTrans::setupTransAnmData(
        &sJugemFlagBckAnmTrans, 
        &sJugemFlagBckMtxCalc, 
        mModel.getModelData(), 
        ObjUtility::getPtrMainArc("/Objects/jg_flag_wy.bca")
    );
}

const char *TJugemFlag::getShadowBmdFileName() {
    return nullptr;
}

const char *TJugemFlag::getBmdFileName() {
    static const char *cBmdName = "/Objects/jg_flag.bmd";
    return cBmdName;
}

void TJugemFlag::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createModel(jkrSolidHeap, param_2, false);
    AnmController *anmCtrl = mAnmCtrl;
    anmCtrl->mTrans = new AnmControlTrans();
    AnmControlTrans *anmCtrlTrans = anmCtrl->mTrans;

    anmCtrlTrans->initAnm(1, &mModel);
    mAnmCtrl->mTrans->registration(0, sJugemFlagBckAnmTrans, sJugemFlagBckMtxCalc);
    anmCtrlTrans = mAnmCtrl->mTrans;

    anmCtrlTrans->getFrameCtrl(0)->setAttribute(2);
}

void TJugemFlag::createColModel(J3DModelData *j3dModelData) {}

void TJugemFlag::changeAnimation(u32 param_1) { // TODO: Is this param just unused?
    mAnmCtrl->Reset();
}
