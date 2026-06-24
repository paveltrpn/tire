# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := .
OUT_DIR := spirv
SRCS := $(SOURCES_DIR)/shader_PushConstants.vert \
        $(SOURCES_DIR)/shader_PushConstants.frag \
	$(SOURCES_DIR)/grid.vert \
	$(SOURCES_DIR)/grid.frag \
    $(SOURCES_DIR)/testbox.vert \
	$(SOURCES_DIR)/testbox.frag \
	$(SOURCES_DIR)/bbox_corner.vert \
	$(SOURCES_DIR)/bbox_corner.frag \
    $(SOURCES_DIR)/vk_ui_FRAGMENT.glsl \
	$(SOURCES_DIR)/vk_ui_VERTEX.glsl

OUT :=  $(SOURCES_DIR)/$(OUT_DIR)/vert_PushConstants.spv \
        $(SOURCES_DIR)/$(OUT_DIR)/frag_PushConstants.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vert_grid.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/frag_grid.spv \
    $(SOURCES_DIR)/$(OUT_DIR)/vert_testbox.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/frag_testbox.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vert_bbox_corner.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/frag_bbox_corner.spv \
    $(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv \
	$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv

SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(OUT)

$(SOURCES_DIR)/$(OUT_DIR)/vert_PushConstants.spv: $(SOURCES_DIR)/shader_PushConstants.vert
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/frag_PushConstants.spv: $(SOURCES_DIR)/shader_PushConstants.frag
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vert_grid.spv: $(SOURCES_DIR)/grid.vert
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/frag_grid.spv: $(SOURCES_DIR)/grid.frag
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vert_testbox.spv: $(SOURCES_DIR)/testbox.vert
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/frag_testbox.spv: $(SOURCES_DIR)/testbox.frag
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vert_bbox_corner.spv: $(SOURCES_DIR)/bbox_corner.vert
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/frag_bbox_corner.spv: $(SOURCES_DIR)/bbox_corner.frag
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv: $(SOURCES_DIR)/vk_ui_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv: $(SOURCES_DIR)/vk_ui_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/$(OUT_DIR)/*.spv
