#include "JSystem/JAudio/JASFakeMatch2.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Sato/ObjUtility.h"
#include "Shiraiwa/JugemRodItem.h"
#include "Shiraiwa/SiUtil.h"


J3DAnmTexPattern *TJugemRodBoard::sJugemRodBoardBtpAnm;
const f32 TJugemRodBoard::scObjScale[4] = { 1.0f, 1.3f, 1.6f, 1.6f };

TJugemRodBoard::TJugemRodBoard() : TJugemRodItem(0x104) {
    NewAnmCtrl();
    reset();
    setObjFlagNorm();
}

TJugemRodBoard::~TJugemRodBoard() {}

void TJugemRodBoard::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    _164.resetFrame();
    int console = (SiUtil::getConsoleNum() & 0xFF) - 1;
#line 62
    JUT_MINMAX_ASSERT(0, console, 4);
    f32 objScale = scObjScale[console];
    mScale.setAll(objScale);
}

void TJugemRodBoard::loadAnimation() {
    J3DModelData *modelData = mModel.getModelData();
    void *ptrMainArc = ObjUtility::getPtrMainArc("/Objects/board01.btp");
    J3DAnmObjMaterial::setupTexPatternAnmData(&sJugemRodBoardBtpAnm, modelData, ptrMainArc);
}

const char *TJugemRodBoard::getBmdFileName() {
    return "/Jugem/jg_board01.bmd";
}

void TJugemRodBoard::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x80000, 0);
    _164.setExModel(&mModel);
    _164.setAnmBase(sJugemRodBoardBtpAnm);
    _164.initFrameCtrl(_164.getAnmBase());
}

void TJugemRodBoard::show(u8 param_1) {
    _164.setFrame(param_1);
    clrObjFlagHidding();
}

void TJugemRodBoard::update() {
    _164.anmFrameProc();
    setModelMatrixAndScale();
    mModel.update(0);
}


J3DAnmTevRegKey *TJugemRodBoard2::sJugemRodBoard2BrkAnm;
const f32 TJugemRodBoard2::scObjScale[4] = { 1.1f, 1.4f, 1.4f, 1.4f };

TJugemRodBoard2::TJugemRodBoard2() : TJugemRodItem(0x105) {
    reset();
    setObjFlagNorm();  
}

TJugemRodBoard2::~TJugemRodBoard2() {}

void TJugemRodBoard2::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    _164.resetFrame();
    int console = (SiUtil::getConsoleNum() & 0xFF) - 1;
#line 171
    JUT_MINMAX_ASSERT(0, console, 4);
    f32 objScale = scObjScale[console];
    mScale.setAll(objScale);
}

void TJugemRodBoard2::loadAnimation() {
    J3DModelData *modelData = mModel.getModelData();
    void *ptrMainArc = ObjUtility::getPtrMainArc("/Objects/BoardFinalLap.brk");
    J3DAnmObjMaterial::setupTevRegAnmData(&sJugemRodBoard2BrkAnm, modelData, ptrMainArc);
}

const char *TJugemRodBoard2::getBmdFileName() {
    return "/Jugem/BoardFinalLap.bmd";
}

void TJugemRodBoard2::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x1000000, 0);
    _164.setExModel(&mModel);
    _164.setAnmBase(sJugemRodBoard2BrkAnm);
    _164.initFrameCtrl(_164.getAnmBase());
}

void TJugemRodBoard2::show(u8) {
    clrObjFlagHidding();
}

void TJugemRodBoard2::update() {
    _164.anmFrameProc();
    setModelMatrixAndScale();
    mModel.update(0);
}

J3DAnmTevRegKey *TJugemRodBoardRev::sJugemRodBoardRevBrkAnm;
const f32 TJugemRodBoardRev::scObjScale[4] = { 1.0f, 1.3f, 1.4f, 1.4f };

TJugemRodBoardRev::TJugemRodBoardRev() : TJugemRodItem(0x106) {
    reset();
    setObjFlagNorm();
}

TJugemRodBoardRev::~TJugemRodBoardRev() {}

void TJugemRodBoardRev::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckGeoHitting();
    clrObjFlagCheckItemHitting();
    setObjFlagHidding();
    _164.resetFrame();
    int console = (SiUtil::getConsoleNum() & 0xFF) - 1;
#line 277
    JUT_MINMAX_ASSERT(0, console, 4);
    f32 objScale = scObjScale[console];
    mScale.setAll(objScale);
}

void TJugemRodBoardRev::loadAnimation() {
    J3DModelData *modelData = mModel.getModelData();
    void *ptrMainArc = ObjUtility::getPtrMainArc("/Objects/BoardReverse.brk");
    J3DAnmObjMaterial::setupTevRegAnmData(&sJugemRodBoardRevBrkAnm, modelData, ptrMainArc);
}

const char *TJugemRodBoardRev::getBmdFileName() {
    return "/Objects/BoardReverse.bmd";
}

void TJugemRodBoardRev::createModel(JKRSolidHeap *jkrSolidHeap, u32 param_2) {
    mModel.createDifferedModel(jkrSolidHeap, param_2, 0x1000000, 0);
    _164.setExModel(&mModel);
    _164.setAnmBase(sJugemRodBoardRevBrkAnm);
    _164.initFrameCtrl(_164.getAnmBase());
}

void TJugemRodBoardRev::show(u8 param_1) {
    _164.setFrame(param_1);
    clrObjFlagHidding();
}

void TJugemRodBoardRev::update() {
    _164.anmFrameProc();
    setModelMatrixAndScale();
    mModel.update(0);
}

void TJugemRodBoardRev::calc() {
    _164.update();
}

void TJugemRodBoard2::calc() {
    _164.update();
}

void TJugemRodBoard::calc() {}
