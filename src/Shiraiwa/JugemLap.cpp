#include "Kaneshige/RaceMgr.h"
#include "Sato/AnmController.h"
#include "Shiraiwa/JugemMain.h"

f32 TJugem::sLapMoveSpeed1 = 0.056f;
f32 TJugem::sLapMoveSpeed2 = 0.045f;
f32 TJugem::sAnimeStartFrame = 60.0f;
f32 TJugem::sLapPauseRatio = 5.5f;
s16 TJugem::sLastStayFrame = 0x32;

Vec TJugem::scLapPointsIn_1p[10] = {
    { 400.0f, 1000.0f, -300.0f },
    { 400.0f, -170.0f, -400.0f },
    { 400.0f, -150.0f, -450.0f },
    { 400.0f,  140.0f, -500.0f },
    { 400.0f,  120.0f, -500.0f },
    { 400.0f,  -50.0f, -500.0f },
    { 400.0f,  100.0f, -500.0f },
    { 400.0f,  100.0f, -500.0f },
    { 400.0f, -450.0f, -500.0f },
    { 400.0f,    0.0f, -500.0f },
};

Vec TJugem::scLapPointsIn_multi[10] = {
    { 550.0f, 1000.0f, -300.0f },
    { 550.0f,  -90.0f, -300.0f },
    { 550.0f,  -70.0f, -450.0f },
    { 550.0f,  160.0f, -500.0f },
    { 550.0f,  140.0f, -500.0f },
    { 550.0f,  -30.0f, -500.0f },
    { 550.0f,  120.0f, -500.0f },
    { 550.0f,  120.0f, -500.0f },
    { 550.0f, -310.0f, -500.0f },
    { 550.0f,    0.0f, -500.0f },
};

void TJugem::initFunc_Lap() {
    wearCap(0);
    if (_24c == RaceMgr::getCurrentManager()->getTotalLapNumber() - 1) {
        hold(1, 3);
        show(0);
    } else {
        hold(1, 1);
        show(_24c - 1);
    }
    changeAnimation(1);

    AnmController *anmController = mAnmPlayer.mController;
    anmController->getFrameCtrl(anmController->getNowTransNo())->setFrame(sAnimeStartFrame);
    anmController = mJugemItem->mJugemAnmPlayer.mController;
    anmController->getFrameCtrl(anmController->getNowTransNo())->setFrame(sAnimeStartFrame);

    switch (getScreenType()) {
        case 0:
            mSplineInterp.setPoint(10, &scLapPointsIn_1p->x, sLapMoveSpeed1, false);
            break;
        case 1:
        case 2:
            mSplineInterp.setPoint(10, &scLapPointsIn_multi->x, sLapMoveSpeed1, false);
            break;
        default:
            break;
    }
    resetJugemOriginNumber();
}

void TJugem::doFunc_Lap() {
    move(getKartNum());
    if (mSplineInterp.getUnknownValue10() + mSplineInterp.getRatio() >= sLapPauseRatio) {
        setState(3);
    }
}

void TJugem::initFunc_LapStay() {}

void TJugem::doFunc_LapStay() {
    move(getKartNum());
    if (getStateCount() > TJugem::sLastStayFrame) {
        setState(4);
    }
}

void TJugem::initFunc_LapEnd() {
    mSplineInterp.setSpeed(sLapMoveSpeed2);
}

void TJugem::doFunc_LapEnd() {
    move(getKartNum());
    if (mSplineInterp.getUnknownValue10() + mSplineInterp.getRatio() > 9.1f) {
        setState(1);
    }
}

bool TJugem::checkLapProc() {
    s32 lap = RaceMgr::getCurrentManager()->getKartChecker(getKartNum())->getLap();
    if (lap > _24c) {
        _24c = lap;
        return true;
    }
    return false;
}
