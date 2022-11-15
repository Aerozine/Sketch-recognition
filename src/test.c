#include <stdio.h>
#include <stdlib.h>
#include "PointLine.h"
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
    int y = 5;
    Point *tab = malloc(sizeof(Point) * y); 

  //for( int i = 0; i < y; i++)
  //{
   tab[0].x = 1;tab[1].x = 1;tab[2].x = 6;tab[3].x = 9;tab[4].x = 11;
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
compress.length = y-1;
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
  Point b={.x=1,.y=1};
  Point c={.x=2,.y=0};
  printf("%f\n",plDistanceToSegment(c,a,b));
}
int main(){
  //checkplDistanceToSegment();
  //polydist();
  testcompress();
  return 0;
}
