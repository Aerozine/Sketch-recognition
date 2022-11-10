#include "PointLine.h"
#include "Sketch.h"
#include <math.h>

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

static double minDistanceToSegment(Sketch *sk, Point p)
{
  double dmin = pow(10.0,37);                                                                      // dmin initial = plus grande valeur double (10^37), dm(n) toujours < dm(0)
  double de   = 0;
  Point p1, p2;
  PolyLine stroke;

  for(int i=0; i < sketchGetNbStrokes(sk), i++)                                                    // comparer distance entre 1 point et chaque segment du sketch
  {
    stroke = sketchGetStroke(sk,i);
    for(int j=0; j < stroke->length-1; j++)
    {
      if(stroke->length > 1)
      {
        p1 = stroke->points[j];
        p2 = stroke->points[j+1];
        de = plDistanceToSegment(p,p1,p2);
        if (de < dmin)
          dmin = de;
      }
        
    }
  }

  return dmin;
}

static double maxDistancePoints(Sketch *sk1, Sketch *sk2)
{
  double dmax = 0;
  double dmin = 0;
  Point p;
  PolyLine stroke;

  for(int i=0; i < sketchGetNbStrokes(sk1); i++)
  {
    stroke = sketchGetStroke(sk1,i);
    for(int j=0; j < stroke->length; j++)
    {
      p = stroke->points[j];
      dmin = minDistanceToSegment(sk2, p);
      if(dmin > dmax)
        dmax = dmin;
    }
  }

  return dmax;
}

double sketchDistanceHausdorff(Sketch *sk1, Sketch *sk2)
{
  double dmax1 = maxDistancePoints(sk1,sk2);
  double dmax2 = maxDistancePoints(sk2,sk1);

  if (dmax1>=dmax2)
    return dmax1;
  else 
    return dmax2;
}

double sketchDistanceCustom(Sketch *sk1, Sketch *sk2)
{
  return sketchDistanceHausdorff(sk1,sk2);                                                         // A faire
}

