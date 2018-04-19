#ifndef JUMPONEJUMP_H
#define JUMPONEJUMP_H

#include <opencv2/core.hpp>
using namespace cv;
using namespace std;

class JumpOneJump
{
public:
    JumpOneJump(double guyZoom);
    Point matchGuy(Mat mat);
    Point matchBench(Point guyLoc);

    Mat roiMat;
    Mat matCooked;
    Mat matguy;
    vector<float> benchLine;
};

#endif // JUMPONEJUMP_H
