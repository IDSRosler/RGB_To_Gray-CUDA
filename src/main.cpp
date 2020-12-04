#include <iostream>
#include <string>
#include <sys/time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "gray_img_CUDA.cuh"

using namespace std;
using namespace cv;

vector<string> Split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void graySerial(Mat &orig, Mat &grey);

long wtime();

int main(int argc, char** argv)
{ 
    string in_path, out_path, flag, output;
    vector<string> output_file_name;
      
    for (int it = 1; it < argc; ++it) {
        flag = argv[it];

        if (flag == "-input") { // input path of image
            in_path = argv[++it];
        } else if (flag == "-output") { // output path of image in grey scale
            out_path = argv[++it];
        } else {
            cout << "Unused Option: " << argv[it];
            cout << "\t" << argv[++it] << endl;
        }
    }
 
    output_file_name = Split(in_path, '/');
    string file_name = output_file_name[output_file_name.size()-1];


    Mat orig = imread(in_path);

    if(orig.empty())
    {
        cout << "Could not read the image: " << endl;
        exit(1);
    }

    Mat gray = Mat::zeros(orig.rows, orig.cols, orig.type());
    Mat gray_p = Mat::zeros(orig.rows, orig.cols, orig.type());

    long start = wtime();
    graySerial(orig, gray);
    long end = wtime();

    double serial_time = (end - start) / 1000.0;

    output = out_path + "Serial_" + file_name;
    imwrite(output, gray);
    
    start = wtime(); 
    grayParallel(orig.data, gray_p.data, orig.rows, orig.cols, orig.channels());
    end = wtime();

    double parallel_time = (end - start) / 1000.0;

    output = out_path + "Parallel_" + file_name;
    imwrite(output, gray_p);

    cout << "Serial time: " << serial_time << " ms" << endl;
    cout << "Parallel time: " << parallel_time << " ms" << endl;

    return 0;
}

void graySerial(Mat &orig, Mat &gray){
    for (int i = 0; i < orig.rows; ++i) {
        for (int j = 0; j < orig.cols; ++j) {
                int grayOffset = i * orig.cols + j;
                int rgbOffset = grayOffset * orig.channels();
                unsigned char r = orig.data[rgbOffset]; // red value for pixel
                unsigned char g = orig.data[rgbOffset + 1]; // green value for pixel
                unsigned char b = orig.data[rgbOffset + 2]; // blue value for pixel

                gray.data[rgbOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
                gray.data[rgbOffset + 1] = 0.21f*r + 0.71f*g + 0.07f*b;
                gray.data[rgbOffset + 2] = 0.21f*r + 0.71f*g + 0.07f*b;
        }
    }

}

long wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000000 + t.tv_usec;
}