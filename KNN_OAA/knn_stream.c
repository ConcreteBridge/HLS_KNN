
#include <math.h>
#include <float.h>
#include "knn_stream.h"

// KNN ONE AGAINST ALL FUNCTINS for 6 CLASSES of STREAM train.
float knn (float train[TRAIN][CLASS], float train_labels[TRAIN], float test_query[CLASS], float dist_index[K][2])
{
#pragma HLS INTERFACE s_axilite port=dist_index
#pragma HLS INTERFACE s_axilite port=test_query
#pragma HLS INTERFACE axis register both port=train_labels
#pragma HLS INTERFACE axis register both port=train
#pragma HLS INTERFACE axis register both port=return

    int i, count[TYPE] = {0};
	int mode, max_index, max = 0;
    float tmp[2], tmp_dist[6];

    for (i = 0; i < K; i++) {
#pragma HLS UNROLL
#pragma HLS loop_tripcount min=2 max=2
        dist_index[i][0] = FLT_MAX;
    }

    for (i = 0; i < TRAIN; i++) {
#pragma HLS PIPELINE
    	// MANHATTAN DISTANCE
		tmp[0] = (fabs(train[i][0] - test_query[0]) + fabs(train[i][1] - test_query[1]) +
			fabs(train[i][2] - test_query[2]) + fabs(train[i][3] - test_query[3]) +
			fabs(train[i][4] - test_query[4]) + fabs(train[i][5] - test_query[5]));
		tmp[1] = train_labels[i];

        // RECORD NEAREST NEIGHBORS
        max_index = 0;
//		if (dist_index[4][0] > dist_index[max_index][0]) { max_index = 4; } else
//		if (dist_index[3][0] > dist_index[max_index][0]) { max_index = 3; } else
//		if (dist_index[2][0] > dist_index[max_index][0]) { max_index = 2; } else
		if (dist_index[1][0] > dist_index[max_index][0]) { max_index = 1; } else
		if (dist_index[0][0] > dist_index[max_index][0]) { max_index = 0; }

		if (tmp[0] <= dist_index[max_index][0]) {
			dist_index[max_index][0] = tmp[0];
			dist_index[max_index][1] = tmp[1];
		}
    }

    // RETURN THE MODE OF THE CLOSEST K NEIGHBORS.
    for (i = 0; i < K; i++) {
#pragma HLS PIPELINE
    	++count[(int)dist_index[i][1] - 1];
    }
    for (i = 0; i < TYPE; i++) {
#pragma HLS PIPELINE
    	if (count[i] > max) {
			max = count[i];
			mode = i;
		}
    }
    return (mode + 1);
}
