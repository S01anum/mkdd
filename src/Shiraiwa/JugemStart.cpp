#include "Kaneshige/RaceMgr.h"
#include "Shiraiwa/JugemMain.h"
#include "Shiraiwa/JugemRodItem.h"
#include "Yamamoto/kartCtrl.h"

Vec TJugem::scStartPoints0_1p[6] = {
    { 0.0f, 375.0f, -350.0f },
    { 0.0f,  75.0f, -350.0f },
    { 0.0f, 175.0f, -350.0f },
    { 0.0f,   0.0f, -350.0f },
    { 0.0f,   0.0f, -350.0f },
    { 0.0f,   scEraseHeight, -350.0f },
};

Vec TJugem::scStartPoints0_2p[5] = {
    { 300.0f, 240.0f, -400.0f },
    { 300.0f, -60.0f, -400.0f },
    { 300.0f,  40.0f, -400.0f },
    { 300.0f, 240.0f, -400.0f },
    { 300.0f,   scEraseHeight, -400.0f },
};

s16 TJugem::sStartWaitCameraFrame;
s16 TJugem::sStartWaitBlendFrame;
f32 TJugem::sStartDownSpeed = 0.008f;
f32 TJugem::sStartUpSpeed = 0.12f;
s16 TJugem::sStartUpFrame = 10;

void TJugem::resetStartPosition() {
    switch (getScreenType()) {
        case 0:
            mSplineInterp.setPoint(6, &scStartPoints0_1p->x, sStartDownSpeed, false);
            break;
        case 1:
        case 2:
            mSplineInterp.setPoint(5, &scStartPoints0_2p->x, sStartDownSpeed, false);
            break;
        default:
            break;
    }
    
    mSplineInterp.update();
    mSplineInterp.stop();

    if (RaceMgr::getCurrentManager()->getRacePhase() == PHASE_CRS_DEMO) {
        hideAll();
        setState(10);
    } else {
        setState(0xb);
    }

    changeAnimation(0);
    resetJugemOriginCourse();
}

void TJugem::initFunc_StartWaitPermission() {
    changeAnimation(0);
}

void TJugem::doFunc_StartWaitPermission() {
    move(getKartNum());
    if (GetKartCtrl()->CheckJugemuSignal()) {
        setState(0xb);
    }
}

void TJugem::initFunc_StartWaitCamera() {
    wearCap(0);
    hold(1, 2);
    show(0);
    changeAnimation(0);
    mSplineInterp.restart();
    _274 = 0;
}

void TJugem::doFunc_StartWaitCamera() {
    move(getKartNum());
    if (getStateCount() >= sStartWaitCameraFrame) {
        setState(0xc);
    }
}

void TJugem::initFunc_StartDown() {}

void TJugem::doFunc_StartDown() {
    move(getKartNum());
    if (mSplineInterp.getUnknownValue10() + mSplineInterp.getRatio() > 1.3f) {
        mSplineInterp.stop();
    }
    if (mSplineInterp.checkUnknownBool13() == false && mAnmPlayer.getCurAnmNumber() != 0) {
        setState(0xd);
    }
}

void TJugem::initFunc_StartWaitBlend() {}

void TJugem::doFunc_StartWaitBlend() {
    move(getKartNum());
    if (getStateCount() > sStartWaitBlendFrame) {
        setState(0xe);
    }
}

void TJugem::initFunc_StartCountDown() {
    startNextAnime();
    mSignal->startCountDown();
}

void TJugem::doFunc_StartCountDown() {
    move(getKartNum());
    bool shouldSetState = false;
    if (mSignal->_184 != 0) {
        int target = (((s16)(TJugemRodSignal::scSignalInterval & 0xFF) * 4) + TJugemRodSignal::sRandomWait) - 10;

        if (mSignal->_188 == target) {
            shouldSetState = true;
        }
    }
    if (shouldSetState) {
        setState(0xf);
    }
}

void TJugem::initFunc_StartUp() {
    startNextAnime();
}

void TJugem::doFunc_StartUp() {
    move(getKartNum());

    if (mIsAbleStart == false && ((s32)mSignal->_164.getFrameCtrl()->getFrame() == 7)) {
        mIsAbleStart = true;
    }
    if (getStateCount() > sStartUpFrame) {
        setState(0x10);
    }
}

void TJugem::initFunc_StartWaitHide() {
    _274 = 1;
    mSplineInterp.restart();
    mSplineInterp.setSpeed(sStartUpSpeed);
}

void TJugem::doFunc_StartWaitHide() {
    move(getKartNum());
    if (!mSplineInterp.checkUnknownBool13()) {
        setState(1);
    }
}

void TJugem::startNextAnime() {
    mAnmPlayer._10 = 1;
    mAnmPlayer._e |= 1;
    TJugemItem *tJugemItem = mJugemItem;
    if (tJugemItem == nullptr) {
        return;
    }
    tJugemItem->mJugemAnmPlayer._10 = 1;
    tJugemItem->mJugemAnmPlayer._e |= 1;
}

int TJugem::getSignalState() {
    int signalState = -1;
    switch (getState()) {
        case 0xc:
        case 0xd:
        case 0xe:
        case 0xf:
            // Why is _17c declared as a `f32`, but used as an `s32` everywhere else...?
            signalState = (s32)mSignal->_164.getFrameCtrl()->getFrame();
    }
    return signalState;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
