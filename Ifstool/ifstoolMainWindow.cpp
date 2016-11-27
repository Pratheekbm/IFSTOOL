/*
 * ifstoolMainWindow.cpp
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */

#include "ifstoolMainWindow.h"
#include "ifstool.h"
#include <strings.h>

//EXTERN VARIABLES
extern Ifstool *gIfstool;

/**********************************************************************************************************************
 * FUNCTION     : IfstoolMainWindow
 * CLASS        : IfstoolMainWindow
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : Initializes stuff
 *********************************************************************************************************************/
IfstoolMainWindow::IfstoolMainWindow() {

    controlPanelGrpBox = new QGroupBox(tr("Control Panel Version 2.1"));
    controlPanelHLayout = new QHBoxLayout;

    imageSelectGrpBox = new QGroupBox(tr("Image Select"));
    imageSelectVLayout = new QVBoxLayout;

    input1GrpBox = new QGroupBox("Input 1");
    input1HLayout = new QHBoxLayout;
    input1GrpBox->setLayout(input1HLayout);
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        input1RButtons[i] = new QRadioButton(tr("%1").arg(i));
        input1RButtons[i]->setAutoExclusive(true);
        if (0 == i) {
            input1RButtons[i]->setChecked(true);
        } else {
            input1RButtons[i]->setChecked(false);
        }
        connect(input1RButtons[i], SIGNAL(clicked()), this, SLOT(updateAccessability()));
        input1HLayout->addWidget(input1RButtons[i]);
    }

    input2GrpBox = new QGroupBox("Input 2");
    input2HLayout = new QHBoxLayout;
    input2GrpBox->setLayout(input2HLayout);
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        input2RButtons[i] = new QRadioButton(tr("%1").arg(i));
        input2RButtons[i]->setAutoExclusive(true);
        if (0 == i) {
            input2RButtons[i]->setChecked(true);
        } else {
            input2RButtons[i]->setChecked(false);
        }
        connect(input2RButtons[i], SIGNAL(clicked()), this, SLOT(updateAccessability()));
        input2HLayout->addWidget(input2RButtons[i]);
    }

    outputGrpBox = new QGroupBox("Output");
    outputHLayout = new QHBoxLayout;
    outputGrpBox->setLayout(outputHLayout);
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        outputRButtons[i] = new QRadioButton(tr("%1").arg(i));
        outputRButtons[i]->setAutoExclusive(true);
        if (0 == i) {
            outputRButtons[i]->setChecked(true);
        } else {
            outputRButtons[i]->setChecked(false);
        }
        connect(outputRButtons[i], SIGNAL(clicked()), this, SLOT(updateAccessability()));
        outputHLayout->addWidget(outputRButtons[i]);
    }

    imageSelectVLayout->addWidget(input1GrpBox);
    imageSelectVLayout->addWidget(input2GrpBox);
    imageSelectVLayout->addWidget(outputGrpBox);

    imageSelectGrpBox->setLayout(imageSelectVLayout);

    commandSelectGrpBox = new QGroupBox(tr("Command"));
    commandSelectHLayout = new QHBoxLayout;

    unaryGrpBox = new QGroupBox(tr("Unary"));
    unaryGridLayout = new QGridLayout;
    unaryGrpBox->setLayout(unaryGridLayout);
    binaryGrpBox = new QGroupBox(tr("Binary"));
    binaryVLayout = new QVBoxLayout;
    binaryGrpBox->setLayout(binaryVLayout);

    addCommandButtons();
    commandSelectHLayout->addWidget(unaryGrpBox);
    commandSelectHLayout->addWidget(binaryGrpBox);
    commandSelectGrpBox->setLayout(commandSelectHLayout);
//    commandSelectHLayout->removeWidget(unaryGrpBox);
//    commandSelectHLayout->removeWidget(binaryGrpBox);
//    commandSelectHLayout->addWidget(binaryGrpBox);
//    commandSelectGrpBox->setLayout(commandSelectHLayout);

    controlPanelHLayout->addWidget(imageSelectGrpBox);
    controlPanelHLayout->addWidget(commandSelectGrpBox);
    controlPanelGrpBox->setLayout(controlPanelHLayout);

    console = new QPlainTextEdit(">> Console");
    console->setReadOnly(true);

    mainLayoutGrpBox = new QGroupBox;
    mainHLayout = new QVBoxLayout;
    mainHLayout->addWidget(controlPanelGrpBox);
    mainHLayout->addWidget(console);
    mainLayoutGrpBox->setLayout(mainHLayout);
    this->setCentralWidget(mainLayoutGrpBox);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();
    changeDisplayAccess(false);
    changeUnaryAccess(false);
    changeBinaryAccess(false);

    childPtrMutex = new QMutex;
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        childWindow[i] = NULL;
    }

    ifsOP = new IfsOp(this);
    lastpath[0] = 0; // haven't opened any files yet
    lastfile[0] = 0; // haven't opened any files yet
    copyBuffer = 0;
}

/**********************************************************************************************************************
 * FUNCTION     : closeEvent
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Handles close event
 *********************************************************************************************************************/
void IfstoolMainWindow::closeEvent(QCloseEvent *event) {
    event->accept();

}

/**********************************************************************************************************************
 * FUNCTION     : addCommandButtons
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Handles creation command buttons
 *********************************************************************************************************************/
