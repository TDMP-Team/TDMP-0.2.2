#pragma once
#include <cmath>
#include "types.h"

td::Vec4 radianToQuaternion(double yaw, double pitch, double roll);
td::Vec3 quat2euler(td::Vec4 quat, int rot, bool invert);
td::Vec4 euler2quat(float x, float y, float z);
td::Vec3 euler2otherEuler(td::Vec3 rot);