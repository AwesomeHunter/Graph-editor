#ifndef H_POINT
#define H_POINT

typedef struct Point{
    double x, y;
} Point;

Point divPoint(Point a, Point b);
Point multPoint(Point a, Point b);
Point addPoint(Point a, Point b);
Point subtrPoint(Point a, Point b);

Point divScalar(Point a, double b);
Point multScalar(Point a, double b);
Point addScalar(Point a, double b);
Point subtrScalar(Point a, double b);

#endif