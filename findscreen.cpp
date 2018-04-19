//Lai Yongtian
//2018-3-29
#include "findscreen.h"
#include <QtCore>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


#define IPHONESEX (640.0/2)
#define IPHONESEY (1136.0/2)


FindScreen::FindScreen()
{

}

bool isIPhoneScreen(RotatedRect rotatedRect, Point2f p[], Point2f screenCenter)
{
    Point2f p_t[4];
    //find three point in 1,2,3 phase
    rotatedRect.points(p_t);
    int index[3]={4,4,4};
    for(int i=0;i<4;i++)
    {
        if((p_t[i]-screenCenter).x < 0 && \
                (p_t[i]-screenCenter).y < 0 )
            index[0]=i;
        if((p_t[i]-screenCenter).x > 0 && \
                (p_t[i]-screenCenter).y < 0 )
            index[1]=i;
        if((p_t[i]-screenCenter).x > 0 && \
                (p_t[i]-screenCenter).y > 0 )
            index[2]=i;
    }
    for(int i=0;i<3;i++)
    {
        if(index[i]==4)
            return false;
        p[i]=p_t[index[i]];
    }

    //find is it an iphone
    double a=((p[0]-p[1]).x)*((p[0]-p[1]).x)+((p[0]-p[1]).y)*((p[0]-p[1]).y);
    double b=((p[1]-p[2]).x)*((p[1]-p[2]).x)+((p[1]-p[2]).y)*((p[1]-p[2]).y);
    double ratioOfScreen = a/b;

    if(abs(ratioOfScreen - (IPHONESEX*IPHONESEX)/(IPHONESEY*IPHONESEY))<0.2 ||\
            abs(ratioOfScreen - (IPHONESEY*IPHONESEY)/(IPHONESEX*IPHONESEX))<0.2 )
    {
        qDebug("%f",ratioOfScreen);
        return true;
    }
    return false;
}

//return the warp mat and size that you can warp it
Mat FindScreen::find(Mat mat)
{
    //mat->screen
    int rows = mat.rows, cols = mat.cols;
    Point2f screenCenter(cols/2,rows/2);

    Mat gray,edges,black;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //vector<RotatedRect> rects;
    Point2f screenCorners[4];

    Point2f dstTri[3];
    screenCorners[0]=Point2f(0,0);
    screenCorners[1]=Point2f(cols,0);
    screenCorners[2]=Point2f(cols,rows);
    screenCorners[3]=Point2f(0,rows);

    black = Mat::zeros(rows,cols, CV_8U);
    screen = Mat::zeros(IPHONESEY,IPHONESEX, CV_8UC3);

    cvtColor(mat, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(5, 5),0);

    Canny(gray,edges,100,200);

    findContours(edges,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
    for(uint i=0;i <hierarchy.size();i++)
    {
        if(hierarchy[i][1]==-1) //Only the last child was needed
        {
            RotatedRect tempRect;
            //qDebug("%d,%d,%d,%d",hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3]);
            //drawContours(black,contours,i,255,1);
            tempRect=minAreaRect(contours[i]);
            if(isIPhoneScreen(tempRect, screenCorners,screenCenter))
            {
                //Drow
                line(mat,screenCorners[0],screenCorners[1],150,10);
                line(mat,screenCorners[1],screenCorners[2],500,10);
                //qDebug("(%f,%f),(%f,%f),(%f,%f)",screenCorners[0].x,screenCorners[0].y,screenCorners[1].x,screenCorners[1].y,screenCorners[2].x,screenCorners[2].y);
                break;
            }
        }
    }

    dstTri[0] = Point2f( IPHONESEX,0 );
    dstTri[1] = Point2f (IPHONESEX,IPHONESEY);
    dstTri[2] = Point2f(0,IPHONESEY);
    warp = getAffineTransform(screenCorners,dstTri);
    warpAffine(mat, screen, warp, screen.size());

//    if(mat.data)
//        imshow("asdf",black);

    return warp;
}

//Make sure after find screen you got the warp
Mat FindScreen::getScreen(Mat mat)
{
    if(warp.data && screen.data)
    {
        warpAffine(mat, screen, warp, screen.size());
    return screen;
    }
    return mat;
}
