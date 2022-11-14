#include "PointLine.h"
#include <stdbool.h>
#include  <stdlib.h> 
#include <math.h>
PolyLine plCompression(PolyLine polyLine,double dMax, bool*dpoint ,int reallength);
PolyLine plRealCompressPolyline(PolyLine polyLine,double dMax);
double plDistance(Point p1, Point p2){
return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

double plDistanceToSegment(Point p, Point p1, Point p2){
// calculate the line
int a=-(p2.y-p1.y);
int b=(p2.x-p1.x);
int c=-p1.y*b-p1.x*a;
if(a&&b){
  return (double)(abs(a*p.x+b*p.y+c))/plDistance(p1,p2);
  }
return plDistance(p,p1);
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
    if(lowest<tmp) 
    lowest=tmp;
  }
  return lowest;
  
  }
PolyLine plCompressPolyline(PolyLine polyLine,double dMax){
bool*delpoint=calloc(polyLine.length,sizeof(bool));
printf("entering plCompression\n");
PolyLine processed=plCompression(polyLine,dMax,delpoint,polyLine.length);
printf("end plCompression\n");
Point *clean=malloc(sizeof(Point)*(processed.length+1));
for (int i = 0,j=0; i < processed.length;j++) {
 if(!delpoint[j]){
      printf(" %d ",processed.points[j].x);
      clean[i]=processed.points[j];
      i++;
    } 
  }
  printf("\n");
free(delpoint);
processed.points=clean;
return processed;
}
PolyLine plCompression(PolyLine polyLine,double dMax, bool*dpoint ,int reallength){
printf("length %d \n",polyLine.length);
for (int i = 1,j=0; i < polyLine.length; j++) {
 if(!dpoint[j]){
      printf("(%d,%d)\t",polyLine.points[j].x,polyLine.points[j].y);
      i++;
    } 
}
  printf("\n");
if(polyLine.length<=1)
    return polyLine;
double d,dtot=-INFINITY;
int star,realstar;
for (int i = 1,j=1; i < polyLine.length; j++) {
    if(!dpoint[j]){
    if((d = plDistanceToSegment(polyLine.points[j],polyLine.points[0],polyLine.points[reallength]))>dtot){
        dtot=d;
        star=i;
        realstar=j;
      }
      i++;
    } 
}

if(dtot>dMax){
  PolyLine p1={.length=star,.points=polyLine.points};
  printf("p1 : %d length \n",star);
  plCompression(p1,dMax,dpoint,reallength);
  PolyLine p2={.length=polyLine.length-star+1,.points=polyLine.points+realstar};
  printf("p2 : %d length \n",polyLine.length-star);
  plCompression(p2,dMax,dpoint+realstar,reallength-realstar);
  polyLine.length=p1.length+p2.length-1;
  return polyLine;
  }else {
    printf("dtot<dMax\n");
    for (int k = 1; k <reallength; k++) {
      dpoint[k]=true;
    }
    printf("clearing %d points\n",reallength-1);
  return polyLine; 
  }

}
/*
PolyLine plCompression(PolyLine polyLine,double dMax, bool*dpoint ,int reallength){
 if(polyLine.length<=1){
    printf("<2(%d %d)(%d %d)2>\n",polyLine.points[0].x,polyLine.points[0].y,polyLine.points[1].x,polyLine.points[1].y);
    return polyLine;
  }
  double val,maxval=-INFINITY;
  int starpos,i,j,starvalue=-2048;
for (i = 1,j=1; i < polyLine.length-1;j++) {
    if(!dpoint[j]){
      i++;
      if((val=plDistanceToSegment(polyLine.points[j],polyLine.points[0],polyLine.points[reallength]))>maxval)
         { 
          maxval=val;
          printf("maxval(%f)\n",maxval);
          printf("star val is %d \n",j);
          starpos=j;
          starvalue=i-1;
    }else {
      printf("incorect val %f at %d i %d j \n",val,i,j);
    }
    }
}
if(maxval<dMax){
    printf("cleaning-%d-\n",reallength,polyLine.length);
    for (int k = 1; k < reallength-1; k++) {
      printf("removing [%d]\n",k);
      dpoint[k]=true; 
    }
    return polyLine;
  }else{
  printf("cuttin\n");
   printf("-----------------\n");
   printf("from 0  to %d \n",starvalue); 
   printf("and from %d  to %d\n",polyLine.length-starvalue,polyLine.length); 
   printf("-----------------\n");
  PolyLine tmp={.length=starvalue,.points=polyLine.points};
  PolyLine result1 =plCompression(tmp,dMax,dpoint,starpos);
  tmp.length=polyLine.length-starvalue+1;
  tmp.points=tmp.points+starpos;
  printf("2 party of the cut \n");
  PolyLine tmp2 =plCompression(tmp,dMax,dpoint+starpos,reallength-starpos);
  polyLine.length=result1.length+tmp2.length;
  printf("after cut \n length is %d \n", polyLine.length);
    return polyLine ;
  }
}

PolyLine plCompressPolyline(PolyLine polyLine, double dMax){
bool*isnewpoint=malloc(sizeof(bool)*polyLine.length);
PolyLine processed=plRealCompressPolyline(polyLine,dMax,isnewpoint,polyLine.length);
Point*newpoints=malloc(processed.length*sizeof(Point));
for (int i = 0,j=0; i < processed.length;){
  if(processed.points[j].x>=0){
      newpoints[i]=processed.points[i];
      i++;
    }
    j++;
}
  free(polyLine.points);
  polyLine.points=newpoints;
return polyLine;
}

PolyLine plRealCompressPolyline(PolyLine polyLine,double dMax,bool *size,int sizebool){
  double maxval=-INFINITY;
  double val;
  unsigned starvalue;
if(polyLine.length<=2){
    return polyLine;
  }
for (int i = 1; i < polyLine.length-1; i++) {
 if((val=plDistanceToSegment(polyLine.points[i],polyLine.points[0],polyLine.points[polyLine.length]))>=maxval){
      maxval=val;
      starvalue=i;
    }
  }
if(maxval<dMax){
    for (int i = 1,j=1; i < polyLine.length-1;j++) {
      if(polyLine.points[j].x>=0){
      //disable all point inside ps and pe
     if(polyLine.points[j].x>0) 
     polyLine.points[j].x*=-1; 
     if(polyLine.points[j].y>0) 
     polyLine.points[j].y*=-1; 
    i++;
    }
    }
    polyLine.length=2;
    return polyLine;
  }else{
    PolyLine first={.length=starvalue,.points=polyLine.points};
    first=plRealCompressPolyline(first,dMax);
    PolyLine second={.length=polyLine.length-starvalue,.points=polyLine.points+starvalue};
    second=plRealCompressPolyline(second,dMax);
    first.length+=second.length;
    return first;
  }
}

  *for each point get the pldistance to segment 
  *get the point with a max dist
  * if p < dmax. get only pin and pout 
  * else plCompressPolyline(pin p ) and p pout  and merge 
  * 
  * return 
*/

