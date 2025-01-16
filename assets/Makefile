# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := shaders
SRCS := $(SOURCES_DIR)/vk_simple_tri_vert.glsl \
	$(SOURCES_DIR)/vk_simple_tri_frag.glsl

SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(SPVS)

$(SOURCES_DIR)/vk_simple_tri_vert.spv: $(SOURCES_DIR)/vk_simple_tri_vert.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/vk_simple_tri_frag.spv: $(SOURCES_DIR)/vk_simple_tri_frag.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/*.spv
