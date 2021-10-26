#include "types.h"
#include "Global.h"

void drawCube(td::Vec3 position, float size, td::Color color) {
    //0.25
    td::Vec3 pos1 = { position.x - size, position.y - size, position.z - size };
    td::Vec3 pos2 = { position.x - size, position.y - size, position.z + size };
    td::Vec3 pos3 = { position.x + size, position.y - size, position.z + size };
    td::Vec3 pos4 = { position.x + size, position.y - size, position.z - size };

    td::Vec3 pos1_high = { position.x - size, position.y + size, position.z - size };
    td::Vec3 pos2_high = { position.x - size, position.y + size, position.z + size };
    td::Vec3 pos3_high = { position.x + size, position.y + size, position.z + size };
    td::Vec3 pos4_high = { position.x + size, position.y + size, position.z - size };

    //bottom square
    glb::oFDL(glb::renderer, pos1, pos2, color, color, false);
    glb::oFDL(glb::renderer, pos2, pos3, color, color, false);
    glb::oFDL(glb::renderer, pos3, pos4, color, color, false);
    glb::oFDL(glb::renderer, pos4, pos1, color, color, false);

    //top square
    glb::oFDL(glb::renderer, pos1_high, pos2_high, color, color, false);
    glb::oFDL(glb::renderer, pos2_high, pos3_high, color, color, false);
    glb::oFDL(glb::renderer, pos3_high, pos4_high, color, color, false);
    glb::oFDL(glb::renderer, pos4_high, pos1_high, color, color, false);

    //walls
    glb::oFDL(glb::renderer, pos1, pos1_high, color, color, false);
    glb::oFDL(glb::renderer, pos2, pos2_high, color, color, false);
    glb::oFDL(glb::renderer, pos3, pos3_high, color, color, false);
    glb::oFDL(glb::renderer, pos4, pos4_high, color, color, false);
}

void drawCubeRotated(glm::vec3 position, glm::quat rotation, float size, td::Color color) {
    glm::vec3 vx = rotation * glm::vec3(-1, 0, 0);
    glm::vec3 vy = rotation * glm::vec3(0, -1, 0);
    glm::vec3 vz = rotation * glm::vec3(0, 0, -1); //(UP)

    glm::vec3 translationFBL = ((vz * (size * 0.5f)) + (vy * (size * 0.5f)) + (vx * (size * 0.5f)));
    glm::vec3 translationBBR = ((vz * (size * 0.5f)) - (vy * (size * 0.5f)) - (vx * (size * 0.5f)));
    glm::vec3 translationBBL = ((vz * (size * 0.5f)) - (vy * (size * 0.5f)) + (vx * (size * 0.5f)));
    glm::vec3 translationFBR = ((vz * (size * 0.5f)) + (vy * (size * 0.5f)) - (vx * (size * 0.5f)));
    glm::vec3 translationFTL = ((vz * (size * -0.5f)) + (vy * (size * 0.5f)) + (vx * (size * 0.5f)));
    glm::vec3 translationBTR = ((vz * (size * -0.5f)) - (vy * (size * 0.5f)) - (vx * (size * 0.5f)));
    glm::vec3 translationBTL = ((vz * (size * -0.5f)) - (vy * (size * 0.5f)) + (vx * (size * 0.5f)));
    glm::vec3 translationFTR = ((vz * (size * -0.5f)) + (vy * (size * 0.5f)) - (vx * (size * 0.5f)));

    td::Vec3 FBL = { position.x - translationFBL.x, position.y - translationFBL.y, position.z - translationFBL.z };
    td::Vec3 BBR = { position.x - translationBBR.x, position.y - translationBBR.y, position.z - translationBBR.z };
    td::Vec3 BBL = { position.x - translationBBL.x, position.y - translationBBL.y, position.z - translationBBL.z };
    td::Vec3 FBR = { position.x - translationFBR.x, position.y - translationFBR.y, position.z - translationFBR.z };

    td::Vec3 FTL = { position.x - translationFTL.x, position.y - translationFTL.y, position.z - translationFTL.z };
    td::Vec3 BTR = { position.x - translationBTR.x, position.y - translationBTR.y, position.z - translationBTR.z };
    td::Vec3 BTL = { position.x - translationBTL.x, position.y - translationBTL.y, position.z - translationBTL.z };
    td::Vec3 FTR = { position.x - translationFTR.x, position.y - translationFTR.y, position.z - translationFTR.z };

    //bottom square
    glb::oFDL(glb::renderer, FBL, FBR, color, color, false);
    glb::oFDL(glb::renderer, FBL, BBL, color, color, false);
    glb::oFDL(glb::renderer, BBL, BBR, color, color, false);
    glb::oFDL(glb::renderer, BBR, FBR, color, color, false);

    //top square
    glb::oFDL(glb::renderer, FTL, FTR, color, color, false);
    glb::oFDL(glb::renderer, FTL, BTL, color, color, false);
    glb::oFDL(glb::renderer, BTL, BTR, color, color, false);
    glb::oFDL(glb::renderer, BTR, FTR, color, color, false);

    //walls
    glb::oFDL(glb::renderer, FTL, FBL, color, color, false);
    glb::oFDL(glb::renderer, FTR, FBR, color, color, false);
    glb::oFDL(glb::renderer, BTL, BBL, color, color, false);
    glb::oFDL(glb::renderer, BTR, BBR, color, color, false);
}