#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Kaneshige/RaceMgr.h"
#include "Shiraiwa/JugemMain.h"
#include "Yamamoto/kartCtrl.h"

void TJugem::initFunc_RescueWait() {}

void TJugem::doFunc_RescueWait() {
    if ((GetKartCtrl()->getKartBody(getKartNum())->mCarStatus & 0x400000ull) == 0) {
        setState(8);
    }
}

void TJugem::initFunc_Rescue() {
    mSplineInterp.stop();
    wearCap(1);
    hold(1, 0);
    changeAnimation(2);
    show(0);
}

void TJugem::doFunc_Rescue() {
    GeographyObj::moveShadowModel();
    if (checkRescueOutProc()) {
        if (RaceMgr::getCurrentManager()->getKartChecker(getKartNum())->isGoal()) {
            setState(9);
        } else {
            setState(0);
        }
    }
}

bool TJugem::checkRescueInProc() {
    switch (getGlobalState()) {
        case 3:
        case 6:
        case 7:
            return false;
    }
    return ((GetKartCtrl()->getKartBody(mKartNum)->mCarStatus & 0x400000) != 0) ? true : false;
}

bool TJugem::checkRescueOutProc() {
    return (getGlobalState() == 6) || ((GetKartCtrl()->getKartBody(getKartNum())->mCarStatus & 0x400000ull) == 0);
}
