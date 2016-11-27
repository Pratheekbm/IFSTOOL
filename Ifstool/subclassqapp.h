/*
 * subClassQApp.h
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#ifndef SUBCLASSQAPP_H
#define SUBCLASSQAPP_H

#include <QApplication>

class SubClassQApp : public QApplication
{
public:
    SubClassQApp(int &argc, char *argv[]);
protected:
    bool event(QEvent *);
};

#endif // SUBCLASSQAPP_H
//Nothing past this point
