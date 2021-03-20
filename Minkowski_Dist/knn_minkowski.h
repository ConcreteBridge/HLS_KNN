
#ifndef KNN_H_
#define KNN_H_

/*
 * MACROS.
 */

#define N       2030
#define TRAIN   1827
#define TEST    203

#define TYPE    5
#define CLASS   6

#define TOP_5   5

/*
 * KNN FUNCTION PROTOTYPE.
 */

float knn_oaa_regression_6D (float data[TRAIN][6], float data_labels[TRAIN], float query[6], float dist_index[TOP_5][2], float power, float weight);

#endif
