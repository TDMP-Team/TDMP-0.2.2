#pragma once
#include "Global.h"
#include "TDFuncs.h"
#include "maths.h"
#include "glm/gtc/quaternion.hpp"
#include "td_memory.h"

//thanks nahu for the entity structures

enum class gameState : int8_t {
	splash = 1,
	menu = 2,
	start_ui = 3,
	ingame = 4,
	editor = 5,
	quitting = 6
};

static const char* gameStateTXT[]{
	"splash",
	"menu",
	"start_ui",
	"ingame",
	"editor",
	"quitting"
};

enum class entityType : int8_t
{
	Body = 1,
	Shape = 2,
	Light = 3,
	Location = 4,
	Water = 5,
	Enemy = 6,
	Joint = 7,
	Vehicle = 8,
	Wheel = 9,
	Screen = 10,
	Trigger = 11,
	Script = 12
};

static const char* entityTypeStr[] = {
	"Body",
	"Shape",
	"Light",
	"Location",
	"Water",
	"Enemy",
	"Joint",
	"Vehicle",
	"Wheel",
	"Screen",
	"Trigger",
	"Script"
};

class Entity
{
public:
	entityType	Type;			//0x08
	int8_t		Flags;			//0x09
	char		pad_000A[2]; //0x000A
	int		Id; //0x000C
	//char		pad_000E[2];	//0x0E
	Entity* pParent;		//0x10
	Entity* pSibling;		//0x18
	Entity* pChild;			//0x20
	virtual void	Destroy(Entity* entity, bool freeMemory);
	virtual Entity* SetParent(Entity* entity, Entity* parent);
	virtual void	Function2();
	virtual void	Function3();
}; // Size: 0x28

class Environment
{
public:
	char* m_Skybox; //0x0000
	char pad_0008[8]; //0x0008
	td::Vec3 m_SkyboxTint; //0x0010
	float m_SkyboxBrightness; //0x001C
	float m_SkyboxRot; //0x0020
	char pad_0024[12]; //0x0024
	td::Vec3 m_SunColorTint; //0x0030
	char pad_003C[16]; //0x003C
	float m_SunBrightness; //0x004C
	float m_SunSpread; //0x0050
	float m_SunLength; //0x0054
	char pad_0058[4]; //0x0058
	float m_SunGlare; //0x005C
	char pad_0060[4]; //0x0060
	td::Vec4 m_Constant; //0x0064
	float m_Ambient; //0x0074
	float m_AmbienetExponent; //0x0078
	td::Vec2 m_Exposure; //0x007C min, max
	float m_Brightness; //0x0084
	td::Vec3 m_FogColor; //0x0088
	char pad_0094[4]; //0x0094
	td::Vec4 m_FogParams; //0x0098
	float m_Wetness; //0x00A8
	float m_PuddleAmount; //0x00AC
	float m_Puddlesize; //0x00B0
	float m_Rain; //0x00B4
	bool m_Nightlight; //0x00B8
	char pad_00B9[87]; //0x00B9
	td::small_string m_Ambience; //0x0110
	float m_Slippery; //0x0128
	float m_FogScale; //0x012C
}; //Size: 0x0114

class TDVox
{
public:
	int32_t sizeX; //0x0000
	int32_t sizeY; //0x0004
	int32_t sizeZ; //0x0008
	char pad_000C[4]; //0x000C
	void* MaterialBuffer; //0x0010
	void* PhysicsBuffer; //0x0018
	float Scale; //0x0020
	char pad_0024[56]; //0x0024
	int32_t VoxelCount; //0x005C
}; //Size: 0x0060


