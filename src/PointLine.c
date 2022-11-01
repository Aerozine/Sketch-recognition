#include "PointLine.h"
double plDistance(Point p1, Point p2){
return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))
}

double plDistanceToSegment(Point p, Point p1, Point p2){

}
double plDistanceToPolyline(Point p, PolyLine polyLine, double distanceMax){

}
PolyLine plCompressPolyline(PolyLine polyLine, double dMax){

}

