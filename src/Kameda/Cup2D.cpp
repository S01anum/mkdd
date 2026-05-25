#include "Kameda/Cup2D.h"
#include "mathHelper.h"

Cup2D *Cup2D::mCup2D;

Cup2D::Cup2D(JKRArchive *) {}

Cup2D::~Cup2D() {
    if (mCup2D)
        mCup2D = nullptr;
}

void *Cup2D::getCupTexture(ERaceGpCup, int) {}


#include "JSystem/JAudio/JASFakeMatch2.h"