void IfstoolMainWindow::addCommandButtons() {
    dxButton = new QPushButton;
    dxButton->setText("DX");
    dxButton->setToolTip("First deriv wrt to x using convolution with derivative of a Gaussian");
    connect(dxButton, SIGNAL(released()), this, SLOT(dx()));
    unaryGridLayout->addWidget(dxButton,0,1);


    dyButton = new QPushButton;
    dyButton->setText("DY");
    dyButton->setToolTip("First deriv wrt to y using convolution with derivative of a Gaussian");
    connect(dyButton, SIGNAL(released()), this, SLOT(dy()));
    unaryGridLayout->addWidget(dyButton,1,1);

    d2xButton = new QPushButton;
    d2xButton->setText("DXX");
    d2xButton->setToolTip("Second deriv wrt to x using convolution with derivative of a Gaussian");
    connect(d2xButton, SIGNAL(released()), this, SLOT(dxx()));
    unaryGridLayout->addWidget(d2xButton,2,1);

    d2yButton = new QPushButton;
    d2yButton->setText("DYY");
    d2yButton->setToolTip("Second deriv wrt to y using convolution with derivative of a Gaussian");
    connect(d2yButton, SIGNAL(released()), this, SLOT(dyy()));
    unaryGridLayout->addWidget(d2yButton,3,1);

    dxdyButton = new QPushButton;
    dxdyButton->setText("DXY");
    dxdyButton->setToolTip("second deriv wrt to xy using convolution with derivative of a Gaussian");
    connect(dxdyButton, SIGNAL(released()), this, SLOT(dxdy()));
    unaryGridLayout->addWidget(dxdyButton,4,1);

    natlogButton = new QPushButton;
    natlogButton->setText("Ln");
    connect(natlogButton, SIGNAL(released()), this, SLOT(natlog()));
    unaryGridLayout->addWidget(natlogButton,5,1);

    expButton = new QPushButton;
    expButton->setText("Exp");
    expButton->setToolTip("Expontiate each pixel.");
    connect(expButton, SIGNAL(released()), this, SLOT(exponent()));
    unaryGridLayout->addWidget(expButton,0,2);

    invButton = new QPushButton;
    invButton->setText("Inv");
    invButton->setToolTip("output=max-input");
    connect(invButton, SIGNAL(released()), this, SLOT(inverse()));
    unaryGridLayout->addWidget(invButton,1,2);

    lapButton = new QPushButton;
    lapButton->setText("Lap");
    lapButton->setToolTip("Calculate the Lapacian at each pixel, using derivative of a Gaussian");
    connect(lapButton, SIGNAL(released()), this, SLOT(laplacian()));
    unaryGridLayout->addWidget(lapButton,2,2);

    curButton = new QPushButton;
    curButton->setText("Curv");
    curButton->setToolTip("Calculate the curvature of the level set at each point");
    connect(curButton, SIGNAL(released()), this, SLOT(curvature()));
    unaryGridLayout->addWidget(curButton,3,2);

    histButton = new QPushButton;
    histButton->setText("Histeq");
    histButton->setToolTip("Histogram equalization");
    connect(histButton, SIGNAL(released()), this, SLOT(histeq()));
    unaryGridLayout->addWidget(histButton,4,2);

    pseudoButton = new QPushButton;
    pseudoButton->setText("CMap");
    pseudoButton->setToolTip("Replace each pixel with a color corresponding to brightness\n 0:gray scale\n1:inverse gray scale\n2:Bronson\n3:hot metal\n4:heated specturm\n5:log\n6:random");
    connect(pseudoButton, SIGNAL(released()), this, SLOT(pseudo()));
    unaryGridLayout->addWidget(pseudoButton,5,2);

    blurButton = new QPushButton;
    blurButton->setText("Blur");
    connect(blurButton, SIGNAL(released()), this, SLOT(blur()));
    unaryGridLayout->addWidget(blurButton,6,2);
    blurButton->setToolTip("Blur the image with a Gaussian with user-selectable variance.");

    addButton = new QPushButton;
    addButton->setText("Add");
    connect(addButton, SIGNAL(released()), this, SLOT(add()));
    binaryVLayout->addWidget(addButton);

    subButton = new QPushButton;
    subButton->setText("Sub");
    connect(subButton, SIGNAL(released()), this, SLOT(sub()));
    binaryVLayout->addWidget(subButton);

    mulButton = new QPushButton;
    mulButton->setText("Mul");
    connect(mulButton, SIGNAL(released()), this, SLOT(mul()));
    binaryVLayout->addWidget(mulButton);

    divButton = new QPushButton;
    divButton->setText("Div");
    connect(divButton, SIGNAL(released()), this, SLOT(div()));
    binaryVLayout->addWidget(divButton);

    divButton->setEnabled(false);
}
/**********************************************************************************************************************
 * FUNCTION     : createActions
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Handles creation of actions and connecting actions to corresponding slots
 *********************************************************************************************************************/
