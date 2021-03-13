
#include <stdio.h>
#include <math.h>
#include "knn.h"

// KNN ONE AGAINST ALL FUNCTINS for 6 CLASSES of STREAM DATA.
float knn (float train[TRAIN][CLASS], float train_labels[TRAIN], float test_query[CLASS], float dist_index[TRAIN][2], int k)
{
#pragma HLS INTERFACE s_axilite port=dist_index
#pragma HLS INTERFACE s_axilite port=test_query
#pragma HLS INTERFACE axis register both port=train_labels
#pragma HLS INTERFACE axis register both port=train
#pragma HLS INTERFACE axis register both port=k
#pragma HLS INTERFACE axis register both port=return

	int i, j, n, m, L1, R1, L2, R2;
    int count[CLASS] = {0};
	int max = 0;
	float alt[TRAIN][2];

    for (i = 0; i < TRAIN; i++) {
#pragma HLS PIPELINE
        // MANHATTAN DISTANCE.
    	dist_index[i][0] =  (fabs(train[i][0] - test_query[0]) + fabs(train[i][1] - test_query[1])
				+ fabs(train[i][2] - test_query[2]) + fabs(train[i][3] - test_query[3])
				+ fabs(train[i][4] - test_query[4]) + fabs(train[i][5] - test_query[5]));
		dist_index[i][1] =  train_labels[i];
    }

    // SORT THE NEIGHBORS BY DISTANCE.
    i = 1;
	while (i < TRAIN) {
#pragma HLS loop_tripcount min=15 max=16
		j = 0;

		while (j < TRAIN) {
#pragma HLS loop_tripcount min=1 max=65204
			L1 = j;
			R1 = j + i - 1;
			L2 = j + i;
			R2 = j + 2 * i - 1;
			if (L2 >= TRAIN)
				break;
			if (R2 >= TRAIN)
				R2 = TRAIN - 1;

			// MERGE.
			m = 0;
			while ((L1 <= R1) && (L2 <= R2)) {
#pragma HLS loop_tripcount min=0 max=4077
				if (dist_index[L1][0] <= dist_index[L2][0]) {
					alt[m][0] = dist_index[L1][0];
					alt[m++][1] = dist_index[L1++][1];
				}
				else {
					alt[m][0] = dist_index[L2][0];
					alt[m++][1] = dist_index[L2++][1];
				}
			}
			while (L1 <= R1) {
#pragma HLS loop_tripcount min=0 max=4077
				alt[m][0] = dist_index[L1][0];
				alt[m++][1] = dist_index[L1++][1];
			}
			while (L2 <= R2) {
#pragma HLS loop_tripcount min=0 max=4077
				alt[m][0] = dist_index[L2][0];
				alt[m++][1] = dist_index[L2++][1];
			}
			// END MERGE.

			for (n = 0; n < R2 - L1 + 1; n++) {
#pragma HLS loop_tripcount min=0 max=4077
				dist_index[j + n][0] = alt[n][0];
				dist_index[j + n][1] = alt[n][1];
			}
			j = j + 2 * i;
		}
		i *= 2;
	}

    // RETURN MODE.
	for (i = 0; i < K; i++) {
#pragma HLS PIPELINE
//#pragma HLS loop_tripcount min=2 max=2
#pragma HLS loop_tripcount min=5 max=5
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
