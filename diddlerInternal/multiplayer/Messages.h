// This file is exact the same as Messages.h of SpaceWar

#ifndef MESSAGES_H
#define MESSAGES_H

#define LittleDWord( val )	( val )
#define LittleQWord(val)	(val)
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#pragma pack( push, 1 )

#include <thread>
#include <mutex>

#include "../types.h"
#include "Main.h"
#include <Windows.h>

// Network message types
enum EMessage
{
	// Server messages
	k_EMsgServerBegin = 0,
	k_EMsgServerSendInfo = k_EMsgServerBegin + 1,
	k_EMsgServerUpdateWorld = k_EMsgServerBegin + 4,
	k_EMsgServerUpdateBody = k_EMsgServerBegin + 5,
	k_EMsgServerExiting = k_EMsgServerBegin + 6,
	k_EMsgServerPingResponse = k_EMsgServerBegin + 7,
	k_EMsgServerAuthed = k_EMsgServerBegin + 8,
	k_EMsgClientBeginAuthentication = k_EMsgServerBegin + 9,
	k_EMsgClientExiting = k_EMsgServerBegin + 10,
	k_EMsgVehicleDriving = k_EMsgServerBegin + 11,
	k_EMsgLuaCallback = k_EMsgServerBegin + 12,

	// Player data
	k_EMsgPlayerTransform = k_EMsgServerBegin + 20,

	// P2P authentication messages
	k_EMsgP2PBegin = 600,
	k_EMsgP2PSendingTicket = k_EMsgP2PBegin + 1,

	// force 32-bit size enum so the wire protocol doesn't get outgrown later
	k_EForceDWORD = 0x7fffffff,
};


struct MsgServerConnectionResult
{
	MsgServerConnectionResult() : m_dwMessageType(LittleDWord(k_EMsgServerSendInfo)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetSuccess(bool s) { isSuccess = s; };
	const bool& GetSuccess() const { return isSuccess; };

private:
	const DWORD m_dwMessageType;
	bool isSuccess;
};

struct MsgClientBeginAuthentication
{
	MsgClientBeginAuthentication() : m_dwMessageType(LittleDWord(k_EMsgClientBeginAuthentication)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetSteamID(uint64 ulSteamID) { m_ulSteamID = LittleQWord(ulSteamID); }
	const uint64& GetSteamID() const { return LittleQWord(m_ulSteamID); }

	void SetToken(const char* pchToken, uint32 unLen) { m_uTokenLen = LittleDWord(unLen); memcpy(m_rgchToken, pchToken, MIN(unLen, sizeof(m_rgchToken))); }
	uint32 GetTokenLen() { return LittleDWord(m_uTokenLen); }
	const char* GetTokenPtr() { return m_rgchToken; }
private:
	const DWORD m_dwMessageType;

	uint32 m_uTokenLen;
	char m_rgchToken[1024];

	uint64 m_ulSteamID;
};

struct MsgVehicle
{
	float throttle;
	float steer;
	float handbrake;

	int16_t id;
};

/// <summary>
/// Contains transfornm data and currently driving vehicle
/// </summary>
struct MsgPlayerData
{
	MsgPlayerData() : m_dwMessageType(LittleDWord(k_EMsgPlayerTransform)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetPlayer(CSteamID steamid, td::Vec3 position, td::Vec4 rotation, td::Vec3 camPosition, td::Vec4 camRotation, MsgVehicle vehicle, float hp, bool ctrl, const char* heldItemName)
	{
		id = LittleQWord(steamid.ConvertToUint64());
		pos = position;
		rot = rotation;
		camPos = camPosition;
		camRot = camRotation;
		curVeh = vehicle;
		this->hp = hp;
		this->ctrl = ctrl;
		strncpy(heldItem, heldItemName, 13);
	}

	void SetPlayer(td::Vec3 position, td::Vec4 rotation, td::Vec3 camPosition, td::Vec4 camRotation)
	{
		pos = position;
		rot = rotation;

		camPos = camPosition;
		camRot = camRotation;

		curVeh.throttle = 0;
		curVeh.steer = 0;
		curVeh.handbrake = 0;
		curVeh.id = 0;

		if (glb::scene->m_CurrentVehicle != 0)
		{
			curVeh.throttle = TDMP::localInputData.W ? 1 : TDMP::localInputData.S ? -1 : 0; // glb::scene->m_CurrentVehicle->m_MoveInput.x
			curVeh.steer = glb::scene->m_CurrentVehicle->m_MoveInput.y;
			curVeh.handbrake = glb::scene->m_CurrentVehicle->m_Handbrake;
			curVeh.id = glb::scene->m_CurrentVehicle->Id;
		}

		hp = glb::player->health;
		strncpy(heldItem, glb::player->heldItemName, 13);

		ctrl = TDMP::localInputData.Ctrl;
	}

	MsgVehicle GetVehicle() { return curVeh; }

	const float GetHealth() const { return hp; }
	const bool GetCtrl() const { return ctrl; }

	const uint64& GetSteamID() const { return LittleQWord(id); }
	const char* GetHeldItem() const { return heldItem; }

	const td::Vec3& GetPosition() const { return pos; }
	const td::Vec4& GetRotation() const { return rot; }
	const td::Vec3& GetCamPosition() const { return camPos; }
	const td::Vec4& GetCamRotation() const { return camRot; }
private:
	const DWORD m_dwMessageType;
	uint64 id;
	td::Vec3 pos;
	td::Vec4 rot;
	td::Vec3 camPos;
	td::Vec4 camRot;
	MsgVehicle curVeh;
	float hp;
	bool ctrl;
	char heldItem[13];
};

struct LuaCallbackQueue
{
	std::string callback;
	std::string json;
};

struct MsgLuaCallback
{
	MsgLuaCallback() : m_dwMessageType(LittleDWord(k_EMsgLuaCallback)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetCallback(const char* callback)
	{
		//callbackName[strlen(callback)];
		//callbackName = callback;
		strncpy(callbackName, callback, strlen(callback) + 1);
		callbackName[strlen(callback) + 1] = '\0';
	}

	void SetJson(const char* js)
	{
		//json[strlen(json)];
		strncpy(json, js, strlen(js) + 1);
		json[strlen(js) + 1] = '\0';
	}

	void SetReliable(bool rel)
	{
		reliable = rel;
	}

	const char* GetJson()
	{
		return json;
	}

	const char* GetCallback()
	{
		return callbackName;
	}

	const bool GetReliable()
	{
		return reliable;
	}
private:
	const DWORD m_dwMessageType;
	bool reliable;
	char callbackName[32];
	char json[1024];
};

struct MsgBody
{
	td::Vec3 pos;
	td::Vec4 rot;
	td::Vec3 vel;
	td::Vec3 rotVel;

