#include "Dataset.h"
#include "Recognizer.h"

// A compléter


kNN *recNearestNeighbors(Sketch *sk, Dataset *ds, int k, double (*distance)(Sketch *, Sketch *)){}
void recFreekNN(kNN *knn){}
char *recGetMajorityLabel(kNN *knn){}
float recEvalkNN(Dataset *referenceset, Dataset *testset, int k, double (*distance)(Sketch *, Sketch *),FILE *out){}

