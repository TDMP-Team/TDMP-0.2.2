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

	// Player data
	k_EMsgPlayerTransform = k_EMsgServerBegin + 20,
	k_EMsgPlayerInput = k_EMsgServerBegin + 21,

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
/// Contains transfornm data, currently driving vehicle and current hold object
/// </summary>
struct MsgPlayerData
{
	MsgPlayerData() : m_dwMessageType(LittleDWord(k_EMsgPlayerTransform)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void SetPlayer(CSteamID steamid, td::Vec3 position, td::Vec4 rotation, MsgVehicle vehicle)
	{
		id = LittleQWord(steamid.ConvertToUint64());
		pos = position;
		rot = rotation;
		curVeh = vehicle;
	}

	void SetPlayer(td::Vec3 position, td::Vec4 rotation)
	{
		pos = position;
		rot = rotation;

		curVeh.throttle = 0;
		curVeh.steer = 0;
		curVeh.handbrake = 0;
		curVeh.id = 0;

		if (glb::scene->m_CurrentVehicle != 0)
		{
			curVeh.throttle = glb::scene->m_CurrentVehicle->m_MoveInput.x;
			curVeh.steer = glb::scene->m_CurrentVehicle->m_MoveInput.y;
			curVeh.handbrake = glb::scene->m_CurrentVehicle->m_Handbrake;
			curVeh.id = glb::scene->m_CurrentVehicle->Id;
		}
	};

	MsgVehicle GetVehicle() { return curVeh; }
	const uint64& GetSteamID() const { return LittleQWord(id); }
	const td::Vec3& GetPosition() const { return pos; }
	const td::Vec4& GetRotation() const { return rot; }
private:
	const DWORD m_dwMessageType;
	uint64 id;
	td::Vec3 pos;
	td::Vec4 rot;
	MsgVehicle curVeh;
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

/*struct MsgUpdateBodies
{
	MsgUpdateBodies() : m_dwMessageType(LittleDWord(k_EMsgServerUpdateWorld)) {}
	DWORD GetMessageType() { return LittleDWord(m_dwMessageType); }

	void PushBodies(std::vector<MsgBody> toPush)
	{
		size = toPush.size();
		void* pBuffer = std::malloc(size * sizeof(MsgBody));

		int i = 0;
		for (MsgBody Body : toPush) {
			i++;
			memcpy(pBuffer + (i * sizeof(MsgBody)), &Body, sizeof(MsgBody));
		}

		bodies = pBuffer;
	}

	std::vector<MsgBody*> GetBodies()
	{
		std::vector<MsgBody*> Bodies;

		//memcpy(&PacketInfo, bodies, sizeof(SPacketInfo));

		for (int i = 0; i < size; i++) {
			void* pMsgBodyBuff = malloc(sizeof(MsgBody));
			memcpy(pMsgBodyBuff, (bodies) + (i * sizeof(MsgBody)), sizeof(MsgBody));
			Bodies.push_back(reinterpret_cast<MsgBody*>(pMsgBodyBuff));
		};

		return Bodies;
	}

	size_t GetBodiesCount()
	{
		return size;
	}
private:
	const DWORD m_dwMessageType;

	size_t size;
	void* bodies;
};*/
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
