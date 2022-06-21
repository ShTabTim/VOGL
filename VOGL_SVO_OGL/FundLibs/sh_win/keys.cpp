#include "Keys.h"

int16_t m_keyOldState[256] = { 0 };
int16_t m_keyNewState[256] = { 0 };
k_stat m_keys[256];
POINT pt = { 0, 0 };
POINT ptOld = { 0, 0 };
POINT START = { 0, 0 };

void get_cur_pos(POINT& pptt) {
	GetCursorPos(&pptt);
	pptt.x = pptt.x - START.x;
	pptt.y = -pptt.y - START.y;
}

void key_loop(HWND hwnd) {
	RECT wRect, cRect;
	GetWindowRect(hwnd, &wRect);
	GetClientRect(hwnd, &cRect);
	START.x = 0.5f * (wRect.right + wRect.left - cRect.right);
	START.y = 0.5f * (wRect.bottom - wRect.top - cRect.bottom) - wRect.bottom;
	for (uint32_t i(256);i--;) {
		m_keyNewState[i] = GetAsyncKeyState(i);

		m_keys[i].pressed = false;
		m_keys[i].released = false;

		if (m_keyNewState[i] != m_keyOldState[i])
			if (m_keyNewState[i] & 0x8000) {
				m_keys[i].pressed = !m_keys[i].held;
				m_keys[i].held = true;
			} else {
				m_keys[i].released = true;
				m_keys[i].held = false;
			}

		m_keyOldState[i] = m_keyNewState[i];
	}
	ptOld = pt;
	get_cur_pos(pt);
}
k_stat get_key(uint32_t id) { return m_keys[id]; }
void set_cursor(uint32_t x, uint32_t y) {
	SetCursorPos(int32_t(x) + START.x, -(int32_t(y) + START.y));
	get_cur_pos(pt);
}
int32_t get_Mx() { return pt.x; }
int32_t get_My() { return pt.y; }
int32_t get_dMx() { return pt.x - ptOld.x; }
int32_t get_dMy() { return pt.y - ptOld.y; }