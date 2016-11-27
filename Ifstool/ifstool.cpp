/*
 * ifstool.cpp
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#include "ifstool.h"

//EXTERN VARIABLES
extern Ifstool *gIfstool;

/**********************************************************************************************************************
 * FUNCTION     : Ifstool
 * CLASS        : Ifstool
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : Initializes variables including main window
 *********************************************************************************************************************/
Ifstool::Ifstool() {
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        images[i] = 0;
    }

    mainWindow = new IfstoolMainWindow;
    //mainWindow->setWindowState(Qt::WindowMaximized);
    mainWindow->show();
}

/**********************************************************************************************************************
 * FUNCTION     : Ifstool
 * CLASS        : Ifstool
 * TYPE         : Destructor
 * IN           : None
 * OUT          : None
 * PROCESS      : Deallocates memory if needed
 *********************************************************************************************************************/
Ifstool::~Ifstool() {

}

/**********************************************************************************************************************
 * FUNCTION     : handleFileByPath
 * CLASS        : Ifstool
 * TYPE         : handleFileByPath
 * IN           : fileName
 * OUT          : None
 * PROCESS      : Opens the file based on incoming filename
 *********************************************************************************************************************/
void Ifstool::handleFileByPath(QString fileName,int bufferNumber) {
    mainWindow->handleFileByPath(fileName,bufferNumber);
}
//Nothing past this point
