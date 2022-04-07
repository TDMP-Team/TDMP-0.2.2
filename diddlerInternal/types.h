#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "memhandler.h"
#include "td_memory.h"


namespace td {

	struct Vec2 {
		float x;
		float y;
	};

	struct Vec3 {
		float x;
		float y;
		float z;
	};

	struct Vec4 {
		float w;
		float x;
		float y;
		float z;
	};

	struct Transform {
		td::Vec3 position;
		td::Vec4 rotation;
	};

	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	const Color whiteColor{ 1.f, 1.f, 1.f, 1.f };
	const Color redColor{ 1.f, 0.f, 0.f, 1.f };
	const Color greenColor{ 0.f, 1.f, 0.f, 1.f };
	const Color blueColor{ 0.f, 0.f, 1.f, 1.f };

	struct particleInfo {
		float U01 = 0.f;
		float U02 = 0.f;
		float U03 = 0.f;
		float U04 = 0.f;
		float U05 = 0.f;
		float U06 = 0.f;
		float U07 = 0.f;
		float U08 = 0.f;
		float U09 = 0.f;
		float U10 = 0.f;
		float U11 = 0.f;
		float U12 = 0.f;
		float U13 = 0.f;
		float U14 = 0.f;
		float U15 = 0.f;
		float U16 = 0.f;
		float U17 = 0.f;
		float U18 = 0.f;
		float U19 = 0.f;
		float U20 = 0.f;
		float U21 = 0.f;
		float U22 = 0.f;
		float U23 = 0.f;
		float U24 = 0.f;
		float U25 = 0.f;
		float U26 = 0.f;
		float U27 = 0.f;
		float U28 = 0.f;
		float U29 = 0.f;
		float U30 = 0.f;
		float U31 = 0.f;
		float U32 = 0.f;
		float U33 = 0.f;
		float U34 = 0.f;
		float U35 = 0.f;
		float U36 = 0.f;
		float U37 = 0.f;
		float U38 = 0.f;
		float U39 = 0.f;
		float U40 = 0.f;
		float U41 = 0.f;
		float U42 = 0.f;
		float U43 = 0.f;
		float U44 = 0.f;
		float U45 = 0.f;
		float U46 = 0.f;
		float U47 = 0.f;
		float U48 = 0.f;
		float U49 = 0.f;
		float U50 = 0.f;
		float U51 = 0.f;
		float U52 = 0.f;
		float U53 = 0.f;
		float U54 = 0.f;
		float U55 = 0.f;
		float U56 = 0.f;
		float U57 = 0.f;
		float U58 = 0.f;
		float U59 = 0.f;
		float U60 = 0.f;
		float U61 = 0.f;
		float U62 = 0.f;
		float U63 = 0.f;
		float U64 = 0.f;
	};

	namespace {
		template <typename T>
		struct Vector2 {
			T m_X; T m_Y;
		};

		template <typename T>
		struct Vector3 {
			T m_X; T m_Y; T m_Z;
		};

		template <typename T>
		struct Vector4 {
			T m_X; T m_Y; T m_Z; T m_W;
		};
	}

	using Vector3_u8 = Vector3<uint8_t>;
	using Vector3_u16 = Vector3<uint16_t>;

	using Vector2_i32 = Vector2<int32_t>;
	using Vector3_i32 = Vector3<int32_t>;
	using Vector4_i32 = Vector4<int32_t>;

	using Vector2_u32 = Vector2<uint32_t>;
	using Vector3_u32 = Vector3<uint32_t>;

	using Vector2_f32 = Vector2<float>;
	using Vector3_f32 = Vector3<float>;
	using Vector4_f32 = Vector4<float>;

	struct Matrix4x4 {
		float m[4][4];
	};

