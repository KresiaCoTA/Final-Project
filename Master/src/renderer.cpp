#include <ass3/renderer.hpp>

#include <stack>

#include <chicken3421/chicken3421.hpp>
#include <ass3/cubemap.hpp>
#include <iostream>

const char *STATIC_MESH_VERT_PATH = "res/shaders/static_mesh.vert";
const char *STATIC_MESH_FRAG_PATH = "res/shaders/static_mesh.frag";
const char *REFLECTIVE_VERT_PATH = "res/shaders/reflective.vert";
const char *REFLECTIVE_FRAG_PATH = "res/shaders/reflective.frag";
const char *PORTAL_VERT_PATH = "res/shaders/portal.vert";
const char *PORTAL_FRAG_PATH = "res/shaders/portal.frag";
const char *WITHLIGHT_VERT_PATH = "res/shaders/withLight.vert";
const char *WITHLIGHT_FRAG_PATH = "res/shaders/withLight.frag";
const char *HDR_VERT_PATH = "res/shaders/hdr.vert";
const char *HDR_FRAG_PATH = "res/shaders/hdr.frag";
const char *KERNAL_VERT_PATH = "res/shaders/kernal.vert";
const char *KERNAL_FRAG_PATH = "res/shaders/kernal.frag";
const char *BLUR_VERT_PATH = "res/shaders/blur.vert";
const char *BLUR_FRAG_PATH = "res/shaders/blur.frag";

float exposure = 1.0f;
bool hdr = true;

void renderQuad ();

namespace {
    int locate(const std::string &name) {
        GLint program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
        int loc = glGetUniformLocation(program, name.c_str());
        chicken3421::expect(loc != -1, "uniform not found: " + name);
        return loc;
    }

    void set_uniform(const std::string &name, float value) {
        glUniform1f(locate(name), value);
    }

    void set_uniform(const std::string &name, int value) {
        glUniform1i(locate(name), value);
    }

    void set_uniform(const std::string &name, glm::vec4 value) {
        glUniform4fv(locate(name), 1, glm::value_ptr(value));
    }

    void set_uniform(const std::string &name, glm::vec3 value) {
        glUniform3fv(locate(name), 1, glm::value_ptr(value));
    }

