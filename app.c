// wengwengweng

#define D_IMPL
#define D_CPU
#include <dirty/d_plat.h>
#include <dirty/d_math.h>
#include <dirty/d_app.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include "utils.h"

static luaL_Enum l_mouse_map[] = {
	{ "left", D_MOUSE_LEFT, },
	{ "right", D_MOUSE_RIGHT, },
	{ "middle", D_MOUSE_MIDDLE, },
	{ NULL, 0, },
};

static luaL_Enum l_key_map[] = {
	{ "q", D_KEY_Q, },
	{ "w", D_KEY_W, },
	{ "e", D_KEY_E, },
	{ "r", D_KEY_R, },
	{ "t", D_KEY_T, },
	{ "y", D_KEY_Y, },
	{ "u", D_KEY_U, },
	{ "i", D_KEY_I, },
	{ "o", D_KEY_O, },
	{ "p", D_KEY_P, },
	{ "a", D_KEY_A, },
	{ "s", D_KEY_S, },
	{ "d", D_KEY_D, },
	{ "f", D_KEY_F, },
	{ "g", D_KEY_G, },
	{ "h", D_KEY_H, },
	{ "j", D_KEY_J, },
	{ "k", D_KEY_K, },
	{ "l", D_KEY_L, },
	{ "z", D_KEY_Z, },
	{ "x", D_KEY_X, },
	{ "c", D_KEY_C, },
	{ "v", D_KEY_V, },
	{ "b", D_KEY_B, },
	{ "n", D_KEY_N, },
	{ "m", D_KEY_M, },
	{ "1", D_KEY_1, },
	{ "2", D_KEY_2, },
	{ "3", D_KEY_3, },
	{ "4", D_KEY_4, },
	{ "5", D_KEY_5, },
	{ "6", D_KEY_6, },
	{ "7", D_KEY_7, },
	{ "8", D_KEY_8, },
	{ "9", D_KEY_9, },
	{ "0", D_KEY_0, },
	{ "f1", D_KEY_F1, },
	{ "f2", D_KEY_F2, },
	{ "f3", D_KEY_F3, },
	{ "f4", D_KEY_F4, },
	{ "f5", D_KEY_F5, },
	{ "f6", D_KEY_F6, },
	{ "f7", D_KEY_F7, },
	{ "f8", D_KEY_F8, },
	{ "f9", D_KEY_F9, },
	{ "f10", D_KEY_F10, },
	{ "f11", D_KEY_F11, },
	{ "f12", D_KEY_F12, },
	{ "-", D_KEY_MINUS, },
	{ "=", D_KEY_EQUAL, },
	{ ",", D_KEY_COMMA, },
	{ ".", D_KEY_PERIOD, },
	{ "`", D_KEY_BACKQUOTE, },
	{ "/", D_KEY_SLASH, },
	{ "\\", D_KEY_BACKSLASH, },
	{ ";", D_KEY_SEMICOLON, },
	{ "'", D_KEY_QUOTE, },
	{ "up", D_KEY_UP, },
	{ "down", D_KEY_DOWN, },
	{ "left", D_KEY_LEFT, },
	{ "right", D_KEY_RIGHT, },
	{ "esc", D_KEY_ESC, },
	{ "tab", D_KEY_TAB, },
	{ " ", D_KEY_SPACE, },
	{ "space", D_KEY_SPACE, },
	{ "bksp", D_KEY_BACKSPACE, },
	{ "backspace", D_KEY_BACKSPACE, },
	{ "enter", D_KEY_ENTER, },
	{ "[", D_KEY_LBRACKET, },
	{ "]", D_KEY_RBRACKET, },
	{ "lshft", D_KEY_LSHIFT, },
	{ "rshft", D_KEY_RSHIFT, },
	{ "lalt", D_KEY_LALT, },
	{ "ralt", D_KEY_RALT, },
	{ "lmeta", D_KEY_LMETA, },
	{ "rmeta", D_KEY_RMETA, },
	{ "lctrl", D_KEY_LCTRL, },
	{ "rctrl", D_KEY_RCTRL, },
	{ NULL, 0, },
};

typedef struct {
	lua_State *lua;
	int init_ref;
	int frame_ref;
	int quit_ref;
	int err_ref;
} l_app_ctx;

static l_app_ctx l_app;

static void l_init_inner() {
	lua_rawgeti(l_app.lua, LUA_REGISTRYINDEX, l_app.init_ref);
	if (lua_isfunction(l_app.lua, -1)) {
		lua_call(l_app.lua, 0, 0);
	}
}

static void l_frame_inner() {
	lua_rawgeti(l_app.lua, LUA_REGISTRYINDEX, l_app.frame_ref);
	if (lua_isfunction(l_app.lua, -1)) {
		lua_call(l_app.lua, 0, 0);
	}
}

static int l_run(lua_State *L) {

	luaL_checktable(L, 1);
	lua_getfield(L, 1, "init");
	l_app.init_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_getfield(L, 1, "frame");
	l_app.frame_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_getfield(L, 1, "title");

	// TODO: luaL_check* does not give meaningful error for table fields
	d_app_run((d_app_desc) {
		.init = l_init_inner,
		.frame = l_frame_inner,
		.title = lua_getfield(L, 1, "title") ? luaL_checkstring(L, -1) : NULL,
		.width = lua_getfield(L, 1, "width") ? luaL_checkinteger(L, -1) : 0,
		.height = lua_getfield(L, 1, "height") ? luaL_checkinteger(L, -1) : 0,
		.vsync = lua_getfield(L, 1, "vsync") ? luaL_checkboolean(L, -1) : true,
		.fullscreen = lua_getfield(L, 1, "fullscreen") ? luaL_checkboolean(L, -1) : false,
	});

	return 0;

}

