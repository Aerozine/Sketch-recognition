#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "PointLine.h"
#include "Sketch.h"
#include "Dataset.h"
#include "Recognizer.h"
#include "easyppm.h"

/** ------------------------------------------------------------------------ *
 * Save a ppm file representing the sketch.
 *
 * PARAMETERS
 * sk                The sketch to represent
 * filename          A string containing the name of the ppm file (should end
 *                   with .ppm)
 * ------------------------------------------------------------------------- */
static void sketch2ppm(Sketch *sk, const char *filename)
{
  int bbxmax = 255;
  int bbymax = 255;

  PPM ppm = easyppm_create(bbxmax + 10, bbymax + 10, IMAGETYPE_PPM);
  ppmcolor black = easyppm_rgb(0, 0, 0);
  ppmcolor white = easyppm_rgb(255, 255, 255);
  easyppm_clear(&ppm, white);

  for (int i = 0; i < sketchGetNbStrokes(sk); i++)
  {
    PolyLine pl = sketchGetStroke(sk, i);
    for (int j = 0; j < pl.length - 1; j++)
      easyppm_draw_line(&ppm, 5 + pl.points[j].x, 5 + pl.points[j].y,
                        5 + pl.points[j + 1].x, 5 + pl.points[j + 1].y, black);
  }

  easyppm_write(&ppm, filename);
  easyppm_destroy(&ppm);
}

/** ------------------------------------------------------------------------ *
 * Print the help message.
 *
 * PARAMETERS
 * programName   The name of the program
 * ------------------------------------------------------------------------- */
static void printHelp(const char *programName)
{
  fprintf(
      stderr,
      "%s %s %s\n",
      "Usage:",
      programName,
      "[-i item_index] [-k n_neighbours] [-c compression_threshold]"
      "[-o output_path] [-d hausdorff|custom] train_filepath test_filepath");
}

/** ------------------------------------------------------------------------ *
 * Free all arguments, according to its type
 *
 * PARAMETERS
 * trainingset   Something to free
 * testingset    Something to free
 * neighbours    Something to free
 * ------------------------------------------------------------------------- */
static void freeAll(Dataset *trainingset, Dataset *testingset,
                    kNN *neighbours)
{
  if (trainingset != NULL)
    dsFree(trainingset);
  if (testingset != NULL)
    dsFree(testingset);
  if (neighbours != NULL)
    recFreekNN(neighbours);
}

/** ------------------------------------------------------------------------ *
 * Save the given sketch in folder `output_path`
 *
 * PARAMETERS
 * output_path   The folder in which to save the sketch
 * sketch        The sketch to save
 * distance      The distance with the query
 * ------------------------------------------------------------------------- */
static void saveNeighbour(const char *output_path, Sketch *sketch,
                          char *label, double distance)
{
  char buffer[256];
  sprintf(buffer, "%s/image_%s_%5.2lf.ppm", output_path,
          label, distance);
  sketch2ppm(sketch, buffer);
}

