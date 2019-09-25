#include "readMatrix.h"
#include <algorithm>
#include <map>
#include  <direct.h>  
#include  <stdio.h> 
#include <istream>
using namespace std;

readMatrix::readMatrix(string path) {
	char buffer[100];
	_getcwd(buffer, 100);
	string pwdpath;
	pwdpath = buffer;
	string configure = "config.txt";
	path = pwdpath + "\\" + configure;
	filePath = path;
}

bool readMatrix::writeIntrinsicMatrix(string matrix) {
	vector<std::string::size_type>flag;
	std::string::size_type index = 0;
	string before = "[";
	string after = "]";
	string main = matrix.substr(before.length(), matrix.length()-after.length()-1);
	std::string::size_type pos = 0;
	string substr = ",";
	size_t count = 0;
	int a = 0;
	while (a < 9) {
		index = main.find(',', pos);
		istringstream iss(main.substr(pos, index));
		double mat;
		iss >> mat;
		if (std::string::npos != pos) {
			if (count == 0) {
				intrinsic_matrix(0, 0) = mat;
			/*	printf("intrinsic_matrix(0, 0)= %f\n",mat);*/
			}
			if (count == 1) {
				intrinsic_matrix(0, 1) = mat;
				/*printf("intrinsic_matrix(0, 1)= %f\n", mat);*/
			}
			if (count == 2) {
				intrinsic_matrix(0, 2) = mat;
				/*printf("intrinsic_matrix(0,2)= %f\n", mat);*/
			}
			if (count == 3) {
				intrinsic_matrix(1, 0) = mat;
				/*printf("intrinsic_matrix(1,0)= %f\n", mat);*/
			}
			if (count == 4) {
				intrinsic_matrix(1, 1) = mat;
				/*printf("intrinsic_matrix(1,1)= %f\n", mat);*/
			}
			if (count == 5) {
				intrinsic_matrix(1, 2) = mat;
				/*printf("intrinsic_matrix(1,2)= %f\n", mat);*/
			}
			if (count == 6) {
				intrinsic_matrix(2, 0) = mat;
				/*printf("intrinsic_matrix(2,0)= %f\n", mat);*/
			}
			if (count == 7) {
				intrinsic_matrix(2, 1) = mat;
				/*printf("intrinsic_matrix(2,1)= %f\n", mat);*/
			}
			pos += substr.length()+(index - pos);
			count++;
		}

		if (count == 8) {
			index = main.rfind(',');
			istringstream iss(main.substr(index+1, main.length()));
			iss >> mat;
			intrinsic_matrix(2, 2) = mat;
			/*printf("intrinsic_matrix(2,2)= %f\n", mat);*/
		}
		a++;
	}
	return true;
}


bool readMatrix::writeDistortionMatrix(string matrix) 
{
	vector<std::string::size_type>flag;
	std::string::size_type index = 0;
	string before = "[";
	string after = "]";
	string main = matrix.substr(before.length(), matrix.length() - after.length() - 1);
	std::string::size_type pos = 0;
	string substr = ",";
	size_t count = 0;
	int a = 0;
	while (a < 5) {
		index = main.find(',', pos);
		istringstream iss(main.substr(pos, index));
		double mat;
		iss >> mat;
		if (std::string::npos != pos) {
			if (count == 0) {
				distortion_coeffs[0] = mat;
				/*printf("distortion_coeffs[0] = %f\n", mat);*/
			}
			if (count == 1) {
				distortion_coeffs[1] = mat;
				/*printf("distortion_coeffs[1] = %f\n", mat);*/
			}
			if (count == 2) {
				distortion_coeffs[2] = mat;
			/*	printf("distortion_coeffs[2] = %f\n", mat);*/
			}
			pos += substr.length() + (index - pos);
			count++;
		}

		if (count == 3) {
			index = main.rfind(',');
			istringstream iss(main.substr(index + 1, main.length()));
			iss >> mat;
			distortion_coeffs[3] = mat;
			/*printf("distortion_coeffs[3]= %f\n", mat);*/
		}
		a++;
	}
	return true;
}

bool readMatrix:: getMatrix()
{
	ifstream configfile;
	if (NULL == filePath.c_str()) {
		printf("can not open config file:%s\n ", filePath.c_str());
		return false;
	}

	configfile.open(filePath.c_str()); //打开文件
	if (!configfile.is_open()) {
		printf("can not open config file:%s\n ", filePath.c_str());
		return false;
	}

	char tmp[1000];
	string line(tmp);
	string matrix_inner;
	string matrix_distort;
	string tmpKey;
	size_t pos;
	while (!configfile.eof()) {
		configfile.getline(tmp, 1000); //每行读取前1000个字符，1000个应该足够了
		string line(tmp);
		pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value  
		if (pos == string::npos) {
			return false;
		}
		tmpKey = line.substr(0, pos);//取=号之前 
		if (INTRINSIC_MATRIX == tmpKey)
		{
			matrix_inner = line.substr(pos + 1).c_str();//取=号之后  
			printf("matrix_inner=%s\n", matrix_inner.c_str());
			writeIntrinsicMatrix(matrix_inner);
		}
		if (DISTORTION_COEFFS == tmpKey)
		{
			matrix_distort = line.substr(pos + 1).c_str();//取=号之后  
			writeDistortionMatrix(matrix_distort);
			printf("matrix_distort=%s\n", matrix_distort.c_str());
		}
	}
	return true;
}

int main() {
	string path;
	readMatrix ptr_a = readMatrix(path);
	ptr_a.getMatrix();
	getchar();
	return 0;
}






/*string path;
	readMatrix ptr_a = readMatrix(path);
	ptr_a.getMatrix();
	Mat image = imread("a.jpg", 1);
	Size image_size = image.size();
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
	int x_expand = 0, y_expand = 0;
	imshow("输入图片", image);
	Mat testImage;
	copyMakeBorder(image, testImage, (int)(y_expand / 2), (int)(y_expand / 2), (int)(x_expand / 2), (int)(x_expand / 2), BORDER_CONSTANT);
	fisheye::initUndistortRectifyMap(ptr_a.intrinsic_matrix, ptr_a.distortion_coeffs, R, ptr_a.intrinsic_matrix, image_size, CV_32FC1, mapx, mapy);
	Mat t = testImage.clone();
	cv::remap(testImage, t, mapx, mapy, INTER_LINEAR);

	imwrite("TestOutput.jpg", t);
	imshow("TestOutput.jpg", t);
	cout << "保存结束" << endl;*/

