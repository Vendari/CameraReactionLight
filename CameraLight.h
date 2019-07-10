#pragma once
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "Windows.h"
#include "stdint.h"
#include <math.h>
#include <vector>
using namespace cv;
using namespace std;

class CameraLight
{
public:
	CameraLight();
	int xLight, yLight, brightnessLight, brightnessAv;
	float red = 0.0f, green=0.0f, blue=0.0f;
	void Calculate();
	void Start();

private:
	struct Pixel {
		int brightness;
		int x;
		int y;
	};

	Mat paintSquare(Mat frame, int x, int y, int x2, int y2);

	VideoCapture cap; // open the default camera
		int maksX = 0, maksY = 0, maks = 0;
		vector<Pixel> makses;
		long int sum = 0, count = 0, av = 0;
		int xL = 0, yL = 0;
		Mat frame, frameGray;
		const int width = 640, height = 480;
	int	sectorBrightness(int xStart, int yStart, cv::Mat frame);
};