/** ------------------------------------------------------------------------ *
 * Search for the k nearest neighbours of a given sketch in a given
 * reference database or compute the error rate over the full testing database.
 *
 * SYNOPSIS
 * search [-i item_index] [-k n_neighbours] [-o output_path] train_filepath
 *        test_filepath
 *
 * DESCRIPTION
 * -i item_index      The index of the test sample to use. If no index is
 *                    provided, compute the error rate over all test examples.
 * -k n_neighbours    The number of neighbours to compute (Default: 5).
 * -o output_path     The folder in which to save the sketches (Default: no save).
 * train_filepath     The path to the reference database
 * test_filepath      The path to the testing database
 * ------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  srand(time(NULL));

  /* ------------------------------ Parsing ------------------------------ */
  if (argc < 3)
  {
    fprintf(stderr, "Aborting; There must be at least 2 parameters"
                    " (given %d). \n",
            argc - 1);
    printHelp(argv[0]);
    return EXIT_FAILURE;
  }

  int index = -1;
  int k = 5;
  double compressionThreshold = 0.0;
  char *output_path = NULL;
  char *train_path = argv[argc - 2];
  char *test_path = argv[argc - 1];
  double (*distance)(Sketch *, Sketch *) = sketchDistanceHausdorff;
  
  int argi = 1;

  while (argi < argc - 2)
  {
    if (strcmp(argv[argi], "-i") == 0)
    {
      size_t itemIndex;
      if (sscanf(argv[++argi], "%zu", &itemIndex) != 1)
      {
        fprintf(stderr, "%s\n", "Aborting; item index ('-i') should be"
                                " an unsigned integer.");
        return EXIT_FAILURE;
      }
      index = itemIndex;
    }
    else if (strcmp(argv[argi], "-k") == 0)
    {
      size_t numberOfNeigbours;
      if (sscanf(argv[++argi], "%zu", &numberOfNeigbours) != 1)
      {
        fprintf(stderr, "%s\n", "Aborting; number of neighbours ('-k')"
                                " should be an unsigned integer.");
        return EXIT_FAILURE;
      }
      k = numberOfNeigbours;
    }
    else if (strcmp(argv[argi], "-o") == 0)
    {
      output_path = argv[++argi];
    }
    else if (strcmp(argv[argi], "-d") == 0)
    {
      argi++;
      if (strcmp(argv[argi], "hausdorff") == 0)
	distance = sketchDistanceHausdorff;
      else if (strcmp(argv[argi], "custom") == 0)
	distance = sketchDistanceCustom;
      else {
	printf("Aborting: unknown distance %s\n", argv[argi]);
	printHelp(argv[0]);
	return EXIT_FAILURE;
      }
    }
    else if (strcmp(argv[argi], "-c") == 0)
    {
      float thres;
      if (sscanf(argv[++argi], "%f", &thres) != 1)
      {
        fprintf(stderr, "%s\n", "Aborting; compression threshold ('-c')"
                                " should be a float number.");
        return EXIT_FAILURE;
      }
      compressionThreshold = thres;
    }
    else
    {
      fprintf(stderr, "%s '%s'\n", "Aborting; unknown parameter",
              argv[argi]);
      printHelp(argv[0]);
      return EXIT_FAILURE;
    }
    argi++;
  }

  Dataset *trainingset = NULL;
  Dataset *testingset = NULL;
  kNN *neighbours = NULL;

  /* --------------------------- Load sketches --------------------------- */
  trainingset = dsLoad(train_path, stdout);
  if (!trainingset)
  {
    fprintf(stderr, "Aborting; could not load training set (%s)\n",
            train_path);
    freeAll(trainingset, testingset, neighbours);
    return EXIT_FAILURE;
  }
  testingset = dsLoad(test_path, stdout);
  if (!testingset)
  {
    fprintf(stderr, "Aborting; could not load testing set (%s)\n",
            test_path);
    freeAll(trainingset, testingset, neighbours);
    return EXIT_FAILURE;
  }
  /* --------------------- Compress datasets ------------------------*/
  if (compressionThreshold > 0.0)
  {
    dsCompress(trainingset, compressionThreshold);
    dsCompress(testingset, compressionThreshold);
  }

  if (index >= 0)
  {
    /* ---------------- Single nearest neighbours search ------------------*/
    index %= dsGetNbSketches(testingset); // to make sure we are in the range

    printf("Class of selected sketch %d is %s (from test set)\n", index,
           dsGetLabelName(testingset, dsGetLabel(testingset, index)));

    neighbours = recNearestNeighbors(dsGetSketch(testingset, index), trainingset,
				     k, distance);
    if (!neighbours)
    {
      fprintf(stderr, "%s\n", "Aborting, could not find the neighbours.");
      freeAll(trainingset, testingset, neighbours);
      return EXIT_FAILURE;
    }

    printf("Neighbours (from training set): \n");
    for (size_t i = 0; i < (size_t)k; ++i)
    {
      printf("%zu) class '%s' with distance %lf\n", i + 1,
             dsGetLabelName(trainingset, dsGetLabel(trainingset, neighbours->neighbors[i])),
             neighbours->distances[i]);
      if (output_path)
        saveNeighbour(output_path,
                      dsGetSketch(trainingset, neighbours->neighbors[i]),
                      dsGetLabelName(trainingset, dsGetLabel(trainingset, neighbours->neighbors[i])),
                      neighbours->distances[i]);
    }

    if (output_path)
    {
      char buffer[256];
      sprintf(buffer, "%s/query.ppm", output_path);
      sketch2ppm(dsGetSketch(testingset, index), buffer);
    }
    freeAll(trainingset, testingset, neighbours);
  }
  else
  {
    /* ----------- Compute the error rate over the test set --------------*/
    printf("Testing %d sketches with a reference set of %d sketches\n",
	   dsGetNbSketches(testingset), dsGetNbSketches(trainingset));
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    double errorrate = recEvalkNN(trainingset, testingset, k, distance, stdout);
    end = clock();
    
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Done in %.2fs\n\nAccuracy: %.2f%%\n", cpu_time_used, errorrate);

    freeAll(trainingset, testingset, NULL);
  }

  return EXIT_SUCCESS;
}
