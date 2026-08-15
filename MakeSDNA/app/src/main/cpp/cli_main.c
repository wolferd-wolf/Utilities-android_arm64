/* Intentionally empty. main() is already provided by libmakesdna.a
 * (this is Blender's own standalone makesdna CLI tool, statically linked)
 * - this file exists purely so add_executable() has something to compile;
 * the linker pulls the real main() out of the imported static lib to
 * satisfy the C runtime's undefined reference to it. */
