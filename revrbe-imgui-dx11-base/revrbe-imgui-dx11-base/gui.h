#ifndef GUI_H
#define GUI_H
#pragma once
#include <d3d11.h>
#include <memory>
#include <chrono>

#include "imgui.h"
#include "magiXOR.h"
#include "imgui_internal.h"

inline ID3D11ShaderResourceView* logoImage = nullptr;

class GUI final
{
public:
	void Draw(HWND hwnd);
	void DrawLoginInterface(HWND hwnd);
	void DrawInterface(HWND hwnd);
	void ApplyColorScheme(const char* colorScheme);

	enum class ColorScheme {
		Dark,
		Light,
		Classic
	};

	const ImVec2 window_pos{ 0, 0 };
	const ImVec2 window_size{ 400, 300 };
	const ImVec2 background{ 401, 301 };

	bool loginFailed = false;
	bool drawLogin = true;
	bool drawInterface = false;

private:
	bool active = true;
	char user_name[255] = "";
	char pass_word[255] = "";

	bool isLoggedIn = false;
};

inline const char* colorSchemes[] = { "Standard", "Cinder", "Green colorway", "Blue colorway", "Purple/orange colorway", "Dark mode", "Pink/green/blue colorway", "Blue and grey colorway", "Boring colorway", "Blue/green colorway", "Green/yellow colorway", "Orange/grey colorway", "Whitey", "Colorful" };

inline std::unique_ptr<GUI> gui = std::make_unique<GUI>();

#endif
