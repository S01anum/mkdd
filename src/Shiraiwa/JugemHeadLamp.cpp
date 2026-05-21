#include "Sato/AnmController.h"
#include "Sato/ObjUtility.h"
#include "Shiraiwa/JugemHeadItem.h"
#include "Shiraiwa/JugemMain.h"
#include "Shiraiwa/SiUtil.h"
#include "types.h"

J3DAnmTevRegKey *TJugemHeadLamp::sJugemHeadLampBrkAnm;
J3DAnmTransform *TJugemHeadLamp::sJugemHeadLampBcaAnm;
J3DMtxCalc *TJugemHeadLamp::sJugemHeadLampBcaMtxCalc;

TJugemHeadLamp::TJugemHeadLamp() : TJugemHeadItem(0x109) {
    NewAnmCtrl();
    setObjFlagNorm();
}

TJugemHeadLamp::~TJugemHeadLamp() {}

void TJugemHeadLamp::reset() {
    resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    mAnmCtrl->Reset();
    _150.resetFrame();
}

void TJugemHeadLamp::loadAnimation() {
    J3DModelData *modelData = mModel.getModelData();
    J3DAnmObjTrans::setupTransAnmData(&sJugemHeadLampBcaAnm, &sJugemHeadLampBcaMtxCalc, modelData, ObjUtility::getPtrMainArc("/Objects/jg_patlamp.bca"));
    J3DAnmObjMaterial::setupTevRegAnmData(&sJugemHeadLampBrkAnm, modelData, ObjUtility::getPtrMainArc("/Objects/jg_patlamp.brk"));
}

void TJugemHeadLamp::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x1000000, false);
    AnmController *anmCtrl = mAnmCtrl;
    anmCtrl->mTrans = new AnmControlTrans();
    AnmControlTrans *anmCtrlTrans = anmCtrl->mTrans;

    anmCtrlTrans->initAnm(1, &mModel);
    mAnmCtrl->mTrans->registration(0, sJugemHeadLampBcaAnm, sJugemHeadLampBcaMtxCalc);
    anmCtrlTrans = mAnmCtrl->mTrans;

    mAnmCtrl->mTrans->getFrameCtrl(0)->setAttribute(2);
    _150.mModel = &mModel;
    _150.setAnmBase(sJugemHeadLampBrkAnm);
    _150.initFrameCtrl(_150.getAnmBase());
}

const char *TJugemHeadLamp::getBmdFileName() {
    static const char *cBmdName = "/Objects/jg_patlamp.bmd";
    return cBmdName;
}

void TJugemHeadLamp::calc() {
    _150.update();
}

void TJugemHeadLamp::update() {
    _150.anmFrameProc();
    setModelMatrixAndScale();
    mModel.update(0);
}

TJugemHeadHear::TJugemHeadHear() : TJugemHeadItem(0x100) {
    reset();
    setObjFlagNorm();
}

TJugemHeadHear::~TJugemHeadHear() {}

void TJugemHeadHear::reset() {
    resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
}

const char *TJugemHeadHear::getBmdFileName() {
    int index = 0;
    
    if (TJugem::isDemoMode() == false && SiUtil::getConsoleNum() != 1) {
        index = 1;
    }

    static const char *cBmdName[2] = {
        "/Objects/jg_hair_model.bmd",
        "/Objects/jg_hair_modelL.bmd"
    };
    
    return cBmdName[index];
}
