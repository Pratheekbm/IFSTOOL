/*
 * main.cpp
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 *
 * Coding guildlines followed: http://geosoft.no/development/cppstyle.html
 */

#include <QApplication>
#include <iostream>
#include <stdio.h>
#include "main.h"
#include "subclassqapp.h"
#include "ifstool.h"
#include "ifstoolMainWindow.h"

//GLOBAL DECLARATION
Ifstool *gIfstool;
int IfsToolFrameNumber;
QString VERSION = "2.1";
/**********************************************************************************************************************
 * FUNCTION		: main
 * CLASS		: None
 * TYPE			: main function
 * IN			: argv, argc
 * OUT			: None
 * PROCESS		: Starting point of the application
 *********************************************************************************************************************/
int main (int argc, char *argv[]) {

    // argv is supposed to have double clicked file name
    // But for mac, it doesnt contain any, so we have to subclass the Qapp and handle it differently

    SubClassQApp app(argc, argv);
    app.setOrganizationName("Ifstool");
    app.setApplicationName("Ifstool");
    gIfstool = new Ifstool;

    return app.exec();
}
//Nothing past this point
