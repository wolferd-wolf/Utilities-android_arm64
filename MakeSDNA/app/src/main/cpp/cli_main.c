/* main() is provided by the linked static libs (this is Blender own
 * standalone makesdna/makesrna CLI tool) - this file exists so
 * add_executable() has something to compile; the linker pulls the real
 * main() out of the imported static libs to satisfy the CRT undefined
 * reference to it.
 *
 * 2026-08-15: also provides minimal stub implementations of the Android
 * log functions. Reasoning: this binary is statically linked so it can
 * run under qemu-user-static without a real Android device/system image
 * present (no /system/bin/linker64 available in that environment - the
 * NDK only ships link-time stub .so's for dynamic linking, not the
 * actual device dynamic linker binary, so full static linking is the only
 * way to run this without a real/emulated device). NDK does not ship a
 * redistributable static liblog.a for general linking, and Blender code
 * calls into Android logging pervasively (CLOG, guardedalloc error
 * reporting, etc. likely route through it on this platform), so a plain
 * -static link risks an undefined-reference failure without these. Real
 * behavior (writing to logcat) is irrelevant here since nothing reads
 * logcat in this environment anyway - these just need to exist and not
 * crash. Prints to stderr instead so the transcript still shows anything
 * logged, which is strictly more useful here than silently discarding it. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int __android_log_print(int prio, const char *tag, const char *fmt, ...) {
    (void)prio;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[android_log:%s] ", tag ? tag : "?");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "
");
    va_end(ap);
    return 0;
}

int __android_log_vprint(int prio, const char *tag, const char *fmt, va_list ap) {
    (void)prio;
    fprintf(stderr, "[android_log:%s] ", tag ? tag : "?");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "
");
    return 0;
}

int __android_log_write(int prio, const char *tag, const char *text) {
    (void)prio;
    fprintf(stderr, "[android_log:%s] %s
", tag ? tag : "?", text ? text : "");
    return 0;
}

void __android_log_assert(const char *cond, const char *tag, const char *fmt, ...) {
    fprintf(stderr, "[android_log_assert:%s] condition: %s
", tag ? tag : "?", cond ? cond : "?");
    if (fmt) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "
");
    }
    /* real implementation aborts - match that so an actual assertion
     * failure in generation logic is not silently swallowed. */
    abort();
}

