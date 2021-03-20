
#include <stdio.h>
#include <stdlib.h>
#include "knn_minkowski.h"

int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf("Error, missing parameters: %s <power> <weight> \n", argv[0]);
        return 1;
    }

    int i, j;
    float power = atof(argv[1]);
    float weight = atof(argv[2]);

/*
 * LOAD DATA FROM FILE.
 */

    float data[N][CLASS], labels[N];
    FILE *f_in = fopen ("data_shuffle_norm.csv", "r");

    fscanf (f_in, "%*s\n");     // Skip labels.
    for (i = 0; i < N; i++) {
        fscanf (f_in, "%*d");   // Skip index.
        fscanf (f_in, ",");

        fscanf (f_in, "%*d");   // Skip IP.
        fscanf (f_in, ".");
        fscanf (f_in, "%*d");
        fscanf (f_in, ".");
        fscanf (f_in, "%*d");
        fscanf (f_in, ".");
        fscanf (f_in, "%*d");
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][0]);     // pkt_count.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][1]);     // pkt_length.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][2]);     // tcp_perc.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][3]);     // udp_perc.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][4]);     // icmp_perc.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &data[i][5]);     // ratio_comm.
        fscanf (f_in, ",");

        fscanf (f_in, "%f", &labels[i]);    // type.
        fscanf (f_in, ",");

        fscanf (f_in, "%*f");   // Skip device_type.
        fscanf (f_in, ",");

        fscanf (f_in, "%*f");   // Skip type_scaled.
        fscanf (f_in, ",");
    }

    fclose (f_in);

/*
 * SEPARATE TRAIN and TEST DATA.
 */

    float train[TRAIN][CLASS], train_labels[TRAIN];
    float test[TEST][CLASS], test_labels[TEST];

    for (i = 0; i < TRAIN; i++) {
        train[i][0] = data[i][0];
        train[i][1] = data[i][1];
        train[i][2] = data[i][2];
        train[i][3] = data[i][3];
        train[i][4] = data[i][4];
        train[i][5] = data[i][5];
        train_labels[i] = labels[i];
    }

    for (i = 0; i < TEST; i++) {
        test[i][0] = data[i + TRAIN][0];
        test[i][1] = data[i + TRAIN][1];
        test[i][2] = data[i + TRAIN][2];
        test[i][3] = data[i + TRAIN][3];
        test[i][4] = data[i + TRAIN][4];
        test[i][5] = data[i + TRAIN][5];
        test_labels[i] = labels[i + TRAIN];
    }

/*
 * RUN THE TEST.
 */

    float mode, dist_index[TOP_5][2];
    int score = 0;
    FILE *f_out = fopen ("knn_stream_predictions.csv", "w");

    for (i = 0; i < TEST; i++) {
        dist_index[0][0] = -1;
        dist_index[1][0] = -1;
        dist_index[2][0] = -1;
        dist_index[3][0] = -1;
        dist_index[4][0] = -1;

        mode = knn_oaa_regression_6D (train, train_labels, test[i], dist_index, power, weight);

        if (test_labels[i] == mode) { ++score; }
        fprintf(f_out, "QUERY\t%f,%f,%f,%f,%f,%f\n", test[i][0], test[i][1], test[i][2], test[i][3], test[i][4], test[i][5]);
        fprintf(f_out, "EXPECT\t%f\n", test_labels[i]);
        fprintf(f_out, "MODE\t%f\n", mode);
        fprintf(f_out, "DATA\ndist,index,,data[0],data[1],data[2]\n");
        for (j = 0; j < TOP_5; j++) {
            fprintf (f_out, "%f,%f,,%f,%f,%f\n", dist_index[j][0], dist_index[j][1],
                                                    data[(int)dist_index[j][1]][0], data[(int)dist_index[j][1]][1], data[(int)dist_index[j][1]][2]);
        }
        fprintf(f_out, "\n");
    }
    fclose (f_out);
    printf("%d,%.1f%%\n", score, (float)score / TEST * 100);

    return 0;

}
