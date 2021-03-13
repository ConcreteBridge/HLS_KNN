
#include <stdio.h>
#include <math.h>
#include "../knn_std.h"

// KNN ONE AGAINST ALL FUNCTINS for 6 CLASSES of STREAM DATA.
float knn (float train[TRAIN][CLASS], float train_labels[TRAIN], float test_query[CLASS], float dist_index[TRAIN][2], int k)
{
#pragma HLS INTERFACE s_axilite port=dist_index
#pragma HLS INTERFACE s_axilite port=test_query
#pragma HLS INTERFACE axis register both port=train_labels
#pragma HLS INTERFACE axis register both port=train
#pragma HLS INTERFACE axis register both port=k
#pragma HLS INTERFACE axis register both port=return

	int i, j, count[CLASS] = {0};
	int max = 0;
	float enum_cts[TRAIN][3];

    for (i = 0; i < TRAIN; i++) {
#pragma HLS PIPELINE
        // MANHATTAN DISTANCE.
    	dist_index[i][0] =  (fabs(train[i][0] - test_query[0]) + fabs(train[i][1] - test_query[1])
				+ fabs(train[i][2] - test_query[2]) + fabs(train[i][3] - test_query[3])
				+ fabs(train[i][4] - test_query[4]) + fabs(train[i][5] - test_query[5]));
		dist_index[i][1] =  train_labels[i];
    }

    // SORT DISTANCE.
    for (i = 0; i < TRAIN; i++) {
        enum_cts[i][0] = dist_index[i][0];
        enum_cts[i][1] = dist_index[i][1];
        enum_cts[i][2] = 0;
        for (j = 0; j < TRAIN; j++) {
            if ((dist_index[i][0] > dist_index[j][0]) || ((dist_index[i][0] == dist_index[j][0]) && (i > j)))
                enum_cts[i][2]++;
        }
    }
    for (i = 0; i < TRAIN; i++) {
        dist_index[(int)enum_cts[i][2]][0] = enum_cts[i][0];
        dist_index[(int)enum_cts[i][2]][1] = enum_cts[i][1];
    }

    // RETURN MODE.
    for (i = 0; i < k; i++) {
#pragma HLS loop_tripcount avg=2
#pragma HLS PIPELINE
        count[(int)dist_index[i][1]]++;
    }

    for (i = 0; i < CLASS; i++) {
#pragma HLS PIPELINE
        if (count[i] > count[max])
            max = i;
    }

    return max;
}
