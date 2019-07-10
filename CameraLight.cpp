#include "CameraLight.h"

CameraLight::CameraLight()
{
}

void CameraLight::Calculate()
{
	sum = count = 0;
	maksX = maksY = maks = xL = yL = 0;
	system("cls");
	cap >> frame;
	cvtColor(frame, frame, 1);
	cvtColor(frame, frameGray, 10);
	for (int i = 0; i <= 576; i += 64) {
		for (int j = 60; j <= 420; j += 48) {
			int bright = sectorBrightness(i, j, frame);
			sum += bright;
			count++;
			makses.push_back(Pixel{ bright, i, j });
		}
	}
	av = sum / count;
	brightnessAv = av;
	int countBrght = 1;
	for (int i = 0; i < makses.size(); i++) {
		if (makses[i].brightness > av+100) {
			brightnessLight += makses[i].brightness;
			frame = paintSquare(frame, makses[i].x, makses[i].y, makses[i].x + 64, makses[i].y + 48);
			xL += (makses[i].x-288)/2;
			yL += makses[i].y-90;
			countBrght++;
		}
	}
	cout << countBrght;

	brightnessLight = brightnessLight / countBrght;
	xLight = xL;
	yLight = yL;
	red = red / 192000.0f;
	green = green / 192000.0f;
	blue = blue / 192000.0f;
	makses.resize(0);
	imshow("xD", frame);
}

void CameraLight::Start()
{
	cap.open(0);
}

Mat CameraLight::paintSquare(Mat frame, int x, int y, int x2, int y2) {
	Vec3b color;
	color[2] = 255;
	for (int i = x; i < x2; i++) {
		for (int j = y; j < y2; j++) {
			if ((i - 5 < x || x2 - 5 < i) || (j - 5 < y || y2 - 5 < j)) {
				frame.at<Vec3b>(Point(i, j)) = color;
			}
		}
	}
	return frame;
}

int CameraLight::sectorBrightness(int xStart, int yStart, cv::Mat frame)
{
	int av = 0;
	long int sum = 0;
	int count = 0;
	for (int i = xStart; i < xStart + 64; i += 16) {
		for (int j = yStart; j < yStart + 48; j += 16) {
			sum += (int)frame.at<Vec3b>(Point(i, j))[0];
			red += (int)frame.at<Vec3b>(Point(i, j))[0];
			sum += (int)frame.at<Vec3b>(Point(i, j))[1];
			green += (int)frame.at<Vec3b>(Point(i, j))[1];
			sum += (int)frame.at<Vec3b>(Point(i, j))[2];
			blue += (int)frame.at<Vec3b>(Point(i, j))[2];
			count++;
		}
	}
	av = sum / count;
	return av;
}

