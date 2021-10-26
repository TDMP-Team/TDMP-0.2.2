#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include <cfloat>
#include "camera.h"
#include "objectSpawner.h"

std::vector<TDShape*> knownShapes = {};
bool init = true;
uintptr_t bufferCopy = 0;

struct particleInfoStruct {
    float R;
    float G;
    float B;
    float A;
    float FadeR;
    float FadeG;
    float FadeB;
    float FadeA;
    float U05;
    float U06;
    float StartSize;
    float EndSize;
    float U09;
    float U10;
    float U11;
    float U12;
};

void printB4(byte obj[4]) {
    if (obj) {
        std::cout << obj[0] << " : " << obj[1] << " : " << obj[2] << " : " << obj[3] << " : " << obj[4] << std::endl;
    }
    else {
        std::cout << "NULL OBJ" << std::endl;
    }
}

void hkDoDamage(uintptr_t a1, uintptr_t a2, td::Vec3* a3, float a4, float a5, uintptr_t a6, uintptr_t a7, uintptr_t a8) {

    return glb::oDamageObject(a1, a2, a3, a4, a5, a6, a7, a8);
}

uintptr_t structContainer;
void hkCreateParticle(uintptr_t* ptr, td::particleInfo& orig, td::Vec3& position, td::Vec3& velocity, float lifetime) {
    uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);

    if (!structContainer) {
        structContainer = (uintptr_t)malloc(256);
    }

    memcpy((void*)structContainer, (void*)&orig, 256);
    std::cout << "InfoStruct: " << structContainer << std::endl;
    std::cout << "Position  : X:" << std::to_string(position.x) << " Y: " << std::to_string(position.y) << " Z: " << std::to_string(position.z) << std::endl;
    std::cout << "Velocity  : X:" << std::to_string(velocity.x) << " Y: " << std::to_string(velocity.y) << " Z: " << std::to_string(velocity.z) << std::endl;
    std::cout << "Lifetime  : " << lifetime << std::endl;
    std::cout << "==========" << std::endl;

    std::cout << std::to_string(orig.U01) << std::endl;
    std::cout << std::to_string(orig.U02) << std::endl;
    std::cout << std::to_string(orig.U03) << std::endl;
    std::cout << std::to_string(orig.U04) << std::endl;
    std::cout << std::to_string(orig.U05) << std::endl;
    std::cout << std::to_string(orig.U06) << std::endl;
    std::cout << std::to_string(orig.U07) << std::endl;
    std::cout << std::to_string(orig.U08) << std::endl;
    std::cout << std::to_string(orig.U09) << std::endl;
    std::cout << std::to_string(orig.U10) << std::endl;
    std::cout << std::to_string(orig.U11) << std::endl;
    std::cout << std::to_string(orig.U12) << std::endl;
    std::cout << std::to_string(orig.U13) << std::endl;
    std::cout << std::to_string(orig.U14) << std::endl;
    std::cout << std::to_string(orig.U15) << std::endl;
    std::cout << std::to_string(orig.U16) << std::endl;
    std::cout << std::to_string(orig.U17) << std::endl;
    std::cout << std::to_string(orig.U18) << std::endl;
    std::cout << std::to_string(orig.U19) << std::endl;
    std::cout << std::to_string(orig.U20) << std::endl;
    std::cout << std::to_string(orig.U21) << std::endl;
    std::cout << std::to_string(orig.U22) << std::endl;
    std::cout << std::to_string(orig.U23) << std::endl;
    std::cout << std::to_string(orig.U24) << std::endl;
    std::cout << std::to_string(orig.U25) << std::endl;
    std::cout << std::to_string(orig.U26) << std::endl;
    std::cout << std::to_string(orig.U27) << std::endl;
    std::cout << std::to_string(orig.U28) << std::endl;
    std::cout << std::to_string(orig.U29) << std::endl;
    std::cout << std::to_string(orig.U30) << std::endl;
    std::cout << std::to_string(orig.U31) << std::endl;
    std::cout << std::to_string(orig.U32) << std::endl;
    std::cout << std::to_string(orig.U33) << std::endl;
    std::cout << std::to_string(orig.U34) << std::endl;
    std::cout << std::to_string(orig.U35) << std::endl;
    std::cout << std::to_string(orig.U36) << std::endl;
    std::cout << std::to_string(orig.U37) << std::endl;
    std::cout << std::to_string(orig.U38) << std::endl;
    std::cout << std::to_string(orig.U39) << std::endl;
    std::cout << std::to_string(orig.U40) << std::endl;
    std::cout << std::to_string(orig.U41) << std::endl;
    std::cout << std::to_string(orig.U42) << std::endl;
    std::cout << std::to_string(orig.U43) << std::endl;
    std::cout << std::to_string(orig.U44) << std::endl;
    std::cout << std::to_string(orig.U45) << std::endl;
    std::cout << std::to_string(orig.U46) << std::endl;
    std::cout << std::to_string(orig.U47) << std::endl;
    std::cout << std::to_string(orig.U48) << std::endl;
    std::cout << std::to_string(orig.U49) << std::endl;
    std::cout << std::to_string(orig.U50) << std::endl;
    std::cout << std::to_string(orig.U51) << std::endl;
    std::cout << std::to_string(orig.U52) << std::endl;
    std::cout << std::to_string(orig.U53) << std::endl;
    std::cout << std::to_string(orig.U54) << std::endl;
    std::cout << std::to_string(orig.U55) << std::endl;
    std::cout << std::to_string(orig.U56) << std::endl;
    std::cout << std::to_string(orig.U57) << std::endl;
    std::cout << std::to_string(orig.U58) << std::endl;
    std::cout << std::to_string(orig.U59) << std::endl;
    std::cout << std::to_string(orig.U60) << std::endl;
    std::cout << std::to_string(orig.U61) << std::endl;
    std::cout << std::to_string(orig.U62) << std::endl;
    std::cout << std::to_string(orig.U63) << std::endl;
    std::cout << std::to_string(orig.U64) << std::endl;

    td::particleInfo testing = { };
    testing.U01 = orig.U01;
    testing.U02 = orig.U02;
    testing.U03 = orig.U03;
    testing.U04 = orig.U04;      //R
    testing.U05 = orig.U05;      //G
    testing.U06 = orig.U06;      //B
    testing.U07 = orig.U07;      //A
    testing.U08 = orig.U08;
    testing.U09 = orig.U09;
    testing.U10 = orig.U10;
    testing.U11 = orig.U11;
    testing.U12 = orig.U12;
    testing.U13 = orig.U13;
    testing.U14 = orig.U14;
    testing.U15 = orig.U15;
    testing.U16 = orig.U16;
    testing.U17 = orig.U17;
    testing.U18 = orig.U18;
    testing.U19 = orig.U19;
    testing.U20 = orig.U20;
    testing.U21 = orig.U21;
    testing.U22 = orig.U22;
    testing.U23 = orig.U23;
    testing.U24 = orig.U24;
    testing.U25 = orig.U25;
    testing.U26 = orig.U26;
    testing.U27 = orig.U27;
    testing.U28 = orig.U28;
    testing.U29 = orig.U29;
    testing.U30 = orig.U30;
    testing.U31 = orig.U31;
    testing.U32 = orig.U32;
    testing.U33 = orig.U33;
    testing.U34 = orig.U34;
    testing.U35 = orig.U35;
    testing.U36 = orig.U36;
    testing.U37 = orig.U37;
    testing.U38 = orig.U38;
    testing.U39 = orig.U39;
    testing.U40 = orig.U40;
    testing.U41 = orig.U41;
    testing.U42 = orig.U42;
    testing.U43 = orig.U43;
    testing.U44 = orig.U44;
    testing.U45 = orig.U45;
    testing.U46 = orig.U46;
    testing.U47 = orig.U47;
    testing.U48 = orig.U48;
    testing.U49 = orig.U49;
    testing.U50 = orig.U50;
    testing.U51 = orig.U51;
    testing.U52 = orig.U52;
    testing.U53 = orig.U53;
    testing.U54 = orig.U54;
    testing.U55 = orig.U55;
    testing.U56 = orig.U56;
    testing.U57 = orig.U57;
    testing.U58 = orig.U58;
    testing.U59 = orig.U59;
    testing.U60 = orig.U60;
    testing.U61 = orig.U61;
    testing.U62 = orig.U62;
    testing.U63 = orig.U63;
    testing.U64 = orig.U64;

    glb::TDspawnParticle((DWORD*)special, (td::particleInfo*)&testing, position, velocity, lifetime);
}

