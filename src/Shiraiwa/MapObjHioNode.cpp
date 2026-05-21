#include "Shiraiwa/Objects/MapObjHioNode.h"
#include "Sato/GeographyObj.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

void TMapObjStamper::resetFrame() {
    for (int i = 0; i < 8; i++) {
        mStampFrame[i] = 0;
        mIsStamping[i] = false;
        GeographyObj::clrUserFlg1(i);
    }
}

void TMapObjStamper::frameProc() {
    for (int i = 0; i < 8; i++) {
        if (!mIsStamping[i]) continue;
        
        mStampFrame[i]++;
        if (checkFrame(i)) {
            GeographyObj::setUserFlg1(i);
        }
    }
}
