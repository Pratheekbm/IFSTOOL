/*
 * ifstoolMainWindow.h
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#ifndef IFSTOOLMAINWINDOW_H
#define IFSTOOLMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMutex>

#include "generictypes.h"
#include "ifs2qt.h"
#include "ifstoolChildWindow.h"
#include "ifsOp.h"
class QAction;
class QMenu;

typedef enum _RADIO_GROUP {
    INPUT1 = 0,
    INPUT2 = 1,
    OUTPUT = 3
} RADIO_GROUP;

typedef enum _UPDATE_TYPE {
    UNSET = 0,
    SET = 1
} UPDATE_TYPE;

typedef enum _BUFFER_RELATION {
    IN_EMPYT = -1,
    NONE,
    IN1_OUT,
    IN2_OUT,
    IN1_IN2_OUT
} BUFFER_RELATION;

typedef enum _OP_TYPE {
    REAL_OP = 0,
    COMPLEX_OP
}OP_TYPE;

/**********************************************************************************************************************
 * FUNCTION     : -
 * CLASS        : IfstoolMainWindow
 * TYPE         : QMainWindow
 * IN           : -
 * OUT          : -
 * PROCESS      : Definition of IfstoolMainWindow class
 *********************************************************************************************************************/
class IfstoolMainWindow: public QMainWindow {
    Q_OBJECT

public:
    Ifs2Qt ifsToQt;
    IfsOp *ifsOP;
    IfstoolMainWindow();
    void addConsoleOutput(QString);
    void handleFileByPath(QString fileName, int);
    void updateChildPtr(const IfstoolChildWindow*, UPDATE_TYPE);
    int getIndexFromChildPtr(void *child);
    void RunScript();


protected:
    void closeEvent(QCloseEvent *event)
    Q_DECL_OVERRIDE;

private slots:
    void open();
    void reopen();
    bool save();
    bool saveAs();
    void runScript();
    void about();
    void view();
    void mode();
    void copy();
    void paste();
    void delete_();
    void mono();
    void increaseBrightness();
    void decreaseBrightness();
    void increaseWarm();
    void decreaseWarm();
    void increaseCool();
    void decreaseCool();
    void dx();
    void dy();
    void dxx();
    void dyy();
    void dxdy();
    void curvature();
    void exponent();
    void inverse();
    void natlog();
    void laplacian();
    void histeq();
    void pseudo();
    void blur();
    void c2mag();
    void c2phase();
    void c2real();
    void c2imag();
    void fft();
    void ifft();
    void add();
    void sub();
    void mul();
    void div();

    void updateAccessability();
    void rerunScript();

private:
    void addCommandButtons();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void changeDisplayAccess(bool);
    void changeUnaryAccess(bool);
    void changeBinaryAccess(bool);
    int getRadioButtonStatus(RADIO_GROUP);
    BUFFER_RELATION getUnaryBuffers(IFSIMG &inImg, IFSIMG &outImg, int &outBufferNumber,OP_TYPE);
    BUFFER_RELATION getBinaryBuffers(IFSIMG &inImg, IFSIMG &inImg2,IFSIMG &outImg, int &outBufferNumber);

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *displayMenu;
    QMenu *unaryMenu;
    QMenu *binaryMenu;
    QMenu *helpMenu;
    QMenu *complexMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *openAct;
    QAction *reopenAct;
    QAction *aboutAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *runScriptAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *deleteAct;

    QAction *viewAct;
    QAction *modeAct;
    QAction *reRunAct;
    QAction *monoAct;
    QAction *increaseBrightnessAct;
    QAction *decreaseBrightnessAct;
    QAction *increaseWarmAct;
    QAction *decreaseWarmAct;
    QAction *increaseCoolAct;
    QAction *decreaseCoolAct;
    QAction *dxAct;
    QAction *dyAct;
    QAction *dxxAct;
    QAction *dyyAct;
    QAction *dxdyAct;
    QAction *curvatureAct;
    QAction *exponentAct;
    QAction *inverseAct;
    QAction *natlogAct;
    QAction *laplacianAct;
    QAction *histAct;
    QAction *pseudoAct;
    QAction *addAct;
    QAction *subAct;
    QAction *mulAct;
    QAction *divAct;

    QAction *c2magAct;
    QAction *c2phaseAct;
    QAction *c2realAct;
    QAction *c2imagAct;
    QAction *fftAct;
    QAction *ifftAct;

    //Layout variables
    QGroupBox *mainLayoutGrpBox;
    QVBoxLayout *mainHLayout;
    QPlainTextEdit *console;
    QGroupBox *controlPanelGrpBox;
    QHBoxLayout *controlPanelHLayout;
    QGroupBox *commandSelectGrpBox;
    QHBoxLayout *commandSelectHLayout;
    QGroupBox *unaryGrpBox;
    QGridLayout *unaryGridLayout;
    QGroupBox *binaryGrpBox;
    QVBoxLayout *binaryVLayout;
    QGroupBox *imageSelectGrpBox;
    QVBoxLayout *imageSelectVLayout;
    QGroupBox *input1GrpBox;
    QGroupBox *input2GrpBox;
    QGroupBox *outputGrpBox;
    QHBoxLayout *input1HLayout;
    QHBoxLayout *input2HLayout;
    QHBoxLayout *outputHLayout;
    QRadioButton *input1RButtons[NUMBER_OF_IMGAES];
    QRadioButton *input2RButtons[NUMBER_OF_IMGAES];
    QRadioButton *outputRButtons[NUMBER_OF_IMGAES];
    IfstoolChildWindow *childWindow[NUMBER_OF_IMGAES];
    QMutex *childPtrMutex;

    QPushButton *dxButton;
    QPushButton *dyButton;
    QPushButton *d2xButton;
    QPushButton *d2yButton;
    QPushButton *dxdyButton;
    QPushButton *curButton;
    QPushButton *expButton;
    QPushButton *invButton;
    QPushButton *lapButton;
    QPushButton *natlogButton;
    QPushButton *histButton;
    QPushButton *pseudoButton;
    QPushButton *blurButton;

    QPushButton *addButton;
    QPushButton *subButton;
    QPushButton *mulButton;
    QPushButton *divButton;

    IFSIMG copyBuffer;
    char lastpath[256]; //last path opened just the path, not the file
    char lastfile[256]; // path and file, both
    QString name1; // file name without path
    char scriptOutputName[256];// Name of file output from a script
    char scriptInputName[256];// Name of file which is input to a script
    char scriptPath[256];// Path to the script directory
    char scriptName[256];
    bool rerunflag;// if 1, a script is being rerun
};

#endif // IFSTOOLMAINWINDOW_H
//Nothing past this point
