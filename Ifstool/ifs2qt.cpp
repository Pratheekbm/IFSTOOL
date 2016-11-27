/*
 * ifs2Qt.cpp
 * Created :    May 16 2015
 * Authors :    Pratheek Mallikarjuna
 *              Wesley Snyder
 * Copyright (c) 2015. All rights reserved.
 */
#include "generictypes.h"
#include "ifs2qt.h"

/**********************************************************************************************************************
 * FUNCTION     : Ifs2Qt
 * CLASS        : Ifs2Qt
 * TYPE         : Constructor
 * IN           : None
 * OUT          : None
 * PROCESS      : None
 *********************************************************************************************************************/
Ifs2Qt::Ifs2Qt() {

}

/**********************************************************************************************************************
 * FUNCTION     : copyFrameInto
 * CLASS        : Ifs2Qt
 * TYPE         : Member Function
 * IN           : IFSIMG: w1,  FrameNumber
 * OUT          : w2
 * PROCESS      : copies a frame of a 2d image into a 3d image
 * Description  : copyframeinto(IFSIMG input, IFSIMG output,int f)
 *                f is the frame number in the output image
 *********************************************************************************************************************/
void Ifs2Qt::copyFrameInto(IFSIMG w1, IFSIMG w2, int frameNumber) {

    int pixelsPerFrame = 0, i = 0;
    int *dims = NULL;
    float *w1ptr = NULL, *w2ptr = NULL;

    dims = ifssiz(w1);
    pixelsPerFrame = dims[1] * dims[2];
    w1ptr = (float *) w1->ifsptr; // pointer to input
    w2ptr = (float *) w2->ifsptr + pixelsPerFrame * frameNumber; // pointer to output

    for (i = 0; i < pixelsPerFrame; i++) {
        *w2ptr++ = *w1ptr++;
    }
    free(dims);
} // end function copyframeinto
/**********************************************************************************************************************
 * FUNCTION     : normimage
 * CLASS        : Ifs2Qt
 * TYPE         : Member Function
 * IN           : IFSIMG im
 * OUT          :  QImage *img
 * PROCESS      : takes a 3frame, 3d ifs image vidscales the input and returns a qt color image
 * Description  : None
 *********************************************************************************************************************/
void Ifs2Qt::normimage(IFSIMG im, QImage *img1) {
    int *dims;
    float min, max;
    int row, col, r, g, b, framesize, nr, nc;
    unsigned char *iptr;
    IFSIMG temp1;
    dims = ifssiz(im);
    nr = dims[2];
    nc = dims[1];
    temp1 = ifscreate((char *) "u8bit", dims, IFS_CR_ALL, 0);
    ifsvidscale(im, temp1, &max, &min, 0); //video scale image
    iptr = (unsigned char *) temp1->ifsptr; // point to input image
    framesize = nr * nc;
    for (row = 0; row < nr; row++)
        for (col = 0; col < nc; col++) {
//    r = ifsigp3d(work, 0, row, col);
            r = *iptr;
//    g = ifsigp3d(work, 1, row, col);
            g = *(iptr + framesize);
//    b = ifsigp3d(work, 2, row, col);
            b = *(iptr + 2 * framesize);
            img1->setPixel(col, row, qRgb(r, g, b));
            iptr++;
        }
} // end of function normimage

/**********************************************************************************************************************
 * FUNCTION     : ifs3d3qt3d
 * CLASS        : Ifs2Qt
 * TYPE         : Member Function
 * IN           : IFSIMG: im
 * OUT          : w2
 * PROCESS      : takes a 3frame, 3d ifs image and returns a qt color image
 * Description  : None
 *********************************************************************************************************************/
