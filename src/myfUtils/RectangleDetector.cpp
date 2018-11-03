//
// Created by cuizhou on 18-9-14.
//

#include "RectangleDetector.h"
#include "Geometry.h"
#include "RectangleOperation.h"

#include <iostream>
#include <glog/logging.h>

bool isParallel(const Vec4f& line_a,const Vec4f& line_b){
    float a_0= line_a[2]-line_a[0];
    float a_1= line_a[3]-line_a[1];
    float length_a = sqrt(pow(a_0,2)+pow(a_1,2));
    if(length_a<0.1)return false;
    a_0=a_0/length_a;
    a_1=a_1/length_a;

    float b_0= line_b[2]-line_b[0];
    float b_1= line_b[3]-line_b[1];
    float length_b = sqrt(pow(b_0,2)+pow(b_1,2));
    if(length_b<0.1)return false;
    b_0=b_0/length_b;
    b_1=b_1/length_b;

    float c=a_0*b_1-b_0*a_1;

    if(fabs(c)<0.1){
        //近似看做０
        return true;
    }else{
        return false;
    }
}
bool isVertical(const Vec4f& line_a,const Vec4f& line_b){
    float a_0= line_a[2]-line_a[0];
    float a_1= line_a[3]-line_a[1];
    float length_a = sqrt(pow(a_0,2)+pow(a_1,2));
    if(length_a<0.1)return false;
    a_0=a_0/length_a;
    a_1=a_1/length_a;

    float b_0= line_b[2]-line_b[0];
    float b_1= line_b[3]-line_b[1];
    float length_b = sqrt(pow(b_0,2)+pow(b_1,2));
    if(length_b<0.1)return false;
    b_0=b_0/length_b;
    b_1=b_1/length_b;

    float c = a_0*b_0+a_1*b_1;

    if(fabs(c)<0.1){
//        cout<<"c = "<<c<<endl;
        //近似看做０
        return true;
    }else{
        return false;
    }
}
RectangleDetector::RectangleDetector(){

}
vector<pairs> RectangleDetector::detectVerticalLines(const Mat& frame){
    vector<Vec4f> lines = detectLines(frame);
    _min_line_length = 0.25 * frame.rows;
    return getVerticalLines(lines);
}
vector<pairs> RectangleDetector::detectParallelLines(const Mat& frame){
    _min_parallel_dist = 0.5 * frame.rows;
    _min_line_length = 0.25 * frame.rows;
    vector<Vec4f> lines = detectLines(frame);
    return getParallelLines(lines);
}

