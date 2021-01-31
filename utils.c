// wengwengweng

#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#ifdef __APPLE__
#import <Foundation/Foundation.h>
#endif

#include "utils.h"

bool luaL_args0(lua_State *L) {
	return lua_gettop(L) == 0;
}

bool luaL_args1(lua_State *L, int type) {
	if (lua_gettop(L) == 1) {
		if (lua_type(L, 1) == type) {
			return true;
		}
	}
	return false;
}

bool luaL_args2(lua_State *L, int t1, int t2) {
	if (lua_gettop(L) == 2) {
		if (lua_type(L, 1) == t1 && lua_type(L, 2) == t2) {
			return true;
		}
	}
	return false;
}

bool luaL_args3(lua_State *L, int t1, int t2, int t3) {
	if (lua_gettop(L) == 3) {
		if (lua_type(L, 1) == t1 && lua_type(L, 2) == t2 && lua_type(L, 3) == t3) {
			return true;
		}
	}
	return false;
}

bool luaL_args4(lua_State *L, int t1, int t2, int t3, int t4) {
	if (lua_gettop(L) == 4) {
		if (lua_type(L, 1) == t1 && lua_type(L, 2) == t2 && lua_type(L, 3) == t3 && lua_type(L, 4) == t4) {
			return true;
		}
	}
	return false;
}

void luaL_checktable(lua_State *L, int pos) {
	luaL_checktype(L, pos, LUA_TTABLE);
}

bool luaL_checkboolean(lua_State *L, int pos) {
	luaL_checktype(L, pos, LUA_TBOOLEAN);
	return lua_toboolean(L, pos);
}

void luaL_opttable(lua_State *L, int pos) {
	if (!lua_isnoneornil(L, pos)) {
		luaL_checktable(L, pos);
	}
}

void *luaL_optudata(lua_State *L, int pos, const char *type, void *def) {
	return lua_isnoneornil(L, pos) ? def : luaL_checkudata(L, pos, type);
}

const char *lua_udatatype(lua_State *L, int pos) {
	if (!lua_getmetatable(L, pos)) {
		return NULL;
	}
	lua_getfield(L, -1, "__name");
	const char *tname = lua_tostring(L, -1);
	// TODO: stack management
	lua_pop(L, 2);
	return tname;
}

void luaL_regfuncs(lua_State *L, luaL_Reg *reg) {
	for (int i = 0; reg[i].name != NULL; i++) {
		lua_register(L, reg[i].name, reg[i].func);
	}
}

int luaL_getenum(lua_State *L, luaL_Enum *map, const char *str) {
	for (int i = 0; map[i].str != NULL; i++) {
		if (strcmp(str, map[i].str) == 0) {
			return map[i].val;
		}
	}
	return 0;
}

void luaL_regenum(lua_State *L, const char *name, luaL_Enum *map) {

	lua_newtable(L);

	for (int i = 0; map[i].str != NULL; i++) {
		lua_pushinteger(L, map[i].val);
		lua_setfield(L, -2, map[i].str);
	}

	lua_setglobal(L, name);

}

void luaL_regtype(lua_State *L, const char *name, luaL_Reg *reg) {

	luaL_newmetatable(L, name);

	for (int i = 0; reg[i].name != NULL; i++) {
		lua_pushcfunction(L, reg[i].func);
		lua_setfield(L, -2, reg[i].name);
	}

	lua_pop(L, 1);
}

bool streq(const char *a, const char *b) {
	return strcmp(a, b) == 0;
}

bool is_file(const char *path) {
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISREG(sb.st_mode);
}

bool is_dir(const char *path) {
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

void to_dir(char *path) {
	int len = strlen(path);
	for (int i = len - 2; i >= 0; i--) {
		if (path[i] == '/') {
			path[i + 1] = '\0';
			return;
		}
	}
	path[0] = '\0';
	return;
}

void get_res_dir(char *buf) {
#ifdef __APPLE__
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	const char* res_path = [[[NSBundle mainBundle] resourcePath] UTF8String];
	strcpy(buf, res_path);
	[pool drain];
#endif
}

