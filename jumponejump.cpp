#include "jumponejump.h"

#define RIOX        0.05
#define RIOY        0.3
#define RIOWidth    0.9
#define RIOHight        0.5

#define BENCHP1     3
#define BENCHP2     3

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


JumpOneJump::JumpOneJump(double guyZoom)
{
    matguy=imread("C:/Users/Public/Pictures/temple/guy.png");
    Size resizeGug=Size(matguy.cols*guyZoom,matguy.rows*guyZoom);
    resize(matguy,matguy,resizeGug,0,0);
}

Point JumpOneJump::matchBench(Point guyLoc)
{
    Mat mask,gray,edge;
    Point p1,p2,benchLoc;

    //close the img to remove the light edge
    GaussianBlur(roiMat, roiMat, Size(19, 19),0);
    Mat element = getStructuringElement(0,Size(19,19),Point(6,6));
    normalize(roiMat,roiMat,0,255,NORM_MINMAX);
    morphologyEx(roiMat,roiMat,MORPH_OPEN,element);

    cvtColor(roiMat,gray,CV_RGB2GRAY);
    //    mask = Mat::zeros(Size(roiMat.cols,roiMat.rows),CV_8UC1);
    //    mask.setTo(255);//
    //    Canny(roiMat,edge,2,5,3);

    //element = getStructuringElement(0,Size(3,3),Point(1,1));
    //morphologyEx(edge,edge,MORPH_CLOSE,element);

    benchLine.clear();

    //edge.copyTo(matCooked);
    if(guyLoc.x<roiMat.cols/2)//lift or right
    {
        //lift
        for(p1.x=roiMat.cols-10;p1.x>guyLoc.x;p1.x--)
        {
            p1.y=guyLoc.y-0.6*(p1.x-guyLoc.x);
            benchLine.push_back(gray.at<uchar>(p1));
        }

        //        line(mask,guyLoc,Point(roiMat.cols,guyLoc.y-0.6*(roiMat.cols-guyLoc.x)),0,1);
        //        edge.setTo(0,mask);
        //        for(p1.x=roiMat.cols-10;p1.x>guyLoc.x;p1.x--)
        //        {
        //            p1.y=guyLoc.y-0.6*(p1.x-guyLoc.x);
        //            if(edge.at<uchar>(p1)>0)
        //                break;
        //        }
        //        for(p2.x=p1.x-10;p2.x>guyLoc.x;p2.x--)
        //        {
        //            p2.y=guyLoc.y-0.6*(p2.x-guyLoc.x);
        //            if(edge.at<uchar>(p2)>0)
        //                break;
        //        }

        //...................
        if (benchLine.size()>5){
            vector<float> t(benchLine.size()-1);
            for(int i=1;i<benchLine.size()&&i<200;i++)
            {
                t[i-1]=benchLine[i]-benchLine[i-1];
                // t[i-1]*=((200.0-i)/200);
            }
            int i=0;
            for(;i<t.size()&&i<200;i++)
            {
                p1.x=roiMat.cols-10-i;
                if(abs(t[i])>BENCHP1)break;
            }

            for( i+=15;i<t.size()&&i<200;i++)
            {
                p2.x=roiMat.cols-10-i;
                if(abs(t[i])>BENCHP2)break;
            }

            //              vector<float>::iterator max = max_element(t.begin(), t.end());
            //              p1.x=roiMat.cols-10-distance(t.begin(), max);

            //            vector<float>::iterator min = min_element(t.begin(), t.end());
            //            p2.x=roiMat.cols-10-distance(t.begin(), min);

            //p2=p1;
        }

        if(p1.x<p2.x){
            Point t=p2;
            p2=p1;
            p1=t;
        }

        if(p1.x-p2.x>50)p2.x=p1.x-50;
        p1.y=guyLoc.y-0.6*(p1.x-guyLoc.x);
        p2.y=guyLoc.y-0.6*(p2.x-guyLoc.x);

        circle(matCooked,p1,3,Scalar(0,0,255),1);
        circle(matCooked,p2,3,Scalar(0,0,255),1);

    }
    else
    {
        for(p1.x=10;p1.x<guyLoc.x;p1.x++)
        {
            p1.y=guyLoc.y-0.6*(guyLoc.x-p1.x);
            benchLine.push_back(gray.at<uchar>(p1));
        }
        //right

        if (benchLine.size()>5){
            vector<float> t(benchLine.size()-1);
            for(int i=1;i<benchLine.size()&&i<200;i++)
            {
                t[i-1]=benchLine[i]-benchLine[i-1];
            }
            int i=0;
            for(;i<t.size()&&i<200;i++)
            {
                p1.x=10+i;
                if(abs(t[i])>BENCHP1)break;
            }

            for( i+=15;i<t.size()&&i<200;i++)
            {
                p2.x=10+i;
                if(abs(t[i])>BENCHP2)break;
            }

        }

        if(p1.x>p2.x){
            Point t=p2;
            p2=p1;
            p1=t;
        }

        if(p2.x-p1.x>50)p2.x=p1.x+50;
        //        line(mask,guyLoc,Point(0,guyLoc.y-0.6*guyLoc.x),0,1);
        //        edge.setTo(0,mask);
        //        for(p1.x=10;p1.x<guyLoc.x;p1.x++)
        //        {
        //            p1.y=guyLoc.y-0.6*(guyLoc.x-p1.x);
        //            if(edge.at<uchar>(p1)>0)
        //                break;
        //        }
        //        for(p2.x=p1.x+10;p2.x<guyLoc.x;p2.x++)
        //        {
        //            p2.y=guyLoc.y-0.6*(guyLoc.x-p2.x);
        //            if(edge.at<uchar>(p2)>0)
        //                break;
        //        }
        p1.y=guyLoc.y-0.6*(guyLoc.x-p1.x);
        p2.y=guyLoc.y-0.6*(guyLoc.x-p2.x);
        circle(matCooked,p1,3,Scalar(0,0,255),1);
        circle(matCooked,p2,3,Scalar(0,0,255),1);
    }
    benchLoc=Point((p1.x+p2.x)/2,(p1.y+p2.y)/2);

    circle(matCooked,benchLoc,3,Scalar(0,0,255),3);
    char pointText[20];
    sprintf(pointText,"%d,%d",benchLoc.x,benchLoc.y);
    putText(matCooked,pointText,benchLoc,CV_FONT_NORMAL,1,Scalar(0,0,255));
    //edge.copyTo(matCooked);

    //roiMat.setTo(0,mask);
    //roiMat.copyTo(matCooked);
    return benchLoc;
}

