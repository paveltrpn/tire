# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := .
OUT_DIR := spirv
SRCS := $(SOURCES_DIR)/shader_PushConstants.vert \
        $(SOURCES_DIR)/shader_PushConstants.frag

OUT :=  $(SOURCES_DIR)/$(OUT_DIR)/vert_PushConstants.spv \
        $(SOURCES_DIR)/$(OUT_DIR)/frag_PushConstants.spv \

SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(OUT)

$(SOURCES_DIR)/$(OUT_DIR)/vert_PushConstants.spv: $(SOURCES_DIR)/shader_PushConstants.vert
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/frag_PushConstants.spv: $(SOURCES_DIR)/shader_PushConstants.frag
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/$(OUT_DIR)/*.spv
