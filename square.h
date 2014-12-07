#ifndef SQUARE_H
#define SQUARE_H

#include <QApplication>
#include <tiffio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <tesseract/baseapi.h>
#include <tesseract/tesseractmain.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

double angle(CvPoint pt1, CvPoint pt2, CvPoint pt0 );
void findSquares(const Mat& image, vector<vector<Point> >& squares );
 void drawSquares( Mat& image,Mat& img_read, const vector<vector<Point> >& squares,const char* wndname);
void rangeByMin(vector<vector<Point> >& squares);
#endif // SQUARE_H
