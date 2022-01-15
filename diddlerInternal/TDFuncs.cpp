#include "TDFuncs.h"
#include "global.h"

void printIntegPercentage(float p) {
    int cCode = 10;
    if (p < 100.f) {
        cCode = 14;
    }
    if (p < 75.f) {
        cCode = 12;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cCode);
    printf_s(" (%0.1f%%)\n", p);
    SetConsoleTextAttribute(hConsole, 15);
}

void hLog(td::small_string* msg)
{
    printf_s("[Game Log] %s\n", msg->c_str());
}

void test(void* a1, void* a2, void* a3)
{
}

void sigscanItems() {
    //live = 0x0041CB00
    bool sigScanError = false;
    float percentage = 0;

    glb::TDOL = *(TDObjectList**)(glb::moduleBase + 0x00456140);
    glb::game = (TDGame*)glb::TDOL;
    glb::scene = (TDScene*)glb::TDOL->objs[9];
    glb::renderer = (TDRenderer*)glb::TDOL->objs[8];
    glb::player = (TDPlayer*)glb::TDOL->objs[20];

    std::cout << "" << std::endl;
    std::cout << "[Function Addresses:       ]" << std::endl;
    std::cout << "cursor               : " << std::hex << glb::ocursor << std::endl;

    glb::oLoadResource = (loadResource)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xD7\xFE\xFF\xFF\xFF\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("loadResource         : %p", glb::oLoadResource);
    printIntegPercentage(percentage);
    if (!glb::oLoadResource) { sigScanError = true; }

    glb::oValidatePath = (validatePath)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xDF\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oValidatePath        : %p", glb::oValidatePath);
    printIntegPercentage(percentage);
    if (!glb::oValidatePath) { sigScanError = true; }

    glb::oCreateTextureThing = (createTextureThing)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\xF2\x48\x8B\xD9\x8B\x51\x0C\x41\x0F\xB6\xF8\xB9\xE1\x0D\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oCreateTextureThing  : %p", glb::oCreateTextureThing);
    printIntegPercentage(percentage);
    if (!glb::oCreateTextureThing) { sigScanError = true; }

    glb::tdUpdateScreen = (updateScreen)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x98\xD7\xFF\xFF\xB8\x68\x29\x00\x00\xE8\x9D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdUpdateScreen       : %p", glb::tdUpdateScreen);
    printIntegPercentage(percentage);
    if (!glb::tdUpdateScreen) { sigScanError = true; }

    glb::tdConstructJoint = (joinConstructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x07\x00\x00\x00\xE8\x3A\x82\x05\x00\xC7\x43\x4C\xCD\xCC\xCC\x3D\x48\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdConstructJoint     : %p", glb::tdConstructJoint);
    printIntegPercentage(percentage);
    if (!glb::tdConstructJoint) { sigScanError = true; }

    glb::tdInitBall = (initBall)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdInitBall           : %p", glb::tdInitBall);
    printIntegPercentage(percentage);
    if (!glb::tdInitBall) { sigScanError = true; }

    glb::tdInitWire = (initWire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xC1\x48\x81\xEC\xD0\x00\x00\x00\x48\xC7\x45\x1F\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdInitWire           : %p", glb::tdInitWire);
    printIntegPercentage(percentage);
    if (!glb::tdInitWire) { sigScanError = true; }

    glb::tdAttachJoint = (attachJoint)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdAttachJoint        : %p", glb::tdAttachJoint);
    printIntegPercentage(percentage);
    if (!glb::tdAttachJoint) { sigScanError = true; }

    glb::oPaint = (tPaint)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x55\x41\x56\x48\x8D\x68\xD8\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxx?????", GetModuleHandle(NULL), &percentage);
    printf_s("tPaint               : %p", glb::oPaint);
    printIntegPercentage(percentage);
    if (!glb::oPaint) { sigScanError = true; }

    glb::oFDL = (frameDrawLine)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x44\x8B\xB9\x10\x89\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("drawLine             : %p", glb::oFDL);
    printIntegPercentage(percentage);
    if (!glb::oFDL) { sigScanError = true; }

    glb::oRC = (rayCast)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x4C\x89\x40\x18\x48\x89\x50\x10\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x18\xF4\xFF\xFF\x48\x81\xEC", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("raycast              : %p", glb::oRC);
    printIntegPercentage(percentage);
    if (!glb::oRC) { sigScanError = true; }

    glb::TDcreateExplosion = (createExplosion)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\xA8\xFD\xFF\xFF\x48\x81\xEC\x30\x03\x00\x00\x48\xC7\x45\x20\xFE\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("create explosion     : %p", glb::TDcreateExplosion);
    printIntegPercentage(percentage);
    if (!glb::TDcreateExplosion) { sigScanError = true; }

    glb::oSpawnVox = (tSpawnVox)mem::FindPattern((PBYTE)"\x4C\x8B\xDC\x57\x48\x81\xEC\xF0\x00\x00\x00\x48\xC7\x44\x24\x60", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("spawn vox            : %p", glb::oSpawnVox);
    printIntegPercentage(percentage);
    if (!glb::oSpawnVox) { sigScanError = true; }

    glb::oWrappedDamage = (wrappedDoDamage)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x44\x24\x78\x4C\x8B\xC2\xF3\x0F\x10\x44", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("make hole            : %p", glb::oWrappedDamage);
    printIntegPercentage(percentage);
    if (!glb::oWrappedDamage) { sigScanError = true; }

    glb::oCreateTexture = (CreateTexture)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("create texture       : %p", glb::oCreateTexture);
    printIntegPercentage(percentage);
    if (!glb::oCreateTexture) { sigScanError = true; }

    glb::oCreatePhysics = (CreatePhysics)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x50\x04\x00\x00\x48\x8B\xD9\xE8\xCF\x67\x00\x00\x48\x8B\x4B\x18\xC7\x43\x5C\x00\x00\x00\x00\xE8\x6F\xD1\x06", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("create physics       : %p", glb::oCreatePhysics);
    printIntegPercentage(percentage);
    if (!glb::oCreatePhysics) { sigScanError = true; }

    glb::oUpdateShapes = (UpdateShapes)mem::FindPattern((PBYTE)"\x40\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x10\xEB\xFF\xFF\xB8\xF0\x15\x00\x00\xE8\x84\x72\x26\x00\x48\x2B\xE0\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update shapes        : %p", glb::oUpdateShapes);
    printIntegPercentage(percentage);
    if (!glb::oUpdateShapes) { sigScanError = true; }

    glb::oB_Constructor = (B_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("construct body       : %p", glb::oB_Constructor);
    printIntegPercentage(percentage);
    if (!glb::oB_Constructor) { sigScanError = true; }

    glb::oS_Constructor = (S_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x02\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("construct shape      : %p", glb::oS_Constructor);
    printIntegPercentage(percentage);
    if (!glb::oS_Constructor) { sigScanError = true; }

    glb::oSetDynamic = (SetDynamic)mem::FindPattern((PBYTE)"\x88\x91\xDC\x00\x00\x00\x4C\x8B\xC1\x84\xD2\x74\x29\x0F\xB6\x81\xE4\x00\x00\x00\xC6\x81\xE4\x00\x00\x00\x3C\x84\xC0\x75\x17\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("set dynamic          : %p", glb::oSetDynamic);
    printIntegPercentage(percentage);
    if (!glb::oSetDynamic) { sigScanError = true; }

    glb::oTMalloc = (TMalloc)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xF9\xE0\x77\x3C\x48\x85\xC9\xB8\x2A\x2A\x2A\x2A\x48\x0F\x44\xD8\xEB\x15\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x25\x48\x8B\xCB\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x19\x48\x8B\x0D\x2A\x2A\x2A\x2A\x4C\x8B\xC3\x33\xD2\xFF\x15\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\xD4\xEB\x0D\xE8\x2A\x2A\x2A\x2A\xC7\x00\x2A\x2A\x2A\x2A\x33\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxx????xx????xxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("malloc               : %p", glb::oTMalloc);
    printIntegPercentage(percentage);
    if (!glb::oTMalloc) { sigScanError = true; }

    glb::oTFree = (TFree)mem::FindPattern((PBYTE)"\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x4C\x8B\xC1\x33\xD2\x48\x8B\x0D\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x85\xC0\x75\x17\xE8\x2A\x2A\x2A\x2A\x48\x8B\xD8\xFF\x15\x2A\x2A\x2A\x2A\x8B\xC8\xE8\x2A\x2A\x2A\x2A", "xxxxxxxxxxxxxxxxxx????xx????xxxxx????xxxxx????xxx????", GetModuleHandle(NULL), &percentage);
    printf_s("free                 : %p", glb::oTFree);
    printIntegPercentage(percentage);
    if (!glb::oTFree) { sigScanError = true; }

    glb::oTRealloc = (TRealloc)mem::FindPattern((PBYTE)"\xE9\x2B\x31\x02\x00", "xxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("realloc                 : %p", glb::oTRealloc);
    printIntegPercentage(percentage);
    if (!glb::oTRealloc) { sigScanError = true; }

    glb::LuaAllocF = (TluaAlloc)mem::FindPattern((PBYTE)"\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x41\x8B\xC1", "xxxx?xxxx?xxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("lua_Alloc                 : %p", glb::LuaAllocF);
    printIntegPercentage(percentage);
    if (!glb::LuaAllocF) { sigScanError = true; }

    glb::oSpawnFire = (spawnFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xB8\x48\x81\xEC\x40\x01\x00\x00\x48\xC7\x45\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x10\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage); //td.exe+B94C0
    printf_s("spawn fire           : %p", glb::oSpawnFire);
    printIntegPercentage(percentage);
    if (!glb::oSpawnFire) { sigScanError = true; }

    glb::oPewpew = (createProjectile)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x80\x00\x00\x00\xF2\x0F\x10\x02\xF3\x0F\x10\x8C\x24\xB8\x00\x00\x00\x8B\x42\x08\xF2\x0F\x11\x44\x24\x30\xF3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("shoot projectile     : %p", glb::oPewpew);
    printIntegPercentage(percentage);
    if (!glb::oPewpew) { sigScanError = true; }

    glb::TDspawnParticle = (spawnParticle)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8D\x99\x80\x00\x00\x00\x49\x8B\xF1\x81\x3B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("spawn particle       : %p", glb::TDspawnParticle);
    printIntegPercentage(percentage);
    if (!glb::TDspawnParticle) { sigScanError = true; }

    glb::oDamagePlayer = (damagePlayer)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x0F\x29\x74\x24\x30\x48\x8B\xD9\x0F\x29", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("damage player        : %p", glb::oDamagePlayer);
    printIntegPercentage(percentage);
    if (!glb::oDamagePlayer) { sigScanError = true; }

    glb::oDamageObject = (damageObject)mem::FindPattern((PBYTE)"\x48\x81\xEC\xC8\x00\x00\x00\x48\xC7\x44\x24\x48\xFE\xFF\xFF\xFF\x48\x8D\x44\x24\x60\x48\x89\x44\x24\x58\xC7\x44\x24\x54\x04\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("damage object 2      : %p", glb::oDamageObject);
    printIntegPercentage(percentage);
    if (!glb::oDamageObject) { sigScanError = true; }

    glb::oCamPos = (cameraPositioning)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x38\xFF\xFF\xFF\x48\x81\xEC\xC0\x01\x00\x00\x48\xC7\x45\x10\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("cam positioner       : %p", glb::oCamPos);
    printIntegPercentage(percentage);
    if (!glb::oCamPos) { sigScanError = true; }

    glb::oSpreadFire = (spreadFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\xF3\x0F\x11\x48\x10\x48\x89\x48\x08\x55\x53\x56\x57", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("fire spread          : %p", glb::oSpreadFire);
    printIntegPercentage(percentage);
    if (!glb::oSpreadFire) { sigScanError = true; }

    glb::oSOA = (SetObjectAttribute)mem::FindPattern((PBYTE)"\x4C\x89\x44\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x48\xC7", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("set attribute        : %p", glb::oSOA);
    printIntegPercentage(percentage);
    if (!glb::oSOA) { sigScanError = true; }

    glb::oSOAS = (SetObjectAttributeSingle)mem::FindPattern((PBYTE)"\x40\x57\x41\x57\x48\x83\xEC\x58\xF6\x41\x09\x01\x4C\x8B\xFA\x48\x8B\xF9\x0F\x84\x21\x01\x00\x00\x48\x89\x5C\x24\x70\x48\x8D\x15", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("remove attribute     : %p", glb::oSOAS);
    printIntegPercentage(percentage);
    if (!glb::oSOAS) { sigScanError = true; }

    glb::oHighlightShape = (highlightShape)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x05\x65\x68\x3F\x00\x0F\x29\x74\x24\x30\x0F\x28\xF2\x80\xB8\x18\x04\x00\x00\x00\x75\x20\x48\x89\x54\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("highlight shape      : %p", glb::oHighlightShape);
    printIntegPercentage(percentage);
    if (!glb::oHighlightShape) { sigScanError = true; }

    glb::oOutlineShape = (outlineShape)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x48\x8B\x05\x13\x68\x3F\x00\x49\x8B\xD8", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("outline shape        : %p", glb::oOutlineShape);
    printIntegPercentage(percentage);
    if (!glb::oOutlineShape) { sigScanError = true; }

    glb::oOutlineBody = (outlineBody)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x57\x48\x81\xEC\x00\x01\x00\x00\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x0F\x29\x70\xE8\x0F\x29\x78\xD8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("outline body         : %p", glb::oOutlineBody);
    printIntegPercentage(percentage);
    if (!glb::oOutlineBody) { sigScanError = true; }

    glb::oQuatEul = (apiQuatEuler)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x58\x08\x57\x48\x81\xEC\x80\x00\x00\x00\x0F\x29\x70\xE8\x48\x8B\xDA\xF3\x0F\x10\x72\x04\x48\x8B\xF9\x0F\x29", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("QuatEuler            : %p", glb::oQuatEul);
    printIntegPercentage(percentage);
    if (!glb::oQuatEul) { sigScanError = true; }

    glb::oSetBody = (setBodySomething)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x59\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2\x48\x85\xDB\x74\x20\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("setBody              : %p", glb::oSetBody);
    printIntegPercentage(percentage);
    if (!glb::oSetBody) { sigScanError = true; }

    glb::TDreadSubobjects = (ReadSubobjectsFromVox)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x57\x41\x56\x48\x8D\x68\xA1\x48\x81\xEC\xB0\x00\x00\x00\x48\xC7\x45\x3F\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("Read subobjects      : %p", glb::TDreadSubobjects);
    printIntegPercentage(percentage);
    if (!glb::TDreadSubobjects) { sigScanError = true; }

    glb::oIsActive = (isActiveWindow)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xFF\x15\x41\x1E\x1D\x00\x48", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oIsActiveWindow      : %p", glb::oIsActive);
    printIntegPercentage(percentage);
    if (!glb::oIsActive) { sigScanError = true; }

    glb::tdUpdateShapeBody = (updateShapeBody)mem::FindPattern((PBYTE)"\x4C\x8B\xC1\x48\x8B\x49\x10\x48\x8B\x41\x20\x49\x3B\xC0\x75\x0A\x49\x8B\x40\x18\x48\x89\x41\x20\xEB\x21\x48\x85\xC0\x74\x1C\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update shape body    : %p", glb::tdUpdateShapeBody);
    printIntegPercentage(percentage);
    if (!glb::tdUpdateShapeBody) { sigScanError = true; }

    glb::tdUpdateFunc = (interestingUpdateFunc)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x59\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2\x48\x85\xDB\x74\x20\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update body weird    : %p", glb::tdUpdateFunc);
    printIntegPercentage(percentage);
    if (!glb::tdUpdateFunc) { sigScanError = true; }

    glb::tdFunRuiner = (funRuiner)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x28", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("fun ruiner           : %p", glb::tdFunRuiner);
    printIntegPercentage(percentage);
    if (!glb::tdFunRuiner) { sigScanError = true; }

    glb::RegisterGameFunctions = (tRegisterGameFunctions)mem::FindPattern((PBYTE)"\x40\x57\x48\x83\xEC\x40\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x89\x5C\x24\x00\x48\x8D\x79\x30", "xxxxxxxxxx?????xxxx?xxxx", GetModuleHandle(NULL), &percentage);
    printf_s("RegisterGameFunctions           : %p", glb::RegisterGameFunctions);
    printIntegPercentage(percentage);
    if (!glb::RegisterGameFunctions) { sigScanError = true; }

    glb::tdRegisterLuaFunction = (tRegisterLuaFunction)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x8B\xDA\x48\x8B\x49\x38\x49\x8B\xD0\x48\x8B\x09\xE8\x00\x00\x00\x00", "xxxx?xxxxxxxxxxxxxxxxxxxxxx????", GetModuleHandle(NULL), &percentage);
    printf_s("tdRegisterLuaFunction           : %p", glb::tdRegisterLuaFunction);
    printIntegPercentage(percentage);
    if (!glb::tdRegisterLuaFunction) { sigScanError = true; }

    glb::oluaL_loadbuffer = (tluaL_loadbuffer)mem::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x85\xC0\x75\x07\xB8\x00\x00\x00\x00\xEB\x57", "x????xxxxx????xx", GetModuleHandle(NULL), &percentage);
    printf_s("oluaL_loadbuffer           : %p", glb::oluaL_loadbuffer);
    printIntegPercentage(percentage);
    if (!glb::oluaL_loadbuffer) { sigScanError = true; }

    glb::oluaD_pcall = (tluaD_pcall)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x20\x4C\x8B\x71\x28\x49\x8B\xD9\x48\x8B\x44\x24\x00\x48\x8B\xF9\x48\x8B\xA9\x00\x00\x00\x00", "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxxxxx?xxxxxx????", GetModuleHandle(NULL), &percentage);
    printf_s("oluaD_pcall           : %p", glb::oluaD_pcall);
    printIntegPercentage(percentage);
    if (!glb::oluaD_pcall) { sigScanError = true; }

    glb::oluaD_call = (tluaD_call)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x66\xFF\x41\x60\x48\x8B\xD9\x0F\xB7\x41\x60", "xxxx?xxxx?xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oluaD_call           : %p", glb::oluaD_call);
    printIntegPercentage(percentage);
    if (!glb::oluaD_call) { sigScanError = true; }

    glb::oluaL_error = (tluaL_error)mem::FindPattern((PBYTE)"\x48\x89\x54\x24\x00\x4C\x89\x44\x24\x00\x4C\x89\x4C\x24\x00\x53\x56\x57\x48\x83\xEC\x20\xBA\x00\x00\x00\x00\x48\x8D\x74\x24\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00", "xxxx?xxxx?xxxx?xxxxxxxx????xxxx?xxxx????", GetModuleHandle(NULL), &percentage);
    printf_s("oluaL_error           : %p", glb::oluaL_error);
    printIntegPercentage(percentage);
    if (!glb::oluaL_error) { sigScanError = true; }

    glb::olua_pushfstring = (tlua_pushfstring)mem::FindPattern((PBYTE)"\x48\x89\x54\x24\x00", "xxxx?", GetModuleHandle(NULL), &percentage);
    printf_s("olua_pushfstring           : %p", glb::olua_pushfstring);
    printIntegPercentage(percentage);
    if (!glb::olua_pushfstring) { sigScanError = true; }

    glb::olua_pushlstring = (tlua_pushlstring)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x4C\x8B\x49\x20\x49\x8B\xF0\x48\x8B\xEA\x48\x8B\xF9\x49\x8B\x41\x70\x49\x39\x41\x78\x72\x05", "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("olua_pushlstring           : %p", glb::olua_pushlstring);
    printIntegPercentage(percentage);
    if (!glb::olua_pushlstring) { sigScanError = true; }

    glb::olua_createtable = (tlua_createtable)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x4C\x8B\x49\x20\x41\x8B\xF0\x8B\xEA\x48\x8B\xF9\x49\x8B\x41\x70\x49\x39\x41\x78\x72\x05", "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("olua_createtable           : %p", glb::olua_createtable);
    printIntegPercentage(percentage);
    if (!glb::olua_createtable) { sigScanError = true; }

    glb::oluaV_settable = (tluaV_settable)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x30\x4D\x8B\xE1\x48\x89\x7C\x24\x00\x4D\x8B\xF8\x4C\x8D\x2D\x00\x00\x00\x00\x4C\x8B\xF2\x48\x8B\xF1\x33\xED", "xxxx?xxxx?xxxxxxxxxxxxxxxxxxxx?xxxxxx????xxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oluaV_settable           : %p", glb::oluaV_settable);
    printIntegPercentage(percentage);
    if (!glb::oluaV_settable) { sigScanError = true; }

    glb::oluaV_gettable = (tluaV_gettable)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x30\x4D\x8B\xF9\x4C\x8D\x25\x00\x00\x00\x00\x4D\x8B\xF0\x48\x8B\xFA\x48\x8B\xF1\x33\xED", "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxx????xxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oluaV_gettable           : %p", glb::oluaV_gettable);
    printIntegPercentage(percentage);
    if (!glb::oluaV_gettable) { sigScanError = true; }

    glb::oluaS_newlstr = (tluaS_newlstr)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x49\x8B\xF0\x41\x8B\xF8", "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oluaS_newlstr           : %p", glb::oluaS_newlstr);
    printIntegPercentage(percentage);
    if (!glb::oluaS_newlstr) { sigScanError = true; }

    glb::olua_index2adr = (tlua_index2adr)mem::FindPattern((PBYTE)"\x85\xD2\x7E\x1F", "xxxx", GetModuleHandle(NULL), &percentage);
    printf_s("olua_index2adr           : %p", glb::olua_index2adr);
    printIntegPercentage(percentage);
    if (!glb::olua_index2adr) { sigScanError = true; }

    glb::oConvertPath = (convertPath)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xA1\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x48\x89\x58\x08\x48\x89\x70\x10\x48\x89\x78\x18\x4C\x89\x70\x20\x49\x8B\xF8", "xxxxxxxxxxx????xxx?????xxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oConvertPath           : %p", glb::oConvertPath);
    printIntegPercentage(percentage);
    if (!glb::oConvertPath) { sigScanError = true; }

    glb::oHasTag = (tHasTag)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x20\xF6\x41\x0A\x01\x4C\x8B\xF2\x48\x8B\xE9\x74\x5B", "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oHasTag           : %p", glb::oHasTag);
    printIntegPercentage(percentage);
    if (!glb::oHasTag) { sigScanError = true; }

    glb::oMakeHole = (tMakeHole)mem::FindPattern((PBYTE)"\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8D\x44\x24\x00\x48\x89\x44\x24\x00", "xxx????xxxx?????xxxx?xxxx?", GetModuleHandle(NULL), &percentage);
    printf_s("oMakeHole           : %p", glb::oMakeHole);
    printIntegPercentage(percentage);
    if (!glb::oMakeHole) { sigScanError = true; }

    glb::GetString = (tGetString)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x30\x49\x8B\xC0", "xxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("GetString           : %p", glb::GetString);
    printIntegPercentage(percentage);
    if (!glb::GetString) { sigScanError = true; }

    DWORD64 Log = mem::FindPattern((PBYTE)"\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????", GetModuleHandle(NULL), &percentage);
    printf_s("Log           : %p", &Log);
    printIntegPercentage(percentage);
    if (!Log) { sigScanError = true; }
    else {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)Log, hLog);
        DetourTransactionCommit();
    }

    //DetourTransactionBegin();
    //DetourUpdateThread(GetCurrentThread());
    //DetourAttach(&(PVOID&)glb::Tick, test);
    //DetourTransactionCommit();

    if (sigScanError) {
        std::cout << "[F] FAILED TO FIND A CRITICAL FUNCTION, EXPECT ISSUES" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "[Object Addresses:         ]" << std::endl;
    std::cout << "Game               : " << std::hex << glb::TDOL << std::endl;
    std::cout << "Player             : " << std::hex << glb::player << std::endl;
    std::cout << "Scene              : " << std::hex << glb::scene << std::endl;
    std::cout << "Renderer           : " << std::hex << glb::renderer << std::endl;
    std::cout << "" << std::endl;

    //for (int i = 0; i < 128; i++) {
    //    std::cout << i << " : " << std::hex << glb::TDOL->objs[i] << std::endl;
    //}

}