class TDBody : public Entity
{
public:
	td::Vec3 Position; //0x0028
	td::Vec4 Rotation; //0x0034
	td::Vec3 Position2; //0x0044
	td::Vec4 Rotation2; //0x0050
	td::Vec3 Position3; //0x0060
	td::Vec4 Rotation3; //0x006C
	td::Vec3 Velocity; //0x007C
	td::Vec3 RotationVelocity; //0x0088
	char pad_0094[8]; //0x0094
	float density;
	float Lightness; //0x00A0
	char pad_00A4[24]; //0x00A4
	float Integrity; //0x00BC
	td::Vec3 N00006C83; //0x00C0
	td::Vec3 N00006C80; //0x00CC
	float N00002F17; //0x00D8
	bool Dynamic; //0x00DC
	char pad_00DD[7]; //0x00DD
	char countDown;
	bool isAwake; //0x00E5
	char pad_00E6[2]; //0x00E6

	int countContainedShapes() {
		Entity* shapePtr = this->pChild;
		int counter = 0;
		while (shapePtr != 0) {
			shapePtr = shapePtr->pSibling;
			counter++;
		}
		return counter;
	}
}; //Size: 0xE8


class TDShape : public Entity
{
public:
	td::Vec3 pOffset; //0x0000
    td::Vec4 rOffset; //0x000C
    td::Vec3 posMin; //0x001C
    td::Vec3 posMax; //0x0028
    char pad_0034[22]; //0x0034
    bool collide; //0x0072
    char pad_004B[9]; //0x004B
    int32_t Texture; //0x007C
    char pad_0080[24]; //0x0080
    TDVox* pVox; //0x0098
    char pad_00A0[16]; //0x00A0
    float Intergrity; //0x00B0
    bool isBroken; //0x00B4

	TDBody* getParentBody() {
		return (TDBody*)(this->pParent);
	}
}; //Size: 0xB0

class TDJoint : public Entity {
public:
	TDShape* m_Attachment0;
	TDShape* m_Attachment1;
	TDJoint* m_AttachmentJoint0;
	TDJoint* m_AttachmentJoint1;
	uint32_t m_JointType;
	float m_Size;
	bool m_Collide;
	bool m_IsConnected; uint8_t pad_52[2];
	float m_Stiffness;
	float m_Springiness;
	int32_t field_5C;
	int64_t field_60;
	td::Vec3 m_AttachmentPosition0;
	td::Vec3 m_AttachmentPosition1;
	td::Vec3 m_AttachmentDirection0;
	td::Vec3 m_AttachmentDirection1;
	td::Vec4 m_Angle;
	td::Vec2 m_Limits; // In meters or radians depending on joint type
	td::Vec2 m_Motor;
	int64_t field_B8; // Pointer to unidentified structure; only used with Wire type
	bool m_Sound; uint8_t pad_C1[7];
};

class TDObjectList {
public:
	uintptr_t objs[128];
};


class TDPlayer {
public:
	td::Vec3 position;			//0x000 - 0x012
	byte paddingA[0x2C];		//0x012 - 0x038
	td::Vec3 velocity;			//0x038 - 0x04A
	byte paddingB[0x36];		//0x04A - 0x07C
	td::Vec3 cameraPosition;	//0x07C - 0x088
	td::Vec4 cameraQuat;		//0x088 - 0x098
	byte paddingC[0x2C];		//0x098 - 0x0CE
	float camPitch;
	float camYaw;
	//byte paddingD[0x2];
	float FMove;				//0x0CE - 0x0D2
	float SMove;				//0x0D2 - 0x0D6
	byte paddingE[0xB];			//0x0D6 - 0x0EA
	byte isAttackingSecondary;
	byte isAttackingPadding;
	byte isAttacking;			//0x0EA - 0x0EB
	byte padding[0x28];
	TDBody* grabbedBody;
	byte paddingF[0x4C - 0x04 - 8];		//0x0EB - 0x154
	//byte paddingF[0x74];
	float health;				//0x15C - 0x160
	byte paddingG[0x484];		//0x160 - 0x418
	//td::small_string heldItemName;		//0x3CB - 0x3D8
	char pad[13];
	TDBody* toolBody;

