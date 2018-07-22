// AUTHOR : GURU PRASAD MURUGESH KUMAR

#include<iostream>
#include"lipa.h"
#include<vector>

using namespace std;

Image3CH testimage(800, 1200);
Image3CH resultimage(800, 1200);
Image1CH grayimage(800, 1200);
Image1CH Erodedimage(800, 1200);
Image1CH bufferimage(800, 1200);
bool visited[800][1200];
double color = 1.0;
int top = -1, z = 1;
vector<int> row;
vector<int>column;
int counter1 = 1, counter2 = 1, counter3 = 1, counter4 = 1;
int thstarting_1, thstarting_2, thstarting_3, thstarting_4, thending_1, thending_2, thending_3, thending_4;
int shstarting_1, shstarting_2, shstarting_3, shstarting_4, shending_1, shending_2, shending_3, shending_4;
int kernel = 3, rows = 800, cols = 1200;


// rgb to gray function

void rgbTogray(Image3CH& rgbImg, Image1CH& grayImg) // arguments with & because we want to save changes to images after performing funtion
{
	//Check if image sizes are equal
	if (rgbImg.width() == grayImg.width() && rgbImg.height() == grayImg.height())
	{
		for (int i = 0; i < rgbImg.width(); i++) //iterate by rows
			for (int j = 0; j < rgbImg.height(); j++) //iterate by cols
			{
				grayImg(i, j).Intensity() = (rgbImg(i, j).Red()*0.2125 + rgbImg(i, j).Green()*0.7154 + rgbImg(i, j).Blue()*0.0721); // I = (1/3)*(R+G+B) (i,j) - (number of row, number of col)
			}
	}
	else
	{
		std::cerr << "Image sizes mismatch" << std::endl; //print error
		return;
	}
}

// binarization function
void binarise(Image1CH &bimg, Image1CH &img, Image3CH &res) {
	for (int i = 0; i < bimg.width(); i++)
		for (int j = 0; j < bimg.height(); j++)
		{
			if (bimg(i, j).Intensity() > 0.45)
			{
				bimg(i, j).Intensity() = 1.0;
				img(i, j).Intensity() = 1.0;
				res(i, j).Red() = 0.0;
				res(i, j).Green() = 0.0;
				res(i, j).Blue() = 0.0;
			}

			else
			{
				bimg(i, j).Intensity() = 0.0;
				img(i, j).Intensity() = 0.0;
				res(i, j).Red() = 0.0;
				res(i, j).Green() = 0.0;
				res(i, j).Blue() = 0.0;
			}
		}
}

// adaptive thresholding

void threshold(Image3CH &img, Image1CH &res, Image1CH &bimg, Image3CH &resimg) {
	if ((img.height() == res.height()) && (img.width() == res.width())) {
		for (int i = 0; i < img.width(); i++) {
			for (int j = 0; j < img.height(); j++) {
				if ((img(i, j).Red() < 0.45) && (img(i, j).Red() > 0.3) && (img(i, j).Green() < 0.22) && (img(i, j).Blue() < 0.22)) {
					res(i, j).Intensity() = 1.0;
					bimg(i, j).Intensity() = 0.0;
					resimg(i, j).Red() = 0.0;
					resimg(i, j).Green() = 0.0;
					resimg(i, j).Blue() = 0.0;
				}
				else {
					res(i, j).Intensity() = 0.0;
					bimg(i, j).Intensity() = 0.0;
					resimg(i, j).Red() = 0.0;
					resimg(i, j).Green() = 0.0;
					resimg(i, j).Blue() = 0.0;
				}
			}
		}
	}
}



// comaprison function
int comp(Image1CH &img, Image1CH &bimg) {
	int c = 1;
	for (int i = 0; i < img.width(); i++) {
		for (int j = 0; j < img.height(); j++) {
			if (img(i, j).Intensity() != bimg(i, j).Intensity()) {
				bimg(i, j).Intensity() = img(i, j).Intensity();
				return c;
			}
		}
	}
	return 0;
}

//visited initialisation function

void initial_func() {

	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 1200; j++) {
			visited[i][j] = false;
		}
	}
}

// stack definition pop function

pair<int, int> pop() {

	int x, y;
	if (top == -1) {
		return make_pair(0, 0);
	}
	else {

		x = row.back();
		row.pop_back();
		y = column.back();
		column.pop_back();
		top--;
		return make_pair(x, y);
	}
}

//stack definition push function
void push(int x, int y) {
	row.push_back(x);
	column.push_back(y);
	top++;
}


// floodfill algorithm 2

