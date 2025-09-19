#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#include "nob_util.c"

#define BUILD_DIR "build/"
#define OBJ_DIR BUILD_DIR "obj/"
#define SRC_DIR "src/"
#define EXM_DIR "examples/"

#define SDL_PATH "Vendor/SDL2-2.32.10/x86_64-w64-mingw32/"
#define SDL_IMAGE_PATH "Vendor/SDL2_image-2.8.8/x86_64-w64-mingw32/"
#define SDL_TTF_PATH "Vendor/SDL2_ttf-2.24.0/x86_64-w64-mingw32/"
#define IMGUI_PATH "Vendor/imgui/"
#define RAYLIB_PATH "Vendor/raylib-5.5/"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

Cmd cmd = {0};
Procs procs = {0};

void def_cmd() {
    cmd_append(&cmd, "g++");
    cmd_append(&cmd, "-std=c++23");
    cmd_append(&cmd, "-Wno-template-body");
    cmd_append(&cmd, "-g", "-O0");
    // cmd_append(&cmd, "-Wall", "-Wextra");
}

static bool compile_sources(StrVec *sources, const char **includes,
                            size_t include_count, const char *obj_subdir) {
    if (!mkdir_if_not_exists(obj_subdir)) {
        nob_log(ERROR, "Failed to create build directory: %s\n", obj_subdir);
        return false;
    }

    da_foreach(char *, src, sources) {
        def_cmd();
        cmd_append(&cmd, "-c");
        cmd_append(&cmd, *src);

        for (size_t i = 0; i < include_count; i++)
            cmd_append(&cmd, "-I", includes[i]);

        cmd_append(&cmd, "-o",
                   temp_sprintf("%s/%s.o", obj_subdir, basename_only(*src)));

        if (!cmd_run(&cmd, .async = &procs)) return false;
    }

    return procs_flush(&procs);
}

bool build_UniGraphics() {
    StrVec core_src = {0};
    collect_files_by_pattern(SRC_DIR "UniGraphics/**/*.cpp", &core_src);

    const char *core_includes[] = {
        SRC_DIR "UniGraphics",
        RAYLIB_PATH "include",
    };

    if (!compile_sources(&core_src, core_includes, ARRAY_SIZE(core_includes), OBJ_DIR "UniGraphics"))
        return false;

    if (!archive_to_lib(OBJ_DIR "UniGraphics/", BUILD_DIR "libUniGraphics.a"))
        return false;

    return true;
}

bool build_imgui(void) {
    StrVec imgui_src = {0};
    collect_files_by_pattern(IMGUI_PATH "*.cpp", &imgui_src);
    da_append(&imgui_src, IMGUI_PATH "backends/imgui_impl_sdl2.cpp");
    da_append(&imgui_src, IMGUI_PATH "backends/imgui_impl_sdlrenderer2.cpp");

    const char *includes[] = {SDL_PATH "include/SDL2/", IMGUI_PATH};

    if (!compile_sources(&imgui_src, includes, ARRAY_SIZE(includes),
                         OBJ_DIR "imgui"))
        return false;

    if (!archive_to_lib(OBJ_DIR "imgui/", BUILD_DIR "libimgui.a")) return false;

    nob_log(INFO, "Created ImGui library!");
    return true;
}

bool build_main() {
    const char *core_includes[] = {SDL_PATH "include", SDL_IMAGE_PATH "include", SDL_TTF_PATH "include",
                                   RAYLIB_PATH "include", SRC_DIR};

    const char *core_libs[] = {"-L" BUILD_DIR,
                               "-L" RAYLIB_PATH "lib",
                               "-L" SDL_PATH "lib",
                               "-L" SDL_IMAGE_PATH "lib",
                               "-L" SDL_TTF_PATH "lib",
                               "-lUniGraphics",
                               "-lraylib",
                               "-lgdi32",
                               "-lopengl32",
                               "-lshell32",
                               "-luser32",
                               "-lkernel32",
                               "-lSDL2main",
                               "-lSDL2",
                               "-lSDL2_ttf",
                               "-lSDL2_image",
                               "-lwinmm",
                               "-lmingw32",
                               "-lDbghelp",
                               "-lpthread"};

    def_cmd();
    // cmd_append(&cmd, EXM_DIR "Live_Switch_Backend.cpp");
    cmd_append(&cmd, EXM_DIR "Capabilities.cpp");
    for (size_t i = 0; i < ARRAY_SIZE(core_includes); i++)
        cmd_append(&cmd, "-I", core_includes[i]);

    for (size_t i = 0; i < ARRAY_SIZE(core_libs); i++)
        cmd_append(&cmd, core_libs[i]);

    cmd_append(&cmd, "-o", BUILD_DIR "Examples");

    if (!cmd_run(&cmd)) return false;

    return true;
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob_util.c");

    if (!mkdir_if_not_exists(BUILD_DIR)) {
        nob_log(ERROR, "Failed to create build directory\n");
        return 1;
    }

    if (!mkdir_if_not_exists(OBJ_DIR)) {
        nob_log(ERROR, "Failed to create build directory\n");
        return 1;
    }

    // if (needs_rebuild1(BUILD_DIR "libimgui.a", IMGUI_PATH "imgui.h")) {
    //     if (!build_imgui()) return 1;
    // }

    if (!build_UniGraphics())
        return 1;

    if (!build_main()) return 1;

    return 0;
}