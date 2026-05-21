#include "Inagaki/GameAudioMain.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/RaceMgr.h"
#include "Shiraiwa/JugemMain.h"

bool TJugem::isDemoMode() {
    return RaceMgr::getCurrentManager() == false;
}

void TJugem::initFunc_DemoHide() {
    hideAll();
}

void TJugem::doFunc_DemoHide() {}

void TJugem::initFunc_DemoIn() {
    wearCap(0);
#line 50
    JUT_MINMAX_ASSERT(0, mDemoPattern, 3);
    switch (mDemoPattern) {
        case 0:
            hold(1, 2);
            changeAnimation(0);
            break;

        case 1:
            hold(1, 5);
            changeAnimation(1);
            break;

        case 2:
            hold(1, 5);
            changeAnimation(2);
            break;

        default:
            break;
    }
    show(mDemoPattern);
}

void TJugem::doFunc_DemoIn() {
    if (mAnmPlayer.getCurAnmNumber() != 0) {
        setState(0x13);
    }
    GetGameAudioMain()->startSystemSe(0x20066);
}

void TJugem::initFunc_DemoWait() {}

void TJugem::doFunc_DemoWait() {
    GetGameAudioMain()->startSystemSe(0x20066);
}

void TJugem::initFunc_WaitDemoOut() {}

void TJugem::doFunc_WaitDemoOut() {
    GetGameAudioMain()->startSystemSe(0x20066);
    if (mAnmPlayer.getCurAnmNumber() >= 1) {
        setState(0x15);
    }
}

void TJugem::initFunc_DemoOut() {
    mAnmPlayer._10 = 1;
    mAnmPlayer._e |= 2;
    TJugemItem *tJugemItem = mJugemItem;
    if (tJugemItem == nullptr) {
        return;
    }
    tJugemItem->mJugemAnmPlayer._10 = 1;
    tJugemItem->mJugemAnmPlayer._e |= 2;
}

void TJugem::doFunc_DemoOut() {
    if (mAnmPlayer.getCurAnmNumber() >= 2) {
        if (mAnmPlayer.getCurAnmNumber() == 2) {
            if (mAnmPlayer.mController->getFrameCtrl(mAnmPlayer.mController->getNowTransNo())->getFrame() <= 1.0f) {
                GetGameAudioMain()->startSystemSe(0x20067);
            }
        }
    } else {
        GetGameAudioMain()->startSystemSe(0x20066);
    }
}

void TJugem::initFunc_Ending() {
    hideAll();
}

void TJugem::doFunc_Ending() { }
