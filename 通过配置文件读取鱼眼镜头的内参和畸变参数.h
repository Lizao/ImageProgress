#include <opencv2\opencv.hpp>
#include <fstream>
using namespace std;
using namespace cv; 

const static string INTRINSIC_MATRIX = "intrinsic_matrix";
const static string DISTORTION_COEFFS = "distortion_coeffs";

class readMatrix {
public:
	readMatrix(string filePath);
	bool getMatrix();
	bool writeIntrinsicMatrix(string matrix);
	bool writeDistortionMatrix(string matrix);

public:
	string filePath;
	cv::Matx33d intrinsic_matrix;
	cv::Vec4d distortion_coeffs;
};
