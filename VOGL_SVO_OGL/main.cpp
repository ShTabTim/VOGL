#include "FundLibs/vogl/vogl.h"
#include "FundLibs/vogl/ogl_svo.h"
#include "FundLibs/vogl/ogl_var.h"
#include "FundLibs/sh_win/keys.h"
#include "FundLibs/sh_win/win.h"
#include "FundLibs/noise/Simplex.h"
#include <chrono>

fwind gr_win;

ogl_svo g_svo;
ogl_var g_var;

float vel[3] = { 0, 0, 0 };
float pos[3] = { 0, 0, 0 };
float ang[3] = { 0, 0, 0 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		gr_win.resize();
		break;
	case WM_PAINT:
///////////////////////////////////////////////////////////////////////////////////////////////////////////

		//>>set dt 
		static std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		static std::chrono::system_clock::time_point tp2 = tp1;
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> dTime = tp2 - tp1;
		tp1 = tp2;
		float dt = dTime.count();
		//<<set dt 

		//>>print fps
		wchar_t s[64];
		swprintf_s(s, 64, L"%s : %5.1f fps", gr_win.get_name(), 1.0f / dt);
		SetWindowText(gr_win.get_hwnd(), s);
		//<<print fps

		//>>key zaloop
		key_loop(gr_win.get_hwnd());
		//<<key zaloop

		//>>keys handling
		if (get_key(VK_ESCAPE).held)
			PostQuitMessage(0);

		ang[0] += (get_key(VK_RIGHT).held - get_key(VK_LEFT).held) * dt;
		ang[1] += ((get_key(VK_UP).held && ang[1] < 1.57079632f) - (get_key(VK_DOWN).held && ang[1] > -1.57079632f)) * dt;

		ang[0] += 6.2832853f * ((ang[0] < 3.14159265f) - (ang[0] > -3.14159265f));

		vel[0] = get_key('D').held - get_key('A').held;
		vel[1] = get_key(VK_SPACE).held - get_key(VK_SHIFT).held;
		vel[2] = get_key('W').held - get_key('S').held;

		pos[0] += dt * (vel[0] * cos(ang[0]) + vel[2] * sin(ang[0]));
		pos[1] += dt * vel[1];
		pos[2] += dt * (vel[2] * cos(ang[0]) - vel[0] * sin(ang[0]));
		//<<keys handling

		//>>set vogl
		vogl_set_cam_up(gr_win.get_h());

		vogl_set_cam_pos(pos[0], pos[1], pos[2]);
		vogl_set_cam_ang(ang[0], ang[1], ang[2]);
		//<<set vogl

		//>>draw voxels
		//>>draw svo
		vogl_set_vox_buf_data(0, g_svo.get_buf_size(), g_svo.get_buf_data());
		vogl_set_mod_size(1);
		vogl_set_mod_pos(1, 0, 1);
		vogl_update_cam();
		vogl_draw_svo();
		//<<draw svo

		//>>draw var
		vogl_set_vox_buf_data(0, g_var.get_buf_size(), g_var.get_buf_data());
		vogl_set_mod_size(1.0f / 64.0f);
		vogl_set_mod_pos(0, 0, 0);
		vogl_update_cam();
		vogl_draw_var();
		//<<draw var
		//>>draw voxels
		vogl_end_draw();

///////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
	gr_win.set_w(100*8);
	gr_win.set_h(80*8);
	gr_win.init(create_wc(hInst, L"VOGL_SVO_OGL", WndProc), L"VOGL");

	gr_win.show(0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//>>init vogl
	vogl_start_init(gr_win.get_hdc());
	vogl_set_size(gr_win.get_w(), gr_win.get_h());
	//>>set using
	vogl_use_var();
	vogl_use_svo();
	//<<set using
	vogl_end_init();
	//<<init vogl
	//>>init voxels
	SimplexNoise nois;
	//<<init svo
	g_svo.set_edit_layer(5);
	g_svo.init();
	for (uint32_t g(2 << g_svo.get_edit_layer()); g--;)
		for (uint32_t f(2 << g_svo.get_edit_layer()); f--;)
			for (uint32_t d(2 << g_svo.get_edit_layer()); d--;)
				if ((nois.fractal(4, g / 64.0f, f / 64.0f, d / 64.0f) + 1) / 2 * (2 << g_svo.get_edit_layer()) > f)
					g_svo.set_vox(g, f, d, ogl_svo_node(
						(uint32_t((nois.noise(g / 64.0f + 0, f / 64.0f + 0, d / 64.0f + 0) + 1) / 2 * 0xFF) << 0) +
						(uint32_t((nois.noise(g / 64.0f + 1, f / 64.0f + 1, d / 64.0f + 1) + 1) / 2 * 0xFF) << 8) +
						(uint32_t((nois.noise(g / 64.0f + 2, f / 64.0f + 2, d / 64.0f + 2) + 1) / 2 * 0xFF) << 16)
					));
	g_svo.optimize_voxel_buffer();
	//>>init svo

	//<<init var
	g_var.set_whd(64, 256, 64);
	g_var.init();
	for (uint32_t g(g_var.get_w()); g--;)
		for (uint32_t f(g_var.get_h()); f--;)
			for (uint32_t d(g_var.get_d()); d--;)
				if ((nois.noise(g / 64.0f, f / 64.0f, d / 64.0f) + 1) / 2 * g_var.get_h() > f)
					g_var.set_vox(g, f, d,
						(uint32_t((nois.noise(g / 64.0f + 0, f / 64.0f + 0, d / 64.0f + 0) + 1) / 2 * 0xFF) << 0) +
						(uint32_t((nois.noise(g / 64.0f + 1, f / 64.0f + 1, d / 64.0f + 1) + 1) / 2 * 0xFF) << 8) +
						(uint32_t((nois.noise(g / 64.0f + 2, f / 64.0f + 2, d / 64.0f + 2) + 1) / 2 * 0xFF) << 16)
					);
	//>>init var
	//<<init voxels

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	MSG msg;
	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		RedrawWindow(gr_win.get_hwnd(), nullptr, nullptr, RDW_INTERNALPAINT);
	} while (msg.message != WM_QUIT);
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	vogl_finit(gr_win.get_hwnd());

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	return msg.wParam;
}