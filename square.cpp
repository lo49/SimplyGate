// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image


#include <QApplication>
#include <tiffio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <tesseract/baseapi.h>
#include <tesseract/tesseractmain.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include "square.h"
#include <math.h>
using namespace cv;
using namespace std;

#define ERREUR 10

bool affichage = false;

int thresh = 1, N = 11;


// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double angle( CvPoint pt1,CvPoint pt2, CvPoint pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}


// the function draws all the squares in the image
 void drawSquares( Mat& image,Mat& img_read, const vector<vector<Point> >& squares,const char* wndname)
{
      Scalar couleur;
    for( size_t i = 0; i < squares.size(); i++ )
    {
        int dHeigh_g = squares[i][1].y - squares[i][0].y ;
        int dHeigh_d = squares[i][2].y - squares[i][3].y ;
        int dWidth_h = squares[i][3].x - squares[i][0].x ;
        int dWidth_b = squares[i][2].x - squares[i][1].x ;


        double rapport_dh =(double)dHeigh_d/(double)dWidth_h;
        double rapport_gb =(double)dHeigh_g/(double)dWidth_b;
        //cout << " rapport droite/haut -> "<< rapport_dh<<"\n" << " rapport gauche/bas -> "<<rapport_gb<<"\n"<<endl;
        if(abs(rapport_dh)< 0.25 && abs(rapport_dh) > 0.15 && abs(rapport_gb)< 0.25 && abs(rapport_gb) > 0.15 ){

            couleur = Scalar(255,255,0);

            //// ------------------ Region Of Interest

            Rect roi = Rect(squares[i][0].x,squares[i][0].y,dWidth_b,dHeigh_d); //( x,y,width,height)
            // ------------------------------------------------------------------------------------------------------------------------------------
            // 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows <<<<----- à vérifier !!
            // ------------------------------------------------------------------------------------------------------------------------------------
            //cout << "roi.x "<< roi.x<< " roi.y "<<roi.y<< " roi.width " << roi.width << " roi.height "<< roi.height <<endl;

            img_read = image(roi);
    //        cvtColor(image_read, roiImg, CV_RGB2GRAY);
    //        equalizeHist(roiImg,roiImg);
            if(affichage){
            imshow("roi",img_read);
            }

        }
       // cout<< "Rapport" << abs(rapport)<<endl;




        //polylines(image, InputArrayOfArrays pts, bool isClosed, const Scalar& color, int thickness=1, int lineType=8, int shift=0
    }
    if(affichage){
        imshow(wndname, image);
    }
   }

 void rangeByMin(vector<vector<Point> >& squares){

// On définit un vector vector point tmp.
   //  vector<vector<Point> > tmp(squares.size());

     // On parcourt la boucle de tout les rectangles :
        for(uint i=0;i<squares.size();i++){


    /*  - Le point en haut à gauche doit être le plus petit x et le plus petit y
        - Le point en bas à gauche doit être le plus petit x et avec le plus grand y
        - Le point en bas à droite doit être le plus grand x et avec le plus grand y
        - le point en haut à droite doit être le plus grand x et le plus petit y*/
        Point hg,bg,bd,hd;

        // On parcours fois les éléments en faisant point :
        Point tmp;
        for(int p = 0;p<2;p++){
            tmp.x = (int)NULL;
            tmp.y = (int)NULL;
            switch (p) {
            case 0 :

                // Le point hg (Haut Gauche) :
                // Le point bg (Bas Gauche) :
                    for(uint e =0;e<squares[i].size();e++){

                        if(tmp.x == (int)NULL && tmp.y == (int)NULL){
                            tmp = squares[i][e];
                        }else{
                            // Si le point e est inférieur avec l'erreur on est alors potentiellement à gauche
                            if(tmp.x+ERREUR>squares[i][e].x){
                                // On regardes alors les y à savoir le quel est le plus petit
                                if(tmp.y>squares[i][e].y){
                                    // Le point BG nouveau est le squares[i] && HG le tmp
                                    hg=squares[i][e];
                                    bg=tmp;

                                }
                                else{
                                    // Sinon c'est l'inverse.
                                    hg=tmp;
                                    bg=squares[i][e];
                                }
                                tmp = squares[i][e];
                            }
                        }
                    }

                break;
             case 1:

            // Le Point hd et bd ;) On fait la même sauf que l'on soustrait l'erreur et on regarde si
                for(uint u =0;u<squares[i].size();u++){
                    if(tmp.x == (int)NULL && tmp.y == (int)NULL){
                        tmp = squares[i][u];
                    }else{
                        // Le point u est supérieur avec l'erreur on est alors potentiellement à droite
                        if(tmp.x-ERREUR<squares[i][u].x){
                            // On regardes alors les y à savoir le quel est le plus petit
                            if(tmp.y>squares[i][u].y){
                                // Le point BG nouveau est le squares[i] && HG le tmp
                                hd=squares[i][u];
                                bd=tmp;

                            }
                            else{
                                // Sinon c'est l'inverse.
                                hd=tmp;
                                bd=squares[i][u];
                            }
                            tmp = squares[i][u];
                        }
                    }
                }

                break;
            }


         }
        // cout << " Trie finis"<< " " << hg <<" "<<bg<<" "<<bd<<" "<<hd<<endl;
        for(int h =0;h<4;h++){
            switch (h) {
            case 0:
                    squares[i][h] = hg;
                break;
            case 1:
                    squares[i][h] = bg;
                break;
            case 2:
                    squares[i][h] = bd;
                break;
            case 3:
                    squares[i][h] = hd;
                break;
            default:
                break;
            }


        }
     }

 }
