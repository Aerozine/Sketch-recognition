#ifndef DATASET_H_DEFINED
#define DATASET_H_DEFINED

#include "Sketch.h"

/* Opaque structure representing a dataset */
typedef struct dataset_t Dataset;

/***
 * Load a dataset from a txt file
 * 
 * 
 * PARAMETERS
 * filepath         The name of the txt file containing the data
 * out              A FILE (e.g., stdout) where to print information
 *                  about the loading process. If NULL, no information
 *                  is printed
 *
 * RETURN
 * dataset          a pointer to the created dataset or NULL
 *
 */

Dataset *dsLoad(const char *filepath, FILE *out);

/***
 * Free a dataset, including all sketches (with sketchFree).
 *
 * PARAMETERS
 * dataset          the dataset to be freed
 *
 */
void dsFree(Dataset *dataset);

/***
 * Return the number of sketches in a dataset.
 * 
 * 
 * PARAMETERS
 * data             The dataset
 *
 * RETURN
 * nbSketches       The number of sketches in the dataset
 *
 */
int dsGetNbSketches(Dataset *dataset);

/***
 * Return the i-th sketch in the dataset.
 * 
 * 
 * PARAMETERS
 * dataset          The dataset
 * i                The index of the sketches to be retrieved
 *                  (an integer between 0 and N-1, with N the number
 *                  of sketches).
 * RETURN
 * sketch           A sketch
 *
 */

Sketch *dsGetSketch(Dataset *dataset, int i);

/***
 * Return the integer code of the label of the i-th sketch in the dataset.
 * The label name can be retrieved from this integer using dsGetNbLabelNames.
 * 
 * 
 * PARAMETERS
 * dataset          The dataset
 * i                The index of the sketches from which the label needs
 *                  to be retrieved (an integer between 0 and N-1, with N 
 *                  the number of sketches).
 *
 * RETURN
 * l                An integer (in {0,1,...,L-1}, with L the number of labels)
 *
 */

int dsGetLabel(Dataset *dataset, int i);

/***
 * Return the number of different labels in the dataset.
 * 
 * 
 * PARAMETERS
 * dataset          The dataset
 *
 * RETURN
 * n                The number of labels in the dataset.
 *
 */

int dsGetNbLabelNames(Dataset *dataset);

/***
 * Return the name (a string) of the i-th label in the dataset.
 * 
 * 
 * PARAMETERS
 * dataset          The dataset
 * i                The integer code of the label
 *
 * RETURN
 * str              A string containing the label name
 *
 */

char *dsGetLabelName(Dataset *dataset, int label_index);

/***
 * Compress the dataset by compressing all sketches in the dataset.
 * This function should overwrite the original dataset.
 * 
 * 
 * PARAMETERS
 * dataset          The dataset
 * threshold        The compression dMax threshold
 *
 */

void dsCompress(Dataset *dataset, double threshold);

#endif
