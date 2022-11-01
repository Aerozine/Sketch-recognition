#ifndef POINTLINE_H
#define POINTLINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* A point is a pair of coordinates (x,y) */
typedef struct point_t
{
  int x, y; // Coordinates of the point
} Point;

/* A polyline is a sequence of points */
typedef struct polyline_t
{
  Point *points; // List of the points of the polyline
  int length;    // Number of points in the polyline
} PolyLine;

/***
 * Compute the euclidean distance between two points
 * 
 * PARAMETERS
 * p1               The first point
 * p2               The second point
 *
 * RETURN
 * d                The euclidean distance between p1 and p2
 *
 */

double plDistance(Point p1, Point p2);

/***
 * Compute the euclidean distance between a point p and a line
 * segment defined by two points p1 and p2.
 *  
 * PARAMETERS
 * p                A point
 * p1               The starting point of the segment
 * p2               The end point of the segment
 *
 * RETURN
 * d                The distance from p to the segment defined
 *                  by p1 and p2.
 *
 */
 
double plDistanceToSegment(Point p, Point p1, Point p2);

/***
 * Compute the shortest distance from a point to a polyline. If the distance
 * is smaller than distanceMax, the function may simply return distanceMax
 * instead of computing the minimal distance.
 *
 * PARAMETERS
 * p                A point
 * polyLine         A polyline
 * distanceMax      A distance threshold
 *
 * RETURN
 * d                The distance from p to the polyline or any value <=
 *                  distanceMax if this distance is <= distanceMax
 */
double plDistanceToPolyline(Point p, PolyLine polyLine, double distanceMax);

/***
 * Compress a polyline using the Ramer-Douglas-Peucker algorithm. The function
 * should return a new polyline and should not modify the polyline given as
 * argument.
 * 
 * PARAMETERS
 * pl               A polyline
 * dMax             The compression threshold
 *
 * RETURN
 * newpl            The compressed polyline
 *
 */

PolyLine plCompressPolyline(PolyLine polyLine, double dMax);

#endif
