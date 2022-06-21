#include "win.h"

WNDCLASSEXW create_wc(HINSTANCE hInst, LPCWSTR pClassName, WNDPROC fWndProc) {
	WNDCLASSEXW wc;

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = fWndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = pClassName;

	RegisterClassEx(&wc);
	return wc;
}

//fwind//

fwind::fwind() {}
fwind::fwind(std::nullptr_t) { hWnd = nullptr; }

void fwind::init(WNDCLASSEXW wc, LPCWSTR name, DWORD dwStyle, HWND parent) {
	this->wc = wc;
	this->name = name;
	this->hWnd = CreateWindow(
		wc.lpszClassName,
		name,
		dwStyle,
		x, y, w, h,
		parent, nullptr, wc.hInstance, nullptr
	);
}

uint32_t fwind::get_x() const { return x; }
uint32_t fwind::get_y() const { return y; }
uint32_t fwind::get_w() const { return w; }
uint32_t fwind::get_h() const { return h; }

void fwind::set_x(uint32_t x) { this->x = x; }
void fwind::set_y(uint32_t y) { this->y = y; }
void fwind::set_w(uint32_t w) { this->w = w; }
void fwind::set_h(uint32_t h) { this->h = h; }

HWND fwind::get_hwnd() const { return hWnd; }
HDC  fwind::get_hdc() const { return GetDC(hWnd); }
LPCWSTR fwind::get_name() const{ return name; }

void fwind::resize() {
	RECT wRect, cRect;
	GetWindowRect(hWnd, &wRect);
	GetClientRect(hWnd, &cRect);

	x = wRect.left;
	y = wRect.top;
	w = cRect.right;
	h = cRect.bottom;
}
void fwind::show(bool Maximized) {
	RECT wRect, cRect;
	
	GetWindowRect(hWnd, &wRect);
	GetClientRect(hWnd, &cRect);
	
	wRect.right  += w - cRect.right - wRect.left;
	wRect.bottom += h - cRect.bottom - wRect.top;
	
	MoveWindow(hWnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);
	
	ShowWindow(hWnd, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}
void fwind::rename(LPCWSTR name) {
	this->name = name;
}