#include "Dataset.h"
#include "Recognizer.h"

// A compléter


kNN *recNearestNeighbors(Sketch *sk, Dataset *ds, int k, double (*distance)(Sketch *, Sketch *))
{
	kNN *KitsuNeNinetails = malloc(sizeof(kNN));
	if(!KitsuNeNinetails)
		return NULL;
	
	int *neighbors = malloc(k*sizeof(int));
	if (!neighbors)
	{
		free(KitsuNeNinetails);
		return NULL;
	}
	
	double *distances = malloc(k*sizeof(double));
	if (!distances)
	{
		free(KitsuNeNinetails);
		free(neighbors);
		return NULL;
	}

	double allDistances[dsGetNbSketches(ds)];

	for(int i=0; i < dsGetNbSketches(ds); i++)
		allDistances[i] = distance(sk,dsGetSketch(i));                 // je suis plus sûr syntaxe pour pointeur fct

	int actualNeighbor = 0;
	int lastNeighbor   = 0;

	for(int i=0; i < k; i++)
	{
		for(int j=0; j < dsGetNbSketches(ds); j++)
			if(j != lastNeighbor && allDistances[j] < allDistances[actualNeighbor])
				actualNeighbor = j;

		lastNeighbor = actualNeighbor;
		neighbors[i] = actualNeighbor;
		distance[i]  = allDistances[actualNeighbor];
	}

	KitsuNeNinetails->k         = k;
	KitsuNeNinetails->Dataset   = ds;
	KitsuNeNinetails->neighbors = neighbors;
	KitsuNeNinetails->distance  = distance;

	return KitsuNeNinetails;
}

void recFreekNN(kNN *knn)
{}

char *recGetMajorityLabel(kNN *knn)
{}

float recEvalkNN(Dataset *referenceset, Dataset *testset, int k, double (*distance)(Sketch *, Sketch *),FILE *out)
{}