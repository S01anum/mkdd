#include "Kameda/J2DManager.h"
#include "kartEnums.h"
#include "mathHelper.h"

char *J2DManager::mKartNumberName[8] = {
    "MarioFont_128.1.bti",
    "MarioFont_128.2.bti",
    "MarioFont_128.3.bti",
    "MarioFont_128.4.bti",
    "MarioFont_128.5.bti",
    "MarioFont_128.6.bti",
    "MarioFont_128.7.bti",
    "MarioFont_128.8.bti",
};

int J2DManager::mStatus2Kart[8];
int J2DManager::mKart2Status[8];

J2DManager *J2DManager::mThis;
bool J2DManager::mNetFlag;

J2DManager::J2DManager(JKRHeap *) {}

void J2DManager::reset() {}

void J2DManager::draw() {}

void J2DManager::calc() {}

void J2DManager::calcParticle() {}

int J2DManager::getKart2Status(int) {}

int J2DManager::getStatus2Kart(int) {}

void J2DManager::setStatus2Kart(int, int) {}

void J2DManager::setUsisi(int, ECharID) {}

void J2DManager::startPreRace() {}

int J2DManager::getPreRaceStatus() {}

int J2DManager::getPreRaceWipeType() {}

void J2DManager::setRace2DDrawFlag(bool) {}

void J2DManager::hideRace2D() {}

void J2DManager::getHideFrameRace2D() {}

bool J2DManager::isHideRace2D() {}

bool J2DManager::isRaceEnd() {}

bool J2DManager::isGoalAnmEnd(int) {}

void J2DManager::getGoalAnmFrame(int) {}

bool J2DManager::isAlarm(int) {}

void J2DManager::startLANNumAnm() {}

void J2DManager::startLANNumResultAnm() {}

void J2DManager::startLANResultAnm(int) {}

void J2DManager::startBombDemo() {}

void J2DManager::endBombDemo() {}

bool J2DManager::isLANDemo() {}

bool J2DManager::isStartGo321() {}

#include "JSystem/JAudio/JASFakeMatch2.h"
