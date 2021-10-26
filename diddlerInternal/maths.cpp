#include <cmath>
#include "types.h"
#include "maths.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

const double pi = 3.1415926535L;

td::Vec3 quat2euler(td::Vec4 quat, int rot = 0, bool invert = false) {
    if (rot == 0) {
        float vecX = (2 * (quat.w * quat.y + quat.z * quat.x));
        float vecY = (2 * (quat.x * quat.y - quat.z * quat.w));
        float vecZ = (1 - 2 * (quat.w * quat.w + quat.x * quat.x));
        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    if (rot == 1) {
        float vecX = (1 - 2 * (quat.x * quat.x + quat.y * quat.y));
        float vecY = (2 * (quat.w * quat.x + quat.z * quat.y));
        float vecZ = (2 * (quat.w * quat.y - quat.z * quat.x));

        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    if (rot == 2) {
        float vecX = (2 * (quat.x * quat.z + quat.w * quat.y));
        float vecY = (1 - 2 * (quat.y * quat.z - quat.w * quat.x));
        float vecZ = (2 * (quat.x * quat.x + quat.y * quat.y));
        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    return { 0, 0, 0 };
}

//td::Vec4 degreeToQuaternion(double x, double y, double z) // yaw (Z), pitch (Y), roll (X)
//{
//    z = z * pi / 180.;
//    y = y * pi / 180.;
//    x = x * pi / 180.;
//
//    return radianToQuaternion(z, y, x);
//}


td::Vec4 radianToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);

    td::Vec4 q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

td::Vec4 euler2quat(float x, float y, float z) {
    float pitch = asin(-y);
    float yaw = atan2(x, z);
    float roll = 0;

    return radianToQuaternion(yaw, pitch, roll);
}

td::Vec3 euler2otherEuler(td::Vec3 rot) {
    float xzLen = cos(rot.x);
    float x = xzLen * cos(rot.y);
    float y = sin(rot.x);
    float z = xzLen * sin(-rot.y);

    return { x, y, z };
}


