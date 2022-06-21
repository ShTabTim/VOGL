#pragma once
#include <Windows.h>
#include <cstdint>

typedef struct k_stat {
	bool pressed;
	bool released;
	bool held;
}k_stat;

void key_loop(HWND hwnd);
k_stat get_key(uint32_t id);
void set_cursor(uint32_t x, uint32_t y);
int32_t get_Mx();
int32_t get_My();
int32_t get_dMx();
int32_t get_dMy();