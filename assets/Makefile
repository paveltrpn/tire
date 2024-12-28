# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := shaders
SRCS := $(SOURCES_DIR)/001_shader_vert.glsl \
	$(SOURCES_DIR)/001_shader_frag.glsl \
	$(SOURCES_DIR)/fsSource-006.glsl \
	$(SOURCES_DIR)/fsSource.glsl \
	$(SOURCES_DIR)/vsSource-006.glsl \
	$(SOURCES_DIR)/vsSource.glsl


SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(SPVS)

$(SOURCES_DIR)/001_shader_vert.spv: $(SOURCES_DIR)/001_shader_vert.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/001_shader_frag.spv: $(SOURCES_DIR)/001_shader_frag.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/fsSource-006.spv: $(SOURCES_DIR)/fsSource-006.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/fsSource.spv: $(SOURCES_DIR)/fsSource.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/vsSource-006.spv: $(SOURCES_DIR)/vsSource-006.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/vsSource.spv: $(SOURCES_DIR)/vsSource.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/*.spv