	// Misuse of these will cause crashes if used improperly!
	// I'm forcing you to initialize them somewhere inside of your code to ensure correct usage.
	// You can find Teardown's instances of malloc and free using signature scanning. If you are exceptionally lazy you can initialize these using pointers to <cstdlib.h> with &malloc and &free.
	// Be aware, if you pass data to Teardown using the incorrect malloc then you will crash, trigger asserts, and leak memory. So don't be lazy; use the signatures in <teardown/game.h>!

	/*
	 * Helper for creating an object of a given type.
	 */
	template<class T>
	T* MemoryNew()
	{
		return static_cast<T*>((void*)containedMalloc(sizeof(T)));
	}

	/*
	 * Helper for creating an array of objects of a given type.
	 */
	template<class T>
	T* MemoryNew(size_t size)
	{
		return static_cast<T*>((void*)containedMalloc(sizeof(T) * size));
	}

	/*
	 * Helper class for interacting with Teardown strings.
	 *
	 * This string is only meant to be used to forward and receive data from the game.
	 *
	 * It does not support:
	 *     Copying and assignment
	 *     Modifications
	 *
	 * You are welcome to submit pull requests for these if you wish.
	 */
	
	/*
	 * Helper class for interacting with Teardown vectors.
	 *
	 * This vector is only meant to be used to forward and receive data from the game.
	 *
	 * It does not support:
	 *     Copying and assignment
	 *     Non-trivially constructible types
	 *     Deletion
	 *
	 * You are welcome to submit pull requests for these if you wish.
	 */
	template<typename T>
	class small_vector {
	public:
		small_vector() {};
		small_vector(uint32_t capacity) { reserve(capacity); };
		small_vector(const small_vector&) = delete;
		void operator=(const small_vector&) = delete;
		~small_vector() { clear(); }

		T* begin() const { return m_Data; }
		T* end() const { return m_Data + m_Size; }
		T* data() const { return m_Data; }
		uint32_t size() const { return m_Size; }
		uint32_t capacity() const { return m_Capacity; }

		void push_back(const T& value)
		{
			if (m_Capacity <= m_Size)
			{
				reserve(2 * m_Capacity + 1);
			}

			m_Data[m_Size] = value;
			++m_Size;
		}

		void reserve(uint32_t capacity) {
			if (auto memory = MemoryNew<T>(capacity))
			{
				T* data = m_Data;
				uint32_t size = m_Size;

				if (data != nullptr)
				{
					if (size >= capacity)
					{
						size = capacity;
					}
					else
					{
						memset(memory, sizeof(T) * size, sizeof(T) * capacity - size);
					}

					memcpy(m_Data, data, sizeof(T) * size);
					containedFree(data);
					m_Size = size;
				}
				else
				{
					m_Size = 0;
					memset(memory, 0, sizeof(T) * capacity);
				}

				m_Data = memory;
				m_Capacity = capacity;
			}
		}

		void clear()
		{
			if (m_Data != nullptr)
			{
				m_Size = 0;
				m_Capacity = 0;
				containedFree(m_Data);
				m_Data = nullptr;
			}
		}

	private:
		uint32_t m_Size = 0;
		uint32_t m_Capacity = 0;
		T* m_Data = nullptr;
	};

	/*
	 * Represents a known good function signature.
	 *
	 * Example usage:
	 * decltype(Teardown::Game::Update)::Type Game_Update = FindSignature(Teardown::Game.Signature);
	 *
	 * FindSignature is a user defined function accepting a const* char which scans the main Teardown.exe module.
	 *
	 * These signatures do not have a "mask" on purpose. I use (*) \x2A to represent wildcards. This maps to an unused instruction in x86/x64.
	 */
	template<typename T>
	struct function_signature
	{
		/// You can retrieve the function type using decltype(Teardown::Game::Update)::Type.
		using Type = T;
		/// You can retrieve the function sigature using Teardown::Game::Update.Signature. Signatures use "\x2A" to represent wild cards.
		const char* Signature = "";
	};

	static_assert(sizeof(small_string) == 16, "small_string size is incorrect!");
	static_assert(sizeof(small_vector<uint8_t>) == 16, "small_vector size is incorrect!");
};
