
#include <math.h>
#include "knn_minkowski.h"

/*
 * KNN ONE AGAINST ALL FUNCTINS for 6 CLASSES of STREAM DATA.
 */

float minkowskiDistance (float pointA[CLASS], float pointB[CLASS], float power, float weight)
{
    float tmpSum = 0;

    for (int i = 0; i < CLASS; i++)
    {
        tmpSum += pow (fabs((pointA[i] - pointB[i]) * weight), power);
    }
    return pow (tmpSum, 1/power);
}

float knn_oaa_regression_6D (float data[TRAIN][CLASS], float data_labels[TRAIN], float query[CLASS], float dist_index[TOP_5][2], float power, float weight)
{
#pragma HLS INTERFACE s_axilite port=dist_index bundle=CTRL
#pragma HLS INTERFACE s_axilite port=query bundle=CTRL
#pragma HLS INTERFACE ap_fifo port=data_labels
#pragma HLS INTERFACE ap_fifo port=data

    int i, count[TYPE], mode, max, max_index;
    float tmp[2], tmp_dist[6];

    for (i = 0; i < TRAIN; i++) {
//#pragma HLS PIPELINE

        /*
         * Calculate the distance for the neighbor.
         */

        tmp[0] = minkowskiDistance(data[i], query, power, weight);
        tmp[1] = data_labels[i];

        /*
         * Insert a new closest neighbor.
         */

		max_index = 0;
		if (dist_index[0][0] > dist_index[max_index][0]) { max_index = 0; }
        if (dist_index[1][0] > dist_index[max_index][0]) { max_index = 1; }
		if (dist_index[2][0] > dist_index[max_index][0]) { max_index = 2; }
		if (dist_index[3][0] > dist_index[max_index][0]) { max_index = 3; }
        if (dist_index[4][0] > dist_index[max_index][0]) { max_index = 4; }

		if (dist_index[0][0] == -1) { max_index = 0; }
		if (dist_index[1][0] == -1) { max_index = 1; }
		if (dist_index[2][0] == -1) { max_index = 2; }
		if (dist_index[3][0] == -1) { max_index = 3; }
		if (dist_index[4][0] == -1) { max_index = 4; }

        if (dist_index[max_index][0] == -1) {
            dist_index[max_index][0] = tmp[0];
            dist_index[max_index][1] = tmp[1];
        }
        else if (tmp[0] <= dist_index[max_index][0]) {
            dist_index[max_index][0] = tmp[0];
            dist_index[max_index][1] = tmp[1];
        }
    }

    /*
     * Determine the mode of the closest neighbors.
     */

    max = 0;
    count[0] = 0;
    count[1] = 0;
    count[2] = 0;
    count[3] = 0;
    count[4] = 0;

    ++count[(int)dist_index[0][1] - 1];
    ++count[(int)dist_index[1][1] - 1];
    ++count[(int)dist_index[2][1] - 1];
    ++count[(int)dist_index[3][1] - 1];
    ++count[(int)dist_index[4][1] - 1];

    if (count[0] > max) {
        max = count[0];
        mode = 0;
    }
    if (count[1] > max) {
        max = count[1];
        mode = 1;
    }
    if (count[2] > max) {
        max = count[2];
        mode = 2;
    }
    if (count[3] > max) {
        max = count[3];
        mode = 3;
    }
    if (count[4] > max) {
        max = count[4];
        mode = 4;
    }
    return (mode + 1);
}
