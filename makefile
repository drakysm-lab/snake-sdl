# -----------------------
# PROJECT CONFIGURATION #
# -----------------------
CC := gcc
# gcc flags + include 
CFLAGS := -Wall -O3
# linker flags (default for windows)
LDFLAGS := -L./src/SDL2/lib

# show/hide output
D_OUT := @

# device-targetered
PLATFORM :=
BIN_FMT :=

# ---------------------------------------------------------------------------
# DETECT PLATFORM (you can also manually set the platform with 'PLATFORM=') #
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
	PLATFORM := win32
else
	ifeq ($(shell uname -s),Linux)
		PLATFORM := linux
	endif
	ifeq ($(shell uname -s),Darwin)
		PLATFORM := darwin
	endif
endif

# -------------------------------------------
# SET BIN FORMAT TO MATCH DETECTED PLATFORM #
# -------------------------------------------
ifeq ($(PLATFORM),win32)
	# Windows static lib and bin format
	CFLAGS += -I./src/SDL2/include
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2
	BIN_FMT := .exe
else
	ifeq ($(PLATFORM),linux)
		# Linux static lib and bin format
		CFLAGS += -D_REENTRANT $(shell pkg-config --cflags sdl2)
		# reassign LDFLAGS
		LDFLAGS := $(shell pkg-config --libs sdl2)
	endif
endif

# ----------------------
# SHOW/HIDE DEBUG MODE #
# ----------------------
ifdef DEBUG
	D_OUT :=
endif

# ---------
# TARGETS #
# ---------
build: ./src/main.c
	$(D_OUT)$(CC) ./src/main.c $(CFLAGS) $(LDFLAGS) -o ./bin/main$(BIN_FMT)
run: ./bin/main$(BIN_FMT)
	@./bin/main$(BIN_FMT)
test: build run
