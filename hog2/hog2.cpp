#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

float distance(float * l, float * m, int n) {
	int totalnum = 0;
	for (int i = 0; i < n; i++) {
		totalnum = totalnum + pow((*(l + i) - *(m + i)), 2);
	}
	return totalnum;
}

void hist(Mat src, int cell_Size, int nX, int nY, float * hist) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	int n = 0;
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			int n_x = j * cell_Size, n_y = i * cell_Size;
			for (int l = n_x; l < cell_Size + n_x; l++) {
				for (int m = n_y; m < cell_Size + n_y; m++) {
					int region = (angle.at<float>(l, m) - 1) / 40;
					*(hist + region + n) += mag.at<float>(l, m);
				}
			}
			n += 9;
		}
	}
}

int main() {

	Mat src = imread("hogTemplate.jpg", 0);
	Mat dst1 = imread("img1.jpg", 0);
	Mat dst2 = imread("img2.jpg", 0);

	int cell_Size = 16;
	int nX = src.cols / cell_Size;
	int nY = src.rows / cell_Size;
	int bins = 9 * nX * nY;

	float * src_hist = new float[bins];
	memset(src_hist, 0, sizeof(float)*bins);
	float * dst1_hist = new float[bins];
	memset(dst1_hist, 0, sizeof(float)*bins);
	float * dst2_hist = new float[bins];
	memset(dst2_hist, 0, sizeof(float)*bins);

	hist(src, cell_Size, nX, nY, src_hist);
	hist(dst1, cell_Size, nX, nY, dst1_hist);
	hist(dst2, cell_Size, nX, nY, dst2_hist);

	int a = distance(src_hist, dst1_hist, bins);
	int b = distance(src_hist, dst2_hist, bins);

	delete[] src_hist;
	delete[] dst1_hist;
	delete[] dst2_hist;

	cout << "the distance of former is " << a << endl;
	cout << "the distance of later is  " << b << endl;

	if (a > b)
		cout << "the later is greater" << endl;
	else
		cout << "the former is greater" << endl;
	return 0;
}
