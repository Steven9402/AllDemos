//
// Created by cuizhou on 18-9-14.
//

#ifndef ALLDEMOS_RECTANGLEDETECTOR_H
#define ALLDEMOS_RECTANGLEDETECTOR_H
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

typedef struct pairs{
    Vec4f line_a;
    Vec4f line_b;
}pairs;

typedef struct std::vector<Vec4f> MyRect;

class RectangleDetector {
public:
    RectangleDetector();
    vector<pairs> detectVerticalLines(const Mat& frame);
    vector<pairs> detectParallelLines(const Mat& frame);
    vector<Rect> detect(const Mat& frame);
private:
    vector<Vec4f> detectLines(const Mat& frame);
    void groupLines(const vector<Vec4f>& lines,vector<Vec4f>& vertical_lines,vector<Vec4f>& horizontal_lines);
    vector<Vec4f> filterLines(const vector<Vec4f>& lines);
    vector<pairs> getVerticalLines(vector<Vec4f>& lines);
    vector<pairs> getParallelLines(vector<Vec4f>& lines);

private:
    float _min_parallel_dist=0;
    float _min_line_length=0;
    float _min_line_length_for_v=0;
    float _resize_ratio = 1.0;

};

struct Center
{
    Point2f location;
    float radius;
    // double confidence;
};
class RectangleDetector2{
public:
    RectangleDetector2();

    void FindSquaresMultiChannel( const Mat& image, vector<vector<Point> >& squares, vector<vector<Point> > colorSquares[]);
    void FindSquaresSingleChannel( const Mat& image,vector<vector<Point> >& Squares);
    void DrawSquares( Mat& image, const vector<vector<Point> > squares[] );
    void DrawSquares( Mat& image, const vector<vector<Point> >& squares );

private:
    void FilterSquares(const Mat& image, vector<vector<Point> > colorSquares[], vector<vector<Point> > &squares);
    void FilterByBGR(const Mat& image, const vector<vector<Point> > &sortedSquares,
                     vector<vector<Point> > &sortedByRGBSquares,
                     const vector<Scalar> &colorRange);
    void ConsolidateSquares(const vector<vector<vector<Point> > > sortedSquares[], vector<vector<Point> > &consolidatedSquares);
    void DumpSortedCenterData(const vector<vector<vector<Point> > > sortedByCenterSquares[]);
    void DumpSortedCenterData(const vector<vector<vector<Point> > > &sortedByCenterSquares);
    void SortByCenters(const vector<vector<Point> > colorSquares[], vector<vector<vector<Point> > > sortededByCentersSquares[],
                       const int minDist = 50);
    void SortByCenters(const vector<vector<Point> > &colorSquares, vector<vector<vector<Point> > > &sortedSquares,
                       const int minDist = 50);
    void FilterByMaxSize(const vector<vector<Point> > colorSquares[], vector<vector<Point> > filteredBySizeSquares[], const int maxsize = 100000);
    void FilterByMaxSize(const vector<vector<Point> > &colorSquares, vector<vector<Point> > &filteredBySizeSquares, const int maxsize = 100000);
    double angle( Point pt1, Point pt2, Point pt0 );

private:
    int thresh = 50, N = 11;
    float resize_ratio = 1.0;
    const char* wndname = "Square Detection Demo";
};


#endif //ALLDEMOS_RECTANGLEDETECTOR_H
