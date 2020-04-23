//
//  main.cpp
//  UltraFaceTest
//
//  Created by vealocia on 2019/10/17.
//  Copyright © 2019 vealocia. All rights reserved.
//
#include <stdio.h>
#include "UltraFace.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char **argv) {
    if (argc <= 3) {
        fprintf(stderr, "Usage: %s <ncnn bin> <ncnn param> [image files...]\n", argv[0]);
        return 1;
    }

    std::string bin_path = argv[1];
    std::string param_path = argv[2];
    UltraFace ultraface(bin_path, param_path, 320, 240, 4, 0.7); // config model input
    VideoCapture cap;
    Mat im;

    if( isdigit(argv[3][0]))
    {
        cap.open(argv[3][0]-'0');
//        if(! cap.isOpened())
//        {
//            std::cerr << "Cannot open the camera." << std::endl;
//            return 0;
//        }
    }

    if( cap.isOpened())
    {
        while(true)
        {
            cap >> im;
            //cout << "Image size: " << im.rows << "X" << im.cols << endl;
            //cv::Mat frame = cv::imread(image_file);
            //std::cout << "size: " << frame.size().240 << " , " << image.size().300 << std::endl;
//          cv::Size dsize = cv::Size(320, 240);
//	        cv::Mat img2 = cv::Mat(dsize, CV_32S);
//	        cv::resize(image, img2, dsize);
            Mat frame = im.clone();
//            Mat temImage, dstImage1;
//	        temImage = frame;
//            resize(temImage, dstImage1, Size(300, 240), 0, 0, INTER_LINEAR);
//            frame = dstImage1;

	        TickMeter cvtm;
            cvtm.start();

	        ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);

            std::vector<FaceInfo> face_info;
            ultraface.detect(inmat, face_info);

            for (int i = 0; i < face_info.size(); i++) {
                auto face = face_info[i];
                cv::Point pt1(face.x1, face.y1);
                cv::Point pt2(face.x2, face.y2);
                cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
            }
            cvtm.stop();
            printf("time = %gms\n", cvtm.getTimeMilli());
            cv::imshow("UltraFace", frame);
//            cv::waitKey();
//            cv::imwrite("result.jpg", frame);
            if((cv::waitKey(2)& 0xFF) == 'q')
                break;
        }
    }
    return 0;
}
//    for (int i = 3; i < argc; i++) {
//        std::string image_file = argv[i];
//        std::cout << "Processing " << image_file << std::endl;

        //cv::Mat frame = cv::imread(image_file);
//        ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);
//
//        std::vector<FaceInfo> face_info;
//        ultraface.detect(inmat, face_info);
//
//        for (int i = 0; i < face_info.size(); i++) {
//            auto face = face_info[i];
//            cv::Point pt1(face.x1, face.y1);
//            cv::Point pt2(face.x2, face.y2);
//            cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
//        }
//
//        cv::imshow("UltraFace", frame);
//        cv::waitKey();
//        cv::imwrite("result.jpg", frame);
//    }
//    return 0;
//}
