#include <iostream>
#include <cmath>

// Imgui includes
#include "includes.hpp"
#include "imgui/imgui.h"

// Hack includes
#include "hacks.hpp"
#include "entity.hpp"
#include "offsets.hpp"


#define DEBUG


using namespace offsets;

// Some ImGui variables
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int screen_height = GetSystemMetrics(SM_CYSCREEN);
int screen_width  = GetSystemMetrics(SM_CXSCREEN);

ImVec2 window_size = { 125, 140 };
ImVec2 window_pos  = { 10, 10 };

static HWND window = NULL;
EndScene oEndScene = NULL;
HANDLE game_module;
WNDPROC oWndProc;

// Hack variables
bool bCheatsEnabled	= false;
bool bMainWindow	= true;
bool bLastPass		= false;
bool bInGame		= false;
bool bInit			= false;

uintptr_t module_base_addr = (uintptr_t)GetModuleHandle(NULL);
EntityList* entity_list;

float x_cam, y_cam, z_cam;
int current_entity = 0;


void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
#ifdef DEBUG
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}


long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!bInit)
	{
		InitImGui(pDevice);
		bInit = true;
	}

	if (entity_list)
	{
		bInGame = entity_list->ingame;
	}

	if (!bInGame)
	{
		current_entity = 0;
		x_cam = 0;
		y_cam = 0;
		z_cam = 0;
	}

	// Update Player position in GUI
	if (bCheatsEnabled && bInGame)
	{
		Hacks::UpdatePlayerCamCoords(XYZ_BASE_ADDR_CAM, XYZ_OFFSETS_CAM, &x_cam, &y_cam, &z_cam);
	}

	// Toggle GUI window
	if ((GetAsyncKeyState(VK_F2) & 1) > 0)
	{
		bMainWindow = !(bMainWindow);
	}

	// Toggle God mode
	if ((GetAsyncKeyState(VK_TAB) & 1) > 0)
	{
		// Update Entity List
		entity_list = *(EntityList**)(module_base_addr + 0x41F83C);

		bCheatsEnabled = !bCheatsEnabled;
		Hacks::GodMode(bCheatsEnabled);
#ifdef DEBUG
		std::cout << "Godmode: " << (bCheatsEnabled ? "Enabled." : "Disabled.") << std::endl;
#endif
	}

	// Previous Entity
	if ((GetAsyncKeyState(VK_OEM_4) & 1) > 0 && bCheatsEnabled && bInGame)
	{
		Hacks::SetCurrentEntity(&current_entity, -1);
#ifdef DEBUG
		std::cout << "Decrementing entity..." << std::endl;
#endif
	}

	// Next Entity
	if ((GetAsyncKeyState(VK_OEM_6) & 1) > 0 && bCheatsEnabled && bInGame)
	{
		Hacks::SetCurrentEntity(&current_entity, 1);
#ifdef DEBUG
		std::cout << "Incrementing entity..." << std::endl;
#endif
	}

	// Teleport to entity
	if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('T') & 1) > 0 && bCheatsEnabled && bInGame)
	{
		Hacks::TeleportToEntity(XYZ_BASE_ADDR_PLAYER, XYZ_OFFSETS_PLAYER, current_entity);
#ifdef DEBUG
		std::cout << "Teleporting to entity[" << current_entity << "]..." << std::endl;
#endif
	}

	// Teleport to camera location
	if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('T') & 1) > 0 && bCheatsEnabled && bInGame)
	{
		Hacks::TeleportToCam(XYZ_BASE_ADDR_PLAYER, XYZ_OFFSETS_PLAYER, x_cam, y_cam, z_cam);
#ifdef DEBUG
		std::cout << "Teleporting to: ";
		std::cout << "x: " << x_cam << " ";
		std::cout << "y: " << y_cam << " ";
		std::cout << "z: " << z_cam << " " << std::endl;
#endif
	}

	// Disable all cheats before ejecting
	if (bLastPass)
	{
		kiero::shutdown();
#ifdef f
		fclose(f);
		FreeConsole();
#endif
	}

	// Initiate last pass
	if ((GetAsyncKeyState(VK_END) & 1) > 0)
	{
		bCheatsEnabled = false;
		bMainWindow	   = false;
		bLastPass      = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bMainWindow)
	{
		ImGui::Begin("Godmode", &bInit, ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(window_pos);
		ImGui::SetWindowSize(window_size);
		ImGui::Checkbox("Active", &bCheatsEnabled);
		ImGui::Text("Enemy: %d", current_entity);
		ImGui::Text("Cam Coordinates");
		if (bCheatsEnabled && bInGame && (x_cam && y_cam && z_cam))
		{
			ImGui::Text("x: %.1f", x_cam);
			ImGui::Text("y: %.1f", y_cam);
			ImGui::Text("z: %.1f", z_cam);
		}
		else
		{
			ImGui::Text("x: ??");
			ImGui::Text("y: ??");
			ImGui::Text("z: ??");
		}

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}


HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}


DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