void IfstoolMainWindow::createActions() {

    openAct = new QAction(QIcon(":/icons/folder_new.png"), tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    reopenAct = new QAction(QIcon(":/icons/img_reopen.png"), tr("&Reopen"), this);
    reopenAct->setStatusTip(tr("Reopen an existing file"));
    connect(reopenAct, SIGNAL(triggered()), this, SLOT(reopen()));

    saveAsAct = new QAction(QIcon(":/icons/save.png"), tr("Save &As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the image selected as OUTPUT under a new name, including conversion."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    runScriptAct = new QAction(QIcon(":/icons/run.png"), tr("Run Script"), this);
    runScriptAct->setStatusTip(tr("Run a script on an image, read output back"));
    connect(runScriptAct, SIGNAL(triggered()), this, SLOT(runScript()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/icons/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
            "clipboard"));

    copyAct = new QAction(QIcon(":/icons/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
            "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/icons/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into output buffer"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    deleteAct = new QAction(QIcon(":/icons/delete.png"), tr("&Delete"), this);
    deleteAct->setShortcuts(QKeySequence::Delete);
    deleteAct->setStatusTip(tr("Deletes the output buffer"));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(delete_()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    viewAct = new QAction(QIcon(":/icons/view.png"), tr("&View"), this);
    viewAct->setStatusTip(tr("Display the selected output buffer"));
    connect(viewAct, SIGNAL(triggered()), this, SLOT(view()));

    modeAct = new QAction(tr("&Mode 1"), this);
    modeAct->setStatusTip(tr("Does mode things"));
    connect(modeAct, SIGNAL(triggered()), this, SLOT(mode()));

    reRunAct = new QAction(QIcon(":/icons/rerun.png"), tr("&ReRun"), this);
    reRunAct->setStatusTip(tr("Run the last script again."));
    connect(reRunAct, SIGNAL(triggered()), this, SLOT(rerunScript()));
    reRunAct->setEnabled(false);

    monoAct = new QAction(tr("&Mono"), this);
    aboutAct->setStatusTip(tr("Converts image into monochrmatic "));
    connect(monoAct, SIGNAL(triggered()), this, SLOT(mono()));

    increaseBrightnessAct = new QAction(tr("Bright +"), this);
    aboutAct->setStatusTip(tr("Increases the brightness slightly"));
    connect(increaseBrightnessAct, SIGNAL(triggered()), this, SLOT(increaseBrightness()));

    decreaseBrightnessAct = new QAction(tr("Bright -"), this);
    aboutAct->setStatusTip(tr("Decreases the brightness slightly"));
    connect(decreaseBrightnessAct, SIGNAL(triggered()), this, SLOT(decreaseBrightness()));

    increaseWarmAct = new QAction(tr("Warm +"), this);
    aboutAct->setStatusTip(tr("Increases the Warmness slightly"));
    connect(increaseWarmAct, SIGNAL(triggered()), this, SLOT(increaseWarm()));

    decreaseWarmAct = new QAction(tr("Warm +"), this);
    aboutAct->setStatusTip(tr("Decreases the Warmness slightly"));
    connect(decreaseWarmAct, SIGNAL(triggered()), this, SLOT(decreaseWarm()));

    increaseCoolAct = new QAction(tr("Cool +"), this);
    aboutAct->setStatusTip(tr("Increases the Coolness slightly"));
    connect(increaseCoolAct, SIGNAL(triggered()), this, SLOT(increaseCool()));

    decreaseCoolAct = new QAction(tr("Cool +"), this);
    aboutAct->setStatusTip(tr("Decreases the Coolness slightly"));
    connect(decreaseCoolAct, SIGNAL(triggered()), this, SLOT(decreaseCool()));

    dxAct = new QAction(tr("DX"), this);
    aboutAct->setStatusTip(tr("First derivative wrt x of the image"));
    connect(dxAct, SIGNAL(triggered()), this, SLOT(dx()));

    dyAct = new QAction(tr("DY"), this);
    aboutAct->setStatusTip(tr("First derivative wrt y of the image"));
    connect(dyAct, SIGNAL(triggered()), this, SLOT(dy()));

    dxxAct = new QAction(tr("DXX"), this);
    aboutAct->setStatusTip(tr("Second derivative wrt x of the image"));
    connect(dxxAct, SIGNAL(triggered()), this, SLOT(dxx()));

    dyyAct = new QAction(tr("DYY"), this);
    aboutAct->setStatusTip(tr("Second derivative wrt y of the image"));
    connect(dyyAct, SIGNAL(triggered()), this, SLOT(dyy()));

    dxdyAct = new QAction(tr("DXDY"), this);
    aboutAct->setStatusTip(tr("Second derivative wrt x and y of the image"));
    connect(dxdyAct, SIGNAL(triggered()), this, SLOT(dxdy()));

    curvatureAct = new QAction(tr("Curvature"), this);
    aboutAct->setStatusTip(tr("Curvature of the image"));
    connect(curvatureAct, SIGNAL(triggered()), this, SLOT(curvature()));

    exponentAct = new QAction(tr("Exponent"), this);
    aboutAct->setStatusTip(tr("Exponent of the image"));
    connect(exponentAct, SIGNAL(triggered()), this, SLOT(exponent()));

    inverseAct = new QAction(tr("Inverse"), this);
    inverseAct->setStatusTip(tr("Inverse of the image"));
    connect(inverseAct, SIGNAL(triggered()), this, SLOT(inverse()));

    natlogAct = new QAction(tr("nat log"), this);
    natlogAct->setStatusTip(tr("log of the image"));
    connect(natlogAct, SIGNAL(triggered()), this, SLOT(natlog()));

    laplacianAct = new QAction(tr("Laplacian"), this);
    laplacianAct->setStatusTip(tr("Laplacian of the image"));
    connect(laplacianAct, SIGNAL(triggered()), this, SLOT(laplacian()));

    histAct = new QAction(tr("HistEq"), this);
    histAct->setStatusTip(tr("Histogram equalization"));
    connect(histAct, SIGNAL(triggered()), this, SLOT(histeq()));

    pseudoAct = new QAction(tr("Psedo Color"), this);
    pseudoAct->setStatusTip(tr("Pseudo Color"));
    connect(pseudoAct, SIGNAL(triggered()), this, SLOT(pseudo()));

    c2magAct = new QAction(tr("ComlexMag"), this);
    c2magAct->setStatusTip(tr("Complex magnitude"));
    connect(c2magAct, SIGNAL(triggered()), this, SLOT(c2mag()));

    c2phaseAct = new QAction(tr("ComlexPhase"), this);
    c2phaseAct->setStatusTip(tr("Complex phase"));
    connect(c2phaseAct, SIGNAL(triggered()), this, SLOT(c2phase()));

    c2realAct = new QAction(tr("Real"), this);
    c2realAct->setStatusTip(tr("Real"));
    connect(c2realAct, SIGNAL(triggered()), this, SLOT(c2real()));

    c2imagAct = new QAction(tr("Imaginary"), this);
    c2imagAct->setStatusTip(tr("Complex imaginary"));
    connect(c2imagAct, SIGNAL(triggered()), this, SLOT(c2imag()));

    fftAct = new QAction(tr("FFT"), this);
    fftAct->setStatusTip(tr("Fast Fourier Tranform"));
    connect(fftAct, SIGNAL(triggered()), this, SLOT(fft()));

    ifftAct = new QAction(tr("IFFT"), this);
    ifftAct->setStatusTip(tr("Inverse Fast Fourier Tranform"));
    connect(ifftAct, SIGNAL(triggered()), this, SLOT(ifft()));

    addAct = new QAction(tr("Add"), this);
    addAct->setStatusTip(tr("Adds inp1 and inp2 and saves it in output"));
    connect(addAct, SIGNAL(triggered()), this, SLOT(add()));

    subAct = new QAction(tr("Sub"), this);
    aboutAct->setStatusTip(tr("Subtracts inp2 from inp1 and saves it in output"));
    connect(subAct, SIGNAL(triggered()), this, SLOT(sub()));

    mulAct = new QAction(tr("Mul"), this);
    mulAct->setStatusTip(tr("Multiplies inp2 from inp1 and saves it in output"));
    connect(mulAct, SIGNAL(triggered()), this, SLOT(mul()));

    divAct = new QAction(tr("div"), this);
    divAct->setStatusTip(tr("divides inp1 by inp1 and saves it in output"));
    connect(divAct, SIGNAL(triggered()), this, SLOT(div()));

    pasteAct->setEnabled(false);
}

/**********************************************************************************************************************
 * FUNCTION     : createMenus
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Handles creation of Menus
 *********************************************************************************************************************/
void IfstoolMainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(reopenAct);
//    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(runScriptAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(deleteAct);

    displayMenu = menuBar()->addMenu(tr("Display"));
    displayMenu->addAction(viewAct);
    displayMenu->addSeparator();
    displayMenu->addAction(monoAct);
    displayMenu->addSeparator();
    displayMenu->addAction(increaseBrightnessAct);
    displayMenu->addAction(decreaseBrightnessAct);
    displayMenu->addSeparator();
    displayMenu->addAction(increaseWarmAct);
    displayMenu->addAction(decreaseWarmAct);
    displayMenu->addAction(increaseCoolAct);
    displayMenu->addAction(decreaseCoolAct);

    unaryMenu = menuBar()->addMenu(tr("Unary"));
    unaryMenu->addAction(dxAct);
    unaryMenu->addAction(dyAct);
    unaryMenu->addAction(dxxAct);
    unaryMenu->addAction(dyyAct);
    unaryMenu->addAction(dxdyAct);
    unaryMenu->addSeparator();
    unaryMenu->addAction(curvatureAct);
    unaryMenu->addAction(exponentAct);
    unaryMenu->addAction(inverseAct);
    unaryMenu->addAction(laplacianAct);

    binaryMenu = menuBar()->addMenu(tr("Binary"));
    binaryMenu->addAction(addAct);
    binaryMenu->addAction(subAct);
    binaryMenu->addAction(mulAct);
    binaryMenu->addAction(divAct);

    complexMenu = menuBar()->addMenu(tr("Complex"));
    complexMenu->addAction(c2magAct);
    complexMenu->addAction(c2phaseAct);
    complexMenu->addAction(c2realAct);
    complexMenu->addAction(c2imagAct);
    complexMenu->addAction(fftAct);
    complexMenu->addAction(ifftAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

/**********************************************************************************************************************
 * FUNCTION     : createToolBars
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Handles creation of Toolbars
 *********************************************************************************************************************/
void IfstoolMainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(reopenAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar = addToolBar(tr("Edit"));
    fileToolBar->addAction(copyAct);
    fileToolBar->addAction(pasteAct);
    fileToolBar->addAction(reRunAct);
    fileToolBar = addToolBar(tr("Display"));
    fileToolBar->addAction(viewAct);
    fileToolBar->addAction(modeAct);
}

/**********************************************************************************************************************
 * FUNCTION     : createStatusBar
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Handles creation of Statusbar
 *********************************************************************************************************************/
void IfstoolMainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

/**********************************************************************************************************************
 * FUNCTION     : readSettings
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::readSettings() {
    QSettings settings("Ifstool", "Image File System Tool");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

/**********************************************************************************************************************
 * FUNCTION     : writeSettings
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::writeSettings() {
    QSettings settings("QtProject", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

/**********************************************************************************************************************
 * FUNCTION     : changeAccessability
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::updateAccessability() {
    int in1BufferNumber = getRadioButtonStatus(INPUT1);
    int in2BufferNumber = getRadioButtonStatus(INPUT2);
    int outBufferNumber = getRadioButtonStatus(OUTPUT);

    if(0 != gIfstool->images[outBufferNumber]) {
        changeDisplayAccess(true);
    } else {
        changeDisplayAccess(false);
    }

    if(0 != gIfstool->images[in1BufferNumber]) {
        changeUnaryAccess(true);
        if(0 != gIfstool->images[in2BufferNumber]) {
            changeBinaryAccess(true);
        } else {
            changeBinaryAccess(false);
        }
    } else {
        changeUnaryAccess(false);
        changeBinaryAccess(false);
    }
}
/**********************************************************************************************************************
 * FUNCTION     : changeDisplayAccess
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::changeDisplayAccess(bool updateType) {
    cutAct->setEnabled(updateType);
    copyAct->setEnabled(updateType);
    deleteAct->setEnabled(updateType);

    viewAct->setEnabled(updateType);
    monoAct->setEnabled(updateType);
    increaseBrightnessAct->setEnabled(updateType);
    decreaseBrightnessAct->setEnabled(updateType);
    increaseWarmAct->setEnabled(updateType);
    decreaseWarmAct->setEnabled(updateType);
    increaseCoolAct->setEnabled(updateType);
    decreaseCoolAct->setEnabled(updateType);
}

/**********************************************************************************************************************
 * FUNCTION     : changeUnaryAccess
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::changeUnaryAccess(bool updateType) {
    dxButton->setEnabled(updateType);
    dyButton->setEnabled(updateType);
    d2xButton->setEnabled(updateType);
    d2yButton->setEnabled(updateType);
    dxdyButton->setEnabled(updateType);
    curButton->setEnabled(updateType);
    expButton->setEnabled(updateType);
    invButton->setEnabled(updateType);
    lapButton->setEnabled(updateType);
    natlogButton->setEnabled(updateType);
    histButton->setEnabled(updateType);
    pseudoButton->setEnabled(updateType);
    blurButton->setEnabled(updateType);


    dxAct->setEnabled(updateType);
    dyAct->setEnabled(updateType);
    dxxAct->setEnabled(updateType);
    dyyAct->setEnabled(updateType);
    dxdyAct->setEnabled(updateType);
    curvatureAct->setEnabled(updateType);
    exponentAct->setEnabled(updateType);
    inverseAct->setEnabled(updateType);
    natlogAct->setEnabled(updateType);
    laplacianAct->setEnabled(updateType);
    histAct->setEnabled(updateType);
    pseudoAct->setEnabled(updateType);
    c2magAct->setEnabled(updateType);
    c2phaseAct->setEnabled(updateType);
    c2realAct->setEnabled(updateType);
    c2imagAct->setEnabled(updateType);
    fftAct->setEnabled(updateType);
    ifftAct->setEnabled(updateType);
}
/**********************************************************************************************************************
 * FUNCTION     : changeBinaryAccess
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : -
 *********************************************************************************************************************/
void IfstoolMainWindow::changeBinaryAccess(bool updateType) {
    addButton->setEnabled(updateType);
    subButton->setEnabled(updateType);
    mulButton->setEnabled(updateType);
    divButton->setEnabled(updateType);

    addAct->setEnabled(updateType);
    subAct->setEnabled(updateType);
    mulAct->setEnabled(updateType);
    divAct->setEnabled(updateType);
}
/**********************************************************************************************************************
 * FUNCTION     : getRadioButtonStatus
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : RADIO_GROUP
 * OUT          : Buffer number
 * PROCESS      : Return buffer number of the radio button clicked
 *********************************************************************************************************************/
int IfstoolMainWindow::getRadioButtonStatus(RADIO_GROUP grp) {
    int bufferNumber = -1;
    QRadioButton *radioButton = NULL;
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        if (INPUT1 == grp) {
            radioButton = input1RButtons[i];
        } else if (INPUT2 == grp) {
            radioButton = input2RButtons[i];
        } else if (OUTPUT == grp) {
            radioButton = outputRButtons[i];
        }
        if (1 == radioButton->isChecked()) {
            bufferNumber = i;
            break;
        }
    }
    return bufferNumber;
}
/**********************************************************************************************************************
 * FUNCTION     : getBinaryBuffers
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : inImg1, inImg2,outImg, outBufferNumber
 * OUT          : Buffer dependency relation
 * PROCESS      : Assigns inImg, outImg with appropriate buffers, returns buffer dependency relations
 *********************************************************************************************************************/
BUFFER_RELATION IfstoolMainWindow::getBinaryBuffers(IFSIMG &inImg1, IFSIMG &inImg2, IFSIMG &outImg,
        int &outBufferNumber) {
    int *dims = NULL;
    int in1BufferNumber = getRadioButtonStatus(INPUT1);
    int in2BufferNumber = getRadioButtonStatus(INPUT2);
    outBufferNumber = getRadioButtonStatus(OUTPUT);

    if((0 == gIfstool->images[in1BufferNumber]) || (0 == gIfstool->images[in2BufferNumber])) {
        return IN_EMPYT;
    }

    if(NULL != childWindow[outBufferNumber]) {
        delete childWindow[outBufferNumber];
    }

    if ((in1BufferNumber == outBufferNumber) || (in2BufferNumber == outBufferNumber)) {
        dims = ifssiz(gIfstool->images[in1BufferNumber]);
        outImg = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        inImg1 = gIfstool->images[in1BufferNumber];
        inImg2 = gIfstool->images[in2BufferNumber];
        free(dims);
        return IN1_IN2_OUT;
    } else {
        if (0 != gIfstool->images[outBufferNumber]) {
            // buffer already occupied
            ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
            gIfstool->images[outBufferNumber] = 0;

        }
        dims = ifssiz(gIfstool->images[in1BufferNumber]);
        gIfstool->images[outBufferNumber] = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        inImg1 = gIfstool->images[in1BufferNumber];
        inImg2 = gIfstool->images[in2BufferNumber];
        outImg = gIfstool->images[outBufferNumber];
        free(dims);
        return NONE;
    }
}

/**********************************************************************************************************************
 * FUNCTION     : getUnaryBuffers
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : inImg, outImg, outBufferNumber
 * OUT          : Buffer dependency relation
 * PROCESS      : Assigns inImg, outImg with appropriate buffers, returns buffer dependency relations
 *********************************************************************************************************************/
BUFFER_RELATION IfstoolMainWindow::getUnaryBuffers(IFSIMG &inImg, IFSIMG &outImg, int &outBufferNumber,
        OP_TYPE complexflag) {
    int *dims = NULL;
    int inBufferNumber = getRadioButtonStatus(INPUT1);

    outBufferNumber = getRadioButtonStatus(OUTPUT);

    if(0 == gIfstool->images[inBufferNumber]) {
        return IN_EMPYT;
    }

    if(NULL != childWindow[outBufferNumber]) {
        delete childWindow[outBufferNumber];
    }

    if (inBufferNumber == outBufferNumber) {
        dims = ifssiz(gIfstool->images[inBufferNumber]);
        if (COMPLEX_OP == complexflag) {
            outImg = ifscreate((char *) "32cmp", dims, IFS_CR_ALL, 0);
        } else {
            outImg = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        }
        inImg = gIfstool->images[inBufferNumber];
        free(dims);
        return IN1_OUT;
    } else {
        if (0 != gIfstool->images[outBufferNumber]) {
            // buffer already occupied
            ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
            gIfstool->images[outBufferNumber] = 0;
        }
        dims = ifssiz(gIfstool->images[inBufferNumber]);
        if (complexflag == COMPLEX_OP) {
            gIfstool->images[outBufferNumber] = ifscreate((char *) "32cmp", dims, IFS_CR_ALL, 0);
        } else {
            gIfstool->images[outBufferNumber] = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        }
        inImg = gIfstool->images[inBufferNumber];
        outImg = gIfstool->images[outBufferNumber];
        free(dims);
        return NONE;
    }
}

/**********************************************************************************************************************
 * FUNCTION     : addConsoleOutput
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : QString
 * OUT          : None
 * PROCESS      : Displays the string in console window
 *********************************************************************************************************************/
void IfstoolMainWindow::addConsoleOutput(QString data) {
    console->moveCursor(QTextCursor::End);
    console->insertPlainText(data);
    console->moveCursor(QTextCursor::End);
}

/**********************************************************************************************************************
 * FUNCTION     : open
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Opens file from the selected path and saves it in selected output buffer
 *********************************************************************************************************************/
void IfstoolMainWindow::open() {
    QString imageFileName;
    FILE *fp;
    QByteArray temp;
    char homepath[256], temp2[256];

    // check if we have opened a file before
    if (lastpath[0] == '\0') {
        // if not, open home directory
        imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    } else {
        //if so, look in the path we last used
        imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), lastpath);
    }

    if (imageFileName.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("cannot open file");
        msgBox.exec();
    } else {
        // here call the fileopen functionality
        int bufferLocation = getRadioButtonStatus(OUTPUT);
        addConsoleOutput("\nOpening " + imageFileName);
        handleFileByPath(imageFileName, bufferLocation); // put in buffer 0 for now
    }
    // now remember what we just did

    // find my home directory
    QString homedir = QDir::homePath();
    QByteArray ba = homedir.toLocal8Bit(); // convert fileName to char
    strcpy(homepath, ba);
#ifdef MacX64
    strcat(homepath,"/.com.ifs.Ifstool");
#endif
#ifdef Linux64
    strcat(homepath,"/.com.ifs.Ifstool");
#endif
    fp = fopen(homepath, "wb"); // open a file in home directory to save last file
    if (fp == NULL) {
        printf(" cannot open logging file\n");
        fflush (stdout);
        return;
    }
    temp = imageFileName.toLocal8Bit();
    // save last file name
    strcpy(temp2, temp);
    fprintf(fp, "%s\n", temp2); // write last file opened to log
    fclose(fp);


    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Reopen
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Opens the last file opened. If none, reads home directory to look for one
 *********************************************************************************************************************/
void IfstoolMainWindow::reopen() {
    int bufferLocation = getRadioButtonStatus(OUTPUT);
    char logfile[256];

    if (lastfile[0] != 0) {
        addConsoleOutput("\nOpening " + (QString)lastfile);
        handleFileByPath(lastfile, bufferLocation);
    } else {
        FILE *fp;
        QString homedir = QDir::homePath();
        QByteArray ba = homedir.toLocal8Bit(); // convert fileName to char
        strcpy(logfile, ba);
        strcat(logfile, "/.com.ifs.Ifstool");

        fflush (stdout);
        fp = fopen(logfile, "rb"); // open loggin file
        if (fp == NULL) {
            printf("cannot open %s\n", logfile);
            fflush(stdout);
            return;
        }

        fscanf(fp, "%s", lastfile);
        addConsoleOutput("\nOpening " + (QString)lastfile);
        handleFileByPath(lastfile, bufferLocation);
        fclose(fp);
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : save
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Saves the concerned file
 *********************************************************************************************************************/
bool IfstoolMainWindow::save() {
    return true;
}

/**********************************************************************************************************************
 * FUNCTION     : saveAs
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Saves the output file, with possibly a new name/type
 *********************************************************************************************************************/
bool IfstoolMainWindow::saveAs() {
    char s[256];
    int status,bufferLocation;
    bool ok;
    int len[4];
    char bdtype[16];
    IFSIMG tempimg;
    bufferLocation = getRadioButtonStatus(OUTPUT);// do this first in case the user tries to save an empty buffer
    if(gIfstool->images[bufferLocation] == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("No image has been stored in the buffer.");
        msgBox.exec();
        return false;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("SaveAs File"),
                                                    QDir::homePath(),
                                                    tr("Images (*.png *.xpm *.jpg *.ifs)"));
    QFileInfo finfo(fileName);// get all the file info
    QString ext=finfo.suffix();
    QByteArray bfilename=fileName.toLocal8Bit();
    strcpy(s,bfilename);
    // get selected buffer number
    if(ext == "ifs" || ext == "IFS")
    {

        printf("saving ifs image to file name %s\n",s);fflush(stdout);
        //popup a dialog box to get the ifs data type
        QString dtype = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                              tr("ifs data type:"), QLineEdit::Normal,
                                              "float", &ok);
        status = IFS_SUCCESS;
        if(ok == true)
        {
            if(dtype.isEmpty())
                strcpy(bdtype,"float");
            else
            {
                bfilename=dtype.toLocal8Bit();// convert data type to char
                strcpy(bdtype,bfilename);
            }

            len[0]=gIfstool->images[bufferLocation]->ifsdims;

            len[1]=ifsdimen(gIfstool->images[bufferLocation],0);
            len[2]=ifsdimen(gIfstool->images[bufferLocation],1);
            if(len[0] == 3) len[3]=ifsdimen(gIfstool->images[bufferLocation],2);
            tempimg = ifscreate(bdtype,len,IFS_CR_ALL,0);
            status=ifsany2any(gIfstool->images[bufferLocation],tempimg);

            status=ifspot(tempimg,s);

            ifsfree(tempimg,IFS_FR_ALL);
        }
        if(status != IFS_SUCCESS){

            QMessageBox msgBox;
            msgBox.setText("output of file to disk failed");
            msgBox.exec();
        }
        return true;
    } else {
        // do stuff to save to a different file type
        // BUG ON NEXT LINE! if ifs 3d, not color, should not be frame zero
        // but the frame being viewed
        addConsoleOutput("\nsaving " );
        if(NULL == childWindow[bufferLocation]) {
            QImage  *outqtimg=ifsToQt.ifs2Qt(gIfstool->images[bufferLocation], 0, 0);
            outqtimg->save(fileName);
        } else {
            QImage  *outqtimg=ifsToQt.ifs2Qt(gIfstool->images[bufferLocation], 0, childWindow[bufferLocation]->ifsToolFrameNumber);
            outqtimg->save(fileName);
        }

    }
    return true;
}

/**********************************************************************************************************************
 * FUNCTION     : view
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot
 * IN           : None
 * OUT          : None
 * PROCESS      : Displays selected output image
 *********************************************************************************************************************/
void IfstoolMainWindow::view() {
    IfstoolChildWindow *localChildWindow;
    int bufferLocation = getRadioButtonStatus(OUTPUT);

    if (NULL != childWindow[bufferLocation]) {
        delete childWindow[bufferLocation];
    }

    if (0 != gIfstool->images[bufferLocation]) {
        if (gIfstool->images[bufferLocation]->dtype == IFST_32CMP) {
            QMessageBox msgBox;
            msgBox.setText("Complex images cannot be viewed. Use complex pull down");
            msgBox.exec();
            return;
        }
        localChildWindow = new IfstoolChildWindow(this, (ifsToQt.ifs2Qt(gIfstool->images[bufferLocation], 0, 0)), bufferLocation);
        localChildWindow->setAttribute(Qt::WA_DeleteOnClose);
        updateChildPtr(localChildWindow, SET);
    }
}

void IfstoolMainWindow::mode() {

    return;
}

/**********************************************************************************************************************
 * FUNCTION     : copy
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot
 * IN           : None
 * OUT          : None
 * PROCESS      : Copies selected output image
 *********************************************************************************************************************/
void IfstoolMainWindow::copy() {
    int outBufferNumber = getRadioButtonStatus(OUTPUT);
    int *dims = NULL;

    if(0 == gIfstool->images[outBufferNumber]) {
        QMessageBox msgBox;
        msgBox.setText("Image Empty");
        msgBox.exec();
        return;
    }

    if(0 != copyBuffer) {
        ifsfree(copyBuffer, IFS_FR_ALL);
        copyBuffer = 0;
    }
    dims = ifssiz(gIfstool->images[outBufferNumber]);
    copyBuffer = ifscreate((char *) gIfstool->images[outBufferNumber]->ifsdt, dims, IFS_CR_ALL, 0);
    ifsany2any(gIfstool->images[outBufferNumber], copyBuffer);
    free(dims);
    pasteAct->setEnabled(true);
}

/**********************************************************************************************************************
 * FUNCTION     : paste
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot
 * IN           : None
 * OUT          : None
 * PROCESS      : Pastes copied image to output image
 *********************************************************************************************************************/
void IfstoolMainWindow::paste() {
    int outBufferNumber = getRadioButtonStatus(OUTPUT);
    int *dims = NULL;

    if(0 == copyBuffer) {
        QMessageBox msgBox;
        msgBox.setText("Nothing to paste");
        msgBox.exec();
        return;
    }

    if(0 != gIfstool->images[outBufferNumber]) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = 0;
        if(NULL != childWindow[outBufferNumber]) {
            delete childWindow[outBufferNumber];
        }
    }
    dims = ifssiz(copyBuffer);
    gIfstool->images[outBufferNumber] = ifscreate((char *) copyBuffer->ifsdt, dims, IFS_CR_ALL, 0);
    ifsany2any(copyBuffer, gIfstool->images[outBufferNumber]);
    free(dims);
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : delete_
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot
 * IN           : None
 * OUT          : None
 * PROCESS      : Deletes output image
 *********************************************************************************************************************/
void IfstoolMainWindow::delete_() {
    int outBufferNumber = getRadioButtonStatus(OUTPUT);
    if(0 != gIfstool->images[outBufferNumber]) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = 0;
        if(NULL != childWindow[outBufferNumber]) {
            delete childWindow[outBufferNumber];
        }
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : mono
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot
 * IN           : None
 * OUT          : None
 * PROCESS      : Converts image to monochrome
 *********************************************************************************************************************/
void IfstoolMainWindow::mono() {

    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : increaseBrightness
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Increases the brightness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::increaseBrightness() {

    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : decreaseBrightness
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Decreases the brightness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::decreaseBrightness() {

}

/**********************************************************************************************************************
 * FUNCTION     : increaseWarm
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Increases the warmness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::increaseWarm() {

    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : decreaseWarm
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Decreases the warmness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::decreaseWarm() {

    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : increaseCool
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Increases the coolness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::increaseCool() {

    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : decreaseCool
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Decreases the coolness of the image slightly
 *********************************************************************************************************************/
void IfstoolMainWindow::decreaseCool() {

}

/**********************************************************************************************************************
 * FUNCTION     : dx
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Takes the first derivative wrt x of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::dx() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPDX, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : dy
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Takes the first derivative wrt y of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::dy() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPDY, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : dxx
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Takes the second derivative wrt x of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::dxx() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPDXX, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : dyy
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Takes the second derivative wrt y of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::dyy() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPDYY, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : dxdy
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Takes the second derivative wrt x and y of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::dxdy() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPDXY, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : curvature
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the curvature of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::curvature() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPCUR, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : exponent
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the exponent of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::exponent() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPEXP, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : inverse
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the inverse of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::inverse() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPINV, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : natural log
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the log of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::natlog() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPNATLOG, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : laplacian
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the laplacian of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::laplacian() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPLAP, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : histogram equalization
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the laplacian of each pixel of the image
 *********************************************************************************************************************/
void IfstoolMainWindow::histeq() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPHISTEQ, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : blur
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Blurs an image
 *********************************************************************************************************************/
void IfstoolMainWindow::blur() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    ifsOP->unary(OPBLUR, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : pseudocolor
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : pseudocolor  image
 *********************************************************************************************************************/
void IfstoolMainWindow::pseudo() {
    int outBufferNumber = -1;
    int dims[4];
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    // the output image must be 3d, no matter what the input is
    if (outImg->ifsdims != 3) {
        ifsfree(outImg, IFS_FR_ALL); // erased the old 2d buffer
        dims[0] = 3;
        dims[1] = ifsdimen(inImg, 0);
        dims[2] = ifsdimen(inImg, 1);
        dims[3] = 3;
        outImg = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
    }
    ifsOP->unary(OPPSEUDO, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Get Complex Magnitude
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the magnitude  of each pixel in a complex image
 *********************************************************************************************************************/
void IfstoolMainWindow::c2mag() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    if (inImg->dtype != IFST_32CMP) {
        //popup non complex imag
        QMessageBox msgBox;
        msgBox.setText("Input Image is not complex");
        msgBox.exec();
        return;
    }
    ifsOP->unary(OPC2MAG, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Get Complex Phase
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the phase  of each pixel in a complex image
 *********************************************************************************************************************/
void IfstoolMainWindow::c2phase() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    if (inImg->dtype != IFST_32CMP) {
        //popup non complex imag
        QMessageBox msgBox;
        msgBox.setText("Image is not complex");
        msgBox.exec();
        return;
    }
    ifsOP->unary(OPC2PHASE, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Get Real part of complex data
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the real part  in a complex image
 *********************************************************************************************************************/
void IfstoolMainWindow::c2real() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    if (inImg->dtype != IFST_32CMP) {
        //popup non complex imag
        QMessageBox msgBox;
        msgBox.setText("Image is not complex");
        msgBox.exec();
        return;
    }
    ifsOP->unary(OPC2REAL, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Get Imaginary part of complex data
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Gets the image part  of each pixel in a complex image
 *********************************************************************************************************************/
void IfstoolMainWindow::c2imag() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, REAL_OP);
    if (inImg->dtype != IFST_32CMP) {
        //popup non complex imag
        QMessageBox msgBox;
        msgBox.setText("Image is not complex");
        msgBox.exec();
        return;
    }
    ifsOP->unary(OPC2IMAG, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Complex Fourier transform
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Take fft of a complex  image
 *********************************************************************************************************************/
void IfstoolMainWindow::fft() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;
//    printf("entering fft\n");
//    fflush (stdout);
    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, COMPLEX_OP);

    ifsOP->unary(OPFFT, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : Inverse Fourier transform
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Take ifft of a real  image
 *********************************************************************************************************************/
void IfstoolMainWindow::ifft() {
    int outBufferNumber = -1;
    IFSIMG inImg = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getUnaryBuffers(inImg, outImg, outBufferNumber, COMPLEX_OP);
    if (inImg->dtype != IFST_32CMP) {
        //popup non complex imag
        QMessageBox msgBox;
        msgBox.setText("Image is not complex");
        msgBox.exec();
        return;
    }
    ifsOP->unary(OPIFFT, inImg, outImg);
    if (IN1_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}
/**********************************************************************************************************************
 * FUNCTION     : add
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Adds inp1 and inp2 and saves in op
 *********************************************************************************************************************/
void IfstoolMainWindow::add() {
    int outBufferNumber = -1;
    IFSIMG inImg1 = 0, inImg2 = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getBinaryBuffers(inImg1, inImg2, outImg, outBufferNumber);
    ifsOP->binary(OPADD, inImg1, inImg2, outImg);
    if (IN1_IN2_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : sub
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Sub in2 from in1 and saves in op
 *********************************************************************************************************************/
void IfstoolMainWindow::sub() {
    int outBufferNumber = -1;
    IFSIMG inImg1 = 0, inImg2 = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getBinaryBuffers(inImg1, inImg2, outImg, outBufferNumber);
    ifsOP->binary(OPSUB, inImg1, inImg2, outImg);
    if (IN1_IN2_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : mul
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : Multiplies in1 and in2 and saves in op
 *********************************************************************************************************************/
void IfstoolMainWindow::mul() {
    int outBufferNumber = -1;
    IFSIMG inImg1 = 0, inImg2 = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getBinaryBuffers(inImg1, inImg2, outImg, outBufferNumber);
    ifsOP->binary(OPMUL, inImg1, inImg2, outImg);
    if (IN1_IN2_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : div
 * CLASS        : IfstoolMainWindow
 * TYPE         : Slot function
 * IN           : None
 * OUT          : None
 * PROCESS      : divides in1 by in2 and saves in in outImg
 *********************************************************************************************************************/
void IfstoolMainWindow::div() {
    int outBufferNumber = -1;
    IFSIMG inImg1 = 0, inImg2 = 0, outImg = 0;
    BUFFER_RELATION relation = NONE;

    relation = getBinaryBuffers(inImg1, inImg2, outImg, outBufferNumber);
    ifsOP->binary(OPDIV, inImg1, inImg2, outImg);
    if (IN1_IN2_OUT == relation) {
        ifsfree(gIfstool->images[outBufferNumber], IFS_FR_ALL);
        gIfstool->images[outBufferNumber] = outImg;
    }
    updateAccessability();
}

/**********************************************************************************************************************
 * FUNCTION     : updateChildPtr
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Updates child display window ptr
 *********************************************************************************************************************/
void IfstoolMainWindow::updateChildPtr(const IfstoolChildWindow *childPtr, UPDATE_TYPE type) {
    childPtrMutex->lock();
    int bufferLocation = getRadioButtonStatus(OUTPUT);

    switch (type) {
    case SET:
        childWindow[bufferLocation] = (IfstoolChildWindow *) childPtr;
        break;

    case UNSET:
        for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
            if (childPtr == childWindow[i]) {
                childWindow[i] = NULL;
            }
        }
        break;

    default:
        break;
    }
    childPtrMutex->unlock();
}

/**********************************************************************************************************************
 * FUNCTION     : about
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member function
 * IN           : None
 * OUT          : None
 * PROCESS      : Displays about message
 *********************************************************************************************************************/
//QString VERSION = "2.1";
void IfstoolMainWindow::about() {
    extern QString VERSION;
    QString a = "Ifstool " + VERSION + " is an Image File System tool which gives wide range of operations on images\nBuilt under Yosemite";
    QMessageBox::about(this, tr("About Application"), a);
}

/**********************************************************************************************************************
 * FUNCTION     : handleFileByPath
 * CLASS        : IfstoolMainWindow
 * TYPE         : handleFileByPath
 * IN           : fileName
 * OUT          : None
 * PROCESS      : Opens the file based on incoming filename
 *********************************************************************************************************************/
void IfstoolMainWindow::handleFileByPath(QString fileName, int bufferNumber) {
    QImage image;
    IFSIMG work, a;
    int red = 0;
    int green = 0;
    int blue = 0;
    int len = 0;
    int row = 0;
    int col = 0;
    int *dims;
    char bpath[360];
    char bname[360];

    // firt convert file name from qstring to byte
//    printf("Entering handleFileByPath\n");
//    fflush (stdout);
    QByteArray ba = fileName.toLocal8Bit();
    QFileInfo finfo(fileName); // learn about this name
    QByteArray path = finfo.absolutePath().toLocal8Bit();
    name1 = finfo.fileName();

    strcpy(bpath, path); // this name is the path, not the file name
    strcpy(bname, ba);
    strcpy(lastpath, ba);
    strcpy(lastfile, ba);
//    printf("Opening %s\n", bname);
//    fflush(stdout);

    len = strlen(bname);
    if (strcmp(bname + (len - 4), "ifsb") == 0) {
        /*                                                       */
        /*                  boundary file                        */
        /*                                                       */
        a = ifsplotcontour(bname, 1.0, 0, 0, 0); // create an image from file
        dims = ifssiz(a);
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        ifsany2any(a, work);
        ifsfree(a, IFS_FR_ALL);
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("message"));
        mbox->setText("Boundary file");
        mbox->show();
        QTimer::singleShot(500, mbox, SLOT(close()));
    } else if (strcmp(bname + (len - 3), "ifs") == 0 || strcmp(bname + (len - 3), "IFS") == 0) {
        /*                                                       */
        /*                  ifs image                            */
        /*                                                       */
        a = ifspin(bname); // get image
        dims = ifssiz(a);
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        ifsany2any(a, work);
        ifsfree(a, IFS_FR_ALL);
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("message"));
        mbox->setText("IFS image");
        mbox->show();
        QTimer::singleShot(500, mbox, SLOT(close()));

    }
    // if not ifs or ifsb, read it in using Qt
    else {
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("message"));
        mbox->setText("non ifs");
        mbox->show();
        QTimer::singleShot(500, mbox, SLOT(close()));

        image.load(fileName);
        QSize dimensions = image.size();
        dims = (int *) malloc(4 * sizeof(int));
        dims[2] = dimensions.height();
        dims[1] = dimensions.width();
        dims[0] = 3;
        dims[3] = 3; //create a 3d ifs image
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        for (row = 0; row < dims[2]; row++)
            for (col = 0; col < dims[1]; col++) {
                QRgb color;
                color = image.pixel(col, row);   // image.pixel(x,y) x is columns (width) y is rows (height)

                red = qRed(color);
                green = qGreen(color);
                blue = qBlue(color);

                ifsfpp3d(work, 0, row, col, red);
                ifsfpp3d(work, 1, row, col, green);
                ifsfpp3d(work, 2, row, col, blue);
            }   // end loop over rows
    }   // end block on else
    //now ifsimg work contains the image we just read
    // put it in the global data array

    if (0 != gIfstool->images[bufferNumber]) {
        ifsfree(gIfstool->images[bufferNumber], IFS_FR_ALL);
        gIfstool->images[bufferNumber] = 0;
        if(NULL != childWindow[bufferNumber]) {
            delete childWindow[bufferNumber];
        }
    }

    gIfstool->images[bufferNumber] = work;
    gIfstool->imagenames[bufferNumber]=name1;
    /*  DONT FREE WORK */
    free(dims);

    /*******block to insert image statistics**********************/
    {
        float mean, variance;
        float *fptr;
        int i, nr, nc, numpixels;
        int *dims;
        dims = ifssiz(work);
        nr = dims[2];
        nc = dims[1];    //done to make the argument one less lookup
        numpixels = nr * nc;    // computing for a 2d image
        mean = 0;
        fptr = (float *) work->ifsptr;
        for (i = 0; i < numpixels; i++)
            mean += *fptr++;
        mean = mean / numpixels;
        fptr = (float *) work->ifsptr;
        variance = 0.0;
        for (i = 0; i < numpixels; i++)
            variance += (*fptr - mean) * (*fptr - mean);
        variance = variance / numpixels;
        addConsoleOutput("\nImage has " + QString::number(nr));
        addConsoleOutput(" rows and " + QString::number(nc) + " Columns\n");
        addConsoleOutput("Mean = " + QString::number(mean) + " variance = " + QString::number(variance));
    }
    updateAccessability();
    return;
}

/**********************************************************************************************************************
 * FUNCTION     : getIndexFromChildPtr
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member Function
 * IN           : child window pointer
 * OUT          : Buffer number connected to the child window
 * PROCESS      : Returns the buffer number connected to the child window
 *********************************************************************************************************************/
int IfstoolMainWindow::getIndexFromChildPtr(void *child) {
    for (int i = 0; i < NUMBER_OF_IMGAES; i++) {
        if(childWindow[i] == (IfstoolChildWindow*) child) {
            return i;
        }
    }
    return -1;
}
/**********************************************************************************************************************
 * FUNCTION     : runScript
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member Function
 * IN           :
 * OUT          :
 * PROCESS      : Writes out the buffer selected as output
 *              : Runs a user-selected script
 *              : Reads script result into buffer selected as input 2
 *********************************************************************************************************************/
void IfstoolMainWindow::runScript() {
    char command[256];

    QString fileName;
    QString qsspath;
    QMessageBox    *msgBox = new QMessageBox;
    QFileDialog *fileDlg = new QFileDialog(this);
    if(rerunflag == false)// if running a script for the first time
    {

        fileName = fileDlg->getOpenFileName(this,
                                            tr("Select Script"), QDir::homePath());
        if (fileName.isEmpty())
        {

            msgBox->setText("ScriptName is empty");
            msgBox->exec();
            return;
        }
        fileDlg->close();
        QFileInfo ScriptNameInfo(fileName);// get all the file info
        qsspath=ScriptNameInfo.absolutePath();
        QByteArray ScriptPath8bit=qsspath.toLocal8Bit();// convert path to char
        QByteArray ScriptName8bit=fileName.toLocal8Bit(); // get name of script file
        strcpy(scriptName,ScriptName8bit.data());// but into char string
        strcpy(scriptPath,ScriptPath8bit.data()); // path to script
        strcpy(scriptInputName,scriptPath);
        strcat(scriptInputName,"/ImgViewTempInput.ifs");
    }

    int bufferLocation = getRadioButtonStatus(OUTPUT);// get the selected output buffer number
    ifspot(gIfstool->images[bufferLocation],scriptInputName);// write image to target directory

    sprintf(command,"(cd %s;%s)",scriptPath,scriptName);
    addConsoleOutput("\nScript started...");
    system(command);
    addConsoleOutput("Script completed\n");
    strcpy(scriptOutputName,scriptPath);
    strcat(scriptOutputName,"/ImgViewTempOutput.ifs");
    QFileInfo ofileinfo(scriptOutputName);
    if(!ofileinfo.exists()) {
        msgBox->setText("Your script did not produce an output file that I know how to process");
        msgBox->exec();
        return;
    }

    bufferLocation=getRadioButtonStatus(INPUT2);
    if(gIfstool->images[bufferLocation] != NULL)
        ifsfree(gIfstool->images[bufferLocation],IFS_FR_ALL);

    gIfstool->images[bufferLocation]=ifspin(scriptOutputName);
    gIfstool->imagenames[bufferLocation]="ReturnedFromScript";
    sprintf(command,"rm %s",scriptOutputName);
    system(command);
    reRunAct->setEnabled(true);

    return;

}
/**********************************************************************************************************************
 * FUNCTION     : rerunScript
 * CLASS        : IfstoolMainWindow
 * TYPE         : Member Function
 * IN           :
 * OUT          :
 * PROCESS      : writes file, runs script, reads file
 *********************************************************************************************************************/
void IfstoolMainWindow::rerunScript() {
    if(scriptInputName == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("You must run a script before you can rerun it.");
        msgBox.exec();
        return;
    }
    rerunflag=true;
    IfstoolMainWindow::runScript();
}
//Nothing past this point
