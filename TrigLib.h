#ifndef _TRIGLIB_H_
#define _TRIGLIB_H_

#ifndef PI
#define PI 3.1415926535
#endif

#include <cmath>

#define cosd(currentAngle) cos((currentAngle) * PI / 180)
#define sind(currentAngle) sin((currentAngle) * PI / 180)
#define tand(currentAngle) tan((currentAngle) * PI / 180)
#define acosd(x) (acos(x) * 180 / PI)
#define asind(x) (asin(x) * 180 / PI)
#define atand(x) (atan(x) * 180 / PI)
#define atand2(y, x) (atan2((y), (x)) * 180 / PI)

#endif