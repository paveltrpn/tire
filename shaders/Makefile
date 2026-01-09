# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := .
OUT_DIR := spirv
SRCS := $(SOURCES_DIR)/vk_simpleTri_VERTEX.glsl \
	$(SOURCES_DIR)/vk_simpleTri_FRAGMENT.glsl \
	$(SOURCES_DIR)/vk_simpleBox_VERTEX.glsl \
	$(SOURCES_DIR)/vk_simpleBox_FRAGMENT.glsl \
	$(SOURCES_DIR)/vk_vertexBuffer_VERTEX.glsl \
	$(SOURCES_DIR)/vk_vertexBuffer_FRAGMENT.glsl \
    $(SOURCES_DIR)/vk_ui_VERTEX.glsl \
	$(SOURCES_DIR)/vk_ui_FRAGMENT.glsl

OUT :=  $(SOURCES_DIR)/$(OUT_DIR)/vk_simpleTri_VERTEX.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleTri_FRAGMENT.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleBox_VERTEX.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleBox_FRAGMENT.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_vertexBuffer_VERTEX.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_vertexBuffer_FRAGMENT.spv \
    $(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv

SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(OUT)

$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleTri_VERTEX.spv: $(SOURCES_DIR)/vk_simpleTri_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleTri_FRAGMENT.spv: $(SOURCES_DIR)/vk_simpleTri_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleBox_VERTEX.spv: $(SOURCES_DIR)/vk_simpleBox_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_simpleBox_FRAGMENT.spv: $(SOURCES_DIR)/vk_simpleBox_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_vertexBuffer_VERTEX.spv: $(SOURCES_DIR)/vk_vertexBuffer_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_vertexBuffer_FRAGMENT.spv: $(SOURCES_DIR)/vk_vertexBuffer_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv: $(SOURCES_DIR)/vk_ui_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv: $(SOURCES_DIR)/vk_ui_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/$(OUT_DIR)/*.spv
