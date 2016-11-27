/*
 * subClassQApp.cpp
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#include "subclassqapp.h"
#include "ifstool.h"

//EXTERN VARIABLES
extern Ifstool *gIfstool;

/**********************************************************************************************************************
 * FUNCTION		: SubClassQApp
 * CLASS		: SubClassQApp
 * TYPE			: Constructor
 * IN			: argv, argc
 * OUT			: None
 * PROCESS		: None
 *********************************************************************************************************************/

SubClassQApp::SubClassQApp(int &argc, char *argv[]) : QApplication(argc, argv) {

}

/**********************************************************************************************************************
 * FUNCTION		: event
 * CLASS		: SubClassQApp
 * TYPE			: Member Function
 * IN			: event
 * OUT			: None
 * PROCESS		: Handle file open even specially
 *********************************************************************************************************************/
bool SubClassQApp::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::FileOpen:
        //  Here we call the FileOpen menu handler functionality
        gIfstool->handleFileByPath(static_cast<QFileOpenEvent *>(event)->file(),0);
        return true;
    default:
        return QApplication::event(event);
    }
}

//Nothing past this point
