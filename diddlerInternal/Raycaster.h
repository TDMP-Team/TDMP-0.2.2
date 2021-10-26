#pragma once
#include "types.h"
#include "Global.h"

namespace raycaster {

	struct VoxelMaterial
	{
		float m_Reflectivity;
		float m_Roughness;
		float m_Metallic;
		float m_Emission;
	};

	struct VoxelColor
	{
		float m_R;
		float m_G;
		float m_B;
		float m_A;
	};

	struct VoxelPaletteInfo
	{
		uint32_t m_Type;
		VoxelColor m_Color;
		VoxelMaterial m_Material;
		uint32_t m_Pad20;
	};

	struct palInfo {
		byte data[128] = {};
	};

	struct rayData {
		bool successful = false;
		float distance;
		td::Vec3 worldPos;
		td::Vec3 angle;
		TDShape* hitShape;
		VoxelPaletteInfo palette = {};
	};

	rayData castRayManual(td::Vec3 position, td::Vec3 rotation, RaycastFilter* filterCus);
	rayData castRayPlayer();

}