void floodfill_init(int k, int l) {

	pair<int, int> ref;
	int x, y, compare;

	push(k, l);

	while (top != -1) {

		ref = pop();
		x = ref.first;
		y = ref.second;

		if ((x < 1) || (y < 1) || (x > 798) || (y > 1198)) {
			continue;
		}
		else if (visited[x][y] == true) {
			continue;
		}
		else if (Erodedimage(x, y).Intensity() != 1.0) {
			//cout << "well done" << endl;
			continue;
		}
		else {
			visited[x][y] = true;
			Erodedimage(x, y).Intensity() = 0.0;

			if (z == 1) {
				resultimage(x, y).Red() = color;
				resultimage(x, y).Green() = 0.0;
				resultimage(x, y).Blue() = 0.0;
			}
			if (z == 2) {
				resultimage(x, y).Red() = 0.0;
				resultimage(x, y).Green() = color;
				resultimage(x, y).Blue() = 0.0;
			}
			if (z == 3) {
				resultimage(x, y).Red() = 0.0;
				resultimage(x, y).Green() = 0.0;
				resultimage(x, y).Blue() = color;
			}
			if (z == 4) {
				resultimage(x, y).Red() = color;
				resultimage(x, y).Green() = color;
				resultimage(x, y).Blue() = 0.0;
			}
			push(x + 1, y + 1);
			push(x, y + 1);
			push(x - 1, y + 1);
			push(x - 1, y);
			push(x + 1, y);
			push(x + 1, y - 1);
			push(x, y - 1);
			push(x - 1, y - 1);
		}

	}

	compare = comp(Erodedimage, bufferimage);
	if (compare == 1) {
		z++;
	}

	return;
}

// Edge parameter calculation function

void params() {
	for (int i = 0; i < resultimage.width(); i++) {
		for (int j = 0; j < resultimage.height(); j++) {
			if (resultimage(i, j).Red() == 1.0 && resultimage(i, j).Blue() == 0.0 && resultimage(i, j).Green() == 0.0) {
				if (counter1 == 1) {
					thstarting_1 = i;
					thending_1 = j + 16;
					counter1++;
				}
				else {
					thstarting_2 = i;
					thending_2 = j - 16;
				}
			}
			else if (resultimage(i, j).Red() == 1.0 && resultimage(i, j).Blue() == 0.0 && resultimage(i, j).Green() == 1.0) {
				if (counter2 == 1) {
					shstarting_1 = i;
					shending_1 = j;
					counter2++;
				}
				else {
					shstarting_2 = i;
					shending_2 = j;
				}
			}
		}
	}

	counter1 = 1;
	counter2 = 1;


	for (int j = 0; j < resultimage.height(); j++) {
		for (int i = 0; i < resultimage.width(); i++) {
			if (resultimage(i, j).Red() == 1.0 && resultimage(i, j).Blue() == 0.0 && resultimage(i, j).Green() == 0.0) {
				if (counter1 == 1) {
					thstarting_3 = i - 4;
					thending_3 = j;
					counter1++;
				}
				else {
					thstarting_4 = i + 5;
					thending_4 = j;
				}
			}
			else if (resultimage(i, j).Red() == 1.0 && resultimage(i, j).Blue() == 0.0 && resultimage(i, j).Green() == 1.0) {
				if (counter2 == 1) {
					shstarting_3 = i + 2;
					shending_3 = j;
					counter2++;
				}
				else {
					shstarting_4 = i - 2;
					shending_4 = j;
				}
			}
		}
	}
	counter1 = 1;
	counter2 = 1;

}

//main function of the program 