Point JumpOneJump::matchGuy(Mat mat)
{
    Rect roi= Rect(mat.cols*RIOX,mat.rows*RIOY,mat.cols*RIOWidth,mat.rows*RIOHight);

    roiMat= mat(roi);
    Mat templateResult;
    double minValue,maxValue;
    Point minLoc,maxLoc,guyLoc;
    roiMat.copyTo(matCooked);

    //cvtColor(roiMat, matCooked, CV_BGR2GRAY);
    GaussianBlur(roiMat, roiMat, Size(5, 5),0);
    //cvtColor(matCooked,matCooked,CV_BGR2GRAY);
    matchTemplate(roiMat,matguy,templateResult,CV_TM_CCOEFF);
    //cv::normalize(matCooked,matCooked,0,255,NORM_MINMAX);
    //floodFill(matCooked,Point(0,0),Scalar(0),0,Scalar(2,2,2),Scalar(2,2,2));
    //equalizeHist(matCooked,matCooked);

    minMaxLoc(templateResult,&minValue,&maxValue,&minLoc,&maxLoc);

    rectangle(matCooked,maxLoc,Point(maxLoc.x+matguy.cols,maxLoc.y+matguy.rows),Scalar(255),1);
    guyLoc=Point(maxLoc.x+0.5*matguy.cols,maxLoc.y+0.85*matguy.rows);
    circle(matCooked,guyLoc,3,Scalar(255,0,0),1);
    char pointText[20];
    sprintf(pointText,"%d,%d",guyLoc.x,guyLoc.y);
    putText(matCooked,pointText,guyLoc,CV_FONT_NORMAL,1,Scalar(255,0,0));

    //Canny(matCooked,matCooked,50,100);

    //circle(roiMat,Point(0,0),3,100,10);

    //mat.release();
    //mat = roiMat;

    //always good
    return guyLoc;
}