	td::Vec3 cameraEuler() {
		float vecX = -(2 * (cameraQuat.w * cameraQuat.y + cameraQuat.z * cameraQuat.x));
		float vecY = -(2 * (cameraQuat.x * cameraQuat.y - cameraQuat.z * cameraQuat.w));
		float vecZ = -(1 - 2 * (cameraQuat.w * cameraQuat.w + cameraQuat.x * cameraQuat.x));

		return { vecX, vecY, vecZ };
	}
};

struct FireSystem {
	struct Fire {
		TDShape* Parent;
		td::Vec3 Position;
		float m_TimeRemaining;
		float m_TotalLifetime;
	};

	void* vftptr_0;
	td::small_vector<Fire> m_Instances;
};

class TDRenderer {

};

class TDLight : Entity {
	enum class Type : uint8_t {
		Sphere = 1,
		Cone = 2,
		Area = 3,
	};

public:
	bool m_Enabled;
	uint8_t pad29[3];
	Type m_LightType;
	uint8_t pad2D[3];
	td::Vec3 m_Transform;
	td::Vec3 m_Color;
	float dword58;
	float m_Reach;
	float m_Size;
	float m_ShadowDistance;
	float m_InnerAngle;
	float m_OuterAngle;
	float m_Fogiter;
	float m_FogScale;
	td::Vec2 m_AreaSize;
	float m_Glare;
	uint64_t qword84;
	uint64_t qword8C;
	bool byte94;
	uint8_t pad95[3];
	uint32_t dword98;
	uint16_t word9C;
	uint8_t pad9D[2];
	uint8_t gap9f[520]; // This is a ShaderBinding instance
	uint8_t qword2A8[56]; // This is a Buffer instance
	uint8_t gap2E0[32]; // This is AnotherBuffer instance
	uint32_t dword300;
	uint32_t dword304;
	uint64_t qword308;
	uint8_t gap310[3072];
	uint32_t dwordF10;
	uint32_t dwordF14;
	uint64_t qwordF18;
	uint8_t gapF20[1024];
	uint64_t qword1320;
	uint64_t qword1328;
	uint64_t qword1330;
	td::small_string m_Sound;
	uint64_t qword1348;
	uint32_t dword1350;
	uint32_t dword1354;
};

class Water : public Entity
{
public:
	char pad_0028[28];	//0x0028
	float m_Depth;		//0x0044
	char pad_0048[792]; //0x0048
	float m_Wave;		//0x0360
	float m_Ripple;		//0x0364
	float m_Motion;		//0x0368
	float m_Foam;		//0x036C
}; //Size: 0x370

class TDScreen : public Entity {
public:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec2 m_Size;
	float m_Bulge;
	glm::ivec2 m_Resolution;
	td::small_string m_Script;
	bool m_Enabled;
	bool m_Interactive; uint8_t pad_68[2];
	float m_Emissive;
	float m_FxRaster;
	float m_FxChromaticAbberation;
	float m_FxNoise;
	float m_FxGlitch;
	int32_t field_80;
	uint8_t gap84[2228];
	int32_t field_938;
	uint8_t gap93C[60];
	int32_t field_978;
	uint8_t gap97C[508];
	int64_t field_B78;
	uint8_t field_B80[56]; // This is a Buffer instance
	uint8_t field_BB8[32]; // This is AnotherBuffer instance
	glm::mat4x4 m_Matrix;
	uint8_t gapBFC[64];
	int64_t field_C58;
	int64_t field_C60;
	int64_t field_C68;
	uint8_t gapC70[4];
	int32_t field_C74;
};

class TDVehicle;