vector<Rect> RectangleDetector::detect(const Mat& frame) {
    vector<Rect> result;
    vector<MyRect> tmp_result;

    Mat new_frame;
    if(frame.rows>280){
        _resize_ratio=280.f/frame.rows;
        resize(frame,new_frame,Size(0,0),_resize_ratio,_resize_ratio);
    }else{
        new_frame=frame.clone();
    }


    _min_parallel_dist = 0.5 * new_frame.rows;
    _min_line_length = 0.25 * new_frame.rows;
    _min_line_length_for_v=0.5*new_frame.rows;
    vector<Vec4f> lines = detectLines(new_frame);
    vector<Vec4f> line_v,line_h;

    groupLines(lines,line_v,line_h);
    vector<Vec4f> filtered_line_v = filterLines(line_v);
    vector<Vec4f> filtered_line_h = filterLines(line_h);
    //采用三边矩形规则，至少有三个直角边，算作矩形。
    //1. 从竖向的直线中找一对对平行线
    vector<pairs> parallel_pairs1 = getParallelLines(filtered_line_v);
    //2. 从横向的直线找剩下的一条边
    for(int i=0;i<filtered_line_h.size();i++){
        for(int j=0;j<parallel_pairs1.size();j++){
            if(isVertical(filtered_line_h[i],parallel_pairs1[j].line_a)){
                MyRect tmp;
                tmp.push_back(parallel_pairs1[j].line_a);
                tmp.push_back(parallel_pairs1[j].line_b);
                tmp.push_back(filtered_line_h[i]);
                tmp_result.push_back(tmp);
            }
        }
    }
    //3. reverse
    vector<pairs> parallel_pairs = getParallelLines(filtered_line_h);
    for(int i=0;i<filtered_line_v.size();i++){
        for(int j=0;j<parallel_pairs.size();j++){
            if(isVertical(filtered_line_v[i],parallel_pairs[j].line_a)){
                MyRect tmp;
                tmp.push_back(parallel_pairs[j].line_a);
                tmp.push_back(parallel_pairs[j].line_b);
                tmp.push_back(filtered_line_v[i]);
                tmp_result.push_back(tmp);
            }
        }
    }
    //4. 组成rect
    for(int i=0;i<tmp_result.size();i++){
        int x_min=9999,y_min=9999;
        int x_max=0,y_max=0;
        for(int j=0;j<tmp_result[i].size();j++){
            int x1=tmp_result[i][j][0];
            int y1=tmp_result[i][j][1];
            int x2=tmp_result[i][j][2];
            int y2=tmp_result[i][j][3];
            if(x1<x_min)x_min=x1;
            if(x2<x_min)x_min=x2;
            if(y1<y_min)y_min=y1;
            if(y2<y_min)y_min=y2;
            if(x1>x_max)x_max=x1;
            if(x2>x_max)x_max=x2;
            if(y1>y_max)y_max=y1;
            if(y2>y_max)y_max=y2;
        }
        Rect tmp = myf::getSafeROI((float)x_min/_resize_ratio,(float)y_min/_resize_ratio,(float)x_max/_resize_ratio,(float)y_max/_resize_ratio,frame.rows,frame.cols);
        result.push_back(tmp);
    }
    //5. nms
    myf::nms(result,0.3);
//    //长直角边
//    for(int i=0;i<filtered_line_h.size();i++){
//        if(myf::lengthLine({filtered_line_h[i][0],filtered_line_h[i][1],filtered_line_h[i][2],filtered_line_h[i][3]})<_min_line_length_for_v)continue;
//        for(int j=0;j<filtered_line_v.size();j++){
//            if(myf::lengthLine({filtered_line_v[j][0],filtered_line_v[j][1],filtered_line_v[j][2],filtered_line_v[j][3]})<_min_line_length_for_v)continue;
//            if(isVertical(filtered_line_h[i],filtered_line_v[j])){
//                MyRect tmp;
//                tmp.push_back(filtered_line_h[i]);
//                tmp.push_back(filtered_line_v[j]);
//                result.push_back(tmp);
//            }
//        }
//    }

    return result;
}

//void drawLines(cv::Mat&frame,vector<Vec4f>& lines)
//{
//
//    for (size_t i = 0; i < lines.size(); i++) {
//        Vec4f l = lines[i];
//        line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23, 180, 55), 1, CV_AA);
//    }
//    imshow("【效果图】", frame);
//    waitKey(0);
//}

vector<Vec4f> RectangleDetector::detectLines(const Mat& frame){
    Mat grayMat;
    cvtColor(frame, grayMat, CV_BGR2GRAY);
    GaussianBlur(grayMat, grayMat, Size(5, 5), 1.5);

    Mat result_mask;
    Canny(grayMat,result_mask, 50, 200);//进行一次canny边缘检测

//    imshow("result_mask",result_mask);
//    waitKey(0);

    //调用一次回调函数，调用一次HoughLinesP函数
    vector<Vec4f> lines_hough;
    int min_length = 0.3 * frame.rows;
    int max_dist = 0.1 * frame.rows;
    HoughLinesP(result_mask, lines_hough, 1, CV_PI / 180, 100,//投票阈值
                min_length, //直线最短长度
                max_dist);//两直线之间的最大距离

//    Mat visMat = frame.clone();
//    drawLines(visMat,lines_hough);

    return lines_hough;
}

typedef struct len_id{
    float len;
    int id;
}len_id;
bool comparison(len_id a,len_id b){
    //>是降序
    return a.len>b.len;
}

