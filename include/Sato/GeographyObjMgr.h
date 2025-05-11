#ifndef GEOGRAPHYOBJMGR_H
#define GEOGRAPHYOBJMGR_H

#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Sato/GeographyObj.h"
#include "Shiraiwa/Balloon.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager : JKRDisposer
{
public:
    GeographyObjManager(const CrsData &);      // 0x80222b4c

    void calc();                               // 0x80222b4c
    void createModel(JKRSolidHeap *, u32);     // 0x802250b4
    void createModel_norm(const JSUListIterator<GeographyObj>&, GeographyObj**, JKRSolidHeap*, u32); // 0x802250b4
    void createModel_shadow(GeographyObj*, GeographyObj**, JKRSolidHeap*, u32);                      // 0x80225444
    void reset(const CrsData &);
    void drawGXObj(u32 cameraNo);
    void drawPrim(u32 cameraNo);

    void update();
    void drawSimpleModel(u32 cameraNo);
    void setCurrentViewNo(u32 cameraNo);
    void viewCalc(u32 cameraNo);

    void update_groundObj();
    void drawSimpleModel_groundObj(u32 cameraNo);    
    void setCurrentViewNo_groundObj(u32 cameraNo);
    void viewCalc_groundObj(u32 cameraNo);

    void update_foreground();
    void drawSimpleModel_foreground(u32 cameraNo);    
    void setCurrentViewNo_foreground(u32 cameraNo);
    void viewCalc_foreground(u32 cameraNo);

    void setKartHitItemBoxTrigger(const int kart_index);

    TJugem *getJugem(int camera_index) const;
    TBalloonManager *getBalloonMgr(int kart_index) const;

    GeographyObj *createSubObj(u32 id);
    GeographyObj *createPrevDrawSubObj(u32 id);

    void Collide_norm(const JSUListIterator<GeographyObj>&, const int&, const JGeometry::TVec3f&, const float&);

    void loadAnimation(GeographyObj*, GeographyObj**);
    void registerAnimation(GeographyObj*);
    void makeSharedDL(GeographyObj*, GeographyObj**);
    void makeShadowSharedDL(GeographyObj*, GeographyObj**);

    f32 getKartHitDepthNormalObj(int kart_index) const { return mHitDepth[kart_index]; }
    static void createMgr(const CrsData &crsData) { gMgr = new GeographyObjManager(crsData); }
    static GeographyObjManager *getGeographyObjManager() { return gMgr; }

    bool isMiniGame() const { return mIsBalloonBattle || mIsBombBattle || mIsRobberyBattle || mIsEscapeBattle; }
    bool isRobberBattle() const { return mIsRobberyBattle; }
    bool isBalloonBattle() const { return mIsBalloonBattle; }
    bool isEscapeBattle() const { return mIsEscapeBattle; }
    bool isBombBattle() const { return mIsBombBattle; }

    static GeographyObjManager *gMgr;

    u8 _18[0x2A8 - 0x18];   //
    f32 mHitDepth[8];       // 2A8
    u8 _328[0x3FC - 0x2C8]; //
    TJugem *mJugem[4];      // 3FC
    u8 _40C[0x491 - 0x40C]; //
    bool mIsBalloonBattle;  // 491
    bool mIsBombBattle;     // 492
    bool mIsRobberyBattle;  // 493
    bool mIsEscapeBattle;   // 494
    u8 _495[0x49C - 0x495]; //
    void* _49C;
    void* _4A0;
    // u8 _495[0x4A4 - 0x495]; //

}; // size 0x4A4

inline GeographyObjManager *GetGeoObjMgr() { return GeographyObjManager::getGeographyObjManager(); };

inline void CreateGeoObjMgr(const CrsData *crsData) { GeographyObjManager::createMgr(*crsData); }

#endif
