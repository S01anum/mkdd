#include "Kameda/Demo2D.h"
#include "Kameda/J2DManager.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/system.h"

#include <JSystem/JAudio/JASFakeMatch2.h>

Demo2DParam::Demo2DParam() : _48(0), _4A(1) {}

Demo2D::Demo2D(JKRHeap *heap) {
    mNode = new (heap, 0) Demo2DHioNode();
    mOrtho = System::getJ2DOrtho();
    mScreen = new (heap, 0) J2DScreen();

    mScreen->set("Demo.blo", 0x0040000, GETJ2DManager()->getArchive());
    init();
}

void Demo2D::init() {}

Demo2D::~Demo2D() {
    delete mNode;
    delete mScreen;
}

void Demo2D::draw() {
    if ((mNode->mParam._4A & 1) && RCMGetManager()->isWaitDemoMode()) {
        mOrtho->setPort();

        mScreen->draw(0.0f, 0.0f, mOrtho);
    }
}

void Demo2D::calc() {
    // stub?
    if (mNode->mParam._4A & 1)
        return;
}