void main() {

	int draw1, end1, draw2, end2, draw3, end3;
	int buf1, buf2, endb1, endb2;
	int count = 0;

	testimage.LoadImage("img\\1.jpg");

	testimage.ShowImage("Image for analysis");

	rgbTogray(testimage, grayimage);

	binarise(grayimage, bufferimage, resultimage);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			bool erode = false;
			for (int ii = 0; ii < kernel; ii++)
			{
				for (int jj = 0; jj < kernel; jj++)
				{
					int w = ii + i; int h = jj + j;
					if (w < 0) w = 0; if (w >= rows) w = rows - 1;
					if (h < 0) h = 0; if (h >= cols) h = cols - 1;

					if (grayimage(w, h).Intensity() == 0.0)
					{
						erode = true;
					}
				}
			}

			if (erode)
			{
				Erodedimage(i, j).Intensity() = 0.0;
			}
			else
			{
				Erodedimage(i, j).Intensity() = 1.0;
			}
		}
	}

	initial_func();

	for (int i = 0; i < Erodedimage.width(); i++) {
		for (int j = 0; j < Erodedimage.height(); j++) {
			if (Erodedimage(i, j).Intensity() == 1.0) {
				floodfill_init(i, j);

			}
		}
	}

	resultimage.ShowImage("Resultant image after segmentation");

	params();

	testimage.DrawPoint(shstarting_3, shending_3, 1.0, 0.0, 0.0);
	testimage.DrawPoint(shstarting_4, shending_4, 1.0, 0.0, 0.0);
	draw1 = ((thstarting_1 + thstarting_2) / 2);
	end1 = ((thending_1 + thending_2) / 2);
	draw3 = ((shstarting_3 + shstarting_4) / 2);
	end3 = ((shending_3 + shending_4) / 2);

	//testimage.DrawLine(draw1, end1, draw2, end2, 0.0, 0.0, 1.0);

	draw1 = ((thstarting_3 + thstarting_2) / 2);
	end1 = ((thending_3 + thending_2) / 2);
	draw2 = ((thstarting_1 + thstarting_4) / 2);
	end2 = ((thending_1 + thending_4) / 2);


	buf1 = (draw1 + draw2) / 2;
	endb1 = (end1 + end2) / 2;

	testimage.DrawPoint(buf1, endb1, 0.0, 1.0, 0.0);
	testimage.DrawLine(buf1, endb1, draw3, end3, 0.0, 0.0, 1.0);

	buf2 = (buf1 + draw1) / 2;
	endb2 = (endb1 + end1) / 2;

	testimage.DrawPoint(buf2, endb2, 0.0, 1.0, 0.0);


	buf1 = (buf1 + draw2) / 2;
	endb1 = (endb1 + end2) / 2;

	testimage.DrawPoint(buf1, endb1, 0.0, 1.0, 0.0);


	testimage.ShowImage("The result of first two tasks");

	testimage.LoadImage("img\\1.jpg");

	threshold(testimage, grayimage, bufferimage, resultimage);

	grayimage.ShowImage("After Thresholding");


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			bool erode = false;
			for (int ii = 0; ii < kernel; ii++)
			{
				for (int jj = 0; jj < kernel; jj++)
				{
					int w = ii + i; int h = jj + j;
					if (w < 0) w = 0; if (w >= rows) w = rows - 1;
					if (h < 0) h = 0; if (h >= cols) h = cols - 1;

					if (grayimage(w, h).Intensity() == 0.0)
					{
						erode = true;
					}
				}
			}

			if (erode)
			{
				Erodedimage(i, j).Intensity() = 0.0;
			}
			else
			{
				Erodedimage(i, j).Intensity() = 1.0;
			}
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			bool erode = false;
			for (int ii = 0; ii < kernel; ii++)
			{
				for (int jj = 0; jj < kernel; jj++)
				{
					int w = ii + i; int h = jj + j;
					if (w < 0) w = 0; if (w >= rows) w = rows - 1;
					if (h < 0) h = 0; if (h >= cols) h = cols - 1;

					if (Erodedimage(w, h).Intensity() == 0.0)
					{
						erode = true;
					}
				}
			}

			if (erode)
			{
				Erodedimage(i, j).Intensity() = 0.0;
			}
			else
			{
				Erodedimage(i, j).Intensity() = 1.0;
			}
		}
	}
	//Erodedimage.ShowImage("Erosion");

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			bool erode = false;
			for (int ii = 0; ii < kernel; ii++)
			{
				for (int jj = 0; jj < kernel; jj++)
				{
					int w = ii + i; int h = jj + j;
					if (w < 0) w = 0; if (w >= rows) w = rows - 1;
					if (h < 0) h = 0; if (h >= cols) h = cols - 1;

					if (Erodedimage(w, h).Intensity() == 1.0)
					{
						erode = true;
					}
				}
			}

			if (erode)
			{
				Erodedimage(i, j).Intensity() = 1.0;
			}
			else
			{
				Erodedimage(i, j).Intensity() = 0.0;
			}
		}
	}
	//Erodedimage.ShowImage("Dilated Image");


	initial_func();

	for (int i = 0; i < Erodedimage.width(); i++) {
		for (int j = 0; j < Erodedimage.height(); j++) {
			if (Erodedimage(i, j).Intensity() == 1.0) {
				z = 1;
				floodfill_init(i, j);
				count++;
			}
		}
	}

	cout << " the number of big hearts in the given image is:  " << count << endl;

	resultimage.ShowImage(" Result of detection ");

	return;
}