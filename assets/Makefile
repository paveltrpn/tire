# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := shaders
SRCS := $(SOURCES_DIR)/vk_simple_tri_VERTEX.glsl \
	$(SOURCES_DIR)/vk_simple_tri_FRAGMENT.glsl \
	$(SOURCES_DIR)/vk_simple_box_VERTEX.glsl \
	$(SOURCES_DIR)/vk_simple_box_FRAGMENT.glsl \


SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(SPVS)

$(SOURCES_DIR)/vk_simple_tri_VERTEX.spv: $(SOURCES_DIR)/vk_simple_tri_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/vk_simple_tri_FRAGMENT.spv: $(SOURCES_DIR)/vk_simple_tri_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/vk_simple_box_VERTEX.spv: $(SOURCES_DIR)/vk_simple_box_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/vk_simple_box_FRAGMENT.spv: $(SOURCES_DIR)/vk_simple_box_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/*.spv
