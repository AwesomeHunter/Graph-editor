#ifndef H_VECTOR
#define H_VECTOR

typedef struct Vector2{
    double x, y;
} Vector2;

Vector2 divVector2(Vector2 a, Vector2 b);
Vector2 multVector2(Vector2 a, Vector2 b);
Vector2 addVector2(Vector2 a, Vector2 b);
Vector2 subtrVector2(Vector2 a, Vector2 b);

Vector2 divScalar(Vector2 a, double b);
Vector2 multScalar(Vector2 a, double b);
Vector2 addScalar(Vector2 a, double b);
Vector2 subtrScalar(Vector2 a, double b);

double lengthVector2(Vector2 a);
double distanceVector2(Vector2 a, Vector2 b);
Vector2 normalizeVector2(Vector2 a);

#endif