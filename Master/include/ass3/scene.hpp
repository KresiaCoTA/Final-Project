#ifndef COMP3421_TUTORIAL_09_SCENE_HPP
#define COMP3421_TUTORIAL_09_SCENE_HPP

#include <glad/glad.h>

#include <glm/ext.hpp>

#include <ass3/camera.hpp>
#include <ass3/model.hpp>
#include <ass3/framebuffer.hpp>

struct node_t {
    /**
     * The different types of models in our scene.
     */
    enum {
        EMPTY,
        STATIC_MESH,
        REFLECTIVE,
        REFLECTIVE_DYNAMIC,
        PORTAL,
        FAN,
        TOY,

        STATIC_MESH_LIGHT,
        LIGHT,
    } kind = EMPTY;
    enum {
        STATIC,
        DECORATE_BALL_1,
        DECORATE_BALL_2,
        TANK,
        TANK2,
        SPINNING_FAN,
    } ani_kind = STATIC;
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 translation = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0);
    std::vector<node_t> children {};

    // Only for STATIC_MESH, REFLECTIVE, PORTAL:
    model_t obj;

    // Only for REFLECTIVE, PORTAL:
    GLuint texture;

    // Only for Light_meshes;
    simple_model_t simple_obj;
    // Only for Light;

    // Only for PORTAL:
    bool is_interior;
};

struct light_t {
    glm::vec3 light_position;
    glm::vec3 light_diffuse;
    glm::vec3 light_specular;
    float constant;
    float linear;
    float quadratic;
};

struct lightScene_t {
    node_t node;
    std::vector<light_t> lights;
};

/**
 * Create the interior scene
 */
node_t make_scene_interior(GLuint portal_texture);

/**
 * Create the exterior scene
 */
node_t make_scene_exterior(GLuint portal_texture);
node_t make_scene_exterior_nowall(GLuint portal_texture);
node_t make_scene_room(GLuint window_texture, node_t room, framebuffer_t frame, GLuint window_withLight_texture);
node_t make_scene_room_reflec(GLuint window_texture);
node_t make_scene_withLight(GLuint portal_texture);

lightScene_t make_scene_forest();
#endif //COMP3421_TUTORIAL_09_SCENE_HPP
