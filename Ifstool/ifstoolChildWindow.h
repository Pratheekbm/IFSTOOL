/*
 * ifstoolChildWindow.h
 * Created :    May 23 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */
#ifndef IFSTOOLCHILDWINDOW_H
#define IFSTOOLCHILDWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <qdockwidget.h>
#include <QSlider>
#include "generictypes.h"
#include "plotMainWindow.h"
//class SlidersGroup;

/**********************************************************************************************************************
 * FUNCTION		: -
 * CLASS		: IfstoolChildWindow
 * TYPE			: QMainWindow
 * IN			: -
 * OUT			: -
 * PROCESS		: Definition of IfstoolChildWindow class
 *********************************************************************************************************************/

class IfstoolChildWindow : public QMainWindow
{
    Q_OBJECT
public:
    IFSIMG localBuffer;
    float maxPixel;
    float minPixel;
    int ifsToolFrameNumber;
    bool profileFlag;
    bool histFlag;
    QVector <int> xCoOrd;
    QVector <int> yCoOrd;
    IfstoolChildWindow(QWidget *parent, QImage *imageToDisplay, int bufferLocation);
    ~IfstoolChildWindow();
    void createActions();
    void createToolBar();
    void addLayoutElements();
    void getRGB(int &r, int &g, int &b);
    void changeRGB(int deltaR, int deltaG, int deltaB);
    double getScaleFromZoom(int currentZoom);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    int BrScale(IFSIMG inimg,IFSIMG outimg,float upper,float lower, float *max, float *min);
    double ifsBound(double min, double data, double max);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:
    void bright(int);
    void warm(int);
    void cool(int);
    void changeR(int);
    void changeG(int);
    void changeB(int);
    void zoom(int);
    void level();
    void window();
    void profile();
    void histogram();
    void save();
    void reset();
    void NewFrame(int);
    void checkIt();

private:
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QImage *incomingQImage;
    void *_parent;

    int lastBright;
    int lastWarm;
    int lastCool;
    int lastR;
    int lastG;
    int lastB;
    int lastZoom;
    int lastLevelLow;
    int lastLevelHigh;
    int DisplayAs2D;

    QToolBar *editToolBar;

    QAction *profileAct;
    QAction *histAct;
    QAction *resetAct;
    QAction *saveAct;

    QDockWidget *rightPanel;
    QGroupBox *rightPanelGrpBox;
    QVBoxLayout *rightPanelVLayout;
    QGroupBox *displayPanelGrpBox;
    QGridLayout *displayPanelGridLayout;
    QGroupBox *rgbPanelGrpBox;
    QGridLayout *rgbPanelGridLayout;
    QGroupBox *editPanelGrpBox;
    QGridLayout *editGridLayout;

    QPushButton *profileButton;
    QPushButton *histogramButton;
    QPushButton *resetButton;
    QPushButton *saveButton;

    QCheckBox *colorCheckBox;
    QSlider *brightSlider;
    QSlider *warmSlider;
    QSlider *coolSlider;
    QSlider *rSlider;
    QSlider *gSlider;
    QSlider *bSlider;
    QSlider *zoomSlider;
    QSlider *levelSlider;
    QSlider *windowSlider;

    QLabel *frameLabel;
    QLabel *brightLabel;
    QLabel *warmLabel;
    QLabel *coolLabel;
    QLabel *rLabel;
    QLabel *gLabel;
    QLabel *bLabel;
    QLabel *zoomLabel;
    QLabel *levelLabel;
    QLabel *windowLabel;

    QSpinBox *brightSpinBox;
    QSpinBox *iSB;
    QSpinBox *warmSpinBox;
    QSpinBox *coolSpinBox;
    QSpinBox *rSpinBox;
    QSpinBox *gSpinBox;
    QSpinBox *bSpinBox;
    QSpinBox *zoomSpinBox;
    QSpinBox *levelSpinBox;
    QSpinBox *windowSpinBox;
};

#endif // IFSTOOLCHILDWINDOW_H
//Nothing past this point
