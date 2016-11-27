/*
 * ifs2Qt.h
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#ifndef IFS2QT_H
#define IFS2QT_H

#include <generictypes.h>

/**********************************************************************************************************************
 * FUNCTION		: -
 * CLASS		: Ifs2Qt
 * TYPE			: -
 * IN			: -
 * OUT			: -
 * PROCESS		: Definition of Ifs2Qt class
 *********************************************************************************************************************/
class Ifs2Qt
{
public:
    Ifs2Qt();
    void copyFrameInto(IFSIMG w1, IFSIMG w2, int frameNumber);
    QImage* ifs3d2qt3d(IFSIMG im);
    QImage* ifs2Qt(IFSIMG img,int ComplexMode,int FrameNumber);
    void normimage(IFSIMG im, QImage *img1);
};
#endif // IFS2QT_H
//Nothing past this point