bool isKnown(TDShape* test) {
    for (TDShape* current : knownShapes) {
        if (current == test) {
            return true;
        }
    }
    return false;
}

void hkCreateExplosion(uintptr_t scene, td::Vec3* pos, float power) {
    std::cout << "PTR: " << scene << std::endl;
    std::cout << "POS: " << pos << std::endl;
    std::cout << "PWR: " << std::to_string(power) << std::endl;

    return glb::TDcreateExplosion(scene, pos, power);
}

void hkItem(TDShape* shape, td::small_string* attribute, td::small_string* attribute2){
 
    //td::Vec3 high = { FLT_MAX, FLT_MAX, FLT_MAX };

    if (!isKnown(shape)) {
        knownShapes.push_back(shape);
        std::cout << "Shape: " << shape << "\nAttrib: " << attribute->c_str() << "\nAttrib2: " << attribute2->c_str() << std::endl;
        std::cout << "==========" << std::endl;
    }

    return glb::oSOA(shape, attribute, attribute2);
}

void hkCreateFire(TDScene* scene, td::Vec3* position) {
    uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
    std::cout << "SCENE: " << scene << std::endl;
    std::cout << "TEST : " << sceneSpecial << std::endl;
}

void hkPointLight(TDRenderer* renderer, td::Vec3* a2, td::Vec3* a3, float a4, float a5, float a6, float a7, int a8) {
    std::cout << "PTR: " << renderer << std::endl;
    std::cout << "A2 : X: " << a2->x << " Y: " << a2->y << " Z: " << a2->z << std::endl;
    std::cout << "A3 : R: " << a3->x << " G: " << a3->y << " B: " << a3->z << std::endl;
    std::cout << "A4 : " << a4 << std::endl;
    std::cout << "A5 : " << a5 << std::endl;
    std::cout << "A6 : " << a6 << std::endl;
    std::cout << "A7 : " << a7 << std::endl;
    std::cout << "A8 : " << a8 << std::endl;
    std::cout << "=====" << std::endl;

    td::Vec3 col = { 1.f, 1.f, 1.f };

    return glb::oCreateLight(renderer, a2, a3, 0.1f, 0.1f, 1.5f, 1.5f);
}

