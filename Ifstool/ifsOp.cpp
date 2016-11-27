/*
 * ifsOp.cpp
 * Created :    May 28 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 Revised June 6
 */

#include "ifsOp.h"

/**********************************************************************************************************************
 * FUNCTION     : IfsOp
 * CLASS        : IfsOp
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : None
 *********************************************************************************************************************/
IfsOp::IfsOp(QWidget *parent) {
    this->parent = parent;
}
/**********************************************************************************************************************
 * FUNCTION     : binary
 * CLASS        : IfsOp
 * TYPE         : Member Function
 * IN           : opcode, inImg1, inImg2,outImg
 * OUT          : None
 * PROCESS      : Handles binary operation on inImg2 and inImg2 and stores in outImg based on opcode
 *********************************************************************************************************************/
void IfsOp::binary(int opcode, IFSIMG &inImg1, IFSIMG &inImg2, IFSIMG &outImg) {
    int status;
    QMessageBox msgBox;
    if (0 == inImg1) {

        msgBox.setText("Input1 empty");
        msgBox.exec();
        return;
    } else if (0 == inImg2) {

        msgBox.setText("Input2 empty");
        msgBox.exec();
        return;
    }

    if (ifsdimen(inImg1, 0) != ifsdimen(inImg2, 0) || ifsdimen(inImg1, 1) != ifsdimen(inImg2, 1)) {
        msgBox.setText("BinaryOps:Input images are different sizes");
        msgBox.exec();
        return;
    }

    switch (opcode) {
    case OPADD:
        fladdv(inImg1, inImg2, outImg);
        break;

    case OPSUB:
        flsubv(inImg1, inImg2, outImg);
        break;

    case OPMUL:
        status = flmultv(inImg1, inImg2, outImg);
        printf("flmutlv return %d\n", status);
        break;

    case OPDIV:
        fldivv(inImg1, inImg2, outImg);
        break;
    } // end switch
} // end binary

/**********************************************************************************************************************
 * FUNCTION     : unary
 * CLASS        : IfsOp
 * TYPE         : Member Function
 * IN           : opcode, inImg, outImg
 * OUT          : None
 * PROCESS      : Handles unary operation on inImg and stores in outImg based on opcode
 *********************************************************************************************************************/
