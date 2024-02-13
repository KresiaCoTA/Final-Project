#include <glad/glad.h>

#include <chicken3421/chicken3421.hpp>

#include <ass3/cubemap.hpp>
#include <ass3/camera.hpp>
#include <ass3/framebuffer.hpp>
#include <ass3/renderer.hpp>

#include <iostream>

namespace {
    const char* side_suffixes[] = {
            "_right", "_left", "_top", "_bottom", "_front", "_back"
    };

    void load_side(int side, const std::string &path) {
        chicken3421::image_t image = chicken3421::load_image(path, false);
        GLenum fmt = image.n_channels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                     0, fmt, image.width, image.height, 0, fmt, GL_UNSIGNED_BYTE, image.data);
        chicken3421::delete_image(image);
    }
}

GLuint make_cubemap(
        const std::string &base_path,
        const std::string &extension) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++) {
        load_side(i, base_path + side_suffixes[i] + extension);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture;
}

GLuint make_dynamic_room_skybox(glm::vec3 pos, const node_t &scene, renderer_t renderer, framebuffer_t frame) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glm::vec3 target[] = {
        pos+glm::vec3(1,0,0), pos+glm::vec3(-1,0,0), pos+glm::vec3(0,-1,0), pos+glm::vec3(0,1,0), pos+glm::vec3(0,0,-1), pos+glm::vec3(0,0,1)
    };
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, 720, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //framebuffer_t frame = make_framebuffer_for_reflection(1280,720);

    for (int i = 0; i < 6; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo);
        camera_t cam = make_camera(pos, target[i]);
        glEnable(GL_CLIP_DISTANCE0);
        //std::cout<<glCheckFramebufferStatus(GL_FRAMEBUFFER)<<std::endl;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);
        render_low(renderer, cam, scene);
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        delete_camera(cam);
        // reset state
    }
    return texture;
}
