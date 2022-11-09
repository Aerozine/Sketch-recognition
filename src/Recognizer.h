#ifndef RECOGNIZER_H_DEFINED
#define RECOGNIZER_H_DEFINED

#include "Sketch.h"

// kNN structure.
typedef struct kNN_t
{
  int      k;             // Number of nearest neighbors.
  Dataset *dataset;  // Dataset.
  int     *neighbors; // indices of the nearest neighbors in the dataset
  double  *distances; // Distances to the neighbors.
} kNN;

/***
 * Create a kNN structure containing the k nearest neighbors of the given sketch.
 * Nearest neighbors should be ordered by increasing distances in the neighbors
 * and distance arrays. The kNN structure output by the function should be freed
 * using recFreekNN. 
 * 
 * PARAMETERS
 * dataset          The dataset
 * sk               The query sketch
 * k                The number of neighbors
 * distance         A function computing distances between sketches
 *
 * RETURN
 * kNN              A pointer to a kNN structure corresponding to the retrieved
 *                  neighbors.
 *
 */

kNN *recNearestNeighbors(Sketch *sk, Dataset *ds, int k, double (*distance)(Sketch *, Sketch *));

/***
 * Free a kNN structure.
 * 
 * 
 * PARAMETERS
 * knn              The kNN structure
 * 
 */

void recFreekNN(kNN *knn);

/***
 * Get the majority label among the k nearest neighbors.
 * 
 * 
 * PARAMETERS
 * knn              The kNN structure
 * 
 * RETURN
 * name             The name of the most frequent label in the knn structure
 */

char *recGetMajorityLabel(kNN *knn);

/***
 * Get the percentage of correctly predicted labels for the sketches from the testset using
 * referenceset as the dataset for the neighbors.
 * 
 * 
 * PARAMETERS
 * referenceset     The reference set
 * testset          The test set
 * k                The value of k, the number of neighbors
 * distance         A distance function between sketches
 * out              A FILE (e.g., stdout) where to print information
 *                  to monitor the progress of computations. If NULL, 
 *                  no information is printed
 * 
 * RETURN
 * accuracy         The percentage of sketches (between 0 and 100) whose label is correctly predicted
 */

float recEvalkNN(Dataset *referenceset, Dataset *testset, int k, double (*distance)(Sketch *, Sketch *),
		 FILE *out);

#endif
