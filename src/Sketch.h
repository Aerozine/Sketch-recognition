#ifndef SKETCH_H_DEFINED
#define SKETCH_H_DEFINED

#include "PointLine.h"

// Opaque structure representing a sketch.
typedef struct sketch_t Sketch;


/***
 * Create a new sketch from a list of strokes.
 * 
 * PARAMETERS
 * nbPoints         The total number of points in the sketch
 * points           A array of points
 * strokestarts     A boolean array indicating where each stroke starts
 *
 * RETURN
 * sketch           A pointer to the newly created sketch
 *
 */

Sketch *sketchCreate(int nbPoints, Point *points, bool *strokestarts);

/***
 * Return the number of strokers in the sketch
 * 
 * PARAMETERS
 * sketch           The sketch
 *
 * RETURN
 * n                The number of strokes in the sketch
 *
 */

int sketchGetNbStrokes(Sketch *sk);

/***
 * Get the i-th stroke (a polyline) of the sketch.
 * 
 * PARAMETERS
 * sketch           The sketch
 * i                The index of the stroke
 *
 * RETURN
 * pl               The i-th stroke (a polyline) of the sketch
 *
 */

PolyLine sketchGetStroke(Sketch *sk, int i);

/***
 * Return the total number of points in the sketch.
 * 
 * PARAMETERS
 * sketch           The sketch
 *
 * RETURN
 * n                The total number of points in the sketch
 *
 */

int sketchGetNbPoints(Sketch *sk);

/***
 * Free the memory used by the sketch.
 * 
 * PARAMETERS
 * sketch           The sketch
 *
 */

void sketchFree(Sketch *sk);

/***
 * Compress the sketch using the Ramer-Douglas-Peucker algorithm.
 * 
 * PARAMETERS
 * sketch           The sketch
 * dMax             The compression threshold
 *
 * RETURN
 * newsketch        A new compressed sketch
 *
 */

Sketch *sketchCompress(Sketch *sk, double dMax);

/***
 * Compute the Hausdorff distance between two sketches
 * 
 * PARAMETERS
 * sketch           The first sketch
 * sketch           The second sketch
 *
 * RETURN
 * d                The Hausdorff distance between the two sketches
 *
 */

double sketchDistanceHausdorff(Sketch *sk1, Sketch *sk2);

/***
 * Compute a customized distance between two sketches
 * 
 * PARAMETERS
 * sketch           The first sketch
 * sketch           The second sketch
 *
 * RETURN
 * d                The custom distance between the two sketches
 *
 */

double sketchDistanceCustom(Sketch *sk1, Sketch *sk2);

#endif // SKETCH_H_DEFINED