//先根据斜率分两组
void RectangleDetector::groupLines(const vector<Vec4f>& lines,vector<Vec4f>& vertical_lines,vector<Vec4f>& horizontal_lines){
    for(int i=0;i<lines.size();i++) {
        float ratio = (lines[i][3]-lines[i][1])/(lines[i][2]-lines[i][0]);
        if(fabs(ratio)<1)horizontal_lines.push_back(lines[i]);
        else vertical_lines.push_back(lines[i]);
    }
}
//每组选出前5张
vector<Vec4f> RectangleDetector::filterLines(const vector<Vec4f>& lines){
    vector<len_id> vec_length;
    for(int i=0;i<lines.size();i++){
        float len = myf::lengthLine({lines[i][0],lines[i][1],lines[i][2],lines[i][3]});
        if(len<_min_line_length)continue;
        len_id a;
        a.len=len;
        a.id=i;
        vec_length.push_back(a);
    }
    sort(vec_length.begin(),vec_length.end(),comparison);

    vector<Vec4f> result;

    const int return_length=5;
    for(int i=0;i<vec_length.size();i++){
        if(i<return_length){
            result.push_back(lines[vec_length[i].id]);
        }
    }
    return result;
}


vector<pairs> RectangleDetector::getVerticalLines(vector<Vec4f>& lines){
    vector<pairs> result;
    for(int i=0;i<(int)lines.size()-1;i++){
        for(int j=i+1;j<lines.size();j++){
            if(isVertical(lines[i],lines[j])){
                pairs vp;
                vp.line_a=lines[i];
                vp.line_b=lines[j];
                result.push_back(vp);
            }
        }
    }
    return result;
}

