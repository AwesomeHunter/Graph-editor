#include "vector2.h"

Vector2 divVector2(Vector2 a, Vector2 b){
    Vector2 point = {a.x / b.x, a.y / b.y};
    return point;
}

Vector2 multVector2(Vector2 a, Vector2 b){
    Vector2 point = {a.x * b.x, a.y * b.y};
    return point;
}

Vector2 addVector2(Vector2 a, Vector2 b){
    Vector2 point = {a.x + b.x, a.y + b.y};
    return point;
}

Vector2 subtrVector2(Vector2 a, Vector2 b){
    Vector2 point = {a.x - b.x, a.y - b.y};
    return point;
}

Vector2 divScalar(Vector2 a, double b){
    Vector2 point = {a.x / b, a.y / b};
    return point;
}

Vector2 multScalar(Vector2 a, double b){
    Vector2 point = {a.x * b, a.y * b};
    return point;
}

Vector2 addScalar(Vector2 a, double b){
    Vector2 point = {a.x + b, a.y + b};
    return point;
}

Vector2 subtrScalar(Vector2 a, double b){
    Vector2 point = {a.x - b, a.y - b};
    return point;
}