#include "Dataset.h"
#include "Recognizer.h"
#include <string.h>
#include <stdlib.h>
/*
   Fusionne les vecteurs v1[a1:b1] et v2[a2:b2], supposés triés,
   et place le résultat dans le vecteur v3[], supposé distinct
   de v1[] et v2[] ainsi que de taille suffisante.
*/  
static void fusion(double v1[][2], int a1, int b1, double v2[][2], int a2, int b2, double v3[][2])
{
  int  i1, i2, i3;

  for (i1 = a1, i2 = a2, i3 = 0; i1 <= b1 || i2 <= b2;)
    if (i1 > b1)
    {
      	v3[i3][0] = v2[i2][0];
      	v3[i3++][1] = v2[i2++][1];
    }
    else
      if (i2 > b2)
      {
      	v3[i3][0] = v1[i1][0];
        v3[i3++][1] = v1[i1++][1];
      }
      else
      {
      	if(v1[i1][0] < v2[i2][0])
      	{
      		v3[i3][0] = v1[i1][0];
      		v3[i3++][1] = v1[i1++][1];
      	}
      	else
      	{
      		v3[i3][0] = v2[i2][0];
      		v3[i3++][1] = v2[i2++][1];
      	}
      }
}

/* 
   Tri du vecteur v[a:b], par ordre croissant, en utilisant
   le vecteur v_aux[a:b] comme espace de travail auxiliaire
*/
static void tri_sousvecteur(double v[][2], int a, int b, double v_aux[][2])
{
  int n, c, i;

  n = b - a + 1;
  if (n < 2)
    return;

  c = a + (n + 1) / 2;
  tri_sousvecteur(v, a, c - 1, v_aux);
  tri_sousvecteur(v, c, b, v_aux);
  
  for (i = a; i <= b; i++)
  {
  	v_aux[i][0] = v[i][0];
  	v_aux[i][1] = v[i][1];
  }

  fusion(v_aux, a, c - 1, v_aux, c, b, v + a);
}

/* 
   Tri du vecteur v de taille n, par ordre croissant, en utilisant
   le vecteur v_aux, de même taille, comme espace de travail auxiliaire
 */
static void tri_fusion(double v[][2], int n, double v_aux[][2])
{
  tri_sousvecteur(v, 0, n - 1, v_aux);
}
/* Les fonctions de mergesort ci-dessus sont des adaptations en 2D (où on ne trie que la 1ere dimension et l'autre suit)
   du mergesort enseigné par M. Boigelot en "Introduction à l'informatique".
 */

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

	int nbSketches = dsGetNbSketches(ds);
	double allDistances[nbSketches][2];
	double v_aux[nbSketches][2];

	for(int i=0; i < nbSketches; i++)
	{
		allDistances[i][0] = distance(sk,dsGetSketch(ds,i));
		allDistances[i][1] = (float) i;
	}
	tri_fusion(allDistances,nbSketches,v_aux);

	for(int i=0;i < k; i++)
	{
		distances[i] = allDistances[i][0];
		neighbors[i] = (int) allDistances[i][1];
	}

	KitsuNeNinetails->k         = k;
	KitsuNeNinetails->dataset   = ds;
	KitsuNeNinetails->neighbors = neighbors;
	KitsuNeNinetails->distances = distances;

	return KitsuNeNinetails;
}

void recFreekNN(kNN *knn)
{
	free(knn->neighbors);
	free(knn->distances);
	free(knn);
	return;
}

char *recGetMajorityLabel(kNN *knn)
{
	int n     = dsGetNbLabelNames(knn->dataset);
	int label = 0;
	int numbers[n];
	double totalDistances[n];
	for(int i=0; i < n; i++)
		{
			numbers[i]        = 0;
			totalDistances[i] = 0;
		}

	for(int i=0; i < knn->k; i++)
		{
			label = dsGetLabel(knn->dataset,knn->neighbors[i]);
			numbers[label]++;
			totalDistances[label] += knn->distances[i];
		}

	int majority = 0;
	for(int i=1; i < n; i++)
	{
		if(numbers[i]>numbers[majority])
			majority = i;
		else if(numbers[i]==numbers[majority])
			if (totalDistances[i] < totalDistances[majority])
			// je gère les égalités en comparant les sommes de toutes distances => minimiser somme erreurs (=?meilleure solution)
				majority = i;
	}
	
	return dsGetLabelName(knn->dataset,majority);

}

float recEvalkNN(Dataset *referenceset, Dataset *testset, int k, double (*distance)(Sketch *, Sketch *),FILE *out)
{
	int n                 = dsGetNbSketches(testset);
	int correctLabelFound = 0;
	char *label;
	char *correctLabel;
	
	for(int i=0; i < n; i++)
	{
		kNN *KitsuNeNinetails = recNearestNeighbors(dsGetSketch(testset,i), referenceset, k, distance);
		label = recGetMajorityLabel(KitsuNeNinetails);
		correctLabel = dsGetLabelName(testset,dsGetLabel(testset,i));
		if(strcmp(label,correctLabel)==0)
			correctLabelFound++;
		else
		{
			printf("neighbors: ");
			for(int j=0; j < k; j++)
			{
				printf("%s (%f), ",dsGetLabelName(referenceset,dsGetLabel(referenceset,KitsuNeNinetails->neighbors[j])),KitsuNeNinetails->distances[j]);
			}
			printf("\n");
		}
		if (out)
		{
			fprintf(out, "Testing sketch n°%d /%d :\n",i+1,n);
			fprintf(out, "Label found: %s , Correct label: %s\n",label,correctLabel);
			fprintf(out, "Correct labels found: %d/%d\n",correctLabelFound,i+1);
			// Il y a un bug sur le label "key" qui est bien compté et comparé mais n'est pas print (probablement à cause du symbole de retour à la ligne)
		}
		recFreekNN(KitsuNeNinetails);
	}
	float accuracy = 100 * (float) correctLabelFound / (float) n;
	return accuracy;
}