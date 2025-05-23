#ifndef JAUDIO_JAUSOUNDOBJECT_H
#define JAUDIO_JAUSOUNDOBJECT_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JGeometry.h"
#include "types.h"

class JAUSoundObject : public JAISoundHandles
{
public:
    JAUSoundObject();
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual void stopOK(JAISoundHandle &);
    virtual void *startSound(int);
    virtual void *startLevelSound(int);
    
    //void startLevelSoundIndex(int, unsigned char);
    //void startSoundIndex(int, unsigned char);
    //void stopSound(int soundID, u32); // unused, not sure

    bool _C;
    u8 _D[3];
    u32 _10;
    JGeometry::TVec3<f32> *_14;
};

class JAUDopplerSoundObject : public JAUSoundObject
{ 
public:
    virtual void process();
    virtual void *startSound(int soundID);
    virtual void *startLevelSound(int soundID);

    u8 _20[0x30 - 0x20];
};

#endif