class TDWheel : public Entity
{
public:
	TDVehicle* m_Vehicle;	  //0x0028
	TDBody* m_VehicleBody;  //0x0030
	TDBody* m_WheelBody;	  //0x0038
	TDShape* m_WheelShape;  //0x0040
	char pad_0048[56];	  //0x0048
	float m_Steer;		  //0x0080
	float m_Drive;		  //0x0084
	glm::vec3 m_Travel;	  //0x0088
	char pad_0094[24];	  //0x0094
	float m_TurnSpeed;	  //0x00AC
	char pad_00B0[64];	  //0x00B0
};

class TDVehicle : public Entity
{
public:
	TDBody* m_Body;					  //0x0028
	glm::vec3 m_Position;			  //0x0030
	glm::quat m_Rotation;			  //0x003C
	char pad_004C[28];				  //0x004C
	td::small_vector<TDWheel*> m_Wheels;	  //0x0068
	char pad_0078[62];				  //0x0078
	int m_State;
	float m_TopSpeed;				  //0x00BC
	char pad_00C0[4];				  //0x00C0
	float m_Spring;					  //0x00C4
	float m_Damping;				  //0x00C8
	float m_Acceleration;			  //0x00CC
	float m_Strength;				  //0x00D0
	float m_Friction;				  //0x00D4
	char pad_00D8[4];				  //0x00D8
	bool m_Driven;					  //0x00DC
	char pad_00DD[3];				  //0x00DD
	float m_Antispin;				  //0x00E0
	float m_SteerAssist;			  //0x00E4
	char pad_00E8[4];				  //0x00E8
	float m_Antiroll;				  //0x00EC
	float m_Difflock;				  //0x00F0
	char pad_00F4[4];				  //0x00F4
	char* m_Sound;					  //0x00F8
	char pad_0100[60];				  //0x0100
	/// <summary>
	/// X - Throttle (1: forward; -1: backward)<para/>
	/// Y - Steering (1: left; -1: right)<para/>
	/// ----no Z - Handbrake (1: Active; 0: Not active)<para/>
	/// 0x013C
	/// </summary>
	glm::vec2 m_MoveInput;			  
	float m_Handbrake;				  //0x0144
	glm::vec2 m_MouseInput;
	//float m_M1Down;					  //0x0148
	//float m_M2Down;					  //0x014C
	char pad_0150[8];				  //0x0150
	float m_Turn;					  //0x0158
	float m_AccelerationCompletion;	  //0x015C
	float m_BrakePower;				  //0x0160
	float m_AccelerationCompletion2;  //0x0164
	char pad_0168[4];				  //0x0168
	float m_VehicleCondition;		  //0x016C
	char pad_0170[264];				  //0x0170
	bool m_RemoteDrive;		      //0x0278
	float m_RemoteThrottle;			  //0x027C
	float m_RemoteSteering;			  //0x0280
	bool m_RemoteHandbrake;			  //0x0284
};							  //Size: 0x0288 (not valid)

class TDScene {
public:
	char pad_0008[136];									//0x0008
	td::small_vector<void*> m_Projectiles;					//0x0090
	Environment* pEnvironment;						//0x00A0
	td::Vec3 m_SpawnPos;								//0x00A8
	char pad_00D4[20];									//0x00B4
	TDLight* m_Flashlight;								//0x00C8
	char pad_00D0[48];									//0x00D0
	TDVehicle* m_CurrentVehicle;								//0x0100
	void* m_CurrentScreen;								//0x0102
	td::Vec3 m_ShadowVolumeSize;						//0x0110
	char pad_0124[252];									//0x0116
	td::small_vector<TDBody*>* m_Bodies;						//0x0220
	td::small_vector<TDShape*>* m_Shapes;						//0x0228
	td::small_vector<TDLight*>* m_Lights;						//0x0230
	td::small_vector<void*>* m_Locations;					//0x0238
	td::small_vector<Water*>* m_Waters;						//0x0240
	td::small_vector<void*>* m_Enemies;						//0x0248
	td::small_vector<void*>* m_Joints;						//0x0250
	td::small_vector<TDVehicle*>* m_Vehicles;					//0x0258
	td::small_vector<TDWheel*>* m_Wheels;						//0x0260
	td::small_vector<TDScreen*>* m_Screens;						//0x0268
	td::small_vector<void*>* m_Triggers;					//0x0270
	td::small_vector<void*>* m_Scripts;					//0x0278
	char pad_0280[664];									//0x0280
	td::small_vector<td::Vec2*> m_Boundaries;				//0x0530
	char pad_0528[864];									//0x0528
	td::small_vector<Entity*> m_Entities;					//0x0888
	char pad_0898[8];									//0x0898
	td::Vec4 m_ColorBalance; 							//0x08A0
	float m_Saturation; 								//0x08B0
	float m_Gamma; 										//0x08B4
	float m_Bloom; 										//0x08B8
	char pad_08BC[4]; 									//0x08BC


	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
};

