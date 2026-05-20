#include "Yamamoto/KartCrash.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

#include "Kameda/MotorManager.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/JPEffectPerformer.h"
#include "Yamamoto/kartCtrl.h"

// comments inside functions are inline functions being called in that function

void KartCrash::Init(int bodyIndex) {
    mBody = GetKartCtrl()->getKartBody(bodyIndex);
    _C = 0;
    _D = 0;
    _E = 0;
    mMatchlessTimer = 0;
}

void KartCrash::DoRabbitMark() {
    KartChecker* checker = RCMGetKartChecker(mBody->mMynum);

    if (RaceMgr::getCurrentManager()->getRaceMode() == ESCAPE_BATTLE && checker->releaseRabbitMark()) {
        mMatchlessTimer = 200;
    }
}

void KartCrash::DoMatchlessTimer() {
    KartBody* body  = mBody;
    RaceMgr* mgr = RaceMgr::getCurrentManager();
    KartChecker* kartChecker = RCMGetKartChecker(body->mMynum);

    if (mgr->getRaceMode() == BALLOON_BATTLE) {
        if (mMatchlessTimer != 0 && (body->mCarStatus & 0x100000) == 0) {
            mMatchlessTimer--;
            if (mMatchlessTimer == 199) {
                if (kartChecker->getBalloonNumber() != 0) {
                    if (!mgr->getKartDrawer(body->mMynum)->isFlashing()) {
                        mgr->getKartDrawer(body->mMynum)->flashOn();
                        JPEffectPerformer::setEffect(JPEffectPerformer::Effect_UnknownD, body->mMynum, body->mPos, 1);
                    }

                }
            }
        }
        else {
            mgr->getKartDrawer(body->mMynum)->flashOff();
        }
    }
    else {
        if (mMatchlessTimer != 0 && (body->mCarStatus & 0x100000) == 0) {
            mMatchlessTimer--;
            if (mMatchlessTimer == 199) {
                if (!mgr->getKartDrawer(body->mMynum)->isFlashing()) {
                    mgr->getKartDrawer(body->mMynum)->flashOn();
                    JPEffectPerformer::setEffect(JPEffectPerformer::Effect_UnknownD, body->mMynum, body->mPos, 1);
                }
            }
        } else {
            mgr->getKartDrawer(body->mMynum)->flashOff();
        }
    }
}

bool KartCrash::CheckMatchlessTimer() {
    if (mMatchlessTimer != 0) return true;
    return false;
}

void KartCrash::SetMatchlessTimer() {
    RaceMgr* mgr = RaceMgr::getCurrentManager();
    if (mgr->getRaceMode() == BALLOON_BATTLE || mgr->getRaceMode() == BOMB_BATTLE) {
        mMatchlessTimer = 200;
    }
}

void KartCrash::ClearMatchlessTimer() {
    KartBody* body = mBody;
    RaceMgr* mgr = RaceMgr::getCurrentManager();
    mMatchlessTimer = 0;
    mgr->getKartDrawer(body->mMynum)->flashOff();
}

void KartCrash::DoDecBalloon() {
    KartBody* body = mBody;
    KartChecker* kartChecker = RCMGetKartChecker(body->mMynum);
    RaceMgr* mgr = RaceMgr::getCurrentManager();

    if (mgr->getRaceMode() == BALLOON_BATTLE && kartChecker->decBalloon()) {
        GetKartCtrl()->getKartSound(body->mMynum)->DoKartsetSeSound(0x40052);
        SetMatchlessTimer();
    }
}

void KartCrash::DoPointBomb(ItemObj *itemObj) {
    KartBody* body = mBody;
    if (RaceMgr::getCurrentManager()->getRaceMode() != BOMB_BATTLE || itemObj->getKind() != 8) {
        return;
    }
    RCMGetKartChecker(body->mMynum)->setBEHitBomb(itemObj);
}

void KartCrash::DoDecPointBomb() {
    KartBody* body = mBody;
    RaceMgr* mgr = RaceMgr::getCurrentManager();
    KartChecker* kartChecker = RCMGetKartChecker(body->mMynum);

    if (mgr->getRaceMode() == BOMB_BATTLE) {
        kartChecker->setBEJugem();
    }
}

void KartCrash::SaveDir() {
    KartBody* body = mBody;
    JGeometry::TVec3f  stack_38;
    JGeometry::TVec3f  stack_2C;
    JGeometry::TVec3f  stack_20;
    JGeometry::TVec3f  stack_14;
    JGeometry::TVec3f  stack_8;

    stack_2C.set(body->_308);

    if (body->_2fc.y < 0.0f) {
        stack_2C.scale(-1.0f);
    }

    stack_38.set(0.0f, 1.0f, 0.0f);
    stack_20.cross(stack_38, stack_2C);
    stack_20.normalize();
    stack_14.cross(stack_2C, stack_20);
    stack_14.normalize();
    stack_2C.cross(stack_20, stack_14);
    stack_2C.normalize();
    body->_338.set(stack_2C);

}

void KartCrash::NonRescue() {
    KartBody* body = mBody;
    CrsArea area;
    area.searchInvValley(body->mPos);

    if (area.isInside()) {
        body->mCarStatus |= 0x40000000;
    }
}

