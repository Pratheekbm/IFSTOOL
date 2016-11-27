/*
 * ifsOp.h
 * Created :    May 28 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */
#ifndef IFSOP_H
#define IFSOP_H
#include "generictypes.h"

typedef enum _OPERATION {
    OPNONE = 0,
    OPDX,
    OPDXX,
    OPDY,
    OPDYY,
    OPDXY,
    OPCUR,
    OPEXP,
    OPINV,
    OPLAP,
    OPHISTEQ,
    OPNATLOG,
    OPPSEUDO,
    OPBLUR,
	OPADD,
	OPSUB,
	OPMUL,
    OPDIV,
    OPC2MAG,
    OPC2PHASE,
    OPC2REAL,
    OPC2IMAG,
    OPFFT,
    OPIFFT,
    OPR2C,
    OPEXTRACTF
} OPERATION;
#include <QMessageBox>
/**********************************************************************************************************************
 * FUNCTION     : -
 * CLASS        : IfsOp
 * TYPE         : -
 * IN           : -
 * OUT          : -
 * PROCESS      : Definition of IfsOp class
 *********************************************************************************************************************/
class IfsOp {
public:
    IfsOp(QWidget *parent);
    void unary(int opcode, IFSIMG &inImg, IFSIMG &outImg);
	void binary(int opcode, IFSIMG &inImg1, IFSIMG &inImg2, IFSIMG &outImg);

private:
    QWidget *parent;
};

#endif // IFSOP_H
//Nothing past this point
