#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// ======================
// Dynamic string vector
// ======================
typedef struct {
    char **items;
    size_t count, capacity;
} StrVec;

// ======================
// Helper Functions
// ======================

// Join two paths with a '/'
static inline char *join_path(const char *a, const char *b) {
    size_t la = strlen(a), lb = strlen(b);
    char *r = (char *)malloc(la + 1 + lb + 1);
    memcpy(r, a, la);
    r[la] = '/';
    memcpy(r + la + 1, b, lb);
    r[la + 1 + lb] = 0;
    return r;
}

// Check if string ends with a given suffix
static inline bool ends_with(const char *s, const char *ending) {
    size_t ls = strlen(s), le = strlen(ending);
    return ls >= le && strcmp(s + ls - le, ending) == 0;
}

// ======================
// Directory scanning
// ======================

// Recursively collect files matching ext
static void collect_from_dir_recursive(const char *parent, const char *ext,
                                       StrVec *out) {
    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(parent, &children)) return;

    for (size_t i = 0; i < children.count; ++i) {
        const char *name = children.items[i];
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;

        char *p = join_path(parent, name);
        Nob_File_Type t = nob_get_file_type(p);

        if (t == NOB_FILE_DIRECTORY) {
            collect_from_dir_recursive(p, ext, out);
            free(p);
        } else if (t == NOB_FILE_REGULAR) {
            if (!ext || ends_with(p, ext)) {
                da_append(out, p);  // ownership transferred
            } else {
                free(p);  // free if not added
            }
        } else {
            free(p);  // unknown type
        }
    }

    free(children.items);
}

// Non-recursive directory scan
static void collect_from_dir(const char *dir, const char *ext, bool recursive,
                             StrVec *out) {
    if (recursive) {
        collect_from_dir_recursive(dir, ext, out);
        return;
    }

    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(dir, &children)) return;

    for (size_t i = 0; i < children.count; ++i) {
        const char *name = children.items[i];
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;

        char *p = join_path(dir, name);
        if (nob_get_file_type(p) == NOB_FILE_REGULAR) {
            if (!ext || ends_with(p, ext))
                da_append(out, p);
            else
                free(p);
        } else {
            free(p);
        }
    }

    free(children.items);
}

// Collect files using simple glob patterns
static void collect_files_by_pattern(const char *pattern, StrVec *out) {
    const char *deep = strstr(pattern, "/**/");
    if (deep) {
        size_t dirlen = deep - pattern;
        char *dir = (char *)malloc(dirlen + 1);
        memcpy(dir, pattern, dirlen);
        dir[dirlen] = 0;

        const char *rest = deep + 4;
        const char *star = strchr(rest, '*');
        const char *ext = star ? strchr(star, '.') : NULL;

        collect_from_dir_recursive(dir, ext, out);
        free(dir);
        return;
    }

    const char *last_slash = strrchr(pattern, '/');
    if (!last_slash) {
        if (!strchr(pattern, '*') &&
            nob_get_file_type(pattern) == NOB_FILE_REGULAR)
            da_append(out, strdup(pattern));  // copy literal filename
        return;
    }

    size_t dirlen = last_slash - pattern;
    char *dir = (char *)malloc(dirlen + 1);
    memcpy(dir, pattern, dirlen);
    dir[dirlen] = 0;

    const char *filepart = last_slash + 1;
    const char *star = strchr(filepart, '*');
    const char *ext = star ? strchr(star, '.') : NULL;

    collect_from_dir(dir, ext, false, out);
    free(dir);
}

// Move all files from StrVec src_dir to dst_dir
static bool move_files(StrVec *files, const char *src_path,
                       const char *dst_path) {
    for (size_t i = 0; i < files->count; ++i) {
        char *src_file = files->items[i];

        // Create the destination file path
        size_t dst_len =
            strlen(dst_path) + 1 + strlen(src_file + strlen(src_path) + 1) + 1;
        char *dst_file = (char *)malloc(dst_len);
        if (!dst_file) continue;

        snprintf(dst_file, dst_len, "%s/%s", dst_path,
                 src_file + strlen(src_path) + 1);

        // Ensure destination subdirectories exist
        char *last_slash = strrchr(dst_file, '/');
        if (last_slash) {
            *last_slash = 0;
            nob_mkdir_if_not_exists(dst_file);
            *last_slash = '/';
        }

        // Move the file
        if (!nob_copy_file(src_file, dst_file)) {
            fprintf(stderr, "Failed to move file: %s\n", src_file);
            free(dst_file);
            return false;
        }

        // Delete the original file
        nob_delete_file(src_file);
        free(dst_file);
    }

    return true;
}

bool archive_objects(const char *lib_path, StrVec *objects) {
    Cmd ar_cmd = {0};
    cmd_append(&ar_cmd, "ar", "rcs", lib_path);

    da_foreach(char *, obj, objects) { cmd_append(&ar_cmd, *obj); }

    return cmd_run(&ar_cmd);
}

void append_compilation_sources(Cmd *cmd, StrVec *sources,
                                const char **include_dirs,
                                size_t include_count) {
    cmd_append(cmd, "-c");

    da_foreach(char *, src, sources) { cmd_append(cmd, *src); }

    for (size_t i = 0; i < include_count; i++)
        cmd_append(cmd, "-I", include_dirs[i]);
}

bool archive_to_lib(const char *obj_dir, const char *lib_name) {
    nob_minimal_log_level = NOB_WARNING;

    StrVec obj_files = {0};
    collect_files_by_pattern(temp_sprintf("%s*.o", obj_dir), &obj_files);

    nob_minimal_log_level = NOB_INFO;

    return archive_objects(lib_name, &obj_files);
}

static inline const char *basename_only(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

bool run_python(const char *name) {
    Cmd cmd = {0};

    char *full_name = temp_sprintf("scripts/%s.py", name);
    cmd_append(&cmd, "python", full_name);

    return cmd_run(&cmd);
}