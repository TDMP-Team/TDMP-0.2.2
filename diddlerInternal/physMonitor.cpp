#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "drawCube.h"
#include "toolgun.h"
#include "camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "dotProjector.h"
#include "glm/gtx/quaternion.hpp"

namespace physMonitor {
	spawner::spawnedObject monitor = {};
    TDScreen* currentScreen = 0;

    void spawnMonitor() {
        spawner::freeObjectSpawnParams osp = {};
        osp.useUserRotation = false;
        monitor = spawner::placeFreeObject("KM_Vox/Default/PVM/object.vox", osp);
        glb::setObjectAttribute(monitor.shapes[0], "nocull", "");

        currentScreen = (TDScreen*)glb::oTMalloc(3384);
        TDScreen* worldScreen = 0;
        for (TDScreen* tds : *(glb::scene->m_Screens)) {
            worldScreen = ((TDScreen*)tds);
        }

        glb::tdConstructScreen(currentScreen, 0);
        glb::osub_140105F30(currentScreen, 2);
        //currentScreen->Position = worldScreen->Position;
        //currentScreen->Rotation = worldScreen->Rotation;
        //currentScreen->m_Size = worldScreen->m_Size;
        //currentScreen->m_Bulge = worldScreen->m_Bulge;
        //currentScreen->m_Resolution = worldScreen->m_Resolution;
        //currentScreen->field_80 = worldScreen->field_80;
        //memcpy((void*)(currentScreen->gap84), (void*)(currentScreen->gap84), 2228);
        //currentScreen->field_938 = worldScreen->field_938;
        //memcpy((void*)(currentScreen->gap93C), (void*)(currentScreen->gap93C), 60);
        //currentScreen->field_978 = worldScreen->field_978;
        //memcpy((void*)(currentScreen->gap97C), (void*)(currentScreen->gap97C), 508);
        //currentScreen->field_B78 = worldScreen->field_B78;
        //memcpy((void*)(currentScreen->field_B80), (void*)(currentScreen->field_B80), 56);
        //memcpy((void*)(currentScreen->field_BB8), (void*)(currentScreen->field_BB8), 32);
        //currentScreen->m_Matrix = worldScreen->m_Matrix;
        //memcpy((void*)(currentScreen->gapBFC), (void*)(currentScreen->gapBFC), 64);
        //currentScreen->field_C58 = worldScreen->field_C58;
        //currentScreen->field_C60 = worldScreen->field_C60;
        //currentScreen->field_C68 = worldScreen->field_C68;
        //memcpy((void*)(currentScreen->gapC70), (void*)(currentScreen->gapC70), 4);
        //currentScreen->field_C74 = worldScreen->field_C74;
        //td::small_string script_path = td::small_string("data/level/lee/script/tv.lua");
        //td::small_string script_path_technical = td::small_string("RAW:data/level/lee/script/tv.lua");
        //td::small_string script_path_type = td::small_string("");
        //void* result = glb::oValidatePath(glb::game + 0xA8, &script_path, &script_path_technical, &script_path_type);
        //currentScreen->m_Script = worldScreen->m_Script;
        std::cout << "SCREEN: " << currentScreen << std::endl;

        uintptr_t staticPtr = 0x000000CE498FEDB0;
        glb::tdInitScreenSecondary((void*)currentScreen, (void*)staticPtr, 0);
        //std::cout << "Ran init screen secondary" << std::endl;
        //glb::tdUpdateScreen(currentScreen);
    }

    void updateMonitor() {

    }

}