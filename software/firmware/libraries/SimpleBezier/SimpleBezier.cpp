#include "SimpleBezier.h"

// Bezier routines
float Bez4(float t) { return t*t*t; }
float Bez3(float t) { return 3*t*t*(1-t); }
float Bez2(float t) { return 3*t*(1-t)*(1-t); }
float Bez1(float t) { return (1-t)*(1-t)*(1-t); }
float bezierAt(float t, float c1,float c2,float c3,float c4) {
  return c1*Bez1(t) + c2*Bez2(t) + c3*Bez3(t) + c4*Bez4(t);
}
