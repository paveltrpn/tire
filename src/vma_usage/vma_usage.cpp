
#include "vma_usage.h"

// Vulkan Memory Allocator comes in form of a "stb-style" single header file.
// While you can pull the entire repository e.g. as Git module, there
// is also Cmake script provided, you don't need to build it as a separate
// library project. You can add file "vk_mem_alloc.h" directly to your project
// and submit it to code repository next to your other source files.
//
// "Single header" doesn't mean that everything is contained in C/C++
// declarations, like it tends to be in case of inline functions or C++
// templates. It means that implementation is bundled with interface in a
// single file and needs to be extracted using preprocessor macro. If you
// don't do it properly, it will result in linker errors.
//
// To do it properly:
//
// 1) Include "vk_mem_alloc.h" file in each CPP file where you want to use the library. This
// includes declarations of all members of the library.
//
// 2) In exactly one CPP file define following macro before this include.
// It enables also internal definitions.

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"

namespace tire {}
