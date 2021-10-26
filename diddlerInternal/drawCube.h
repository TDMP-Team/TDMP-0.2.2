#pragma once
#include "types.h"
#include "glm/glm.hpp"

void drawCube(td::Vec3 position, float size, td::Color color);
void drawCubeRotated(glm::vec3 position, glm::quat rotation, float size, td::Color color);