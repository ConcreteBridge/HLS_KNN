
#ifndef KNN_H_
#define KNN_H_

// INPUT FILES
#define train_data_file_name	"Data/new_data_train_0.01.csv"
#define test_data_file_name		"Data/new_data_test_0.01.csv"

// METAPARAMETERS
#define N       72448
#define TRAIN   724
#define TEST    71724

#define K		2
#define TYPE    4
#define CLASS   6

// KNN FUNCTION PROTOTYPE.
float knn (float data[TRAIN][CLASS], float data_labels[TRAIN], float query[CLASS], float dist_index[TRAIN][2], int k);

#endif

//N					72448

//TRAIN_0.01.CSV	724
//TEST_0.01.CSV		71724

//TRAIN_0.05.CSV	3624
//TEST_0.05.CSV		68824

//TRAIN_0.1.CSV		7244
//TEST_0.1.CSV		65204

//TRAIN_0.3.CSV		21736
//TEST_0.3.CSV		50712

//TRAIN_0.5.CSV		36224
//TEST_0.5.CSV		36224

//TRAIN_0.7.CSV		50712
//TEST_0.7.CSV		21736

//TRAIN_0.9.CSV		65204
//TEST_0.9.CSV		7244