vector<pairs> RectangleDetector::getParallelLines(vector<Vec4f>& lines){
    vector<pairs> result;
    for(int i=0;i<(int)lines.size()-1;i++){
        for(int j=i+1;j<lines.size();j++) {
            float dist1 = myf::dist2line({lines[i][0],lines[i][1],lines[i][2],lines[i][3]},{lines[j][0],lines[j][1]});

//            float dist2 = myf::dist2line(cvPoint2D32f(lines[i][0], lines[i][1]),
//                                         cvPoint2D32f(lines[i][2], lines[i][3]),
//                                         cvPoint2D32f(lines[j][0], lines[j][1]));

//            cout<<"dist1 = "<<dist1<<endl;
//            cout<<"dist2 = "<<dist2<<endl;

            if(isParallel(lines[i],lines[j])&&dist1>_min_parallel_dist){
                pairs vp;
                vp.line_a=lines[i];
                vp.line_b=lines[j];
                result.push_back(vp);
            }
        }
    }
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
RectangleDetector2::RectangleDetector2(){

}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double RectangleDetector2::angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


//*****************************************************************************
/*!
 *  \brief  FilterByMaxSize
 *
 *  \param  colorSquares, filteredBySizeSquares, maxsize
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::FilterByMaxSize(const vector<vector<Point> > &colorSquares, vector<vector<Point> > &filteredBySizeSquares, const int maxsize)
{
    // Eliminate squares that are greater than maximum allowable size (min size has already been filtered out)
    vector<vector<Point> >::const_iterator citer = colorSquares.begin();
    for (; citer != colorSquares.end(); ++citer)
    {
        if (fabs(contourArea(Mat(*citer))) < maxsize)
        {
            filteredBySizeSquares.push_back(*citer);
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  FilterByMaxSize
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::FilterByMaxSize(const vector<vector<Point> > colorSquares[], vector<vector<Point> > filteredBySizeSquares[], const int maxsize)
{
    const int numOfColorPlanes = 3;
    // Eliminate squares that are greater than maximum allowable size (min size has already been filtered out)
    for (int c=0; c<numOfColorPlanes; ++c)
    {
        FilterByMaxSize(colorSquares[c], filteredBySizeSquares[c], maxsize);
    }
}


//*****************************************************************************
/*!
 *  \brief  SortByCenters
 *
 *  \param  colorSquares, sortedSquares, minDist
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::SortByCenters(const vector<vector<Point> > &colorSquares, vector<vector<vector<Point> > > &sortedSquares,
                          const int minDist)
{
    for (size_t i=0; i<colorSquares.size(); ++i)
    {
        bool isNew = true;
        Center curCenter;
        minEnclosingCircle(Mat(colorSquares[i]), curCenter.location, curCenter.radius);

        for (size_t j=0; j<sortedSquares.size() && isNew; ++j)
        {
            Center sortedCenter;

            // For this algorithm we pick the sorted square in the middle of the array to check against since they are sorted by radius size
            minEnclosingCircle(Mat(sortedSquares[j][ sortedSquares[j].size()/2 ]), sortedCenter.location, sortedCenter.radius);

            double dist = norm(sortedCenter.location - curCenter.location);
            isNew = dist >= minDist && dist >= sortedCenter.radius && dist >= curCenter.radius;
            if (!isNew)
            {
                // Determine where this radius fits in the group
                size_t k = sortedSquares[j].size() - 1;
                minEnclosingCircle(Mat(sortedSquares[j][k]), sortedCenter.location, sortedCenter.radius);
                while( k > 0 && curCenter.radius<sortedCenter.radius )
                {
                    k--;
                    minEnclosingCircle(Mat(sortedSquares[j][k]), sortedCenter.location, sortedCenter.radius);
                }
                if (curCenter.radius>sortedCenter.radius)
                {
                    ++k;
                }
                sortedSquares[j].insert(sortedSquares[j].begin() + k, colorSquares[i]);
            }
        }
        if (isNew)
        {
            // Start a new group of squares
            sortedSquares.push_back(std::vector<std::vector<Point> > (1, colorSquares[i]));
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  SortByCenters
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::SortByCenters(const vector<vector<Point> > colorSquares[], vector<vector<vector<Point> > > sortededByCentersSquares[],
                          const int minDist)
{
    const int numOfColorPlanes = 3;
    // Eliminate squares that are greater than maximum allowable size (min size has already been filtered out)
    for (int c=0; c<numOfColorPlanes; ++c)
    {
        SortByCenters(colorSquares[c], sortededByCentersSquares[c], minDist);
    }
}


//*****************************************************************************
/*!
 *  \brief  DumpSortedCenterData
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::DumpSortedCenterData(const vector<vector<vector<Point> > > &sortedByCenterSquares)
{
    vector<vector<vector<Point> > >::const_iterator cCenterDataIter = sortedByCenterSquares.begin();
    for (; cCenterDataIter != sortedByCenterSquares.end(); ++cCenterDataIter)
    {
        vector<vector<Point> >::const_iterator cCenterIter = cCenterDataIter->begin();
        for (; cCenterIter != cCenterDataIter->end(); ++cCenterIter)
        {
            Center center;
            minEnclosingCircle(Mat(*cCenterIter), center.location, center.radius);
            cout << "L: " << center.location << " R: " << center.radius << "\t";
        }
        cout << endl;
    }

    cout << endl;
}


//*****************************************************************************
/*!
 *  \brief  DumpSortedCenterData
 *
 *  \version
 *      - W Parsons   01/12/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::DumpSortedCenterData(const vector<vector<vector<Point> > > sortedByCenterSquares[])
{
    const int numOfColorPlanes = 3;
    // Eliminate squares that are greater than maximum allowable size (min size has already been filtered out)
    for (int c=0; c<numOfColorPlanes; ++c)
    {
        cout << "Dumping Data from layer \"" << c << "\" sortedByCenterSquares:" << endl;
        DumpSortedCenterData(sortedByCenterSquares[c]);
    }
}


//*****************************************************************************
/*!
 *  \brief  ConsolidateSquares
 *
 *  \version
 *      - W Parsons   01/13/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::ConsolidateSquares(const vector<vector<vector<Point> > > sortedSquares[], vector<vector<Point> > &consolidatedSquares)
{
    // Make sure we don't have any residual data in this vector or we could end up with repeats
    consolidatedSquares.clear();

    const int numOfColorPlanes = 3;
    vector<vector<vector<Point> > > sortedByCenterSquares;
    const int minDist = 50;

    // For this algorithm we simply select the square located in the middle of the vector since they are sorted by radius size

    // Pull center from each plane into one vector
    for (int c=0; c<numOfColorPlanes; ++c)
    {
        vector<vector<vector<Point> > >::const_iterator cSortedIter = sortedSquares[c].begin();
        for (; cSortedIter != sortedSquares[c].end(); ++cSortedIter)
        {
            size_t middle = cSortedIter->size()/2;
            vector<Point> middleSquare = cSortedIter->at(middle);
            consolidatedSquares.push_back(middleSquare);
        }
    }

    // Reduce to one unique square
    //	Color information is gone by this point
    SortByCenters(consolidatedSquares, sortedByCenterSquares, minDist);

    DumpSortedCenterData(sortedByCenterSquares);

    consolidatedSquares.clear();

    vector<vector<vector<Point> > >::const_iterator cSortedIter = sortedByCenterSquares.begin();
    for (; cSortedIter != sortedByCenterSquares.end(); ++cSortedIter)
    {
        size_t middle = cSortedIter->size()/2;
        vector<Point> middleSquare = cSortedIter->at(middle);
        consolidatedSquares.push_back(middleSquare);
    }
}


//*****************************************************************************
/*!
 *  \brief  FilterByBGR
 *
 *  \version
 *      - W Parsons   01/13/2014
 *        Initial Version
 *
 *****************************************************************************/
void RectangleDetector2::FilterByBGR(const Mat& image, const vector<vector<Point> > &sortedSquares,
                        vector<vector<Point> > &sortedByRGBSquares,
                        const vector<Scalar> &colorRange)
{
    // Make sure that we don't have any residual data in the returned vector
    sortedByRGBSquares.clear();

    Mat upperMat;
    Mat lowerMat;
    Mat resultMat;

    bool isInRange(true);

    vector<vector<Point> >::const_iterator cSortedSquaresIter = sortedSquares.begin();
    for (; cSortedSquaresIter != sortedSquares.end(); ++cSortedSquaresIter)
    {
        Rect rect = boundingRect(*cSortedSquaresIter);

        // Reduce rect to 25% (50% per side) with center as anchor point
        Point2i topLeft(rect.tl());
        Point2i botRight(rect.br());
        Point2i delta = botRight - topLeft;
        topLeft += (delta * 0.25);
        botRight -= (delta * 0.25);

        rect = Rect(topLeft, botRight);

        // Define the Region of Interest (ROI)
        Mat imageRect = image(rect);

        lowerMat = Mat(imageRect.size(), CV_8UC3, colorRange[0]);
        upperMat = Mat(imageRect.size(), CV_8UC3, colorRange[1]);
        resultMat = Mat::zeros(imageRect.size(), CV_8U);

        inRange(imageRect, lowerMat, upperMat, resultMat);

        vector<Mat> planes;
        split(imageRect, planes);
        for (int c=0; c<3; ++c)
        {
            resultMat.copyTo(planes[c]);
        }
        merge(planes, imageRect);

        // For this algorithm, we make sure that we have 100% containment within the color range
        // 	For this we use a pixel-wise logical-AND operation
        int nr = resultMat.rows;
        int nc = resultMat.cols; // * resultMat.channels(); // Should be only one channel, but just in case...
        for (int i=0; i<nr && isInRange; ++i)
        {
            uchar* data = resultMat.ptr<uchar>(i);
            for (int j=0; j<nc && isInRange; ++j)
            {
                if (data[j] != 255)
                {
                    isInRange = false;
                }
            }
        }

        if (isInRange)
        {
            sortedByRGBSquares.push_back(*cSortedSquaresIter);
        }
        else
        {
            isInRange = true;
        }
    }
}


// filters out squares found based on color, position, and size
void RectangleDetector2::FilterSquares(const Mat& image, vector<vector<Point> > colorSquares[], vector<vector<Point> > &squares)
{
    squares.clear();

    const int maxsize = 100000;
    const int minDstBtnCtrs = 50;
    vector<vector<Point> > filteredBySizeSquares[3];
    vector<vector<vector<Point> > > sortedByCenterSquares[3];
    vector<vector<Point> > consolidatedSquares;
    vector<vector<Point> > sortedByRGBSquares;

    // Eliminate squares that are greater than maximum allowable size (min size has already been filtered out)
    FilterByMaxSize(colorSquares, filteredBySizeSquares, maxsize);

    // Sorts Squares into groups which share the same approximate center
    //	This is calculated based on the containing circle of the contours as well as the distance between centers
    SortByCenters(filteredBySizeSquares, sortedByCenterSquares, minDstBtnCtrs);

    DumpSortedCenterData(sortedByCenterSquares);

    // Reduces the squares to a single, unique square across all color planes
    ConsolidateSquares(sortedByCenterSquares, consolidatedSquares);

    // This is a very permissive range being the whole upper-half of the gray-scale spectrum
    //		Later on the algorithm will reject any image with any pixels out of this range, so we need it to be permissive
    vector<Scalar> colorRange;
    colorRange.push_back(Scalar(125,125,125));
    colorRange.push_back(Scalar(255,255,255));

    // Removes squares that do not fit within the prescribed color range
    FilterByBGR(image, consolidatedSquares, sortedByRGBSquares, colorRange);

//TODO: Add FilterByHSL()???   // This might come in useful as resistance to different lighting conditions

    for (size_t c=0; c<3; ++c)
    {
        // Make sure we don't need the old, unfiltered squares data here any more
        colorSquares[c].clear();
        std::copy(filteredBySizeSquares[c].begin(), filteredBySizeSquares[c].end(), std::back_inserter(colorSquares[c]));
    }

    std::copy(sortedByRGBSquares.begin(), sortedByRGBSquares.end(), std::back_inserter(squares));
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void RectangleDetector2::FindSquaresMultiChannel( const Mat& image, vector<vector<Point> >& squares, vector<vector<Point> > colorSquares[])
{
    squares.clear();


    Mat new_image;
    if(image.rows>480){
        resize_ratio=480.f/image.rows;
        resize(image,new_image,Size(0,0),resize_ratio,resize_ratio);
    }else{
        new_image=image.clone();
    }

    Mat timg, gray0(new_image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    Mat pyr;
    pyrDown(new_image, pyr, Size(new_image.cols/2, new_image.rows/2));
    pyrUp(pyr, timg, new_image.size());

    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for ( int c=0; c<3; ++c )
    {
        colorSquares[c].clear();

        int ch[] = {c, 0};

        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 ) {
                        vector<Point> approx_resize;
                        for(int ind=0;ind<approx.size();ind++) {
                            approx_resize.push_back(Point((float)approx[ind].x/resize_ratio,(float)approx[ind].y/resize_ratio));
                        }
                        colorSquares[c].push_back(approx_resize);
                    }
                }
            }
        }
    }

