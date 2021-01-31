// wengwengweng

#include <lua.h>
#include <lauxlib.h>
#include <dirty/d_plat.h>

int luaopen_plat(lua_State *L) {
#if defined(D_MACOS)
	lua_pushstring(L, "macos");
#elif defined(D_IOS)
	lua_pushstring(L, "ios");
#elif defined(D_WINDOWS)
	lua_pushstring(L, "windows");
#elif defined(D_LINUX)
	lua_pushstring(L, "linux");
#elif defined(D_ANDROID)
	lua_pushstring(L, "android");
#elif defined(D_WEB)
	lua_pushstring(L, "web");
#endif
	return 1;
}