void hkEnvUpdate(uintptr_t environment) {

    std::cout << "Env: " << environment << std::endl;

    return glb::oEnvUpdate(environment);
}

uintptr_t hkSpawnVox(td::small_string* path, td::small_string* subpath, float scale) {
    std::cout << "Path: " << path->c_str() << std::endl;
    std::cout << "SPath: " << subpath->c_str() << std::endl;
    std::cout << "==========" << std::endl;

    td::small_string cube = ("vox\\Default\\dbgcube_glassBig\\object.vox");

    return glb::oSpawnVox(&cube, subpath, scale);
}

void hkSetAttribute(TDShape* object, td::small_string* a1, td::small_string* a2) {

    std::cout << "SOA | Object: " << object << " | A1: " << a1->c_str() << " | A2: " << a2->c_str() << std::endl;

    glb::oSOA(object, a1, a2);

    return;
}


void hkContextItem(char* a1, int a2, int a3, float* a4) {

    std::cout << std::string(a1) << std::endl;

    return glb::oAddCItem(a1, a2, a3, a4);
}

void* hkreadVoxInfo(void* a1, void* a2) {

    std::cout << "A1: " << a1 << " Path: " << ((td::small_string*)(a2))->c_str() << std::endl;

    return glb::oIUnReadVox(a1, a2);
}

__int64 hk_S140152540(__int64 a1, __int64 a2, __int64 a3) { //decompress TDBin file

    std::cout << "DECOMPRESSION:" << std::endl;
    std::cout << "a1:" << std::hex << a1 << std::endl;
    std::cout << "a2:" << std::hex << a2 << std::endl;
    std::cout << "a3:" << std::hex << a3 << std::endl;


    return glb::o_S140152540(a1, a2, a3);
}

__int64 hk_S1400C4F70(__int64 a1, __int64 a2) { //decode TDBin

    std::cout << "DECODE:" << std::endl;
    std::cout << "a1:" << std::hex << a1 << std::endl;
    std::cout << "a2:" << std::hex << a2 << std::endl;

    return glb::o_S1400C4F70(a1, a2);
}

