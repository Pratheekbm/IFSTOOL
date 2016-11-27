/*
 * ifstoolChildWindow.cpp
 * Created :    May 23 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */
#include <iostream>
#include "generictypes.h"
#include "ifstoolChildWindow.h"
#include "ifstoolMainWindow.h"

#include "ifstool.h"
extern Ifstool *gIfstool;
/**********************************************************************************************************************
 * FUNCTION     : IfstoolChildWindow
 * CLASS        : IfstoolChildWindow
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : Initializes stuff
 *********************************************************************************************************************/
IfstoolChildWindow::IfstoolChildWindow(QWidget *parent, QImage *imageToDisplay, int bufferLocation) :
    QMainWindow(parent) {
    int *dims = NULL;
    dims = ifssiz(gIfstool->images[bufferLocation]);
    localBuffer = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
    BrScale(gIfstool->images[bufferLocation], localBuffer, 255, 0, &maxPixel, &minPixel);
    //    free(dims);

    this->_parent = parent;
    incomingQImage = imageToDisplay;
    this->setWindowTitle(QString::number(bufferLocation)+":"+gIfstool->imagenames[bufferLocation]);
    imageLabel = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCentralWidget(scrollArea);
    rightPanel = new QDockWidget("Operations", this);
    rightPanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::RightDockWidgetArea, rightPanel, Qt::Vertical);
    imageLabel->setWindowTitle(tr("Accumulator"));

    rightPanelGrpBox = new QGroupBox(rightPanel);
    rightPanelVLayout = new QVBoxLayout(rightPanelGrpBox);
    rightPanelGrpBox->setLayout(rightPanelVLayout);

    displayPanelGrpBox = new QGroupBox("Display", rightPanel);
    displayPanelGridLayout = new QGridLayout(displayPanelGrpBox);
    displayPanelGrpBox->setLayout(displayPanelGridLayout);

    //displayPanelGrpBox->setMaximumWidth(200);

    rgbPanelGrpBox = new QGroupBox("RGB", rightPanel);
    rgbPanelGridLayout = new QGridLayout(rgbPanelGrpBox);
    rgbPanelGrpBox->setLayout(rgbPanelGridLayout);

    editPanelGrpBox = new QGroupBox("Edit", rightPanel);
    editGridLayout = new QGridLayout(editPanelGrpBox);
    editPanelGrpBox->setLayout(editGridLayout);

    rightPanelVLayout->addWidget(displayPanelGrpBox);
    rightPanelVLayout->addWidget(rgbPanelGrpBox);
    rightPanelVLayout->addWidget(editPanelGrpBox);

    //    displayPanelGrpBox->hide();
    //    rgbPanelGrpBox->hide();
    //    editPanelGrpBox->hide();

    rightPanel->setWidget(rightPanelGrpBox);

    lastBright = 0;
    lastWarm = 0;
    lastCool = 0;
    lastR = 0;
    lastG = 0;
    lastB = 0;
    lastZoom = 0;
    lastLevelLow = 0;
    lastLevelHigh = 255;
    profileFlag = false;
    histFlag = false;
    ifsToolFrameNumber = 0;

    createActions();
    createToolBar();
    addLayoutElements();

    if(3 != localBuffer->ifsdims) {
        warmSlider->setEnabled(false);
        warmSpinBox->setEnabled(false);
        coolSlider->setEnabled(false);
        coolSpinBox->setEnabled(false);
        rSlider->setEnabled(false);
        rSpinBox->setEnabled(false);
        gSlider->setEnabled(false);
        gSpinBox->setEnabled(false);
        bSlider->setEnabled(false);
        bSpinBox->setEnabled(false);
    }
    if(localBuffer->ifsdims == 2) {
        iSB->setMaximum(0);
        iSB->setMinimum(0);
    } else {
        iSB->setMaximum(ifsdimen(localBuffer,2)-1);
        iSB->setMinimum(0);
    }
    if(dims[1]<1025) {
        resize(dims[1]+330, 320);
    } else {
        resize(1280, 320);
    }
    //move(1024, 512);
    /* check if displaying individual frames */
    if(!colorCheckBox->isChecked())
    {

    }
    else
    {
        imageLabel->setPixmap(QPixmap::fromImage(*imageToDisplay));
        imageLabel->resize(1 * imageLabel->pixmap()->size());
    }
    free(dims);
    this->show();
}

/**********************************************************************************************************************
 * FUNCTION     : IfstoolChildWindow
 * CLASS        : IfstoolChildWindow
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : Deletes stuff
 *********************************************************************************************************************/
IfstoolChildWindow::~IfstoolChildWindow() {

    this->hide();
    this->setCentralWidget(NULL);
    delete imageLabel;
    delete scrollArea;
    delete incomingQImage;
    IfstoolMainWindow *parent = (IfstoolMainWindow *) _parent;
    parent->updateChildPtr(this, UNSET);
}

/**********************************************************************************************************************
 * FUNCTION     : closeEvent
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Handles close event
 *********************************************************************************************************************/
void IfstoolChildWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

/**********************************************************************************************************************
 * FUNCTION     : addActions
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Adds required layout elements
 *********************************************************************************************************************/
