/*
 * ifstool.h
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#ifndef IFSTOOL_H
#define IFSTOOL_H

#include <generictypes.h>
#include "ifstoolMainWindow.h"

/**********************************************************************************************************************
 * FUNCTION		: -
 * CLASS		: Ifstool
 * TYPE			: -
 * IN			: -
 * OUT			: -
 * PROCESS		: Definition of Ifstool class
 *********************************************************************************************************************/
class Ifstool {
public:
    IFSIMG images[NUMBER_OF_IMGAES];
    QString imagenames[NUMBER_OF_IMGAES];// image names, so they can be displayed later
    Ifstool();
    ~Ifstool();
    void handleFileByPath (QString fileName,int);
    IfstoolMainWindow* mainWindow;
protected:

private:

};

#endif // IFSTOOL_H
//Nothing past this point
