#include "Shiraiwa/JugemMain.h"

Vec TJugem::scGoalPoints0[4] = {
    { 0.0f,    0.0f, 0.0f },
    { 0.0f,  750.0f, 0.0f },
    { 0.0f, -170.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
};

void TJugem::initFunc_Goal() {
    if (!checkJugemAppearRank()) {  // o_O - checkJugemAppearRank always returns true...
        setState(0);                // ... so this never gets executed? Why?!
    } else {
        wearCap(0);
        hold(2, 0);
        show(0);
        changeAnimation(4);
        resetJugemOriginNumber();
        mSplineInterp.setPoint(4, &scGoalPoints0->x, 0.03f, false);
    }
}

void TJugem::doFunc_Goal() {
    move(getKartNum());
}

bool TJugem::checkJugemAppearRank() {
    return true;
}