void IfsOp::unary(int opcode, IFSIMG &inImg, IFSIMG &outImg) {
    int status = 0, iv, ifr;
    float max, min, row, col,sigma;
    int rememberdims;
    int dims[4];
    bool ok;
    IFSIMG temp1, temp2, temp3;
    QMessageBox msgBox;
    if (0 == inImg) {

        msgBox.setText("Input empty");
        msgBox.exec();
        return;
    }
    switch (opcode) {
    case OPDX:

        sigma = QInputDialog::getInt(parent, QObject::tr("Dx"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,1,0);
        break;

    case OPDXX:
        sigma = QInputDialog::getInt(parent, QObject::tr("Dxx"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,2,0);
        break;

    case OPDY:
        sigma = QInputDialog::getInt(parent, QObject::tr("Dy"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,1,1);
        break;

    case OPDYY:
        sigma = QInputDialog::getInt(parent, QObject::tr("Dyy"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,2,1);
        break;

    case OPDXY:
        sigma = QInputDialog::getInt(parent, QObject::tr("Dxy"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,2,2);
        break;

    case OPCUR:
        flcurv(inImg, outImg);
        break;

    case OPINV:
        flinv(inImg, outImg);

        break;

    case OPNATLOG:
        flln(inImg, outImg);
        break;

    case OPLAP:
        bool ok;
        float v;
        v = QInputDialog::getDouble(parent, QObject::tr("Laplacian"), QObject::tr("sigma (-neg. means squared)"), 0,
                -255.0, 255.0, 2, &ok);
        flDoG(inImg, outImg, v, 0, 3);
        break;

    case OPHISTEQ:
        flcp(inImg, outImg);
        status = HistogramSub(outImg, 20);
        break;

    case OPPSEUDO: {
        //pseudocolor
        dims[0] = inImg->ifsdims;
        dims[1] = ifsdimen(inImg, 0); //cols
        dims[2] = ifsdimen(inImg, 1); //rows
        dims[3] = 2; //frames

        iv = QInputDialog::getInt(parent, QObject::tr("Pseudocolor"), QObject::tr("Color map number"), 0, 0.0, 6.0, 1,
                &ok);

        if (inImg->ifsdims != 2) {
            dims[0] = 2;

            ifr = QInputDialog::getInt(parent, QObject::tr("Pseudocolor"), QObject::tr("Input Frame No"), 0, 0.0, 255.0,
                    1, &ok);
            if (ifr >= dims[3]) {
                QMessageBox msgBox;
                msgBox.setText("Only 6 color maps are defined.");
                msgBox.exec();
                return;
            }
            temp1 = ifsslice(inImg, (char *) "f", ifr); // extract the working float frame
            dims[0] = 2;
            temp2 = ifscreate((char *) "u8bit", dims, IFS_CR_ALL, 0);
            ifsvidscale(temp1, temp2, &max, &min, 0);
            ifsfree(temp1, IFS_FR_ALL);
        } else {
            dims[0] = 2;
            temp2 = ifscreate((char *) "u8bit", dims, IFS_CR_ALL, 0);
            ifsvidscale(inImg, temp2, &max, &min, 0);
        }

        rememberdims = dims[0];
        dims[0] = 3;
        dims[3] = 3;
        temp3 = ifscreate((char *) "u8bit", dims, IFS_CR_ALL, 0); // make output image 3D
        printf("point 2\n");
        fflush (stdout);
        ifscolormap(temp2, temp3, iv); // produces a 3d 8 bit image

        ifsany2any(temp3, outImg);

        ifsfree(temp2, IFS_CR_ALL);
        ifsfree(temp3, IFS_CR_ALL);
    } // end pseudocolor
        break;
    case OPBLUR:
        sigma = QInputDialog::getInt(parent, QObject::tr("Blur"), QObject::tr("Sigma of blur"), 0, 0.0, 255.0,
                1, &ok);
        flDoG(inImg,outImg,sigma,0,0);

        break;
    case OPC2MAG: {
        status = ifsc2mag(inImg, outImg);
        if (status != 0) {
            QMessageBox msgBox;
            msgBox.setText("Error returned by ifsc2mag");
            msgBox.exec();
        }
    }
        break;
    case OPC2PHASE:
        status = ifsc2phase(inImg, outImg);
        status = ifsc2phase(inImg, outImg);
        if (status != 0) {
            QMessageBox msgBox;
            msgBox.setText("Error returned by ifsc2phase");
            msgBox.exec();
        }
        break;

    case OPC2REAL:
        status = ifsc2real(inImg, outImg);
        status = ifsc2real(inImg, outImg);
        if (status != 0) {
            QMessageBox msgBox;
            msgBox.setText("Error returned by ifsc2real");
            msgBox.exec();
        }
        break;

    case OPC2IMAG:
        status = ifsc2imag(inImg, outImg);
        status = ifsc2imag(inImg, outImg);
        if (status != 0) {
            QMessageBox msgBox;
            msgBox.setText("Error returned by ifsc2imag");
            msgBox.exec();
        }
        break;

    case OPFFT:
        int numpixels, i;
        float *ptri, *ptro;

        dims[0] = 2;
        dims[1] = ifsdimen(inImg, 0);
        dims[2] = ifsdimen(inImg, 1);
        // notice that if the input is 3d, including color,  only the first
        // frame will be passed to fft.
        temp1 = ifscreate((char *) "32cmp", dims, IFS_CR_ALL, 0);
        numpixels = dims[1] * dims[2];
        ptri = (float *) inImg->ifsptr;
        ptro = (float *) temp1->ifsptr;
        for (i = 0; i < numpixels; i++) {
            *ptro++ = *ptri++; // set real part to input image
            *ptro++ = 0.0; // set imaginary part to zero
        }
//        printf("unary:calling fft\n");
//      fflush (stdout);
        status = ifscfft2d(temp1,-1);// take FFT
//        printf("unary:returned from fft\n");
//        fflush(stdout);

        switch (status)  {
        case 0: break;
        case -1:

            msgBox.setText("FFT:Image must be square");
            msgBox.exec();
            return;
        case -2:

            msgBox.setText("FFT:Image dimensions must be a power of 2" );
            msgBox.exec();
            return;
        case -3:

            msgBox.setText("FFT:Image must be complex float");
            msgBox.exec();
            return;
        }

        flcp(temp1, outImg); // flcp works on complex data
        ifsfree(temp1, IFS_FR_ALL);
        break;

    case OPIFFT:
        //already have a complex image in
        dims[0] = 2;
        dims[1] = ifsdimen(inImg, 0);
        dims[2] = ifsdimen(inImg, 1);

        temp1 = ifscreate((char *) "32cmp", dims, IFS_CR_ALL, 0);
        flcp(inImg, temp1); // flcp works on complex data
        status = ifscfft2d(temp1,1);// take inverse FFT

        switch (status)  {
        case 0: break;
        case -1:

            msgBox.setText("FFT:Image must be square");
            msgBox.exec();
            return;
        case -2:

            msgBox.setText("FFT:Image dimensions must be a power of 2" );
            msgBox.exec();
            return;
        case -3:

            msgBox.setText("FFT:Image must be complex float");
            msgBox.exec();
            return;
        }
        flcp(temp1, outImg);        // flcp works on complex data
        ifsfree(temp1, IFS_FR_ALL);
        break;

    case OPR2C: // convert real to complex
        dims[1] = ifsdimen(inImg, 0);
        dims[2] = ifsdimen(inImg, 1);
        for (row = 0; row < dims[2]; row++)
            for (col = 0; col < dims[1]; col++)
                ifsfpp(outImg, row, col, ifsfgp(inImg, row, col));
        break;
    case OPEXTRACTF:
        break; // not yet coded
    } // end of switch
}
//Nothing past this point
