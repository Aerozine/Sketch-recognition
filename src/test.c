#include <stdio.h>
#include <stdlib.h>
#include "PointLine.h"
#include "Sketch.h"
void checkplDistanceToSegment();


void polydist();
void polydist(){
   int y = 5;
   Point *tab = malloc(sizeof(Point) * y); 
   tab[0].x = 1;tab[1].x = 1;tab[2].x = 6;tab[3].x = 9;tab[4].x = 11;
   tab[0].y = 1;tab[1].y = 6;tab[2].y = 6;tab[3].y = 3;tab[4].y = 1 ;
   //tab[0].x = 1;tab[1].x = 6;tab[2].x = 9;tab[3].x = 11;
   //tab[0].y = 1;tab[1].y = 6;tab[2].y = 3;tab[3].y = 1 ;
PolyLine compress;
compress.length = y-1;
compress.points = tab;
double dMax = 0.2;
Point hello={.x=3,.y=9};
printf("%f---\n",plDistanceToPolyline(tab[0],compress,dMax));
}

void testcompress();
void testcompress(){
    int y = 6;
    Point *tab = malloc(sizeof(Point) * y); 

  //for( int i = 0; i < y; i++)
  //{
   tab[0].x = 1;tab[1].x = 1;tab[2].x = 6;tab[3].x = 9;tab[4].x = 11;tab[5].x=11;tab[5].y=1;
   tab[0].y = 1;tab[1].y = 6;tab[2].y = 6;tab[3].y = 3;tab[4].y = 1 ;
   //tab[0].x = 1;tab[1].x = 6;tab[2].x = 9;tab[3].x = 11;
   //tab[0].y = 1;tab[1].y = 6;tab[2].y = 3;tab[3].y = 1 ;
  //}

for(int i = 0; i < y; i++)
  {
    printf("[%d , %d]", tab[i].x, tab[i].y); 
  }
  printf("\n");

printf("compression: \n");

PolyLine compress;
compress.length = y;
compress.points = tab;
double dMax = 0.2;
PolyLine new;

new = plCompressPolyline( compress,dMax );
y = new.length;

printf(" taille du nouveau poly = %d\n",y);

printf("\ [x , y]:\n");
for(int i = 0; i < y; i++)
    printf("[%d , %d]", new.points[i].x, new.points[i].y); 
  
printf("\n");

}
void checkplDistanceToSegment(){
  Point a={.x=1,.y=1};
  Point b={.x=11,.y=1};
  Point c={.x=6,.y=1};
  printf("%f\n",plDistanceToSegment(c,a,b));
}
void allocsketch(){
 Point tabofpoint[12]={{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=1,.y=1},{.x=2,.y=2}};
 Point tabofpoint2[12]={{.x=0,.y=0},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2},{.x=0,.y=2}};
 bool stokestar[]={1,0,0,0,0,0,0,1,0,0,0,0};
  Sketch*draw=sketchCreate(12,tabofpoint,stokestar);
  Sketch*draw2=sketchCreate(12,tabofpoint2,stokestar);
  int nbpoint=sketchGetNbPoints(draw);
int nbstroke= sketchGetNbStrokes(draw);
printf("|------------print all polyline---------------|\n");
  for (int i = 0; i < nbstroke; i++) {
    PolyLine poly= sketchGetStroke(draw,i);
    printf("-->%d polyline with size %d \n",i,poly.length);
   for (int k = 0; k <poly.length ; k++) {
   printf("(%d,%d)",poly.points[k].x,poly.points[k].y); 
   } 
    printf("\n");
  }
printf("nb of point:\t%d\nnb of stroke:\t%d\n",nbpoint,nbstroke);
printf("|------------------compression----------------|\n");
Sketch *beautified=sketchCompress(draw,0.2);
    nbstroke=sketchGetNbStrokes(beautified);
  for (int i = 0; i < nbstroke; i++) {
    PolyLine poly= sketchGetStroke(beautified,i);
    printf("-->%d polyline with size %d \n",i,poly.length);
   for (int k = 0; k <poly.length ; k++) {
   printf("(%d,%d)",poly.points[k].x,poly.points[k].y); 
   } 
    printf("\n");
  }
printf("|------------------Hausdroff<3-----------------|\n");
printf("%f\n",sketchDistanceHausdorff(draw, draw2));
sketchFree(draw);
}



int main(){
 checkplDistanceToSegment();
  //polydist();
//  testcompress();
  allocsketch();
  return 0;
}
