#include "VisionCamera.h"
#include <GL\glew.h>
#include "tigl.h"
#include <glm\ext\matrix_clip_space.hpp>

VisionCamera::VisionCamera(VideoCapture vidCap) : cap(vidCap)
{
}

VisionCamera::~VisionCamera()
{
}

Point myPoint;

//not used
void VisionCamera::colorSettings()
{
	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true)
	{
		cap.read(img);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		cout << "hmin: " << hmin << ", smin: " << smin << ", vmin: " << vmin << endl;
		cout << "hmax: " << hmax << ", smax: " << smax << ", vmax: " << vmax << endl;
		imshow("Image", img);
		imshow("Mask", mask);
		waitKey(1);
	}
}

Point VisionCamera::getContours()
{
	vector<vector<Point>> contours;
	Point myPoint(0, 0);
	if (!mask.empty())
	{
		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);

			String objectType;

			if (area > 500)
			{
				float peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

				boundRect[i] = boundingRect(conPoly[i]);
				myPoint.x = boundRect[i].x + boundRect[i].width / 2;
				myPoint.y = boundRect[i].y + boundRect[i].height / 2;
				drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			}
		}
	}

	return myPoint;
}

void VisionCamera::findColor()
{
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		inRange(imgHSV, lower, upper, mask);

		myPoint = getContours();

		if (myPoint.x != 0 && myPoint.y != 0) {
			if (i == 0) {
				redDetected = false;
				currentCrosshair = 0;
			}
			else if (i == 1) {
				redDetected = true;
				currentCrosshair = 1;
			}
			circle(img, myPoint, 5, Scalar(255, 255, 0), FILLED);
			currentPoint = myPoint;
		}
	}
}

//not used
void VisionCamera::displayImage()
{
	//Create image - Row, Column, 8bit [signed = -127 to 127, unsigned = 0 - 255] C = Num channels, BGR values.
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

	//Draw a circle in the image - input, center point, size of circle, BGR calue, thickness of border brush/circle (or put FILLED)
	circle(img, Point(256, 256), 155, Scalar(0, 0, 255), FILLED);

	//Draw a rectangle in the image - input, rect(x, y, width, height), BGR values, thickness of border brush/circle (or put FILLED)
	//Can also make from points, replaces rect with 2 points (point 1 = top left, point 2 = bottom right)
	rectangle(img, Point(130, 226), Point(382, 286), Scalar(0, 0, 0), FILLED);

	imshow("Image", img);
	waitKey(1);
}

void VisionCamera::update() {

	cap.read(img);
	findColor();
	imshow("Video", img);

	videoHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	videoWidth = cap.get(CAP_PROP_FRAME_WIDTH);

}

glm::vec2 VisionCamera::getCrossHairCoords()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	float percentageX = currentPoint.x / 640.0f;
	float percentageY = currentPoint.y / 480.0f;

	float xCoordinate = (float)viewport[2] * percentageX;
	float yCoordinate = (float)viewport[3] * percentageY;

	xCoordinate = (float)viewport[2] - xCoordinate; //Alleen voor Nicholas (inverted camera)
	return glm::vec2(xCoordinate, yCoordinate);
}