class CGameSettings
{
public:
	int m_RenderScale;		//0x0000
	int m_RenderQuality;	//0x0004
	int m_GammaCorrection;	//0x0008
	int m_FOV;				//0x000C
	int m_MotionBlur;		//0x0010
	int m_BarrelDist;		//0x0014
	int m_DOF;				//0x0018
	int m_VSync;			//0x001C
	int m_SoundVol;			//0x0020
	int m_AmbienceVol;		//0x0024 (Unused?)
	int m_MusicVol;			//0x0028
	int m_MouseSmoothing;	//0x002C
	int m_MouseSensitivity; //0x0030
	int m_InvertLook;		//0x0034
	int m_HeadBob;			//0x0038
}; //Size: 0x003C

class TDGame {
public:
	int32_t m_ResX;					//0x0000
	int32_t m_ResY;					//0x0004
	gameState State;
	int32_t m_TargetState;			//0x000C
	float m_LoadingEffect;			//0x000D
	bool m_Playing;					//0x0014
	char pad_0015[3];				//0x0015
	float m_LoadingEffect2;			//0x0018
	bool m_MapOpen;					//0x001C
	char pad_001D[43];				//0x001D
	TDScene* m_Scene;				//0x0048
	char pad_0058[40];				//0x0058
	void* m_MenuScript;				//0x0080
	char pad_0088[24];				//0x0088
	TDPlayer* m_Player;				//0x00A0
	void* m_PathMgr;				//0x00A8
	//char pad_00A8[48];				//0x00B0
	void* N00000084; //0x00B0
	void* N00000085; //0x00B8
	void* m_Registry; //0x00C0
	void* N00000087; //0x00C8
	void* N00000088; //0x00D0
	void* m_Debug; //0x00D8
	void* m_SteamInterface;			//0x00E0 - To be used for mod sync
	char pad_00E8[48];				//0x00E8
	bool m_Paused;					//0x0118
	char pad_0119[3];				//0x0119
	int32_t m_iElapsedTime;			//0x011C
	int32_t m_iPlayedTime;			//0x0120
	float m_SoundTimeScale;			//0x0124
	float m_Timescale;				//0x0128
	float m_TimeStep;				//0x012C
	float m_TimeDelta;				//0x0130
	float m_fElapsedTime;			//0x0134
	float m_fPlayedTime;			//0x0138
	float m_TimeDelta2;				//0x013C
	char pad_0140[28];				//0x0140
	char pad_matrix_1[128];			//0x015C
	char pad_matrix_2[128];			//0x01DC	
	char pad_025C[36];				//0x025C
	CGameSettings m_GameSettings;	//0x0280
	char pad_02BC[4];				//0x02BC
	td::small_string m_LevelId;			//0x02C0
	td::small_string m_LevelLayer;		//0x02D8
	char pad_02F0[56];				//0x02F0
	td::small_string m_LevelId2;		//0x0328
	td::small_string m_LevelLayer2;		//0x0340
	char pad_0358[80];				//0x0358
	td::small_string m_TempPath;	//0x03A8
};

