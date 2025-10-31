# ---- Project ----
APP       = rotate3d
SRC       = rotate3d.c xinit.c rot3dutils.c phongtex.c pbenvtex.c envmap.c phongenv.c texturemap.c envbump.c polygon.c
OBJ_DIR   = build
OBJ       = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
DEPS      = $(OBJ:.o=.d)

# ---- Toolchain ----
CC        = gcc
CPPFLAGS  = -DHAVE_XSHM_EXTENSION
BASE_CFLAGS = -Wno-error=incompatible-pointer-types -O2
LDFLAGS   =
LDLIBS    = -lXext -lm -lX11

# ---- Platform detection (override with: make ARCH=alpha or ARCH=ultrasparc) ----
ARCH ?= $(shell uname -m)

# Normalize to a platform tag
ifeq ($(findstring sparc,$(ARCH)),sparc)
PLATFORM := ultrasparc
else ifneq (,$(findstring sun4,$(ARCH)))
PLATFORM := ultrasparc
else ifneq (,$(findstring alpha,$(ARCH)))
PLATFORM := alpha
else
PLATFORM := other
endif

# ---- CFLAGS selection ----
CFLAGS := $(BASE_CFLAGS)
ifeq ($(PLATFORM),ultrasparc)
CFLAGS += -mcpu=ultrasparc3 -mvis2
endif
ifeq ($(PLATFORM),alpha)
# Note: GCC uses -mcpu=ev6 (not -mpcu)
CFLAGS += -mcpu=ev6
endif



# ---- Targets ----
all: $(APP)

$(APP): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDLIBS)

# Compile source → object (with automatic dependency generation)
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Create build directory if missing
$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(APP)

.PHONY: all clean
-include $(DEPS)
