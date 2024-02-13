#ifndef COMP3421_TUTORIAL_09_FRAMEBUFFER_HPP
#define COMP3421_TUTORIAL_09_FRAMEBUFFER_HPP

#include <glad/glad.h>

struct framebuffer_t {
    GLuint fbo;
    GLuint texture;
    GLuint rbo;
};

/**
 * Create a framebuffer
 * @returns struct containing handle to FBO and the resulting texture
 */
framebuffer_t make_framebuffer(int width, int height);


framebuffer_t make_framebuffer_for_reflection(int width, int height);

/**
 * Destroy a framebuffer, releasing GPU resources
 * @param framebuffer The framebuffer to destroy
 */
void delete_framebuffer(framebuffer_t &framebuffer);

#endif //COMP3421_TUTORIAL_09_FRAMEBUFFER_HPP