static int l_quit(lua_State *L) {
	d_app_quit();
	return 0;
}

static int l_time(lua_State *L) {
	lua_pushnumber(L, d_app_time());
	return 1;
}

static int l_dt(lua_State *L) {
	lua_pushnumber(L, d_app_dt());
	return 1;
}

static int l_key_pressed(lua_State *L) {
	const char *k = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_key_pressed(luaL_getenum(L, l_key_map, k)));
	return 1;
}

static int l_key_rpressed(lua_State *L) {
	const char *k = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_key_rpressed(luaL_getenum(L, l_key_map, k)));
	return 1;
}

static int l_key_released(lua_State *L) {
	const char *k = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_key_released(luaL_getenum(L, l_key_map, k)));
	return 1;
}

static int l_key_down(lua_State *L) {
	const char *k = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_key_down(luaL_getenum(L, l_key_map, k)));
	return 1;
}

static int l_mouse_pressed(lua_State *L) {
	const char *m = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_mouse_pressed(luaL_getenum(L, l_mouse_map, m)));
	return 1;
}

static int l_mouse_released(lua_State *L) {
	const char *m = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_mouse_released(luaL_getenum(L, l_mouse_map, m)));
	return 1;
}

static int l_mouse_down(lua_State *L) {
	const char *m = luaL_checkstring(L, 1);
	lua_pushboolean(L, d_app_mouse_down(luaL_getenum(L, l_mouse_map, m)));
	return 1;
}

static int l_mouse_pos(lua_State *L) {
	vec2 mpos = d_app_mouse_pos();
	lua_pushudata(L, vec2, &mpos);
	return 1;
}

static int l_mouse_dpos(lua_State *L) {
	vec2 dpos = d_app_mouse_dpos();
	lua_pushudata(L, vec2, &dpos);
	return 1;
}

static int l_mouse_moved(lua_State *L) {
	lua_pushboolean(L, d_app_mouse_moved());
	return 1;
}

static int l_input(lua_State *L) {
	char c = d_app_input();
	if (c) {
		lua_pushlstring(L, &c, 1);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

static int l_width(lua_State *L) {
	lua_pushinteger(L, d_app_width());
	return 1;
}

static int l_height(lua_State *L) {
	lua_pushinteger(L, d_app_height());
	return 1;
}

static int l_fullscreen(lua_State *L) {
	if (lua_isnoneornil(L, 1)) {
		lua_pushboolean(L, d_app_fullscreen());
		return 1;
	} else {
		bool b = luaL_checkboolean(L, 1);
		d_app_set_fullscreen(b);
		return 0;
	}
}

static int l_mouse_locked(lua_State *L) {
	if (lua_isnoneornil(L, 1)) {
		lua_pushboolean(L, d_app_mouse_locked());
		return 1;
	} else {
		bool b = luaL_checkboolean(L, 1);
		d_app_set_mouse_locked(b);
		return 0;
	}
}

static int l_mouse_hidden(lua_State *L) {
	if (lua_isnoneornil(L, 1)) {
		lua_pushboolean(L, d_app_mouse_hidden());
		return 1;
	} else {
		bool b = luaL_checkboolean(L, 1);
		d_app_set_mouse_hidden(b);
		return 0;
	}
}

static int l_keyboard_shown(lua_State *L) {
	if (lua_isnoneornil(L, 1)) {
		lua_pushboolean(L, d_app_keyboard_shown());
		return 1;
	} else {
		bool b = luaL_checkboolean(L, 1);
		d_app_set_keyboard_shown(b);
		return 0;
	}
}

static int l_title(lua_State *L) {
	if (lua_isnoneornil(L, 1)) {
		return 0;
// 		lua_pushstring(L, d_app_title());
// 		return 1;
	} else {
		const char *title = luaL_checkstring(L, 1);
		d_app_set_title(title);
		return 0;
	}
}

void l_app_quit(lua_State *L) {
	luaL_unref(L, LUA_REGISTRYINDEX, l_app.init_ref);
	luaL_unref(L, LUA_REGISTRYINDEX, l_app.frame_ref);
	luaL_unref(L, LUA_REGISTRYINDEX, l_app.quit_ref);
	luaL_unref(L, LUA_REGISTRYINDEX, l_app.err_ref);
}

static luaL_Reg funcs[] = {
	{ "run", l_run, },
	{ "quit", l_quit, },
	{ "time", l_time, },
	{ "dt", l_dt, },
	{ "width", l_width, },
	{ "height", l_height, },
	{ "key_pressed", l_key_pressed, },
	{ "key_rpressed", l_key_rpressed, },
	{ "key_released", l_key_released, },
	{ "key_down", l_key_down, },
	{ "mouse_pressed", l_mouse_pressed, },
	{ "mouse_released", l_mouse_released, },
	{ "mouse_down", l_mouse_down, },
	{ "mouse_pos", l_mouse_pos, },
	{ "mouse_dpos", l_mouse_dpos, },
	{ "mouse_moved", l_mouse_moved, },
	{ "input", l_input, },
	{ "fullscreen", l_fullscreen, },
	{ "mouse_locked", l_mouse_locked, },
	{ "mouse_hidden", l_mouse_hidden, },
	{ "keyboard_shown", l_keyboard_shown, },
	{ "title", l_title, },
	{ NULL, NULL, },
};

int luaopen_app(lua_State *L) {
	l_app.lua = L;
	luaL_newlib(L, funcs);
	return 1;
}

