#include "VisionCamera.h"
#include <GL\glew.h>
#include "tigl.h"
#include <glm\ext\matrix_clip_space.hpp>

#define KEY_Q 113

// Constructor
VisionCamera::VisionCamera(VideoCapture vidCap) : videoCapture(vidCap)
{}

// Destructor
VisionCamera::~VisionCamera()
{}

// Method to set color regocnitision values
void VisionCamera::colorSettings(vector<int>* colorValues)
{
	// Create track slider bars so you can change colors
	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &colorValues->at(0), 179);
	createTrackbar("Hue Max", "Trackbars", &colorValues->at(3), 179);
	createTrackbar("Sat Min", "Trackbars", &colorValues->at(1), 255);
	createTrackbar("Sat Max", "Trackbars", &colorValues->at(4), 255);
	createTrackbar("Val Min", "Trackbars", &colorValues->at(2), 255);
	createTrackbar("Val Max", "Trackbars", &colorValues->at(5), 255);

	while (true)
	{
		// Read the frame
		videoCapture.read(img);
		// Converts an image from one color space to another.
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		// Upper and lower scalar variables
		Scalar lower(colorValues->at(0), colorValues->at(1), colorValues->at(2));
		Scalar upper(colorValues->at(3), colorValues->at(4), colorValues->at(5));
		// Tresholding operation
		inRange(imgHSV, lower, upper, mask);

		// Show the image
		imshow("Image", img);
		imshow("Press q to quit", mask);

		// Wait 1 ms if the key is pressed
		char key = (char)waitKey(1);
		if (key == KEY_Q) {
			destroyAllWindows();
			break;
		}
	}
}

// Method to receive contours
Point VisionCamera::getContours()
{
	// Creating variables we can use in this method, contours and mypoint
	vector<vector<Point>> contours;
	Point myPoint(0, 0);
	// If mask is not empty
	if (!mask.empty())
	{
		// Find the contours
		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		// Create an array/list with points, and a list with rectangles
		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());

		// Loop trough the whole of contours
		for (int i = 0; i < contours.size(); i++)
		{
			// Request the area of the current countour
			int area = contourArea(contours[i]);

			String objectType;

			// If area is over 500
			if (area > 500)
			{
				// Request size and draw points and contours on screen
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

// Method to find color
void VisionCamera::findColor()
{
	// Convert colors
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	// Loop trough mycolors
	for (int i = 0; i < myColors.size(); i++)
	{
		// Create scalars with lower and upper values
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);

		// Check if colors are in range
		inRange(imgHSV, lower, upper, mask);

		// Request contours
		myPoint = getContours();

		// Check if the color is detected
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

// Update function for VisionCamera
void VisionCamera::update() {
	// Read the webcam
	videoCapture.read(img);
	// Find the color
	findColor();
	// Show video
	imshow("Video", img);

	// Get height and width
	videoHeight = videoCapture.get(CAP_PROP_FRAME_HEIGHT);
	videoWidth = videoCapture.get(CAP_PROP_FRAME_WIDTH);

}

// Method to retreive crosshair coordinates
glm::vec2 VisionCamera::getCrossHairCoords()
{
	// Create viewport
	int viewport[4];
	//glGet � return the value or values of a selected parameter
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Store coordinates and percentages
	float percentageX = currentPoint.x / 640.0f;
	float percentageY = currentPoint.y / 480.0f;

	float xCoordinate = (float)viewport[2] * percentageX;
	float yCoordinate = (float)viewport[3] * percentageY;

	xCoordinate = (float)viewport[2] - xCoordinate; //Alleen voor Nicholas (inverted camera)
	// Return coordinates
	return glm::vec2(xCoordinate, yCoordinate);
}