void KartCrash::DoRollCrashCrl() {
    KartBody* body = mBody;
    GetKartCtrl()->getKartSound(body->mMynum)->DoRollCrashSound();

    switch (body->_588) {
        case 1:
            break;
        case 0:
            body->_594++;

            if (body->_594 < 80 && body->_594 > 20 && body->getTouchNum() >= 3) {
                body->_588 = 1;
            } else if (body->_594 >= 80) {
                _D = 1;
            }
            GetKartCtrl()->ChaseFnumber(&body->_4b4, body->_4bc, 0.1f);
            if (body->_584 == 3) {
                if (body->_594 <= 20) {
                    body->mWg.scale(0.65f);
                    body->mWg.z += body->_4b4;
                    body->mWg.x  += body->_4b4 * body->_4b8;
                }
            }
            else if (body->_594 <= 20) {
                    body->mWg.scale(0.65f);
                    body->mWg.z -= body->_4b4;
                    body->mWg.x  += body->_4b4 * body->_4b8;
            }
        break;
    }
}

void KartCrash::DoPitchCrashCrl() {
    KartBody* body = mBody;
    GetKartCtrl()->getKartSound(body->mMynum)->DoRollCrashSound();

    switch (body->_588) {
        case 1:
            break;
        case 0:
            body->_594++;

            if (body->_594 < 80 && body->_594 > 20 && body->getTouchNum() >= 3) {
                body->_588 = 1;
            } else if (body->_594 >= 80) {
                body->_588 = 1;
            }
            GetKartCtrl()->ChaseFnumber(&body->_4b4, body->_4bc, 0.1f);
            if (body->_594 <= 20) {
                    body->mWg.scale(0.4f);
                    body->mWg.x += body->_4b4;
                    body->mWg.z  += body->_4b4 * body->_4b8;
            }
        break;
    }
}

void KartCrash::MakeBombCrash(ItemObj *itemObj) {
    KartBody* body = mBody;
    u32 num = body->mMynum;
    if ((body->mCarStatus & 0x2280100c00000) == 0 && (body->getDossin()->_14 & 3) == 0) {
        body->_584 = 6;
        body->_588 = 0;
        body->mCarStatus &= 0xfffffff7fed7ffff;
        body->getTumble()->_0[4] = 0;
        body->mVel.zero();
        body->_594 = 0;
        f32 val = 600.0f * body->_3a4;
        body->_2cc.y += val;
        body->_4bc = 0.174444f;
        NonRescue();
        body->mCarStatus |= 0x900000;
        SaveDir();
        body->getGame()->MakeClear();
        body->getDamage()->SetBigDamageAnime();
        body->getItem()->FallItem();
        body->getStrat()->DoMotor(MotorManager::MotorType_12);
        body->getGame()->ItemWatchMan(itemObj);
        GetKartCtrl()->getKartSound(num)->DoShootVoice();
        GetKartCtrl()->getKartSound(num)->DoRollCrashStartSound();
        SetMatchlessTimer();
        DoPointBomb(itemObj);
        JPEffectPerformer::setEffect(JPEffectPerformer::Effect_UnknownF, num, body->mPos, 1);
    }
}

void KartCrash::DoBombCrashCrl() {
    KartBody* body = mBody;
    GetKartCtrl()->getKartSound(body->mMynum)->DoRollCrashSound();
    switch (body->_588) {
        case 1:
            break;
        case 0:
            body->_594++;
            if (body->_594 < 6) {
                body->mWg.x  += 0.314f;
                GetKartCtrl()->ChaseFnumber(&body->_4bc, -0.209333f, 0.098f);
                body->mWg.y += body->_4bc;
            }
            if (body->_594 > 100) {
                body->_588 = 1;
            }
        break;
    }
}

void KartCrash::MakeKameCrash(ItemObj *itemObj) {
    KartBody* body = mBody;
    u32 num = body->mMynum;

    if (body->getChecker()->CheckCrash() == false) {
        body->getGame()->DoRollThrow();
        body->_584 = 5;
        body->_588 = 0;
        body->_4b8 = 0.2f * (body->mVel.dot(body->_308) / 180.0f);
        body->_594 = 0;
        body->_4b4 = 0.0f;
        body->_4bc = 0.610555f;
        NonRescue();
        body->mCarStatus |= 0x300000;
        SaveDir();
        body->getGame()->MakeClear();
        body->getDamage()->SetDamageAnime();
        body->getItem()->FallItem();
        GetKartCtrl()->getKartSound(num)->DoRollCrashStartSound();
        body->getStrat()->DoMotor(MotorManager::MotorType_10);
        body->getGame()->ItemWatchMan(itemObj);
        GetKartCtrl()->getKartSound(body->mMynum)->DoRollCrashVoice();
        JPEffectPerformer::setEffect(JPEffectPerformer::Effect_Unknown0, num, body->mPos, 1);
        SetMatchlessTimer();
    }
}

void KartCrash::MakeTornado() {
    KartBody* body = mBody;
    u32 num = body->mMynum;

    if (body->getChecker()->CheckCrash() == false) {
        GeographyObj* obj = GetGeoObjMgr()->getKartReactHitObjectList(body->mMynum)[16];
        _4 = obj->mPos;

    }
}

void KartCrash::DoTornadeCenter() {}

void KartCrash::DotornadeCrashCrl() {}

void KartCrash::MakeSpin(ItemObj *) {}

void KartCrash::DoSpinCrashCrl() {}

void KartCrash::MakeHalfSpin(ItemObj *) {}

void KartCrash::DoHalfSpinCrashCrl() {}

void KartCrash::MakeThunderSpin() {}

void KartCrash::MakeBurn(ItemObj *) { 
    // void ItemFireBall::IsEfctTypeRed() const {}
    // void ItemObj::getItemColorID() const {}
}

void KartCrash::MakeFreezeCrash() {}

void KartCrash::DoFreezeCrashCrl() {}
