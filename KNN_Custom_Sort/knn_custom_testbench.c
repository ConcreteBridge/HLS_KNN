
#include <stdio.h>
#include "knn_custom.h"

int main (int argc, char* argv[])
{
    int i, j;

    static float train[TRAIN][CLASS], train_labels[TRAIN];
    static float test[TEST][CLASS], test_labels[TEST];

    // LOAD TRAIN and TEST DATA FROM FILES.
    FILE *f_train_in = fopen (train_data_file_name, "r");
    fscanf (f_train_in, "%*s\n");   				// Skip labels.
    for (i = 0; i < TRAIN; i++) {
        fscanf (f_train_in, "%f", &train[i][0]);	// pkt_count.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train[i][1]);    // pkt_length.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train[i][2]);    // ratio_comm.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train[i][3]);    // icmp_perc.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train[i][4]);    // udp_perc.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train[i][5]);    // tcp_perc.
        fscanf (f_train_in, ",");

        fscanf (f_train_in, "%f", &train_labels[i]);	// type_2.
    }
    fclose (f_train_in);

    FILE *f_test_in = fopen (test_data_file_name, "r");
    fscanf (f_test_in, "%*s\n");   				// Skip labels.
	for (i = 0; i < TEST; i++) {
		fscanf (f_test_in, "%f", &test[i][0]);		// pkt_count.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test[i][1]);    	// pkt_length.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test[i][2]);    	// ratio_comm.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test[i][3]);    	// icmp_perc.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test[i][4]);    	// udp_perc.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test[i][5]);    	// tcp_perc.
		fscanf (f_test_in, ",");

		fscanf (f_test_in, "%f", &test_labels[i]);	// type_2.
	}
	fclose (f_test_in);

    // RUN THE TEST.
    float mode, dist_index[K][2];
    int score = 0;

    for (i = 0; i < TEST; i++) {
        mode = knn(train, train_labels, test[i], dist_index);

        if (test_labels[i] == mode)
        	++score;
    }

    printf("score: %d, %.1f%%\n", score, (float)score / TEST * 100);

    return 0;
}
