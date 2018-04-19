#ifndef PHONEHACKCV_H
#define PHONEHACKCV_H

#include <vector>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


class FindScreen
{
public:
    FindScreen();

    Mat find(Mat mat);
    Mat getScreen(Mat mat);
    //
    Mat warp;
    Mat screen;
};

#endif // PHONEHACKCV_H