//    FilterSquares(image, colorSquares, squares);

}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void RectangleDetector2::FindSquaresSingleChannel( const Mat& image, vector<vector<Point> >& squares)
{
    squares.clear();

    Mat new_image;
    if(image.rows>480){
        resize_ratio=480.f/image.rows;
        resize(image,new_image,Size(0,0),resize_ratio,resize_ratio);
    }else{
        new_image=image.clone();
    }

    Mat timg, gray0(new_image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    Mat pyr;
    pyrDown(new_image, pyr, Size(new_image.cols/2, new_image.rows/2));
    pyrUp(pyr, timg, new_image.size());
//
//    GaussianBlur(new_image, timg, Size(3, 3), 1.5);

    // find squares in GRAY channel
    cvtColor(timg,gray0,CV_BGR2GRAY);

//    imshow("gra",gray0);
//    waitKey(0);

//    // try several threshold levels
//    for( int l = 0; l < N; l++ )
//    {
//        // hack: use Canny instead of zero threshold level.
//        // Canny helps to catch squares with gradient shading
//        if( l == 0 )
//        {
//            // apply Canny. Take the upper threshold from slider
//            // and set the lower to 0 (which forces edges merging)
//            Canny(gray0, gray, 0, thresh, 5);
//            // dilate canny output to remove potential
//            // holes between edge segments
//            dilate(gray, gray, Mat(), Point(-1,-1));
//        }
//        else
//        {
//            // apply threshold if l!=0:
//            //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
//            gray = gray0 >= (l+1)*255/N;
//        }
//
//        cout<<"l = "<<l<<endl;
//        imshow("gray",gray);
//        waitKey(0);
//
//        // find contours and store them all as a list
//        vector<vector<Point> > contours;
//        findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//
//        vector<Point> approx;
//
//        // test each contour
//        for( size_t i = 0; i < contours.size(); i++ )
//        {
//            // approximate contour with accuracy proportional
//            // to the contour perimeter
//            approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
//
//            // square contours should have 4 vertices after approximation
//            // relatively large area (to filter out noisy contours)
//            // and be convex.
//            // Note: absolute value of an area is used because
//            // area may be positive or negative - in accordance with the
//            // contour orientation
//            if( approx.size() == 4 &&
//                fabs(contourArea(Mat(approx))) > 1000 &&
//                isContourConvex(Mat(approx)) )
//            {
//                double maxCosine = 0;
//
//                for( int j = 2; j < 5; j++ )
//                {
//                    // find the maximum cosine of the angle between joint edges
//                    double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
//                    maxCosine = MAX(maxCosine, cosine);
//                }
//
//                // if cosines of all angles are small
//                // (all angles are ~90 degree) then write quandrange
//                // vertices to resultant sequence
//                if( maxCosine < 0.3 ){
//                    vector<Point> approx_resize;
//                    for(int ind=0;ind<approx.size();ind++){
//                        approx_resize.push_back(Point((float)approx[ind].x/resize_ratio,(float)approx[ind].y/resize_ratio));
//                    }
//                    squares.push_back(approx_resize);
//                }
//            }
//        }
//    }



    // apply Canny. Take the upper threshold from slider
    // and set the lower to 0 (which forces edges merging)
    Canny(gray0, gray, 50, 200);
    // dilate canny output to remove potential
    // holes between edge segments
    dilate(gray, gray, Mat(), Point(-1,-1));


//    imshow("gray",gray);
//    waitKey(0);

    // find contours and store them all as a list
    vector<vector<Point> > contours;
    findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    vector<Point> approx;

    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 &&
            fabs(contourArea(Mat(approx))) > 1000 &&
            isContourConvex(Mat(approx)) )
        {
            double maxCosine = 0;

            for( int j = 2; j < 5; j++ )
            {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if( maxCosine < 0.3 ){
                vector<Point> approx_resize;
                for(int ind=0;ind<approx.size();ind++){
                    approx_resize.push_back(Point((float)approx[ind].x/resize_ratio,(float)approx[ind].y/resize_ratio));
                }
                squares.push_back(approx_resize);
            }
        }
    }


}

// the function draws all the squares in the image
void RectangleDetector2::DrawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
    }

    imshow(wndname, image);
}


// the function draws all the squares in the image on their respective color planes and colors
void RectangleDetector2::DrawSquares( Mat& image, const vector<vector<Point> > squares[] )
{
    const int numOfColorPlanes = 3;
    Scalar color;
    const Scalar CV_BLUE(255, 0, 0);
    const Scalar CV_GREEN(0, 255, 0);
    const Scalar CV_RED(0, 0, 255);

    for (size_t c=0; c<numOfColorPlanes; ++c)
    {
        switch(c)
        {
            case 0:
                color = CV_BLUE;
                break;
            case 1:
                color = CV_GREEN;
                break;
            case 2:
                color = CV_RED;
                break;
            default:
                break;
        }

        for( size_t i = 0; i < squares[c].size(); i++ )
        {
            const Point* p = &squares[c][i][0];
            int n = (int)squares[c][i].size();
            polylines(image, &p, &n, 1, true, color, 3, CV_AA);
        }
    }

    imshow(wndname, image);
}



