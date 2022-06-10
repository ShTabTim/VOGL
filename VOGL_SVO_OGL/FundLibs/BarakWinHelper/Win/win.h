#pragma once
#include <Windows.h>
#include <cinttypes>

WNDCLASSEXW create_wc(HINSTANCE hInst, LPCWSTR pClassName, WNDPROC fWndProc);

typedef struct fwind {
protected:
	LPCWSTR name;
	HWND hWnd;
	//uint32_t x = 0, y = 0, w = 256, h = 256;
	uint32_t x = CW_USEDEFAULT, y = CW_USEDEFAULT, w = 256, h = 256;
	WNDCLASSEXW wc;
public:
	fwind();
	fwind(std::nullptr_t);
	
	void init(WNDCLASSEXW wc, LPCWSTR name = L"Windows window", DWORD dwStyle = WS_OVERLAPPEDWINDOW, HWND parent = nullptr);

	uint32_t get_x() const;
	uint32_t get_y() const;
	uint32_t get_w() const;
	uint32_t get_h() const;

	void set_x(uint32_t x);
	void set_y(uint32_t y);
	void set_w(uint32_t w);
	void set_h(uint32_t h);

	HWND get_hwnd() const;
	HDC  get_hdc() const;
	LPCWSTR get_name() const;

	void resize();
	void show(bool Maximized);
	void rename(LPCWSTR name);
}fwind;