#include <stdio.h>
#include <stdlib.h>

#include "Dataset.h"
#include "Sketch.h"
#include "PointLine.h"

struct dataset_t
{
  char   **labelNames; // The label names
  int      nbLabels;   // The number of labels
  Sketch **sketches;   // The array of sketches
  int     *labels;     // The array of labels
  int      nbSketches; // The number of sketches
};

Dataset *dsLoad(const char *filepath, FILE *out)
{

  Dataset *ds = malloc(sizeof(Dataset));

  if (!ds)
    return NULL;

  FILE *fp = fopen(filepath, "r");
  if (!fp)
  {
    free(ds);
    return NULL;
  }

  // extract the labels

  fscanf(fp, "%d", &(ds->nbLabels));

  char buffer[1000];
  fgets(buffer, 1000, fp);
  fgets(buffer, 1000, fp);

  ds->labelNames = malloc(ds->nbLabels * sizeof(char *));

  int indexLabel = 0;
  int i = 0;
  while (buffer[i] != '\n' && buffer[i] != '\0')
  {
    int j = i;
    while (buffer[j] != ',' && buffer[j] != '\0' && buffer[j] != '\n')
      j++;
    ds->labelNames[indexLabel] = malloc((j - i + 1) * sizeof(char));
    for (int k = i; k < j; k++)
      ds->labelNames[indexLabel][k - i] = buffer[k];
    ds->labelNames[indexLabel][j - i] = '\0';
    indexLabel++;
    i = j + 1;
  }

  // extract the sketches and their labels

  fscanf(fp, "%d", &(ds->nbSketches));

  if (out != NULL)
    fprintf(out, "Reading %d sketches from '%s'...\n", ds->nbSketches, filepath);

  ds->sketches = malloc(ds->nbSketches * sizeof(Sketch *));
  if (!ds->sketches)
  {
    free(ds);
    fclose(fp);
    return NULL;
  }
  ds->labels = malloc(ds->nbSketches * sizeof(int));
  if (!ds->labels)
  {
    free(ds->sketches);
    free(ds);
    fclose(fp);
    return NULL;
  }

  for (int i = 0; i < ds->nbSketches; i++)
  {
    int size_ts;
    fscanf(fp, "%d", &(ds->labels[i]));
    fscanf(fp, "%d", &size_ts);

    Point points[size_ts];
    bool strokeStarts[size_ts];

    for (int j = 0; j < size_ts; j++)
    {
      int bv;
      fscanf(fp, "%d %d %d", &(points[j].x), &(points[j].y), &bv);
      strokeStarts[j] = bv;
    }
    ds->sketches[i] = sketchCreate(size_ts, points, strokeStarts);
  }

  fclose(fp);

  return ds;
}

void dsFree(Dataset *dataset)
{
  if (!dataset)
    return;

  for (int i = 0; i < dataset->nbSketches; i++)
    sketchFree(dataset->sketches[i]);

  free(dataset->sketches);
  free(dataset->labels);
  free(dataset);
}

void dsCompress(Dataset *ds, double threshold)
{
  int pointbefore = 0;
  int pointafter = 0;

  printf("Dataset compression...");

  for (int i = 0; i < ds->nbSketches; i++)
  {
    pointbefore += sketchGetNbPoints(ds->sketches[i]);
    Sketch *newsketch = sketchCompress(ds->sketches[i], threshold);
    sketchFree(ds->sketches[i]);
    ds->sketches[i] = newsketch;
    pointafter += sketchGetNbPoints(ds->sketches[i]);
  }

  printf("Done (from %d points to %d, %d%%)\n", (int)pointbefore, (int)pointafter,
         (int)((float)(pointbefore - pointafter) / (float)pointbefore * 100));
}

int dsGetNbSketches(Dataset *dataset)
{
  return dataset->nbSketches;
}

Sketch *dsGetSketch(Dataset *dataset, int i)
{
  return dataset->sketches[i];
}

int dsGetLabel(Dataset *dataset, int i)
{
  return dataset->labels[i];
}

int dsGetNbLabelNames(Dataset *dataset)
{
  return dataset->nbLabels;
}

char *dsGetLabelName(Dataset *dataset, int label_index)
{
  return dataset->labelNames[label_index];
}
