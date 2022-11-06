#include "PointLine.h"
#include "Sketch.h"

struct sketch_t // juju est triste
{
  // A compléter
};

// A compléter

Sketch *sketchCreate(int nbPoints, Point *points, bool *strokestarts){}

int sketchGetNbStrokes(Sketch *sk){}
PolyLine sketchGetStroke(Sketch *sk, int i){}
int sketchGetNbPoints(Sketch *sk){}

void sketchFree(Sketch *sk){}

Sketch *sketchCompress(Sketch *sk, double dMax){}
double sketchDistanceHausdorff(Sketch *sk1, Sketch *sk2){}

double sketchDistanceCustom(Sketch *sk1, Sketch *sk2){}

