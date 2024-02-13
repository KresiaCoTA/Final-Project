#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

#include <chicken3421/chicken3421.hpp>

#include <ass3/camera.hpp>
#include <ass3/memes.hpp>
#include <ass3/renderer.hpp>
#include <ass3/scene.hpp>
#include <ass3/framebuffer.hpp>
#include <ass3/external_animator.hpp>

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

int counter = 0;

int main() {
    chicken3421::enable_debug_output();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1280, 720, "Assignment 3"));
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_MULTISAMPLE);


    camera_t cam = make_camera({ 4.5, 5, 7.5 }, { 0, 5, -15 });
    int fb_width, fb_height;
    glfwGetFramebufferSize(win, &fb_width, &fb_height);
    framebuffer_t framebuffer = make_framebuffer(fb_width, fb_height);
    framebuffer_t framebuffer_withLight = make_framebuffer(fb_width, fb_height);
    framebuffer_t framebuffer_sharpened = make_framebuffer(fb_width, fb_height);
    framebuffer_t framebuffer_wake = make_framebuffer(fb_width, fb_height);
    framebuffer_t framebuffer_for_reflection = make_framebuffer_for_reflection(fb_width,fb_height);

    //node_t scene_interior = make_scene_interior(framebuffer.texture);
    node_t scene_exterior_moon = make_scene_exterior(framebuffer.texture);
    node_t scene_wake = make_scene_exterior_nowall(framebuffer.texture);
    node_t scene_withLight = make_scene_exterior_nowall(framebuffer_withLight.texture);
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.001, 1000.0));
    node_t scene_room_reflection = make_scene_room_reflec(framebuffer.texture);
    node_t scene_room = make_scene_room(framebuffer.texture,scene_room_reflection,framebuffer_for_reflection,framebuffer_withLight.texture);
    
    lightScene_t lightScene = make_scene_forest();
    animator_t animator = make_animator();

    bool camera_in_room = true;
    bool camera_in_moon = false;
    bool camera_in_Light = false;
    bool wake = false;

    while (!glfwWindowShouldClose(win)) {
        float dt = time_delta();
        glm::vec3 f = cam.pos + glm::vec3(get_view(cam) * glm::vec4(0,0,-0.2,0));
        if (camera_in_room && 
            f.x > 8.9f && f.x < 10.9f &&
            f.y > 3.0f && f.y < 7.0f &&
            f.z > 0.0f && f.z < 4.0f) {
            if(camera_in_room && f.x > 9.7f) {
                camera_in_room = false;
                camera_in_moon = true;
            }
        } else if (camera_in_room && 
            f.x < -8.9f && f.x > -10.9f &&
            f.y > 3.0f && f.y < 7.0f &&
            f.z > 0.0f && f.z < 4.0f) {
            if(camera_in_room && f.x < -9.7f) {
                camera_in_room = false;
                camera_in_Light = true;
            }
        } else if (camera_in_Light && 
            f.x < -195.0f && f.x > -205.0f &&
            f.z < -195.0f && f.z > -205.0f) {
            if(glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
                cam.pos = glm::vec3(4.5f, 5.0f, 7.5f);
                camera_in_room = true;
                camera_in_Light = false;
            }
        }else if (camera_in_moon) {
            if(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f) {
                cam.pos = glm::vec3(4.5f, 5.0f, 7.5f);
                camera_in_moon = false;
                wake = true;
            }
        }

        update_camera(cam, win, dt, camera_in_room||wake);

        animate_external(animator, scene_exterior_moon, time_now(), dt);
        animate_room(animator, scene_room, time_now(), dt);
        animate_room(animator, scene_room_reflection, time_now(), dt);

        update_reflection(scene_room, scene_room_reflection, renderer, framebuffer_for_reflection);

        // render to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
        glEnable(GL_CLIP_DISTANCE0);
        render(renderer, cam, scene_exterior_moon);
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_wake.fbo);
        glEnable(GL_CLIP_DISTANCE0);
        if(wake){
            render(renderer, cam, scene_wake);
        }
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_withLight.fbo);
        glEnable(GL_CLIP_DISTANCE0);
        render_withLight(renderer, cam, lightScene);
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_sharpened.fbo);
        glEnable(GL_CLIP_DISTANCE0);
        if(camera_in_moon){
            render_kernal(renderer, framebuffer.texture, cam, win);
        }
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      

        // render normal scene
        if (camera_in_room) {
            render(renderer, cam, scene_room);
        } else if (camera_in_moon) {
            render_blur(renderer, framebuffer_sharpened.texture, cam, win);
        }else if (camera_in_Light) {
            if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
                render_blur(renderer, framebuffer_withLight.texture, cam, win);
            }else{
                render_hdr(renderer, framebuffer_withLight.texture, cam, win);
            }
            
        }else if (wake) {
            if (counter < 100){
                render(renderer, cam, scene_wake);
                counter ++;
            }else {
                if (counter >= 140){
                    counter = 100;
                } else if ( counter >= 120 && counter < 140){
                    render_kernal(renderer, framebuffer_wake.texture, cam, win);
                    counter ++;
                } else if (counter >= 100 && counter < 120){
                    render_blur(renderer, framebuffer_wake.texture, cam, win);
                    counter ++;
                }
            }
        }


        glfwSwapBuffers(win);
        glfwPollEvents();

        // not entirely correct as a frame limiter, but close enough
        // it would be more correct if we knew how much time this frame took to render
        // and calculated the distance to the next "ideal" time to render and only slept that long
        // the current way just always sleeps for 16.67ms, so in theory we'd drop frames
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }

    // deleting the whole window also removes the opengl context, freeing all our memory in one fell swoop.
    chicken3421::delete_opengl_window(win);

    return EXIT_SUCCESS;
}


float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float)glfwGetTime();
}
