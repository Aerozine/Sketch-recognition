#include "PointLine.h"
#include <stdbool.h>
#include  <stdlib.h> 
#include <math.h>
PolyLine plCompression(PolyLine poly, double dmax,bool *dpoint,int *finalLength);
PolyLine plRealCompressPolyline(PolyLine polyLine,double dMax);
double plDistance(Point p1, Point p2){
return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

double plDistanceToSegment(Point p, Point p1, Point p2){
 int A = p.x - p1.x;
 int B = p.y - p1.y;
 int C = p2.x - p1.x;
 int D = p2.y - p1.y;
 Point tmp={.x=0,.y=0};
 int dot = A * C + B * D;
 int len_sq = C * C + D * D;
 double param = -1;
  if (len_sq != 0){ 
      param = (double)dot / (double)len_sq;
  }else {
    return 0;
  }

  if (param < 0) {
    tmp.x = p1.x;
    tmp.y= p1.y;
  }
  else if (param > 1) {
    tmp.x = p2.x;
    tmp.y = p2.y;
  }
  else {
   tmp.x = p1.x + param * C;
   tmp.y = p1.y + param * D;
  }
  return plDistance(p,tmp);
}

double plDistanceToPolyline(Point p, PolyLine polyLine, double distanceMax){
  double lowest=INFINITY;
  double tmp;
  int i;
for (i = 1; i < polyLine.length;i++) {
    if((tmp = plDistanceToSegment(p,polyLine.points[i-1],polyLine.points[i]))<=distanceMax)
    {
      return distanceMax;
    }
    if(lowest>tmp) 
    lowest=tmp;
  }
  return lowest;
  
  }
PolyLine plCompressPolyline(PolyLine poly,double dMax){
bool*delpoint=calloc(poly.length,sizeof(bool));
PolyLine compressed={.length=poly.length};
PolyLine processed=plCompression(poly,dMax,delpoint,&compressed.length);
compressed.points=malloc(sizeof(Point)*compressed.length);
for (int i = 0,j=0; i <compressed.length;j++) {
 if(!delpoint[j]){
      compressed.points[i]=processed.points[j];
      i++;
    } 
  }
  printf("\n");
free(delpoint);
return compressed;
}

PolyLine plCompression(PolyLine poly, double dmax,bool *dpoint,int *finalLength){

if(poly.length<3)
    return poly;
int star;
double d,dtot=-INFINITY;
  for (int i = 1,j=1; i < poly.length-1; j++) {
 if(!dpoint[j]){
      i++;
        if((d=plDistanceToSegment(poly.points[j],poly.points[0],poly.points[poly.length-1]))>dtot)
      {
        dtot=d,star=i;
      }
      } 
  }
if(dtot>dmax){
    PolyLine tmp={.points=poly.points,.length=star};
    plCompression(tmp,dmax,dpoint,finalLength);
      star--;
    tmp.points=(poly.points+star),tmp.length=poly.length-star;
    plCompression(tmp,dmax,dpoint+star,finalLength);
    return poly;
  }else{
    for (int k = 1; k < poly.length-1; k++) {
      dpoint[k]=true;
      *finalLength=*finalLength-1;
    }
    return poly;
  }
}
