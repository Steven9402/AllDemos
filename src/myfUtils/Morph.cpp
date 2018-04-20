//
// Created by cuizhou on 18-4-5.
//

#include "myfUtils/Morph.h"
namespace myf {

    void erosion(const Mat &src, Mat &erosion_dst, int erosion_size, int erosion_elem) {
        if (src.empty())return;
        int erosion_type;
        if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
        else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
        else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

        Mat element = getStructuringElement(erosion_type,
                                            Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                            Point(erosion_size, erosion_size));

        /// 腐蚀操作
        erode(src, erosion_dst, element);
    }

    void Dilation(const Mat &src, Mat &dilation_dst, int dilation_size, int dilation_elem) {
        if (src.empty())return;
        int dilation_type;
        if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
        else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
        else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

        Mat element = getStructuringElement(dilation_type,
                                            Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                            Point(dilation_size, dilation_size));
        /// 膨胀操作
        dilate(src, dilation_dst, element);
    }
}