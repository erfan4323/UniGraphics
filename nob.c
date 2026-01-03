#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "nob_util.c"

// -------------------- Paths --------------------
#define BUILD_DIR "build/"
#define OBJ_DIR BUILD_DIR "obj/"
#define SRC_DIR "src/"
#define EXM_DIR "examples/"

#define VCPKG_PATH       "vcpkg_installed/x64-mingw-dynamic/"
#define VCPKG_LIB_PATH   VCPKG_PATH "lib"
#define VCPKG_INC_PATH   VCPKG_PATH "include"

#define SDL_PATH         "Vendor/SDL2-2.32.10/x86_64-w64-mingw32/"
#define SDL_IMAGE_PATH   "Vendor/SDL2_image-2.8.8/x86_64-w64-mingw32/"
#define SDL_TTF_PATH     "Vendor/SDL2_ttf-2.24.0/x86_64-w64-mingw32/"
#define IMGUI_PATH       "Vendor/imgui/"
#define RAYLIB_PATH      "Vendor/raylib-5.5/"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// -------------------- Globals --------------------
Cmd cmd = {0};
Procs procs = {0};

// -------------------- Command Helper --------------------
void def_cmd() {
    cmd_append(&cmd, "g++");
    cmd_append(&cmd, "-std=c++23");
    cmd_append(&cmd, "-Wno-template-body");
    cmd_append(&cmd, "-g", "-O0");
    // cmd_append(&cmd, "-Wall", "-Wextra");
}

// -------------------- Compile Helper --------------------
static bool compile_sources(StrVec *sources, const char **includes, size_t include_count, const char *obj_subdir) {
    if (!mkdir_if_not_exists(obj_subdir)) {
        nob_log(ERROR, "Failed to create build directory: %s\n", obj_subdir);
        return false;
    }

    da_foreach(char *, src, sources) {
        def_cmd();
        cmd_append(&cmd, "-c", *src);

        for (size_t i = 0; i < include_count; i++)
            cmd_append(&cmd, "-I", includes[i]);

        cmd_append(&cmd, "-o", temp_sprintf("%s/%s.o", obj_subdir, basename_only(*src)));

        if (!cmd_run(&cmd, .async = &procs))
            return false;
    }

    return procs_flush(&procs);
}

// -------------------- Build Functions --------------------
bool build_UniGraphics_Core() {
    StrVec src = {0};
    collect_files_by_pattern(SRC_DIR "UniGraphics/core/**/*.cpp", &src);

    const char *includes[] = {
        SRC_DIR "UniGraphics/interfaces",
        SRC_DIR "UniGraphics/"
    };

    if (!compile_sources(&src, includes, ARRAY_SIZE(includes), OBJ_DIR "UniGraphicsCore"))
        return false;

    return archive_to_lib(OBJ_DIR "UniGraphicsCore/", BUILD_DIR "libUniGraphicsCore.a");
}

bool build_unigraphics_backend(const char *backend_name, const char *backend_path, const char *obj_dir) {
    StrVec src = {0};
    collect_files_by_pattern(SRC_DIR "UniGraphics/core/**/*.cpp", &src);
    collect_files_by_pattern(backend_path, &src);  // backend-specific sources

    const char *includes[] = {
        SRC_DIR "UniGraphics/interfaces",
        SRC_DIR "UniGraphics/",
        backend_name
    };

    if (!compile_sources(&src, includes, ARRAY_SIZE(includes), obj_dir))
        return false;

    return archive_to_lib(obj_dir, temp_sprintf(BUILD_DIR "libUniGraphics%s.a", backend_name));
}

bool build_unigraphics_sdl() {
    return build_unigraphics_backend("SDL", SRC_DIR "UniGraphics/backends/sdl/**/*.cpp", OBJ_DIR "UniGraphicsSDL");
}

bool build_unigraphics_raylib() {
    return build_unigraphics_backend("Raylib", SRC_DIR "UniGraphics/backends/raylib/**/*.cpp", OBJ_DIR "UniGraphicsRaylib");
}

// -------------------- Main Build --------------------
bool build_main(bool use_sdl) {
    const char *core_includes[] = {VCPKG_INC_PATH, SRC_DIR};

    const char *sdl_libs[] = {
        "-L" BUILD_DIR,
        "-L" VCPKG_LIB_PATH,
        "-lUniGraphicsSDL",
        "-lSDL2main", "-lSDL2", "-lSDL2_ttf", "-lSDL2_image", "-lraylib"  // optional raylib
    };

    const char *raylib_libs[] = {
        "-L" BUILD_DIR,
        "-L" VCPKG_LIB_PATH,
        "-lUniGraphicsRaylib",
        "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lSDL2main"
    };

    def_cmd();
    cmd_append(&cmd, EXM_DIR "Capabilities.cpp");

    for (size_t i = 0; i < ARRAY_SIZE(core_includes); i++)
        cmd_append(&cmd, "-I", core_includes[i]);

    if (use_sdl) {
        for (size_t i = 0; i < ARRAY_SIZE(sdl_libs); i++)
            cmd_append(&cmd, sdl_libs[i]);
    } else {
        for (size_t i = 0; i < ARRAY_SIZE(raylib_libs); i++)
            cmd_append(&cmd, raylib_libs[i]);
    }

    cmd_append(&cmd, "-o", BUILD_DIR "Examples");

    return cmd_run(&cmd);
}

// -------------------- Entry Point --------------------
int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob_util.c");

    if (!mkdir_if_not_exists(BUILD_DIR) || !mkdir_if_not_exists(OBJ_DIR)) {
        nob_log(ERROR, "Failed to create build directory\n");
        return 1;
    }

    if (!build_UniGraphics_Core()) return 1;

    bool use_sdl = false;
    if (use_sdl) {
        if (!build_unigraphics_sdl()) return 1;
    } else {
        if (!build_unigraphics_raylib()) return 1;
    }

    // if (!build_main(use_sdl)) return 1;

    return 0;
}
