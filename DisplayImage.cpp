#include <stdio.h>
#include <unistd.h>

#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

typedef struct borderline_corner {

   uint16_t markerId; 
   uint32_t x;
   uint32_t y;
   uint8_t isTopLeft;

} BorderlineCorner;

#define CORNER_AMOUNT 4
#define MARKERID_PARITY_VAL 256 

int main(int argc, char **argv) {
    int i;
    int EXIT_CODE;
    uint16_t markerIdSum;
    cv::Mat inputImage;
    std::vector<int> markerIds;
    std::vector< std::vector<cv::Point2f> > markerCorners;
    cv::Ptr<cv::aruco::DetectorParameters> parameters;
    cv::Ptr<cv::aruco::Dictionary> dictionary; 
    BorderlineCorner *processedCorners;

    EXIT_CODE = 0;

    inputImage = cv::imread(argv[1], 1);
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);

    cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds);


    if(markerCorners.size() != 4) {
        EXIT_CODE = 1;
    }

    processedCorners = (BorderlineCorner *) malloc(CORNER_AMOUNT * sizeof(struct borderline_corner));
    markerIdSum = 0;

    // Remap the detected markers into the structure
    for(std::vector< std::vector<cv::Point2f> >::size_type i = 0; i != markerCorners.size(); i++) {
        processedCorners[i].markerId = markerIds[i] & 0b0011111111;
        processedCorners[i].x = markerCorners[i][0].x;
        processedCorners[i].y = markerCorners[i][0].y;
        processedCorners[i].isTopLeft = (markerIds[i] & 0b1100000000) >> 8;

        markerIdSum += processedCorners[i].markerId;
    }

    // Check Parity or EXIT_CODE 2
    if(markerIdSum != MARKERID_PARITY_VAL) {
        EXIT_CODE = 2;
    }

    for(i = 0; i < CORNER_AMOUNT; i++) {
        fprintf(stdout, "%d:%d:%d\n", processedCorners[i].markerId, processedCorners[i].x, processedCorners[i].y);
    }

    return EXIT_CODE;
}

