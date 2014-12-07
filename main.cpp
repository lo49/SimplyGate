//#include "mainwindow.h"
#include <QApplication>
#include <tiffio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <tesseract/baseapi.h>
#include <tesseract/tesseractmain.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include "square.h"


//// Maths methods
//#define max(a, b) ((a) > (b) ? (a) : (b))
//#define min(a, b) ((a) < (b) ? (a) : (b))
//#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)




const char* wndname = "Square Detection Demo";


const char* wndname2 = "Test luminosité";

// Step mooving for object min & max
#define STEP_MIN 5
#define STEP_MAX 100
using namespace std;
IplImage *image;
bool traitement = FALSE;
// Position of the object we overlay
CvPoint objectPos = cvPoint(-1, -1);
// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0, tolerance = 40;



 using namespace tesseract;




void tess(Mat image,char* rep){



    if(image.data){
    tesseract::TessBaseAPI api;

    if (api.Init("/usr/include/tesseract/tessdata/", "eng", tesseract::OEM_DEFAULT))  {
        fprintf(stderr, "Could not initialize tesseract.\n");
      }

     cout << " Tess"<< endl;

    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());

    char * outText;

    outText = api.GetUTF8Text();
    char * text_corri = new char(100);

    int i = 0;
    int t =0;
    // On parcours le pointeur outText ;)
    while(*outText != '\0'){
        i++;

        //cout << *outText<< " char <-> int "<< (int)*outText<<endl;
        if((65<=(int)*outText && (int)*outText<=90) || (48 <= (int)*outText && (int)*outText <=57) || (int)*outText == 45){
            *text_corri = *outText;
            text_corri++;
            t++;
        }
         outText++;

    }
    for(int u=0;u<i;u++){
        outText--;
    }
    for(int r=0;r<t;r++){
        text_corri--;
    }
    cout<< "OCR Output :"<< outText <<endl;
    cout<< "Correction : "<< text_corri<<endl;
//        printf("OCR output: %s \n", outText);
//        printf("Correction: %s \n", text_corri);

    }
}


int main(int argc, char *argv[])
{


    // Image & hsvImage
   // IplImage *hsv;
    // Video Capture
    CvCapture *capture;
    // Key for keyboard event
    char key;

    // Initialize the video Capture (200 => CV_CAP_V4L2)
    capture = cvCreateCameraCapture(200);

    // Check if the capture is ok

     vector<vector<Point> > squares;
     //Mat image = imread("/home/sotchi/Documents/ENIB/openCV/example/pic1.png", 1);
     Mat image = (Mat)cvQueryFrame(capture);
     Mat img_read;


     namedWindow( wndname, 1 );

  char * reponse;


//     findSquares(image, squares);
//     rangeByMin(squares);
//     drawSquares(image,img_read, squares,wndname);
//     tess(img_read,reponse);

    // While we don't want to quit

    while(key != 'Q' && key != 'q') {

        // We get the current image
        image = (Mat)cvQueryFrame(capture);





        findSquares(image, squares);
        rangeByMin(squares);
        drawSquares(image,img_read, squares,wndname);
        tess(img_read,reponse);
        // We wait 10 ms
        key = cvWaitKey(10);

    }

    // Destroy the windows we have created
    cvDestroyWindow("GeckoGeek Color Tracking");
    cvDestroyWindow("GeckoGeek Mask");
    cvDestroyWindow(wndname);
    // Destroy the capture
    cvReleaseCapture(&capture);





//    waitKey();






    QApplication a(argc, argv);
   // MainWindow w;
   //w.show();

    return a.exec();
}
