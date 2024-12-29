CC=gcc


CFLAGS += -Wall 
CFLAGS += -Wextra 
CFLAGS += -pedantic
CFLAGS += -Ofast 
CFLAGS += -Icore

LIBS=-lm -lalloc


CACHE=.cache
OUTPUT=$(CACHE)/release


CORE_SRC=core
EASE_SRC=ease

CORE_TARGET=
TARGET=ease


#ifeq ($(OS),Windows_NT)
CORE_TARGET=libease.dll
#else
CORE_TARGET=libease.so
CFLAGS += -fPIC
#endif

MAIN += main.o
MAIN += lexer.o
MAIN += ast.o
MAIN += parser.o
MAIN += ir_emit.o
EASE_OBJ=$(addprefix $(CACHE)/,$(MAIN))


CORE += err.o
CORE += ir.o
CORE += asm_x86_64.o
CORE += asm_x86_64_emit.o
CORE += asm_win_x86_64_emit.o
CORE += asm_linux_x86_64_emit.o
CORE_OBJ=$(addprefix $(CACHE)/,$(CORE))


all: env $(EASE_OBJ) $(CORE_OBJ)
	$(CC) $(CFLAGS) $(CORE_OBJ) $(LIBS) -shared -o $(OUTPUT)/$(CORE_TARGET)
	$(CC) $(CFLAGS) $(EASE_OBJ) $(LIBS) $(OUTPUT)/$(CORE_TARGET) -o $(OUTPUT)/$(TARGET)


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


.PHONY: env dep clean test


ARGS =

exec: all 
	clear
	$(OUTPUT)/$(TARGET) $(ARGS)

test: all
	clear
	$(OUTPUT)/$(TARGET) $(ARGS) > tmp.s && nasm -fwin64 ./tmp.s -o tmp.o && ld ./tmp.o -e _start -o main -lkernel32 -lmsvcrt


dep:
	$(CC) -MM $(CORE_SRC)/*.c $(EASE_SRC)/*.c | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list


env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)


clean: 
	rm -vrf $(CACHE)


