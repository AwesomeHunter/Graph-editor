#include "point.h"

Point divPoint(Point a, Point b){
    Point point = {a.x / b.x, a.y / b.y};
    return point;
}

Point multPoint(Point a, Point b){
    Point point = {a.x * b.x, a.y * b.y};
    return point;
}

Point addPoint(Point a, Point b){
    Point point = {a.x + b.x, a.y + b.y};
    return point;
}

Point subtrPoint(Point a, Point b){
    Point point = {a.x - b.x, a.y - b.y};
    return point;
}

Point divScalar(Point a, double b){
    Point point = {a.x / b, a.y / b};
    return point;
}

Point multScalar(Point a, double b){
    Point point = {a.x * b, a.y * b};
    return point;
}

Point addScalar(Point a, double b){
    Point point = {a.x + b, a.y + b};
    return point;
}

Point subtrScalar(Point a, double b){
    Point point = {a.x - b, a.y - b};
    return point;
}