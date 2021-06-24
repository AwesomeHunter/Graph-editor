#include "vector2.h"

#include <math.h>

Vector2 divVector2(Vector2 a, Vector2 b){
    Vector2 vec = {a.x / b.x, a.y / b.y};
    return vec;
}

Vector2 multVector2(Vector2 a, Vector2 b){
    Vector2 vec = {a.x * b.x, a.y * b.y};
    return vec;
}

Vector2 addVector2(Vector2 a, Vector2 b){
    Vector2 vec = {a.x + b.x, a.y + b.y};
    return vec;
}

Vector2 subtrVector2(Vector2 a, Vector2 b){
    Vector2 vec = {a.x - b.x, a.y - b.y};
    return vec;
}

Vector2 divScalar(Vector2 a, double b){
    Vector2 vec = {a.x / b, a.y / b};
    return vec;
}

Vector2 multScalar(Vector2 a, double b){
    Vector2 vec = {a.x * b, a.y * b};
    return vec;
}

Vector2 addScalar(Vector2 a, double b){
    Vector2 vec = {a.x + b, a.y + b};
    return vec;
}

Vector2 subtrScalar(Vector2 a, double b){
    Vector2 vec = {a.x - b, a.y - b};
    return vec;
}

double lengthVector2(Vector2 a){
    return sqrt((a.x * a.x) + (a.y * a.y));
}

double distanceVector2(Vector2 a, Vector2 b){
    return lengthVector2(subtrVector2(a, b));
}

Vector2 normalizeVector2(Vector2 a){
    double len = lengthVector2(a);
    Vector2 vec = divScalar(a, len);
    return vec;
}
