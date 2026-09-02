# glsl -> spirv shaders makefile

CC       := glslc
FLAGS    :=
LIBS 	 :=
RM       := rm -f

SOURCES_DIR := .
OUT_DIR := spirv
SRCS := $(SOURCES_DIR)/vk_grid_VERTEX.glsl \
	    $(SOURCES_DIR)/vk_grid_FRAGMENT.glsl \
        $(SOURCES_DIR)/vk_testbox_VERTEX.glsl \
	    $(SOURCES_DIR)/vk_testbox_FRAGMENT.glsl \
        $(SOURCES_DIR)/vk_ui_FRAGMENT.glsl \
	    $(SOURCES_DIR)/vk_ui_VERTEX.glsl

OUT :=  $(SOURCES_DIR)/$(OUT_DIR)/vk_grid_VERTEX.spv \
	    $(SOURCES_DIR)/$(OUT_DIR)/vk_grid_FRAGMENT.spv \
        $(SOURCES_DIR)/$(OUT_DIR)/vk_testbox_VERTEX.spv \
	    $(SOURCES_DIR)/$(OUT_DIR)/vk_testbox_FRAGMENT.spv \
        $(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv \
	    $(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv

SPVS := $(SRCS:.glsl=.spv)

$(info SHADERS MAKEFILE)

all: $(OUT)

$(SOURCES_DIR)/$(OUT_DIR)/vk_grid_VERTEX.spv: $(SOURCES_DIR)/vk_grid_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_grid_FRAGMENT.spv: $(SOURCES_DIR)/vk_grid_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_testbox_VERTEX.spv: $(SOURCES_DIR)/vk_testbox_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_testbox_FRAGMENT.spv: $(SOURCES_DIR)/vk_testbox_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@


$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_FRAGMENT.spv: $(SOURCES_DIR)/vk_ui_FRAGMENT.glsl
	$(CC) $(FLAGS) -fshader-stage=fragment $< -o $@

$(SOURCES_DIR)/$(OUT_DIR)/vk_ui_VERTEX.spv: $(SOURCES_DIR)/vk_ui_VERTEX.glsl
	$(CC) $(FLAGS) -fshader-stage=vertex $< -o $@

clean:
	$(RM) $(SOURCES_DIR)/$(OUT_DIR)/*.spv