	int16_t id;
};

struct MsgUpdateBody {
	MsgUpdateBody() : m_dwMessageType(LittleDWord(k_EMsgServerUpdateBody)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetBody(TDBody* body)
	{
		bodyData = MsgBody{ body->Position, body->Rotation, body->Velocity, body->RotationVelocity, body->Id };
	}

	MsgBody GetBody()
	{
		return bodyData;
	}
private:
	const DWORD m_dwMessageType;

	MsgBody bodyData;
};

// My attempts to send vector with bodies
struct MsgUpdateBodies
{
	MsgUpdateBodies() : m_dwMessageType(LittleDWord(k_EMsgServerUpdateWorld)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void PushBody(int id, MsgBody toPush)
	{
		if (id >= 24)
			return;
		
		size = id + 1;
		bodies[id] = toPush;
	}

	MsgBody* GetBodies()
	{
		return bodies;
	}

	int GetBodiesCount()
	{
		return size;
	}
private:
	const DWORD m_dwMessageType;

	int size;
	MsgBody bodies[24];
};

/*struct SPacketInfo
{
	signed char m_Type;
	int m_Size;
};

struct MsgUpdateBodies {
	void Serialize(std::vector<MsgBody> bodies) {
		char* pBuffer = (char*)std::malloc((bodies.size() * sizeof(MsgBody)) + sizeof(SPacketInfo));
		SPacketInfo PacketInfo;
		PacketInfo.m_Size = (int)bodies.size();

		std::memcpy(pBuffer, &PacketInfo, sizeof(SPacketInfo));

		int i = 0;
		for (MsgBody Body : bodies) {
			i++;
			memcpy((pBuffer + sizeof(SPacketInfo)) + (i * sizeof(MsgBody)), &Body, sizeof(MsgBody));
		}

		buffer = pBuffer;
	};

	void Deserialize() {
		SPacketInfo PacketInfo;
		memcpy(&PacketInfo, buffer, sizeof(SPacketInfo));

		for (int i = 0; i < PacketInfo.m_Size; i++) {
			void* pMsgBodyBuff = malloc(sizeof(MsgBody));
			memcpy(pMsgBodyBuff, (buffer + sizeof(SPacketInfo)) + (i * sizeof(MsgBody)), sizeof(MsgBody));
			m_Bodies.push_back(reinterpret_cast<MsgBody*>(pMsgBodyBuff));
		};
	};

	std::vector<MsgBody*> GetBodies()
	{
		return m_Bodies;
	}
private:
	std::vector<MsgBody*> m_Bodies;
	char* buffer;
};*/

struct MsgServerExiting
{
	MsgServerExiting() : m_dwMessageType(LittleDWord(k_EMsgServerExiting)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }
private:
	const DWORD m_dwMessageType;
};

struct MsgClientExiting
{
	MsgClientExiting() : m_dwMessageType(LittleDWord(k_EMsgClientExiting)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	/// <summary>
	/// Sets the player who disconnected. Doesn't makes any sense on clientside
	/// </summary>
	void SetPlayer(CSteamID steamid)
	{
		id = LittleQWord(steamid.ConvertToUint64());
	}

	const uint64& GetSteamID() const { return LittleQWord(id); }
private:
	const DWORD m_dwMessageType;
	uint64 id;
};

/// <summary>
/// TODO: Vehicle is driven and we need to let all clients know about it
/// </summary>
struct MsgServerVehicleDriven
{
	MsgServerVehicleDriven() : m_dwMessageType(LittleDWord(k_EMsgVehicleDriving)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetVehicle(TDVehicle* veh)
	{
		data.id = veh->Id;

		data.handbrake = veh->m_Handbrake;
		data.steer = veh->m_MoveInput.y;
		data.throttle = veh->m_MoveInput.x;
	}
private:
	const DWORD m_dwMessageType;
	MsgVehicle data;
};

struct MsgServerAuthed
{
	MsgServerAuthed() : m_dwMessageType(LittleDWord(k_EMsgServerAuthed)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }
private:
	const DWORD m_dwMessageType;
};
#pragma pack( pop )

#endif // MESSAGES_H