QImage* Ifs2Qt::ifs3d2qt3d(IFSIMG im) {
    // now convert that to Qt
    // create a buffer of ints
    IFSIMG work;
    int epsilon = 0, pad = 0, oldw = 0;
    int frame = 0, row = 0, col = 0;
    int *dims = NULL;

    dims = ifssiz(im);
    epsilon = *(dims + 1) % 4;    // check if num cols divisible by 4

    // image col length already dvisible by 4
    if (epsilon != 0) {
        // here if num cols not divisible by 4
        pad = 4 - epsilon; // amount to pad

        oldw = dims[1]; // remember what it used to be
        dims[1] += pad; // make the col length longer
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0); // create a new image
        for (frame = 0; frame < 3; frame++) {
            for (row = 0; row < dims[2]; row++) {
                for (col = 0; col < oldw; col++) {
                    ifsfpp3d(work, frame, row, col, ifsfgp3d(im, frame, row, col));
                }
                for (int i = col; i < col + pad; i++) {
                    ifsfpp3d(work, frame, row, i, ifsfgp3d(im, frame, row, col - 1));
                }
            }
        }
        QImage *img1 = new QImage(dims[1], dims[2], QImage::Format_RGB32);
        /*---------------------------brightness normalization------------------*/
        /* at this point, the input image, whatever its format originally,     */
        /* is now a 3d, 3frame float image                                     */
        /* call a function to video scale the image                            */
        normimage(work, img1);
        ifsfree(work, IFS_FR_ALL);
        return img1;
    } else {
        QImage *img1 = new QImage(dims[1], dims[2], QImage::Format_RGB32);
        //Copy data to QImage directly from im
        // and vidscale
        normimage(im, img1);
        return img1;
    }         // end else

    free(dims);

}

/**********************************************************************************************************************
 * FUNCTION     : ifs2Qt
 * CLASS        : Ifs2Qt
 * TYPE         : Member Function
 * IN           : IFSIMG:img. int:complexMode,frameNumber
 *              : complex mode is what the user wants to display, 0=mag, 1-phase, 2=real,3=imag
 * OUT          : Qt image
 * PROCESS      : takes an ifs image of type float, returns a Qt image
 * Description  : None
 *********************************************************************************************************************/
QImage* Ifs2Qt::ifs2Qt(IFSIMG img, int complexMode, int frameNumber) {
    IFSIMG work, work2;
    int *dims = NULL;

    dims = ifssiz(img);

    //color
    if (img->ifsdims == 3 && ifsdimen(img, 2) == 3) {
        // check for color
        // process ifs color image
        return ifs3d2qt3d(img);
    }

    //2d complex
    if (img->ifsdims == 2 && img->dtype == IFST_32CMP) {
        // process  2d complex image
        work2 = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        dims[0] = 3;
        dims[3] = 3; // create a color ifs image
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        dims[0] = 2; // back to 2d
        switch (complexMode) {
        case (0):
            ifsc2mag(img, work2);
            break;
        case (1):
            ifsc2phase(img, work2);
            break;
        case (2):
            ifsc2real(img, work2);
            break;
        case (3):
            ifsc2imag(img, work2);
            break;
        }
        // now work2 contains a 2d real image
        // make a 3d ifsimage
        copyFrameInto(work2, work, 0);
        copyFrameInto(work2, work, 1);
        copyFrameInto(work2, work, 2);
        ifsfree(work2, IFS_FR_ALL);
    }

    //2d noncomplex
    if (img->ifsdims == 2 && img->dtype != IFST_32CMP) {
        // process 2d, non complex image
        dims[0] = 3;
        dims[3] = 3; // create a color ifs image
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        dims[0] = 2; // back to 2d
        copyFrameInto(img, work, 0);
        copyFrameInto(img, work, 1);
        copyFrameInto(img, work, 2);
    }

    //3d non color
    if (img->ifsdims == 3 && ifsdimen(img, 2) != 3) {
        // process 3d, non color image
        dims[0] = 3;
        dims[3] = 3; // create a color ifs image
        work = ifscreate((char *) "float", dims, IFS_CR_ALL, 0);
        dims[0] = 2; // back to 2d
        // extract a slice from input image
        work2 = ifsslice(img, (char *) "f", frameNumber);
        copyFrameInto(work2, work, 0);
        copyFrameInto(work2, work, 1);
        copyFrameInto(work2, work, 2);
        ifsfree(work2, IFS_FR_ALL);
    }// end of case 3D non color, but 3 frames.
    //3D not 3 frames


    // at this point, the input image has been converted to a single
    //ifs 3dframe, 3d image
    return ifs3d2qt3d(work);

}
//Nothing past this point