void IfstoolChildWindow::createActions() {

    profileAct = new QAction(tr("Profiler OFF"), this);
    connect(profileAct, SIGNAL(triggered()), this, SLOT(profile()));

    histAct = new QAction(tr("Histogram Off"), this);
    connect(histAct, SIGNAL(triggered()), this, SLOT(histogram()));

    resetAct = new QAction(tr("Reset"), this);
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    saveAct = new QAction(tr("Save"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

}

/**********************************************************************************************************************
 * FUNCTION     : createToolBar
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Adds required layout elements
 *********************************************************************************************************************/
void IfstoolChildWindow::createToolBar() {
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(profileAct);
    editToolBar->addAction(histAct);
    editToolBar->addAction(resetAct);
    editToolBar->addAction(saveAct);
}

/**********************************************************************************************************************
 * FUNCTION     : addLayoutElements
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Adds required layout elements
 *********************************************************************************************************************/
void IfstoolChildWindow::addLayoutElements() {
    int step = 10;
    iSB = new QSpinBox(displayPanelGrpBox);

    //frame select spinbox
    iSB->setRange(0, 200);
    iSB->setSingleStep(1);
    iSB->setValue(0);
    iSB->setToolTip("Select frame to be displayed (if not displaying color)");
    //   iSB->setPrefix(tr("F "));


    colorCheckBox = new QCheckBox("Color",displayPanelGrpBox);
    colorCheckBox->setCheckable(true);
    //  colorCheckBox->setGeometry(1,110,200,30);
    colorCheckBox->setAutoExclusive(false);
    colorCheckBox->setChecked(true);
    colorCheckBox->setToolTip("If checked, 3 frame images will be displayed as color\n else as a set of frames.");


    brightSlider = new QSlider(Qt::Horizontal, displayPanelGrpBox);
    brightSlider->setMaximum(255);
    brightSlider->setMinimum(-255);
    brightSlider->setRange(-255, 255);
    brightSlider->setSingleStep(step);
    brightSlider->setValue(0);

    warmSlider = new QSlider(Qt::Horizontal, displayPanelGrpBox);
    warmSlider->setMaximum(255);
    warmSlider->setMinimum(-255);
    warmSlider->setRange(-255, 255);
    warmSlider->setSingleStep(step);
    warmSlider->setValue(0);

    coolSlider = new QSlider(Qt::Horizontal, displayPanelGrpBox);
    coolSlider->setMaximum(255);
    coolSlider->setMinimum(-255);
    coolSlider->setRange(-255, 255);
    coolSlider->setSingleStep(step);
    coolSlider->setValue(0);

    frameLabel = new QLabel("Frame No", displayPanelGrpBox);
    brightLabel = new QLabel("Bright", displayPanelGrpBox);
    warmLabel = new QLabel("Warm", displayPanelGrpBox);
    coolLabel = new QLabel("Cool", displayPanelGrpBox);

    QFont font = brightLabel->font();
    font.setPointSize(10);
    frameLabel->setFont(font);
    brightLabel->setFont(font);
    warmLabel->setFont(font);
    coolLabel->setFont(font);

    brightSpinBox = new QSpinBox(displayPanelGrpBox);
    brightSpinBox->setMaximum(255);
    brightSpinBox->setMinimum(-255);
    brightSpinBox->setRange(-255, 255);
    brightSpinBox->setSingleStep(step);
    brightSpinBox->setValue(0);

    warmSpinBox = new QSpinBox(displayPanelGrpBox);
    warmSpinBox->setMaximum(255);
    warmSpinBox->setMinimum(-255);
    warmSpinBox->setRange(-255, 255);
    warmSpinBox->setSingleStep(step);
    warmSpinBox->setValue(0);

    coolSpinBox = new QSpinBox(displayPanelGrpBox);
    coolSpinBox->setMaximum(255);
    coolSpinBox->setMinimum(-255);
    coolSpinBox->setRange(-255, 255);
    coolSpinBox->setSingleStep(step);
    coolSpinBox->setValue(0);

    brightSpinBox->setFont(font);
    brightSpinBox->setMaximumWidth(45);
    warmSpinBox->setFont(font);
    warmSpinBox->setMaximumWidth(45);
    coolSpinBox->setFont(font);
    coolSpinBox->setMaximumWidth(45);

    connect(brightSpinBox, SIGNAL(valueChanged(int)),brightSlider,SLOT(setValue(int)));
    connect(brightSlider,SIGNAL(valueChanged(int)),brightSpinBox,SLOT(setValue(int)));

    connect(warmSpinBox, SIGNAL(valueChanged(int)),warmSlider,SLOT(setValue(int)));
    connect(warmSlider,SIGNAL(valueChanged(int)),warmSpinBox,SLOT(setValue(int)));

    connect(coolSpinBox, SIGNAL(valueChanged(int)),coolSlider,SLOT(setValue(int)));
    connect(coolSlider,SIGNAL(valueChanged(int)),coolSpinBox,SLOT(setValue(int)));

    connect(brightSlider, SIGNAL(valueChanged(int)),this, SLOT(bright(int)));
    connect(coolSlider, SIGNAL(valueChanged(int)),this, SLOT(cool(int)));
    connect(warmSlider, SIGNAL(valueChanged(int)),this, SLOT(warm(int)));
    connect(colorCheckBox, SIGNAL(stateChanged(int)),this, SLOT(checkIt()));

    displayPanelGridLayout->addWidget(frameLabel, 0, 0, 1, 1);
    displayPanelGridLayout->addWidget(brightLabel, 1, 0, 1, 1);
    displayPanelGridLayout->addWidget(warmLabel, 2, 0, 1, 1);
    displayPanelGridLayout->addWidget(coolLabel, 3, 0, 1, 1);

    displayPanelGridLayout->addWidget(iSB, 0, 2, 1, 1);
    displayPanelGridLayout->addWidget(brightSlider, 1, 2, 1, 7);
    displayPanelGridLayout->addWidget(warmSlider, 2, 2, 1, 7);
    displayPanelGridLayout->addWidget(coolSlider, 3, 2, 1, 7);

    displayPanelGridLayout->addWidget(colorCheckBox, 0, 5, 1, 1);
    displayPanelGridLayout->addWidget(brightSpinBox, 1, 9, 1, 1);
    displayPanelGridLayout->addWidget(warmSpinBox, 2, 9, 1, 1);
    displayPanelGridLayout->addWidget(coolSpinBox, 3, 9, 1, 1);

    iSB->setEnabled(false);
    rSlider = new QSlider(Qt::Horizontal, rgbPanelGrpBox);
    rSlider->setMaximum(255);
    rSlider->setMinimum(-255);
    rSlider->setRange(-255, 255);
    rSlider->setSingleStep(step);
    rSlider->setValue(0);

    gSlider = new QSlider(Qt::Horizontal, rgbPanelGrpBox);
    gSlider->setMaximum(255);
    gSlider->setMinimum(-255);
    gSlider->setRange(-255, 255);
    gSlider->setValue(0);

    bSlider = new QSlider(Qt::Horizontal, rgbPanelGrpBox);
    bSlider->setMaximum(255);
    bSlider->setMinimum(-255);
    bSlider->setRange(-255, 255);
    bSlider->setValue(0);

    rLabel = new QLabel("Red", rgbPanelGrpBox);
    gLabel = new QLabel("Green", rgbPanelGrpBox);
    bLabel = new QLabel("Blue", rgbPanelGrpBox);

    rLabel->setFont(font);
    gLabel->setFont(font);
    bLabel->setFont(font);

    rSpinBox = new QSpinBox(rgbPanelGrpBox);
    rSpinBox->setMaximum(255);
    rSpinBox->setMinimum(-255);
    rSpinBox->setRange(-255, 255);
    rSpinBox->setSingleStep(step);
    rSpinBox->setValue(0);

    gSpinBox = new QSpinBox(rgbPanelGrpBox);
    gSpinBox->setMaximum(255);
    gSpinBox->setMinimum(-255);
    gSpinBox->setRange(-255, 255);
    gSpinBox->setSingleStep(step);
    gSpinBox->setValue(0);

    bSpinBox = new QSpinBox(rgbPanelGrpBox);
    bSpinBox->setMaximum(255);
    bSpinBox->setMinimum(-255);
    bSpinBox->setRange(-255, 255);
    bSpinBox->setSingleStep(step);
    bSpinBox->setValue(0);

    rSpinBox->setFont(font);
    rSpinBox->setMaximumWidth(45);
    gSpinBox->setFont(font);
    gSpinBox->setMaximumWidth(45);
    bSpinBox->setFont(font);
    bSpinBox->setMaximumWidth(45);

    connect(rSpinBox, SIGNAL(valueChanged(int)),rSlider,SLOT(setValue(int)));
    connect(rSlider,SIGNAL(valueChanged(int)),rSpinBox,SLOT(setValue(int)));

    connect(gSpinBox, SIGNAL(valueChanged(int)),gSlider,SLOT(setValue(int)));
    connect(gSlider,SIGNAL(valueChanged(int)),gSpinBox,SLOT(setValue(int)));

    connect(bSpinBox, SIGNAL(valueChanged(int)),bSlider,SLOT(setValue(int)));
    connect(bSlider,SIGNAL(valueChanged(int)),bSpinBox,SLOT(setValue(int)));

    connect(rSlider, SIGNAL(valueChanged(int)),this, SLOT(changeR(int)));
    connect(gSpinBox, SIGNAL(valueChanged(int)),this, SLOT(changeG(int)));
    connect(bSpinBox, SIGNAL(valueChanged(int)),this, SLOT(changeB(int)));

    connect(iSB,SIGNAL(valueChanged(int)),this,SLOT(NewFrame(int)));

    rgbPanelGridLayout->addWidget(rLabel, 0, 0, 1, 1);
    rgbPanelGridLayout->addWidget(gLabel, 1, 0, 1, 1);
    rgbPanelGridLayout->addWidget(bLabel, 2, 0, 1, 1);

    rgbPanelGridLayout->addWidget(rSlider, 0, 2, 1, 7);
    rgbPanelGridLayout->addWidget(gSlider, 1, 2, 1, 7);
    rgbPanelGridLayout->addWidget(bSlider, 2, 2, 1, 7);

    rgbPanelGridLayout->addWidget(rSpinBox, 0, 9, 1, 1);
    rgbPanelGridLayout->addWidget(gSpinBox, 1, 9, 1, 1);
    rgbPanelGridLayout->addWidget(bSpinBox, 2, 9, 1, 1);

    zoomSlider = new QSlider(Qt::Horizontal, editPanelGrpBox);
    zoomSlider->setMaximum(255);
    zoomSlider->setMinimum(-255);
    zoomSlider->setRange(-255, 255);
    zoomSlider->setSingleStep(step);
    zoomSlider->setValue(0);

    zoomLabel = new QLabel("Zoom", editPanelGrpBox);
    zoomLabel->setFont(font);
    zoomSpinBox = new QSpinBox(editPanelGrpBox);
    zoomSpinBox->setMaximum(255);
    zoomSpinBox->setMinimum(-255);
    zoomSpinBox->setRange(-255, 255);
    zoomSpinBox->setSingleStep(step);
    zoomSpinBox->setValue(0);
    zoomSpinBox->setFont(font);
    zoomSpinBox->setMaximumWidth(45);

    levelSlider = new QSlider(Qt::Horizontal, editPanelGrpBox);
    levelSlider->setMaximum(255);
    levelSlider->setMinimum(0);
    levelSlider->setRange(0, 255);
    levelSlider->setSingleStep(step);
    levelSlider->setValue(0);

    windowSlider = new QSlider(Qt::Horizontal, editPanelGrpBox);
    windowSlider->setMaximum(255);
    windowSlider->setMinimum(0);
    windowSlider->setRange(0, 255);
    windowSlider->setSingleStep(step);
    windowSlider->setValue(255);

    levelLabel = new QLabel("Low Level", editPanelGrpBox);
    levelLabel->setFont(font);
    windowLabel = new QLabel("Window", editPanelGrpBox);
    windowLabel->setFont(font);

    levelSpinBox = new QSpinBox(editPanelGrpBox);
    levelSpinBox->setMaximum(255);
    levelSpinBox->setMinimum(0);
    levelSpinBox->setRange(0, 255);
    levelSpinBox->setSingleStep(step);
    levelSpinBox->setValue(0);
    levelSpinBox->setFont(font);
    levelSpinBox->setMaximumWidth(45);

    windowSpinBox = new QSpinBox(editPanelGrpBox);
    windowSpinBox->setMaximum(255);
    windowSpinBox->setMinimum(0);
    windowSpinBox->setRange(0, 255);
    windowSpinBox->setSingleStep(step);
    windowSpinBox->setValue(255);
    windowSpinBox->setFont(font);
    windowSpinBox->setMaximumWidth(45);

    levelSlider->setRange(0, 255 - windowSlider->value());
    levelSpinBox->setRange(0, 255 - windowSlider->value());

    connect(zoomSpinBox, SIGNAL(valueChanged(int)),zoomSlider,SLOT(setValue(int)));
    connect(zoomSlider,SIGNAL(valueChanged(int)),zoomSpinBox,SLOT(setValue(int)));

    connect(levelSpinBox, SIGNAL(valueChanged(int)),levelSlider,SLOT(setValue(int)));
    connect(levelSlider,SIGNAL(valueChanged(int)),levelSpinBox,SLOT(setValue(int)));

    connect(windowSpinBox, SIGNAL(valueChanged(int)), windowSlider,SLOT(setValue(int)));
    connect(windowSlider,SIGNAL(valueChanged(int)), windowSpinBox,SLOT(setValue(int)));

    connect(zoomSlider, SIGNAL(valueChanged(int)),this, SLOT(zoom(int)));
    connect(levelSlider, SIGNAL(valueChanged(int)),this, SLOT(level()));
    connect(windowSlider, SIGNAL(valueChanged(int)),this, SLOT(window()));

    editGridLayout->addWidget(zoomLabel, 0, 0, 1, 1);
    editGridLayout->addWidget(zoomSlider, 0, 2, 1, 7);
    editGridLayout->addWidget(zoomSpinBox, 0, 9, 1, 1);

    editGridLayout->addWidget(levelLabel, 1, 0, 1, 1);
    editGridLayout->addWidget(levelSlider, 1, 2, 1, 7);
    editGridLayout->addWidget(levelSpinBox, 1, 9, 1, 1);
    editGridLayout->addWidget(windowLabel, 2, 0, 1, 1);
    editGridLayout->addWidget(windowSlider, 2, 2, 1, 7);
    editGridLayout->addWidget(windowSpinBox, 2, 9, 1, 1);
}
/**********************************************************************************************************************
 * FUNCTION     : mousePressEvent
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Handles mouse press event
 *********************************************************************************************************************/
void IfstoolChildWindow::mousePressEvent(QMouseEvent *event){
    if(true == profileFlag) {
        xCoOrd.clear();
        yCoOrd.clear();
    }
}
/**********************************************************************************************************************
 * FUNCTION     : mouseMoveEvent
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Handles mouse move event when mouse is pressed
 *********************************************************************************************************************/
void IfstoolChildWindow::mouseMoveEvent(QMouseEvent *event) {
    IfstoolMainWindow *parent = (IfstoolMainWindow *) _parent;
    QPoint imgCoOrdinate;
    int x = 0, y = 0;
    int *dims = NULL;

    int imgIndex = parent->getIndexFromChildPtr(this);
    if (-1 == imgIndex) {
        return;
    }

    imgCoOrdinate = imageLabel->mapFromGlobal(event->globalPos());
    double scale = getScaleFromZoom(zoomSlider->value());
    x = imgCoOrdinate.x() / scale;
    y = imgCoOrdinate.y() / scale;

    dims = ifssiz(gIfstool->images[imgIndex]);
    if ((x > dims[1]) || (y > dims[2]) || (0 > x) || (0 > y)) {
        free(dims);
        return;
    }

    if((true == profileFlag) || (true == histFlag)) {
        QPoint imgCoOrdinate;
        const QPixmap *pixmap = imageLabel->pixmap();
        QImage image = pixmap->toImage();
        imgCoOrdinate = imageLabel->mapFromGlobal(event->globalPos());
        image.setPixel(imgCoOrdinate.x(), imgCoOrdinate.y(), qRgb(255, 0, 0));
        imageLabel->setPixmap(QPixmap::fromImage(image));
        double scale = getScaleFromZoom(zoomSlider->value());
        xCoOrd.append(imgCoOrdinate.x() / scale);
        yCoOrd.append(imgCoOrdinate.y() / scale);
    }

    free(dims);
}
/**********************************************************************************************************************
 * FUNCTION     : mouseReleaseEvent
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Handles mouse release event
 *********************************************************************************************************************/
void IfstoolChildWindow::mouseReleaseEvent(QMouseEvent *event) {
    IfstoolMainWindow *parent = (IfstoolMainWindow *) _parent;
    QPoint imgCoOrdinate;
    float f = 0, r = 0, g = 0, b = 0;
    int x = 0, y = 0;
    QString ranges;
    int *dims = NULL;

    int imgIndex = parent->getIndexFromChildPtr(this);
    if (-1 == imgIndex) {
        return;
    }

    imgCoOrdinate = imageLabel->mapFromGlobal(event->globalPos());
    double scale = getScaleFromZoom(zoomSlider->value());
    x = imgCoOrdinate.x() / scale;
    y = imgCoOrdinate.y() / scale;

    dims = ifssiz(gIfstool->images[imgIndex]);
    if ((x > dims[1]) || (y > dims[2]) || (0 > x) || (0 > y)) {
        free(dims);
        return;
    }

    float maxThreshold = (levelSlider->value() + windowSlider->value());
    float minThreshold = levelSlider->value();
    if(0 == (maxThreshold - minThreshold)) {
        maxThreshold = maxThreshold + 0.0001;
    }

    if((true == profileFlag || true == histFlag) && (0 != xCoOrd.size()) && (0 != yCoOrd.size())) {
        if(true == profileFlag) {
            QVector<double> r;
            QVector<double> g;
            QVector<double> b;
            for(int i = 0; i < xCoOrd.size(); i++) {
                double red = 0, green = 0, blue = 0;

                red = ifsfgp3d(localBuffer, 0, yCoOrd[i], xCoOrd[i]) + rSlider->value();
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                red = ifsBound(0, red, 255);

                green = ifsfgp3d(localBuffer, 1, yCoOrd[i], xCoOrd[i]) + gSlider->value();
                green = ifsBound(0, green, 255);
                green = (green - minThreshold) * 255 / (maxThreshold - minThreshold);
                green = ifsBound(0, green, 255);

                blue = ifsfgp3d(localBuffer, 2, yCoOrd[i], xCoOrd[i]) + bSlider->value();
                blue = ifsBound(0, blue, 255);
                blue = (blue - minThreshold) * 255 / (maxThreshold - minThreshold);
                blue = ifsBound(0, blue, 255);

                r.append((red - 0) * (maxPixel - minPixel)/ 255);
                g.append((green - 0) * (maxPixel - minPixel)/ 255);
                b.append((blue - 0) * (maxPixel - minPixel)/ 255);
            }
            MainWindow *w = new MainWindow(this);
            w->plotData(r, g, b, RGB);
            w->show();
        }

        if(true == histFlag) {
            QVector<double> r;
            QVector<double> g;
            QVector<double> b;
            r.clear();
            g.clear();
            b.clear();
            for(int i = 0; i <= 255/5; i++) {
                r.append(0);
                g.append(0);
                b.append(0);
            }

            for(int i = 0; i < xCoOrd.size(); i++) {
                double red = 0, green = 0, blue = 0;

                red = ifsfgp3d(localBuffer, 0, yCoOrd[i], xCoOrd[i]) + rSlider->value();
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                red = ifsBound(0, red, 255);

                green = ifsfgp3d(localBuffer, 1, yCoOrd[i], xCoOrd[i]) + gSlider->value();
                green = ifsBound(0, green, 255);
                green = (green - minThreshold) * 255 / (maxThreshold - minThreshold);
                green = ifsBound(0, green, 255);

                blue = ifsfgp3d(localBuffer, 2, yCoOrd[i], xCoOrd[i]) + bSlider->value();
                blue = ifsBound(0, blue, 255);
                blue = (blue - minThreshold) * 255 / (maxThreshold - minThreshold);
                blue = ifsBound(0, blue, 255);

                r[red/5]++;
                g[green/5]++;
                b[blue/5]++;
            }
            MainWindow *w = new MainWindow(this);
            w->plotData(r, g, b, HIST);
            w->show();
        }
        free(dims);
        return;
    }

    if (gIfstool->images[imgIndex]->ifsdims == 3 && ifsdimen(gIfstool->images[imgIndex], 2) == 3) {
        double red = 0, green = 0, blue = 0;

        red = ifsfgp3d(localBuffer, 0, y, x) + rSlider->value();
        red = ifsBound(0, red, 255);
        red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
        red = ifsBound(0, red, 255);

        green = ifsfgp3d(localBuffer, 1, y, x) + gSlider->value();
        green = ifsBound(0, green, 255);
        green = (green - minThreshold) * 255 / (maxThreshold - minThreshold);
        green = ifsBound(0, green, 255);

        blue = ifsfgp3d(localBuffer, 2, y, x) + bSlider->value();
        blue = ifsBound(0, blue, 255);
        blue = (blue - minThreshold) * 255 / (maxThreshold - minThreshold);
        blue = ifsBound(0, blue, 255);

        r = minPixel + (red - 0) * ((maxPixel - minPixel) / (255));
        g = minPixel + (green - 0) * ((maxPixel - minPixel) / (255));
        b = minPixel + (blue - 0) * ((maxPixel - minPixel) / (255));
        f = sqrt(r * r + g * g + b * b);
        ranges.sprintf("\n[%d , %d] \t  [Bright = %6.3f]\t  [R = %6.2f , G = %6.2f , B = %6.2f]", y, x, f, r, g, b);
        parent->addConsoleOutput(ranges);
    } //end color
    else {
        double value = 0;
        value = ifsfgp3d(localBuffer, 0, y, x) + brightSlider->value();
        value = ifsBound(0, value, 255);
        value = (value - minThreshold) * 255 / (maxThreshold - minThreshold);
        value = ifsBound(0, value, 255);

        f = minPixel + (value - 0) * (maxPixel - minPixel)/ 255;
        ranges = QString("\n[%1 , %2] \t  [Bright = %3]").arg(y).arg(x).arg(f);
        parent->addConsoleOutput(ranges);
    }
    free(dims);
}
/**********************************************************************************************************************
 * FUNCTION     : changeImage
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Changes Brightness based on input value
 *********************************************************************************************************************/
void IfstoolChildWindow::changeRGB(int r, int g, int b) {
    //    if((lastR == r) && (lastG == g) && (lastB == b)) {
    //        return;
    //    }
    const QPixmap *pixmap = imageLabel->pixmap();
    QImage image = pixmap->toImage();
    QSize dimensions = image.size();
    int width = dimensions.width(), height = dimensions.height();
    double red = 0, green = 0, blue = 0;
    float maxThreshold = (levelSlider->value() + windowSlider->value());
    float minThreshold = levelSlider->value();
    if(0 == (maxThreshold - minThreshold)) {
        maxThreshold = maxThreshold + 0.0001;
    }

    for (int f1 = 0; f1 < width; f1++) {
        for (int f2 = 0; f2 < height; f2++) {
            if(3 == localBuffer->ifsdims) {
                red = ifsfgp3d(localBuffer, 0, f2, f1) + r;
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                red = ifsBound(0, red, 255);

                green = ifsfgp3d(localBuffer, 1, f2, f1) + g;
                green = ifsBound(0, green, 255);
                green = (green - minThreshold) * 255 / (maxThreshold - minThreshold);
                green = ifsBound(0, green, 255);

                blue = ifsfgp3d(localBuffer, 2, f2, f1) + b;
                blue = ifsBound(0, blue, 255);
                blue = (blue - minThreshold) * 255 / (maxThreshold - minThreshold);
                blue = ifsBound(0, blue, 255);

                image.setPixel(f1, f2, qRgb(red, green, blue));
            } else {
                red = ifsfgp3d(localBuffer, 0, f2, f1) + r;
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                red = ifsBound(0, red, 255);
                image.setPixel(f1, f2, qRgb(red, red, red));
            }
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
    lastR = r;
    lastG = g;
    lastB = b;
}
/**********************************************************************************************************************
 * FUNCTION     : checkIt
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Service routine invoked when color check box is clicked
 *********************************************************************************************************************/
void IfstoolChildWindow::checkIt()
{
    if(!colorCheckBox->isChecked()) {
        DisplayAs2D = 1;
        iSB->setEnabled(true);
        NewFrame(iSB->value());

    }
    else {
        DisplayAs2D = 0;
        iSB->setEnabled(false);
        reset();
    }
}

/**********************************************************************************************************************
 * FUNCTION     : getRGB
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Changes Brightness based on input value
 *********************************************************************************************************************/
void IfstoolChildWindow::getRGB(int &r, int &g, int &b) {
    int brightness = brightSlider->value();
    int warmness = warmSlider->value();
    int coolness = coolSlider->value();

    r = brightness + warmness;
    g = brightness + warmness;
    b = brightness + coolness;
}

/**********************************************************************************************************************
 * FUNCTION     : bright
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Changes Brightness based on input value
 *********************************************************************************************************************/
void IfstoolChildWindow::bright(int currentBright) {
    int r = currentBright;
    int g = currentBright;
    int b = currentBright;
    getRGB(r, g, b);
    changeRGB(r, g, b);
    rSlider->setValue(lastR);
    gSlider->setValue(lastG);
    bSlider->setValue(lastB);
}
/**********************************************************************************************************************
 * FUNCTION     : warm
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Changes Warmness
 *********************************************************************************************************************/
void IfstoolChildWindow::warm(int currentWarm) {
    int r = currentWarm;
    int g = currentWarm;
    int b = 0;
    getRGB(r, g, b);
    changeRGB(r, g, b);
    rSlider->setValue(lastR);
    gSlider->setValue(lastG);
    bSlider->setValue(lastB);
}
/**********************************************************************************************************************
 * FUNCTION     : NewFrame
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Load a different frame
 *********************************************************************************************************************/
void IfstoolChildWindow::NewFrame(int Fno)
{
    // get the specified frame into a new ifs image
    int *dims;
    int row,col;
    float v;
    const QPixmap *pixmap = imageLabel->pixmap();
    QImage image = pixmap->toImage();
    dims=ifssiz(localBuffer);

    for(row=0;row<dims[2];row++)for(col=0;col<dims[1];col++)
    {
        v=ifsfgp3d(localBuffer,Fno,row,col);
        image.setPixel(col,row,qRgb(v, v, v));
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
    ifsToolFrameNumber=iSB->value();
    free(dims);
}

/**********************************************************************************************************************
 * FUNCTION     : cool
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Change Coolness
 *********************************************************************************************************************/
void IfstoolChildWindow::cool(int currentCool) {
    int r = 0;
    int g = 0;
    int b = currentCool;
    getRGB(r, g, b);
    changeRGB(r, g, b);
    rSlider->setValue(lastR);
    gSlider->setValue(lastG);
    bSlider->setValue(lastB);
}

/**********************************************************************************************************************
 * FUNCTION     : changeR
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Change Red
 *********************************************************************************************************************/
void IfstoolChildWindow::changeR(int currentR) {
    int r = 0;
    int g = 0;
    int b = 0;
    getRGB(r, g, b);
    changeRGB(r, g, currentR);
}

/**********************************************************************************************************************
 * FUNCTION     : changeG
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Change Green
 *********************************************************************************************************************/
void IfstoolChildWindow::changeG(int currentG) {
    int r = 0;
    int g = 0;
    int b = 0;
    getRGB(r, g, b);
    changeRGB(r, currentG, b);
}

/**********************************************************************************************************************
 * FUNCTION     : changeB
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Change Blue
 *********************************************************************************************************************/
void IfstoolChildWindow::changeB(int currentB) {
    int r = 0;
    int g = 0;
    int b = 0;
    getRGB(r, g, b);
    changeRGB(r, g, currentB);
}

/**********************************************************************************************************************
 * FUNCTION     : getScaleFromZoom
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Zoom in or out
 *********************************************************************************************************************/
double IfstoolChildWindow::getScaleFromZoom(int currentZoom) {
    double scale = 1.0;
    if (0 == currentZoom) {
        return scale;
    } else if (0 < currentZoom) {
        return (1.0 + currentZoom / 20.0);
    } else {
        return (1.0 + currentZoom / 100.0);
    }
}

/**********************************************************************************************************************
 * FUNCTION     : zoom
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Zoom in or out
 *********************************************************************************************************************/
void IfstoolChildWindow::zoom(int currentZoom) {
    double scale = getScaleFromZoom(currentZoom);
    imageLabel->resize(scale * imageLabel->pixmap()->size());
}

/**********************************************************************************************************************
 * FUNCTION     : level
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Change the level for window and levelling
 *********************************************************************************************************************/
void IfstoolChildWindow::level() {
    changeRGB(rSlider->value(), gSlider->value(), bSlider->value());
}

/**********************************************************************************************************************
 * FUNCTION     : window
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None∫
 * PROCESS      : Change the window for window and levelling
 *********************************************************************************************************************/
void IfstoolChildWindow::window() {
    levelSlider->setRange(0, 255 - windowSlider->value());
    levelSpinBox->setRange(0, 255 - windowSlider->value());
    changeRGB(rSlider->value(), gSlider->value(), bSlider->value());
}

/**********************************************************************************************************************
 * FUNCTION     : profile
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Plots Profile
 *********************************************************************************************************************/
void IfstoolChildWindow::profile() {
    if(true == profileFlag) {
        profileFlag = false;
        profileAct->setIconText("Profiler OFF");
    } else {
        profileFlag = true;
        profileAct->setIconText("Profiler ON");
    }
}

/**********************************************************************************************************************
 * FUNCTION     : histogram
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Plots Histogram
 *********************************************************************************************************************/
void IfstoolChildWindow::histogram() {
    if(true == histFlag) {
        histFlag = false;
        histAct->setIconText("Histogram OFF");
    } else {
        histFlag = true;
        histAct->setIconText("Histogram ON");
    }
}

/**********************************************************************************************************************
 * FUNCTION     : save
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Saves Modified data back to buffer pool
 *********************************************************************************************************************/
void IfstoolChildWindow::save() {
    IfstoolMainWindow *parent = (IfstoolMainWindow *) _parent;
    int bufferLocation = parent->getIndexFromChildPtr(this);
    int *dims = NULL;
    dims = ifssiz(gIfstool->images[bufferLocation]);
    float red = 0, blue = 0, green = 0;
    float maxThreshold = (levelSlider->value() + windowSlider->value());
    float minThreshold = levelSlider->value();
    if(0 == (maxThreshold - minThreshold)) {
        maxThreshold = maxThreshold + 0.0001;
    }

    for (int f1 = 0; f1 < dims[1]; f1++) {
        for (int f2 = 0; f2 < dims[2]; f2++) {
            if(3 == localBuffer->ifsdims) {
                red = ifsfgp3d(localBuffer, 0, f2, f1) + rSlider->value();
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                red = ifsBound(0, red, 255);

                green = ifsfgp3d(localBuffer, 1, f2, f1) + gSlider->value();
                green = ifsBound(0, green, 255);
                green = (green - minThreshold) * 255 / (maxThreshold - minThreshold);
                green = ifsBound(0, green, 255);

                blue = ifsfgp3d(localBuffer, 2, f2, f1) + bSlider->value();
                blue = ifsBound(0, blue, 255);
                blue = (blue - minThreshold) * 255 / (maxThreshold - minThreshold);
                blue = ifsBound(0, blue, 255);

                ifsfpp3d(gIfstool->images[bufferLocation], 0, f2, f1, minPixel + (red - 0) * (maxPixel - minPixel)/ 255);
                ifsfpp3d(gIfstool->images[bufferLocation], 1, f2, f1, minPixel + (green - 0) * (maxPixel - minPixel)/ 255);
                ifsfpp3d(gIfstool->images[bufferLocation], 2, f2, f1, minPixel + (blue - 0) * (maxPixel - minPixel)/ 255);
            } else {
                red = ifsfgp3d(localBuffer, 0, f2, f1) + brightSlider->value();
                red = ifsBound(0, red, 255);
                red = (red - minThreshold) * 255 / (maxThreshold - minThreshold);
                ifsfpp3d(gIfstool->images[bufferLocation], 0, f2, f1, minPixel + (red - 0) * (maxPixel - minPixel)/ 255);
            }
        }
    }


}
/**********************************************************************************************************************
 * FUNCTION     : reset
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Resets any modification made to the image before saving
 *********************************************************************************************************************/
void IfstoolChildWindow::reset() {
    brightSlider->setValue(0);
    warmSlider->setValue(0);
    coolSlider->setValue(0);
    rSlider->setValue(0);
    gSlider->setValue(0);
    bSlider->setValue(0);
    windowSlider->setValue(254);
    windowSlider->setValue(255);
    levelSlider->setValue(0);
}

/**********************************************************************************************************************
 * FUNCTION     : BrScale
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Scals the input image to a give min and max thresholds
 *********************************************************************************************************************/
int IfstoolChildWindow::BrScale(IFSIMG inimg, IFSIMG outimg, float upper, float lower, float *max, float *min) {
    int i;
    int numpixels;
    float *ptri, *ptro;
    if (inimg->dtype != IFST_32FLT)
        return -1;
    if (outimg->dtype != IFST_32FLT)
        return -2;
    if (upper < lower)
        return -3;
    numpixels = ifsdimen(inimg, 0) * ifsdimen(inimg, 1);
    if (inimg->ifsdims == 3)
        numpixels = ifsdimen(inimg, 2) * numpixels;
    ptri = (float *) inimg->ifsptr;
    ptro = (float *) outimg->ifsptr;
    float localMax = -100000000;
    float localMin = 100000000;
    for (i = 0; i < numpixels; i++) {
        if (*ptri > localMax)
            localMax = *ptri;
        if (*ptri < localMin)
            localMin = *ptri;
        ptri++;
    }

    ptri = (float *) inimg->ifsptr;
    ptro = (float *) outimg->ifsptr;

    for (i = 0; i < numpixels; i++) {
        *ptro++ = (lower) + ((upper - lower) * (*ptri++ - localMin)) / (localMax - localMin);
    }
    *max = localMax;
    *min = localMin;
    return 0;

}

/**********************************************************************************************************************
 * FUNCTION     : BrScale
 * CLASS        : IfstoolChildWindow
 * TYPE         : Member function
 * IN           : event
 * OUT          : None
 * PROCESS      : Scals the input image to a give min and max thresholds
 *********************************************************************************************************************/
double IfstoolChildWindow::ifsBound(double min, double data, double max) {
    if(data < min) {
        return min;
    } else if(data > max) {
        return max;
    } else {
        return data;
    }
}

//Nothing past this point
