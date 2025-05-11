#include "Sato/GeographyObjMgr.h"

void GeographyObjManager::createModel_norm(const JSUListIterator<GeographyObj>& param_1, GeographyObj** param_2, JKRSolidHeap* param_3, u32 param_4)
{
    GeographyObj *pGVar1;
    GeographyObj *pGVar2;
    
    pGVar2 = param_1.getObject();
    loadAnimation(pGVar2,param_2);
    pGVar1 = param_1.getObject();
    pGVar1->createModel(param_3,param_4);
    registerAnimation(pGVar2);
    pGVar2->configAnimationMode();
    makeSharedDL(pGVar2,param_2);
    createModel_shadow(param_1.getObject(),param_2,param_3,param_4);
    pGVar2->conditionallySetSomeFlag();
    return;
}
void GeographyObjManager::createModel_shadow(GeographyObj* obj, GeographyObj** param_2, JKRSolidHeap* heap, u32 viewNo)
{
    ShadowModel *pSVar1;
  
    SYSDBG_SetHeapGroup("OBJECT SHADOW MODEL", nullptr);
    obj->createShadowModel(heap, viewNo);
    makeShadowSharedDL(obj,param_2);
    SYSDBG_SetHeapGroup("OBJECT MODEL", nullptr);
    obj->lockMdl();
    if (obj->getShadowModel()) {
        obj->getShadowModel()->getModel()->lock();
    }
    if (!obj->tstObjFlagShadow()) {
        return;
    }
    if (ShadowModel *shadowModel = obj->getShadowModel()) {
        shadowModel->setSomeFlag();
    }
}

void GeographyObjManager::Collide_norm(const JSUListIterator<GeographyObj>&, const int&, const JGeometry::TVec3f&, const float&)
{
    
}
