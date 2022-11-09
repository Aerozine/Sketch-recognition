#include "PointLine.h"
#include "Sketch.h"

struct sketch_t
{
  int nbPoints;
  int nbStrokes;
  PolyLine *strokes;
};

Sketch *sketchCreate(int nbPoints, Point *points, bool *strokestarts)
{
  Sketch *sk = malloc(sizeof(Sketch))
  if(!sk)
    return NULL;

  int nbStrokes = 0;
  for(int i=0; i < nbPoints; i++)                                                                  // compter nombre de strokes
    if(strokestarts[i])
      nbStrokes++;

  PolyLine *strokes = malloc(nbStrokes*sizeof(PolyLine))
  if(!strokes)
  {
    free(sk);
    return NULL;
  }

  int lastEnd = 0;

  for(int i=0; i < nbStrokes; i++)
  {
    for(int length=0; (lastEnd+length) < nbPoints && !strokestarts[lastEnd+length]; length++);     // compter longueur à chaque stroke

    Point *stPoints = malloc(length*sizeof(Point));
    if(!stPoints)
    {
      for(int k=0; k < i; k++)
        free(strokes[k]->points);                                                                  // tout free si erreur d'allocation
      free(strokes)
      free(sk);
      return NULL;
    }
    
    for(int k=0; k < length; k++)
      stPoints[k] = points[lastEnd+k];

    strokes[i]->points = stPoints;
    strokes[i]->length = length;
    lastEnd += length;
  }

  sk->nbPoints  = nbPoints;
  sk->nbStrokes = nbStrokes;
  sk->strokes   = strokes;

  return sk;
}

void sketchFree(Sketch *sk)
{
  for(int i=0; i < sketchGetNbStrokes(sk); i++)
    free(sk->strokes[i]->points);
  free(sk->strokes);
  free(sk);
  return;
}

int sketchGetNbPoints(Sketch *sk)
{
  return sk->nbPoints;
}

int sketchGetNbStrokes(Sketch *sk)
{
  return sk->nbStrokes;
}

PolyLine sketchGetStroke(Sketch *sk, int i)
{
  return sk->strokes[i];
}

Sketch *sketchCompress(Sketch *sk, double dMax)
{
  Sketch *compactSk = malloc(sizeof(Sketch));
  if(!compactSk)
    return NULL;

  int nbStrokes = sketchGetNbStrokes(sk);
  
  PolyLine *strokes = malloc(nbStrokes*sizeof(PolyLine));
  if(!strokes)
  {
    free(sk);
    return NULL;
  }

  int nbPoints = 0;
  for(int i=0; i < nbStrokes; i++)
  {
    strokes[i] = plCompress(sketchGetStroke(sk,i),dMax);
    nbPoints  += strokes[i]->length;
  }

  compactSk->nbPoints  = nbPoints;
  compactSk->nbStrokes = nbStrokes;
  compactSk->strokes   = strokes;

  return compactSk;
}

double sketchDistanceHausdorff(Sketch *sk1, Sketch *sk2)
{}

double sketchDistanceCustom(Sketch *sk1, Sketch *sk2)
{}

