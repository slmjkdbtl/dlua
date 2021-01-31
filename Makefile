
CFLAGS += -Ilua/src
CFLAGS += -Ilib
CFLAGS += -fPIC

CFLAGS += -ObjC

LDFLAGS += -Llua/src
LDFLAGS += -llua
LDFLAGS += -shared

LDFLAGS += -framework Cocoa

LUA_LIB := lua/src/liblua.a

LIBS := fs.so plat.so app.so

.PHONY: all
all: $(LIBS)


%.so: %.c $(LUA_LIB)
	cc $< utils.c $(CFLAGS) $(LDFLAGS) -o $@

$(LUA_LIB):
	cd lua/src; \
		make a

.PHONY: clean
clean:
	rm *.so

