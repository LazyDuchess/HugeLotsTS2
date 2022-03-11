#include "pch.h"
#include "hugelots.h"
#include "LDHooking/hooking.h"
#define LIGHTMAP_RESOLUTION_ADDR 0x6B49FE
#define LIGHTMAP_RESOLUTION_ADDR2 0x604B03
#define TEXEL_MAPPING_X_ADDR 0x6B37D4
#define TEXEL_MAPPING_Y_ADDR 0x6B3888
#define LOTIMPOSTER_RESOLUTION_ADDR 0x607A87
#define LOTIMPOSTER_DELAUNEY_MAPPING_ADDR 0x600480
#define LOTIMPOSTER_RENDERING_COORDS_ADDR 0x604ADF
#define LOTIMPOSTER_RENDERING_COORDS_ADDR2 0x604B03
#define LOTIMPOSTER_SLICERESOLUTION_ADDR 0x5FFD75
#define LOTIMPOSTER_CHAMFER_ADDR 0x5FE21A
#define LIGHTMAP_FLOORING_FLOAT_ADDR 0xDE48EE
#define LOTIMPOSTER_ROOF_MATERIALNAME_ADDR 0xE36954

namespace HugeLots {

	char* lightMapResolutionHookReturn;

	__declspec(naked) void lightMapResolutionHook()
	{
		__asm {
			mov int ptr [esi + 0x44], 0x400
			mov int ptr [esi + 0x4C], 0x200
			jmp lightMapResolutionHookReturn
		}
	}

	char lightMapResolutionHook2[] = { 0xB8, 0x80, 0x00, 0x00, 0x00 };

	char texelMappingHook[] = { 0xC1, 0xE9, 0x07 };

	char* lotImposterResolutionHookReturn;

	__declspec(naked) void lotImposterResolutionHook()
	{
		__asm {
			mov int ptr [esi + 0x9C], 0x80
			mov int ptr [esi + 0xA0], 0x100
			jmp lotImposterResolutionHookReturn
		}
	}

	char delauneyMappingHook[] = { 0xC7, 0x44, 0x24, 0x60, 0x00, 0x00, 0x00, 0x3C, 0xC7, 0x44, 0x24, 0x64, 0x00, 0x00, 0x00, 0x3C };

	char lotImposterRenderingHook[] = { 0x68, 0x00, 0x00, 0x80, 0x42, 0x68, 0x00, 0x00, 0x80, 0xC2, 0x68, 0x00, 0x00, 0x80, 0x42, 0x68, 0x00, 0x00, 0x80, 0xC2 };

	char lotImposterRenderingHook2[] = { 0xC7, 0x44, 0x24, 0x74, 0x00, 0x00, 0x80, 0x42, 0xC7, 0x44, 0x24, 0x78, 0x00, 0x00, 0x80, 0x42 };

	char lotImposterSliceResolutionHook[] = { 0xC7, 0x44, 0x24, 0x24, 0x00, 0x00, 0x00, 0x43, 0xC7, 0x44, 0x24, 0x28, 0x00, 0x00, 0x00, 0x43 };

	char lotImposterChamferHook[] = { 0xE9, 0x09, 0x01, 0x00, 0x00, 0x00 };

	char lightMapFlooringHook[] = { 0x00 };

	//Make a separate material for 128x128 roofs, so that vanilla imposters still look fine.
	char lotImposterRoofMaterialName[] = "ImposterRoofMaterialCS";

	void Run() {
		HMODULE module = GetModuleHandleA(NULL);
		char* modBase = (char*)module;
		lightMapResolutionHookReturn = modBase + LIGHTMAP_RESOLUTION_ADDR + 6;
		Hooking::MakeJMP((BYTE*)(modBase + LIGHTMAP_RESOLUTION_ADDR), (DWORD)lightMapResolutionHook,6);

		Hooking::WriteToMemory((DWORD)(modBase + LIGHTMAP_RESOLUTION_ADDR2), lightMapResolutionHook2, sizeof(lightMapResolutionHook2) / sizeof(*lightMapResolutionHook2));
		Hooking::WriteToMemory((DWORD)(modBase + TEXEL_MAPPING_X_ADDR), texelMappingHook, sizeof(texelMappingHook) / sizeof(*texelMappingHook));
		Hooking::WriteToMemory((DWORD)(modBase + TEXEL_MAPPING_Y_ADDR), texelMappingHook, sizeof(texelMappingHook) / sizeof(*texelMappingHook));

		lotImposterResolutionHookReturn = modBase + LOTIMPOSTER_RESOLUTION_ADDR + 12;
		Hooking::MakeJMP((BYTE*)(modBase + LOTIMPOSTER_RESOLUTION_ADDR), (DWORD)lotImposterResolutionHook, 12);

		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_DELAUNEY_MAPPING_ADDR), delauneyMappingHook, sizeof(delauneyMappingHook) / sizeof(*delauneyMappingHook));
		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_RENDERING_COORDS_ADDR), lotImposterRenderingHook, sizeof(lotImposterRenderingHook) / sizeof(*lotImposterRenderingHook));
		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_RENDERING_COORDS_ADDR2), lotImposterRenderingHook2, sizeof(lotImposterRenderingHook2) / sizeof(*lotImposterRenderingHook2));
		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_SLICERESOLUTION_ADDR), lotImposterSliceResolutionHook, sizeof(lotImposterSliceResolutionHook) / sizeof(*lotImposterSliceResolutionHook));
		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_CHAMFER_ADDR), lotImposterChamferHook, sizeof(lotImposterChamferHook) / sizeof(*lotImposterChamferHook));
		Hooking::WriteToMemory((DWORD)(modBase + LIGHTMAP_FLOORING_FLOAT_ADDR), lightMapFlooringHook, sizeof(lightMapFlooringHook) / sizeof(*lightMapFlooringHook));

		Hooking::WriteToMemory((DWORD)(modBase + LOTIMPOSTER_ROOF_MATERIALNAME_ADDR), lotImposterRoofMaterialName, sizeof(lotImposterRoofMaterialName) / sizeof(*lotImposterRoofMaterialName));
	}
}