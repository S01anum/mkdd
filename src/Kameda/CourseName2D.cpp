#include "Kameda/CourseName2D.h"

CourseName2D *CourseName2D::mCourseName2D;

CourseName2D::CourseName2D(JKRArchive *archive) {}

CourseName2D::~CourseName2D() {
    if (mCourseName2D)
        mCourseName2D = nullptr;
}

void *CourseName2D::getCourseNameTexture(ECourseID courseID) {}

#include "JSystem/JAudio/JASFakeMatch2.h"
