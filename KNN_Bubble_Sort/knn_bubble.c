
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

	int i, j, max = 0;
	int count[CLASS] = {0};
	float tmp[2];

	// CALCULATE NEIGHBOR DISTANCES.
	for (i = 0; i < TRAIN; i++) {
#pragma HLS PIPELINE
        // MANHATTAN DISTANCE.
        dist_index[i][0] =  (fabs(train[i][0] - test_query[0]) + fabs(train[i][1] - test_query[1])
        		+ fabs(train[i][2] - test_query[2]) + fabs(train[i][3] - test_query[3])
				+ fabs(train[i][4] - test_query[4]) + fabs(train[i][5] - test_query[5]));
        dist_index[i][1] =  train_labels[i];
    }

    // SORT THE NEIGHBORS BY DISTANCE - ASCENDING BUBBLE.
	for (i = 0; i < k; i++) {
#pragma HLS loop_tripcount min=2 max=2
		for (j = TRAIN - 1; j > 0; j--) {
#pragma HLS PIPELINE
			// CHECK ORDER.
			if (dist_index[j][0] < dist_index[j - 1][0]) {
				// SWAP.
				tmp[0] = dist_index[j][0];
				tmp[1] = dist_index[j][1];
				dist_index[j][0] = dist_index[j - 1][0];
				dist_index[j][1] = dist_index[j - 1][1];
				dist_index[j - 1][0] = tmp[0];
				dist_index[j - 1][1] = tmp[1];
			}
		}
	}

    // RETURN THE MODE OF THE CLOSEST K NEIGHBORS.
	for (i = 0; i < k; i++) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=2 max=2
		count[(int)dist_index[i][1]]++;
	}

	for (i = 0; i < CLASS; i++) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=6 max=6
		if (count[i] > count[max])
			max = i;
	}

	return max;
}
