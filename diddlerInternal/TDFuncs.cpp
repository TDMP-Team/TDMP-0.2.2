#include "TDFuncs.h"
#include "global.h"

void sigscanItems() {
    glb::oPaint = (tPaint)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x55\x41\x56\x48\x8D\x68\xD8\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxx?????", GetModuleHandle(NULL));
    glb::oFDL = (frameDrawLine)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x44\x8B\xB9\x10\x89\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oRC = (rayCast)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x4C\x89\x40\x18\x48\x89\x50\x10\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x18\xF4\xFF\xFF\x48\x81\xEC", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDcreateExplosion = (createExplosion)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\xA8\x18\xFE\xFF\xFF\x48\x81\xEC\xD0\x02\x00\x00\x48\xC7\x45\x18\xFE\xFF\xFF\xFF\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSpawnVox = (tSpawnVox)mem::FindPattern((PBYTE)"\x4C\x8B\xDC\x57\x48\x81\xEC\xF0\x00\x00\x00\x48\xC7\x44\x24\x60", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oTdDelBdy = (deleteBody)mem::FindPattern((PBYTE)"\x48\x83\xEC\x28\x48\x8B\xCA\x33\xD2\xE8\x82\x16\x0F\x00\x48\x8B", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMakeHole = (makeHole)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x53\x48\x81\xEC\x90\x00\x00\x00\x0F\x29\x70\xE8\x4C\x8D\x40\xA8\x48\x8B\xDA\x0F\x29\x78\xD8\x44\x0F\x29\x40\xC8\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreateTexture = (CreateTexture)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreatePhysics = (CreatePhysics)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x50\x04\x00\x00\x48\x8B\xD9\xE8\x7F\x5C\x00\x00\x48\x8B\x4B\x18\xC7\x43\x5C\x00\x00\x00\x00\xE8\x0F\x87\x06", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oUpdateShapes = (UpdateShapes)mem::FindPattern((PBYTE)"\x40\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x10\xEB\xFF\xFF\xB8\xF0\x15\x00\x00\xE8\x54\x91\x25\x00\x48\x2B\xE0\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oB_Constructor = (B_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oS_Constructor = (S_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x02\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSetDynamic = (SetDynamic)mem::FindPattern((PBYTE)"\x88\x91\xDC\x00\x00\x00\x4C\x8B\xC1\x84\xD2\x74\x29\x0F\xB6\x81\xE4\x00\x00\x00\xC6\x81\xE4\x00\x00\x00\x3C\x84\xC0\x75\x17\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oTMalloc = (TMalloc)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xF9\xE0\x77\x3C\x48\x85\xC9\xB8\x2A\x2A\x2A\x2A\x48\x0F\x44\xD8\xEB\x15\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x25\x48\x8B\xCB\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x19\x48\x8B\x0D\x2A\x2A\x2A\x2A\x4C\x8B\xC3\x33\xD2\xFF\x15\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\xD4\xEB\x0D\xE8\x2A\x2A\x2A\x2A\xC7\x00\x2A\x2A\x2A\x2A\x33\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxx????xx????xxxxxxxx", GetModuleHandle(NULL));
    glb::oTFree = (TFree)mem::FindPattern((PBYTE)"\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x4C\x8B\xC1\x33\xD2\x48\x8B\x0D\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x85\xC0\x75\x17\xE8\x2A\x2A\x2A\x2A\x48\x8B\xD8\xFF\x15\x2A\x2A\x2A\x2A\x8B\xC8\xE8\x2A\x2A\x2A\x2A", "xxxxxxxxxxxxxxxxxx????xx????xxxxx????xxxxx????xxx????", GetModuleHandle(NULL));
    glb::oSpawnFire = (spawnFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xB8\x48\x81\xEC\x40\x01\x00\x00\x48\xC7\x45\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x10\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL)); //td.exe+B94C0
    glb::oPewpew = (createProjectile)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x57\x48\x81\xEC\x80\x00\x00\x00", "xxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDspawnParticle = (spawnParticle)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8D\x99\x80\x00\x00\x00\x49\x8B\xF1\x81\x3B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamagePlayer = (damagePlayer)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x0F\x29\x74\x24\x30\x48\x8B\xD9\x0F\x29", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMAL = (modApiLinker)mem::FindPattern((PBYTE)"\x40\x55\x48\x8D\xAC\x24\x80\xFA\xFF\xFF\x48\x81\xEC\x80\x06\x00", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamageObject = (damageObject)mem::FindPattern((PBYTE)"\x48\x81\xEC\xC8\x00\x00\x00\x48\xC7\x44\x24\x48\xFE\xFF\xFF\xFF\x48\x8D\x44\x24\x60\x48\x89\x44\x24\x58\xC7\x44\x24\x54\x04\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCamPos = (cameraPositioning)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x6C\x24\x80\x48\x81\xEC\x80\x01\x00\x00\x48\xC7\x45\x10\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x18", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oTDmain = (tdMain)mem::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx", GetModuleHandle(NULL));
    glb::oCreateLight = (createLight)mem::FindPattern((PBYTE)"\x48\x83\xEC\x58\x48\x8B\x05\x95\x25\x3F\x00\x80\xB8\x10\x01\x00\x00\x00\x0F\x85\xA6\x00\x00\x00\xF2\x0F\x10\x02\x8B\x42\x08\xF3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oEnvUpdate = (environmentUpdate)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x53\x56\x57\x41\x56\x48\x8D\xA8\x38\xFE\xFF\xFF\x48\x81\xEC\xA0\x02\x00\x00\x48\xC7\x45\x80\xFE\xFF\xFF\xFF\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSpreadFire = (spreadFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\xF3\x0F\x11\x48\x10\x48\x89\x48\x08\x55\x53\x56\x57", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSOA = (SetObjectAttribute)mem::FindPattern((PBYTE)"\x4C\x89\x44\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x48\xC7", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oHighlightShape = (highlightShape)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x05\x65\x68\x3F\x00\x0F\x29\x74\x24\x30\x0F\x28\xF2\x80\xB8\x18\x04\x00\x00\x00\x75\x20\x48\x89\x54\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oOutlineShape = (outlineShape)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x48\x8B\x05\x13\x68\x3F\x00\x49\x8B\xD8", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oOutlineBody = (outlineBody)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x57\x48\x81\xEC\x00\x01\x00\x00\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x0F\x29\x70\xE8\x0F\x29\x78\xD8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::plankPatchFunction = (uintptr_t)mem::FindPattern((PBYTE)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x50\xCF\xFF\xFF\xB8\xB0\x31\x00\x00\xE8\xF2\xE8\x19\x00\x48\x2B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oQuatEul = (apiQuatEuler)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x58\x08\x57\x48\x81\xEC\x80\x00\x00\x00\x0F\x29\x70\xE8\x48\x8B\xDA\xF3\x0F\x10\x72\x04\x48\x8B\xF9\x0F\x29", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSetBody = (setBodySomething)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x59\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2\x48\x85\xDB\x74\x20\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oWrappedDamage = (wrappedDoDamage)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x44\x24\x78\x4C\x8B\xC2\xF3\x0F\x10\x44", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSOAS = (SetObjectAttributeSingle)mem::FindPattern((PBYTE)"\x40\x57\x41\x57\x48\x83\xEC\x58\xF6\x41\x09\x01\x4C\x8B\xFA\x48\x8B\xF9\x0F\x84\x21\x01\x00\x00\x48\x89\x5C\x24\x70\x48\x8D\x15", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oHandleBombObjs = (handleBombObjs)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x57\x41\x56\x48\x8D\xA8\x18\xFF\xFF\xFF\x48\x81", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSetShapeParentBody = (setShapeParentBody)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8D\x05\x93\x51\x24\x00\x88\x51\x08", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oAddCItem = (addContextItem)mem::FindPattern((PBYTE)"\x44\x89\x44\x24\x18\x88\x54\x24\x10\x55\x56\x57\x41\x56\x48\x8D\x6C\x24\xF8\x48\x81\xEC\x08\x01\x00\x00\x4C\x8B\x15\x5F\xE5\x3B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDreadSubobjects = (ReadSubobjectsFromVox)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x57\x41\x56\x48\x8D\x68\xA1\x48\x81\xEC\xB0\x00\x00\x00\x48\xC7\x45\x3F\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oIUnReadVox = (unknReadVoxData)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x88\xD9\xFF\xFF\xB8\x78\x27\x00\x00\xE8\x6D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oIsActive = (isActiveWindow)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xFF\x15\x11\xFF\x1C\x00\x48", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oLtDBin = (loadTDBIN)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x58\xFF\xFF\xFF\x48\x81\xEC\xA0\x01\x00\x00\x48\xC7\x44\x24\x68\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::o_S140152540 = (S140152540)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x48\x48\x89\x6C\x24\x50\x48\x89\x74\x24\x58\x41\x8B\xF0\x48\x8B\xEA\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::o_S1400C4F70 = (S1400C4F70)mem::FindPattern((PBYTE)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xD9\x48\x81\xEC\xB0\x00\x00\x00\x48\xC7\x45\x1F\xFE\xFF\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::o_S140152740 = (S140152740)mem::FindPattern((PBYTE)"\x40\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x40\x48\x8B\xF9\x48\x8D\x05\x92\xFD\x1F\x00\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oValidate = (validateFileExistance)mem::FindPattern((PBYTE)"\x40\x57\x48\x83\xEC\x60\x48\xC7\x44\x24\x28\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x49\x8B\xF8\x48\x8B\xDA\xC7\x44\x24\x20\x00\x00\x00\x00\x48\x8D\x4C\x24\x50\xE8\xF4\xFA\x02\x00\x90\x48\x8B\xD0", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDoQuicksave = (doQuicksave)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x8B\xD9\x48\x8B\x0D\xE4\xB3\x36\x00\x48\x8B\x89\xA0", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDoQuickload = (doQuickload)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x57\x48\x83\xEC\x70\x48\xC7\x40\xA8\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x10\x48\x8B\xF1\x48\x8B\x0D\xEE\xB4", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdConstructJoint = (joinConstructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x07\x00\x00\x00\xE8\x3A\x82\x05\x00\xC7\x43\x4C\xCD\xCC\xCC\x3D\x48\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdAttachJoint = (attachJoint)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdInitWire = (initWire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xC1\x48\x81\xEC\xD0\x00\x00\x00\x48\xC7\x45\x1F\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdInitBall = (initBall)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdUpdateFunc = (interestingUpdateFunc)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x59\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2\x48\x85\xDB\x74\x20\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdUpdateShapeBody = (updateShapeBody)mem::FindPattern((PBYTE)"\x4C\x8B\xC1\x48\x8B\x49\x10\x48\x8B\x41\x20\x49\x3B\xC0\x75\x0A\x49\x8B\x40\x18\x48\x89\x41\x20\xEB\x21\x48\x85\xC0\x74\x1C\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreateTextureThing = (createTextureThing)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\xF2\x48\x8B\xD9\x8B\x51\x0C\x41\x0F\xB6\xF8\xB9\xE1\x0D\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    //glb::tdConstructScreen = (constructScreen)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x07\x00\x00\x00\xE8\x6A\x77\x05\x00\xC7\x43\x4C\xCD\xCC\xCC\x3D\x48\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdUpdateScreen = (updateScreen)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x98\xD7\xFF\xFF\xB8\x68\x29\x00\x00\xE8\x9D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    //glb::oUnknGraphicsInitFunction = (unknGraphicsInitFunction)mem::FindPattern((PBYTE)"\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\xE9\xBA\x7D\x14\x00", "xxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oValidatePath = (validatePath)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xDF\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oLoadResource = (loadResource)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xD7\xFE\xFF\xFF\xFF\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    //glb::tdInitScreenSecondary = (initScreenSecondary)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x48\x8B\xF9\xE8\x3B\xA7\xFD\xFF\x48\x8D\x57\x28\x48\x8B\xCB\xE8\x4F\x94\x01", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdRaycastCrashA = (raycastCrashA)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x7C\x24\x10\x55\x48\x8B\xEC\x48\x83\xEC\x70\xF3\x0F\x10\x02\x48\x8B\xF9\xF3\x41\x0F\x58\x00\xF3\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::tdRaycastCrashB = (raycastCrashB)mem::FindPattern((PBYTE)"\x48\x83\xEC\x08\x4C\x8B\xD2\x4C\x8B\xC9\x8B\x52\x70\x85\x11\x75\x07\x32\xC0\x48\x83\xC4\x08\xC3\x8B\x41\x04\x23\xC2\x3B\x41\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));

    glb::osub_140105F30 = (sub_140105F30)mem::FindPattern((PBYTE)"\x48\x8B\x41\x10\x48\x85\xC0\x74\x18\x0F\x1F\x80\x00\x00\x00\x00", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::osub_140146470 = (sub_140146470)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8D\x41\x28\x48\x8B\xDA\x48\x8B\x49", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::osub_140032EA0 = (sub_140032EA0)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x68\x10\x48\x89\x70\x18\x57", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL));


    //sadly cant sigscan for this, yet
    //0x0041CB00 requires further investigation - looks like a table of pointers for various game functions

    //live = 0x0041CB00

    glb::TDOL = *(TDObjectList**)(glb::moduleBase + 0x00441BE0);
    glb::game = (TDGame*)glb::TDOL;
    glb::scene = (TDScene*)glb::TDOL->objs[9];
    glb::renderer = (TDRenderer*)glb::TDOL->objs[8];
    glb::player = (TDPlayer*)glb::TDOL->objs[20];

    std::cout << "" << std::endl;
    std::cout << "[Function Addresses:       ]" << std::endl;
    std::cout << "cursor               : " << std::hex << glb::ocursor << std::endl;
    std::cout << "tdRaycastCrashB      : " << std::hex << glb::tdRaycastCrashB << std::endl;
    //std::cout << "tdInitScreenSec      : " << std::hex << glb::tdInitScreenSecondary << std::endl;
    std::cout << "loadResource         : " << std::hex << glb::oLoadResource << std::endl;
    //std::cout << "sub_140105F30        : " << std::hex << glb::osub_140105F30 << std::endl;
    //std::cout << "sub_140146470        : " << std::hex << glb::osub_140146470 << std::endl;
    //std::cout << "sub_140032EA0        : " << std::hex << glb::osub_140032EA0 << std::endl;
    std::cout << "oValidatePath        : " << std::hex << glb::oValidatePath << std::endl;
    //std::cout << "oUnknGrapInitFunc    : " << std::hex << glb::oUnknGraphicsInitFunction << std::endl;
    std::cout << "oCreateTextureThing  : " << std::hex << glb::oCreateTextureThing << std::endl;
    //std::cout << "tdConstructScreen    : " << std::hex << glb::tdConstructScreen << std::endl;
    std::cout << "tdUpdateScreen       : " << std::hex << glb::tdUpdateScreen << std::endl;
    std::cout << "tdConstructJoint     : " << std::hex << glb::tdConstructJoint << std::endl;
    std::cout << "tdInitBall           : " << std::hex << glb::tdInitBall << std::endl;
    //std::cout << "tdInitHinge          : " << std::hex << glb::tdInitHinge << std::endl;
    //std::cout << "tdInitPrismatic      : " << std::hex << glb::tdInitPrismatic << std::endl;
    std::cout << "tdInitWire           : " << std::hex << glb::tdInitWire << std::endl;
    std::cout << "tdAttachJoint        : " << std::hex << glb::tdAttachJoint << std::endl;
    //std::cout << "tdUpdateJoint        : " << std::hex << glb::tdUpdateJoint << std::endl;
    //std::cout << "oDoQuicksave         : " << std::hex << glb::oDoQuicksave << std::endl;
    //std::cout << "oDoQuickload         : " << std::hex << glb::oDoQuickload << std::endl;
    //std::cout << "oValidate            : " << std::hex << glb::oValidate << std::endl;
    //std::cout << "load TDBIN           : " << std::hex << glb::oLtDBin << std::endl;
    //std::cout << "o_S140152540         : " << std::hex << glb::o_S140152540 << std::endl;
    //std::cout << "o_S1400C4F70         : " << std::hex << glb::o_S1400C4F70 << std::endl;
    //std::cout << "o_S140152740         : " << std::hex << glb::o_S140152740 << std::endl;
    std::cout << "tPaint               : " << std::hex << glb::oPaint << std::endl;
    std::cout << "drawLine             : " << std::hex << glb::oFDL << std::endl;
    std::cout << "raycast              : " << std::hex << glb::oRC << std::endl;
    std::cout << "create explosion     : " << std::hex << glb::TDcreateExplosion << std::endl;
    std::cout << "spawn vox            : " << std::hex << glb::oSpawnVox << std::endl;
    //std::cout << "delete object        : " << std::hex << glb::oTdDelBdy << std::endl;
    std::cout << "make hole            : " << std::hex << glb::oWrappedDamage << std::endl;
    std::cout << "create texture       : " << std::hex << glb::oCreateTexture << std::endl;
    std::cout << "create physics       : " << std::hex << glb::oCreatePhysics << std::endl;
    std::cout << "update shapes        : " << std::hex << glb::oUpdateShapes << std::endl;
    std::cout << "construct body       : " << std::hex << glb::oB_Constructor << std::endl;
    std::cout << "construct shape      : " << std::hex << glb::oS_Constructor << std::endl;
    std::cout << "set dynamic          : " << std::hex << glb::oSetDynamic << std::endl;
    std::cout << "malloc               : " << std::hex << glb::oTMalloc << std::endl;
    std::cout << "free                 : " << std::hex << glb::oTFree << std::endl;
    std::cout << "spawn fire           : " << std::hex << glb::oSpawnFire << std::endl;
    std::cout << "shoot projectile     : " << std::hex << glb::oPewpew << std::endl;
    std::cout << "spawn particle       : " << std::hex << glb::TDspawnParticle << std::endl;
    std::cout << "damage player        : " << std::hex << glb::oDamagePlayer << std::endl;
    std::cout << "damage object 2      : " << std::hex << glb::oDamageObject << std::endl;
    std::cout << "cam positioner       : " << std::hex << glb::oCamPos << std::endl;
    //std::cout << "winMain              : " << std::hex << glb::oTDmain << std::endl;
    //std::cout << "create point light   : " << std::hex << glb::oCreateLight << std::endl;
    //std::cout << "update environment   : " << std::hex << glb::oEnvUpdate << std::endl;
    std::cout << "fire spread          : " << std::hex << glb::oSpreadFire << std::endl;
    std::cout << "set attribute        : " << std::hex << glb::oSOA << std::endl;
    std::cout << "set attribute single : " << std::hex << glb::oSOAS << std::endl;
    std::cout << "highlight shape      : " << std::hex << glb::oHighlightShape << std::endl;
    std::cout << "outline shape        : " << std::hex << glb::oOutlineShape << std::endl;
    std::cout << "outline body         : " << std::hex << glb::oOutlineBody << std::endl;
    std::cout << "QuatEuler            : " << std::hex << glb::oQuatEul << std::endl;
    std::cout << "createPlankVox       : " << std::hex << glb::oSetBody << std::endl;
    //std::cout << "handle bombs         : " << std::hex << glb::oHandleBombObjs << std::endl;
    //std::cout << "set shape body       : " << std::hex << glb::oSetShapeParentBody << std::endl;
    //std::cout << "Add context item     : " << std::hex << glb::oAddCItem << std::endl;
    std::cout << "Read subobjects      : " << std::hex << glb::TDreadSubobjects << std::endl;
    //std::cout << "oIUnReadVox          : " << std::hex << glb::oIUnReadVox << std::endl;
    std::cout << "oIsActiveWindow      : " << std::hex << glb::oIsActive << std::endl;
    std::cout << "update shape body    : " << std::hex << glb::tdUpdateShapeBody << std::endl;

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