    void set_uniform(const std::string &name, const glm::mat4 &value) {
        glUniformMatrix4fv(locate(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    GLuint load_program(const std::string &vs_path, const std::string &fs_path) {
        GLuint vs = chicken3421::make_shader(vs_path, GL_VERTEX_SHADER);
        GLuint fs = chicken3421::make_shader(fs_path, GL_FRAGMENT_SHADER);
        GLuint handle = chicken3421::make_program(vs, fs);
        chicken3421::delete_shader(vs);
        chicken3421::delete_shader(fs);

        return handle;
    }
}

renderer_t make_renderer(const glm::mat4 &projection) {
    renderer_t renderer {};

    renderer.projection = projection;
    renderer.prog_static_mesh = load_program(
            STATIC_MESH_VERT_PATH, STATIC_MESH_FRAG_PATH);
    renderer.prog_reflective = load_program(
            REFLECTIVE_VERT_PATH, REFLECTIVE_FRAG_PATH);
    renderer.prog_portal = load_program(
            PORTAL_VERT_PATH, PORTAL_FRAG_PATH);
    renderer.prog_withLight = load_program(
            WITHLIGHT_VERT_PATH, WITHLIGHT_FRAG_PATH);
    renderer.prog_hdr = load_program(
            HDR_VERT_PATH, HDR_FRAG_PATH);
    renderer.prog_kernal = load_program(
            KERNAL_VERT_PATH, KERNAL_FRAG_PATH);
    renderer.prog_blur = load_program(
            BLUR_VERT_PATH, BLUR_FRAG_PATH);

    return renderer;
}

void draw_model(const renderer_t &renderer, const glm::mat4 &mv, const model_t &m) {
    glUseProgram(renderer.prog_static_mesh);

    set_uniform("u_model_view", mv);
    set_uniform("u_projection", renderer.projection);
    set_uniform("u_clip_plane", renderer.portal_clip_plane);

    for (const mesh_t &mesh: m.meshes) {
        if (mesh.material_id != -1) {
            const material_t &mtl = m.materials[mesh.material_id];

            glBindTexture(GL_TEXTURE_2D, mtl.texture);
            set_uniform("u_tex_factor", mtl.texture == 0 ? 0.f : 1.f);
            set_uniform("u_color", mtl.color);
        } else {
            // default material
            set_uniform("u_tex_factor", 0.f);
            set_uniform("u_color", glm::vec4(1, 1, 1, 1));
        }

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void draw_portal(const renderer_t &renderer, const glm::mat4 &mv, const model_t &m, GLuint texture) {
    glUseProgram(renderer.prog_portal);

    glBindTexture(GL_TEXTURE_2D, texture);
    set_uniform("u_model_view", mv);
    set_uniform("u_projection", renderer.projection);

    for (const mesh_t &mesh: m.meshes) {
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void draw_reflective(const renderer_t &renderer, const glm::mat4 &mv, const model_t &m, GLuint texture) {
    glUseProgram(renderer.prog_reflective);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    set_uniform("u_model_view", mv);
    set_uniform("u_view", renderer.view);
    set_uniform("u_projection", renderer.projection);

    for (const mesh_t &mesh: m.meshes) {
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void draw_withLight(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model) {
    for (const mesh_t &mesh: model.meshes) {
        if (mesh.material_id != -1) {
            const material_t &mtl = model.materials[mesh.material_id];

            set_uniform("model", m);
            glBindTexture(GL_TEXTURE_2D,  mtl.texture);
        } else {
            const material_t &mtl = model.materials[mesh.material_id];

            set_uniform("model", m);
            glBindTexture(GL_TEXTURE_2D,  mtl.texture);
        }

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}




void render(renderer_t &renderer, const camera_t &cam, const node_t &scene) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    renderer.view = get_view(cam);

    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{ scene, scene.transform }}};
    while (!renderables.empty()) {
        const auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;
        xform *= glm::translate(glm::mat4(1.0), node.translation);
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.z, glm::vec3(0, 0, 1));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.y, glm::vec3(0, 1, 0));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.x, glm::vec3(1, 0, 0));
        xform *= glm::scale(glm::mat4(1.0), node.scale);

        switch (node.kind) {
            case node_t::STATIC_MESH:
                draw_model(renderer, renderer.view * xform, node.obj);
                break;
            case node_t::REFLECTIVE_DYNAMIC:
                draw_reflective(renderer, renderer.view * xform, node.obj, node.texture);
                glDeleteTextures(1, &node.texture);
                break;
            case node_t::REFLECTIVE:
                draw_reflective(renderer, renderer.view * xform, node.obj, node.texture);
                break;
            case node_t::PORTAL:
                draw_portal(renderer, renderer.view * xform, node.obj, node.texture);
                break;
            default:
                draw_model(renderer, renderer.view * xform, node.obj);
                break;
        }

        for (const node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }
}

void render_withLight(const renderer_t &renderer, const camera_t &cam, const lightScene_t &lightScene) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(renderer.prog_withLight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    glm::mat4 p = renderer.projection;
    glm::mat4 v = get_view(cam);

    set_uniform("viewPos", cam.pos);
    set_uniform("material.shininess", 32.0f);
    set_uniform("view", v);
    set_uniform("projection", p);

    set_uniform("pointLights[0].position", glm::vec3(-20, 5, -10));
    set_uniform("pointLights[0].ambient", glm::vec3(0.05f, 0.00f, 0.00f));
    set_uniform("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    set_uniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    set_uniform("pointLights[0].constant", 0.0001f);
    set_uniform("pointLights[0].linear", 0.0009f);
    set_uniform("pointLights[0].quadratic", 0.0032f);

    set_uniform("pointLights[1].position", glm::vec3(-40, 5, -35));
    set_uniform("pointLights[1].ambient", glm::vec3(0.05f, 0.00f, 0.00f));
    set_uniform("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    set_uniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    set_uniform("pointLights[1].constant", 0.01f);
    set_uniform("pointLights[1].linear", 0.09f);
    set_uniform("pointLights[1].quadratic", 0.0032f);

    set_uniform("pointLights[2].position", glm::vec3(-80, 5, -60));
    set_uniform("pointLights[2].ambient", glm::vec3(0.05f, 0.00f, 0.00f));
    set_uniform("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    set_uniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    set_uniform("pointLights[2].constant", 0.01f);
    set_uniform("pointLights[2].linear", 0.09f);
    set_uniform("pointLights[2].quadratic", 0.0032f);

    set_uniform("pointLights[3].position", glm::vec3(-120, 5, -100));
    set_uniform("pointLights[3].ambient", glm::vec3(0.05f, 0.00f, 0.00f));
    set_uniform("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    set_uniform("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    set_uniform("pointLights[3].constant", 0.01f);
    set_uniform("pointLights[3].linear", 0.09f);
    set_uniform("pointLights[3].quadratic", 0.0032f);

    set_uniform("pointLights[4].position", glm::vec3(-200, 5, -200));
    set_uniform("pointLights[4].ambient", glm::vec3(0.05f, 0.00f, 0.00f));
    set_uniform("pointLights[4].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    set_uniform("pointLights[4].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    set_uniform("pointLights[4].constant", 0.0001f);
    set_uniform("pointLights[4].linear", 0.0009f);
    set_uniform("pointLights[4].quadratic", 0.0032f);
    
    node_t scene = lightScene.node;
    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{ scene, scene.transform }}};
    while (!renderables.empty()) {
        const auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;
        xform *= glm::translate(glm::mat4(1.0), node.translation);
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.z, glm::vec3(0, 0, 1));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.y, glm::vec3(0, 1, 0));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.x, glm::vec3(1, 0, 0));
        xform *= glm::scale(glm::mat4(1.0), node.scale);

        switch (node.kind) {
            default:
                draw_withLight(renderer, p, v, xform, node.obj);
                //draw_model(renderer, renderer.view * xform, node.obj);
                break;
        }

        for (const node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }
}

void render_hdr(renderer_t &renderer, GLuint colorBuffer, camera_t cam, GLFWwindow *win){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    glUseProgram(renderer.prog_hdr);
    //exposure = ((fabs(cam.pos.x) + fabs(cam.pos.y))/40.0f);
    if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS) {
        hdr = true;
    } else if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS) {
        hdr = false;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glUniform1i(glGetUniformLocation(renderer.prog_hdr, "hdr"), hdr);
    glUniform1f(glGetUniformLocation(renderer.prog_hdr, "exposure"), exposure);
    renderQuad();       
}

void render_kernal(renderer_t &renderer, GLuint colorBuffer, camera_t cam, GLFWwindow *win){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    glUseProgram(renderer.prog_kernal);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    // glUniform1i(glGetUniformLocation(renderer.prog_hdr, "hdr"), hdr);
    // glUniform1f(glGetUniformLocation(renderer.prog_hdr, "exposure"), exposure);
    renderQuad();       
}

void render_blur(renderer_t &renderer, GLuint colorBuffer, camera_t cam, GLFWwindow *win){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    glUseProgram(renderer.prog_blur);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    // glUniform1i(glGetUniformLocation(renderer.prog_hdr, "hdr"), hdr);
    // glUniform1f(glGetUniformLocation(renderer.prog_hdr, "exposure"), exposure);
    renderQuad();       
}


void render_low(renderer_t &renderer, const camera_t &cam, const node_t &scene) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    renderer.view = get_view(cam);

    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{ scene, scene.transform }}};
    while (!renderables.empty()) {
        const auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;
        xform *= glm::translate(glm::mat4(1.0), node.translation);
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.z, glm::vec3(0, 0, 1));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.y, glm::vec3(0, 1, 0));
        xform *= glm::rotate(glm::mat4(1.0), node.rotation.x, glm::vec3(1, 0, 0));
        xform *= glm::scale( glm::mat4(1.0), node.scale);

        switch (node.kind) {
            case node_t::STATIC_MESH:
                draw_model(renderer, renderer.view * xform, node.obj);
                break;
            case node_t::REFLECTIVE:
                break;
            case node_t::PORTAL:
                break;
            default:
                draw_model(renderer, renderer.view * xform, node.obj);
                break;
        }

        for (const node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }
}

GLuint quadVAO = 0;
GLuint quadVBO;
void renderQuad (){
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}