void* hk_S140152740(void* a1) {

    return glb::o_S140152740(a1);
}


char hkLoadTDBIN(__int64 a1, td::small_string* a2) {


    return glb::oLtDBin(a1, a2);
}

void* hkValidateFileExistance(__int64 a1, void* a2, td::small_string* path) {
    
    std::cout << "Validate: " << path->c_str() << std::endl;

    if (strcmp(path->c_str(), "quicksave.bin") == 0) {
        td::small_string testPath = "CustomQuicksave.bin";

        std::cout << "Saving custom: " << testPath.c_str() << std::endl;


        return glb::oValidate(a1, a2, &testPath);
    }

    return glb::oValidate(a1, a2, path);
}

bool hkDoQuicksave(TDScene* a1) {

    std::cout << "Quicksave called : " << std::hex << a1 << std::endl;

    return glb::oDoQuicksave(a1);
}

bool hkDoQuickload(TDScene* a1) {

    std::cout << "Quickload called : " << std::hex << a1 << std::endl;

    return glb::oDoQuickload(a1);
}

void hkCreateBallJoint(TDJoint* joint, TDShape* shape1, TDShape* shape2, td::Vec3* point1, td::Vec3* point2) {

    std::cout << "Shape 1: " << std::hex << (uintptr_t)shape1 << std::endl;
    std::cout << "Shape 2: " << std::hex << (uintptr_t)shape2 << std::endl;


    std::cout << "Point1 X: " << std::to_string(point1->x) << " Y: " << std::to_string(point1->y) << " Z:" << std::to_string(point1->z) << std::endl;
    std::cout << "Point2 X: " << std::to_string(point2->x) << " Y: " << std::to_string(point2->y) << " Z:" << std::to_string(point2->z) << std::endl;

    std::cout << "==========" << std::endl;

    glb::tdInitBall(joint, shape1, shape2, point1, point2);
}

uintptr_t hkMalloc(size_t bytes) {
    uintptr_t newPtr = glb::oTMalloc(bytes);
    if (bytes > 1024) {
        std::cout << "Allocated memory sized " << std::to_string(bytes) << "B at " << std::hex << newPtr << std::endl;
    }

    return newPtr;
}

__int64 hkUnknownGraphicsThing(void* a1) {
    std::cout << "UGT: " << ((td::small_string*)a1)->c_str() << std::endl;
    return glb::oUnknGraphicsInitFunction(a1);
}

__int64 hkcreateTextureThing(void* texture, void* pixelBuffer, bool a3) {


    int formatSwitch = *(int*)texture;
    int format;

    if (formatSwitch == 1) {
        format = 0x1903;
        std::cout << "TEXTURE: " << texture << " PXBUFFER: " << pixelBuffer << " X: " << std::to_string(((int*)texture)[1]) << " Y: " << std::to_string(((int*)texture)[2]) << " FORM: RED" << std::endl;
    }
    else if (formatSwitch == 2) {
        format = 0x1907;
        std::cout << "TEXTURE: " << texture << " PXBUFFER: " << pixelBuffer << " X: " << std::to_string(((int*)texture)[1]) << " Y: " << std::to_string(((int*)texture)[2]) << " FORM: RGBA" << std::endl;
    }
    else if (formatSwitch == 3) {
        format = 0x1908;
        std::cout << "TEXTURE: " << texture << " PXBUFFER: " << pixelBuffer << " X: " << std::to_string(((int*)texture)[1]) << " Y: " << std::to_string(((int*)texture)[2]) << " FORM: RGB" << std::endl;
    }

    if (((int*)texture)[1] == 0x280) {
        //find the openGL texture for the TV
        //camera::alt_texture = texture;
    }

    return glb::oCreateTextureThing(texture, pixelBuffer, a3);
}

void hkGlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels) {
    std::cout << "glTexImage2D" << std::endl;
    return glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

__int64 hkUpdateShapeBody(uintptr_t a1, uintptr_t a2) {

    std::cout << "A1: " << std::hex << a1 << " A2: " << std::hex << a2 << std::endl;

    return glb::tdUpdateShapeBody(a1, a2);
}

void* hkValidatePath(void* special, td::small_string* path, td::small_string* path_technical, td::small_string* type) {

    const char* ignoreStrings[] = { "regular.ttf", "bold.ttf", "menu.ogg" };
    const char* thisStr = path_technical->c_str();
    bool swtch = true;

    for (const char* thiChp : ignoreStrings) {
        if (strcmp(thiChp, thisStr) == 0) {
            swtch = false;
        }
    }

    void* returns = glb::oValidatePath(glb::game + 0xA8, path, path_technical, type);

    return returns;
}

void* hk_sub_140105F30(TDScreen* screen, int a2) {
    std::cout << "sub_140105F30: SCREEN: " << screen << " A2: " << a2 << std::endl;
    return glb::osub_140105F30(screen, a2);
}

void* hk_sub_140146470(__int64 a1, __int64 a2) {
    std::cout << "sub_140105F30: A1: " << a1 << " A2: " << a2 << std::endl;
    return glb::osub_140146470(a1, a2);
}

void* hk_sub_140032EA0(float* a1, __int64 a2, float* a3) {
    std::cout << "sub_140032EA0: A1: " << a1 << " A2: " << a2 << " A3: " << a3 << std::endl;
    return glb::osub_140032EA0(a1, a2, a3);
}

void* hkLoadResource(void* a1, void* a2, int a3) {
    std::cout << "A1: " << a1 << " A2 " << ((td::small_string*)a2)->c_str() << " A3: " << std::to_string(a3) << std::endl;
    return glb::oLoadResource(a1, a2, a3);
}

__int64 hkInitScreen(TDScreen* a1, uintptr_t a2) {
    std::cout << "[InitScreen1] Screen: " << a1 << " UN: " << a2 << std::endl;
    return glb::tdConstructScreen(a1, a2);
}

void* hkInitScreenSecondary(void* a1, void* a2, void* a3) {
    std::cout << "[InitScreen2] A1: " << a1 << " A2: " << a2 << " A3: " << a3 << std::endl;
    return glb::tdInitScreenSecondary(a1, a2, 0);
}

void hkUpdateScreen(TDScreen* screen, void* a2) {
    std::cout << "[UpdateScreen] Screen: " << screen << std::endl;
    return glb::tdUpdateScreen(screen);
}

char hkRayCrashA(void* a1, float* a2, float* a3, float* a4, float* a5) {
    if (a1 < (void*)0xFF) {
        printf_s("E: A1: %p A2: %p A3: %p A4: %p A5: %p\n", a1, a2, a3, a4, a5);
        return 0;
    }
    return glb::tdRaycastCrashA(a1, a2, a3, a4, a5);
}

char hkRayCrashB(void* a1, void* a2) {
    if (a2 < (void*)0xFF) {
        printf_s("E: A1: %p A2: %p\n", a1, a2);
        return 0;
    }
    return glb::tdRaycastCrashB(a1, a2);
}

void initTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //DetourAttach(&(PVOID&)glb::oCreateTextureThing, hkcreateTextureThing);
    //DetourAttach(&(PVOID&)glb::tdConstructScreen, hkInitScreen);
    //DetourAttach(&(PVOID&)glb::tdInitScreenSecondary, hkInitScreenSecondary);
    //DetourAttach(&(PVOID&)glb::tdUpdateScreen, hkUpdateScreen);
    //DetourAttach(&(PVOID&)glb::tdRaycastCrashA, hkRayCrashA);
    //DetourAttach(&(PVOID&)glb::tdRaycastCrashB, hkRayCrashB);
    DetourTransactionCommit();
}

void terminateTestHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //DetourDetach(&(PVOID&)glb::oCreateTextureThing, hkcreateTextureThing);
    //DetourDetach(&(PVOID&)glb::tdConstructScreen, hkInitScreen);
    //DetourDetach(&(PVOID&)glb::tdInitScreenSecondary, hkInitScreenSecondary);
    //DetourDetach(&(PVOID&)glb::tdUpdateScreen, hkUpdateScreen);
    //DetourDetach(&(PVOID&)glb::tdRaycastCrashA, hkRayCrashA);
    //DetourAttach(&(PVOID&)glb::tdRaycastCrashB, hkRayCrashB);
    DetourTransactionCommit();
}