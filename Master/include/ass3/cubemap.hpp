#ifndef COMP3421_TUTORIAL_09_CUBEMAP_HPP
#define COMP3421_TUTORIAL_09_CUBEMAP_HPP

#include <glad/glad.h>
#include <string>
#include <ass3/renderer.hpp>

/**
 * Create a cubemap from 6 textures
 * @param base_path Path of cubemap textures, without extension
 * @param extension File extension for cubemap textures, with dot
 * @return OpenGL texture handle
 */
GLuint make_cubemap(const std::string &base_path, const std::string &extension = ".jpg");
GLuint make_dynamic_room_skybox(glm::vec3 pos, const node_t &scene, renderer_t renderer, framebuffer_t frame);

#endif //COMP3421_TUTORIAL_09_CUBEMAP_HPP
