/* Intentionally empty. main() is already provided by the linked static
 * libs (this is Blender's own standalone makesrna CLI tool) - this file
 * exists purely so add_executable() has something to compile; the linker
 * pulls the real main() out of the imported static libs to satisfy the
 * C runtime's undefined reference to it. */
