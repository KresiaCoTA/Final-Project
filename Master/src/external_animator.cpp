#include <ass3/external_animator.hpp>

#include <iostream>

#include <cmath>
#include <stack>

#include <ass3/cubemap.hpp>

// Repeat each animation every 10 seconds
const float PERIOD = 10.f;

/**
 * Following De Casteljau's algorithm, after linearly interpreting the 4 control points, we end up with this cubic eqn.
 * Easily derivable by hand.
 * The coefficients come from the binomial equation, but the nomenclature also calls these "Bernstein polynomials"
 * If you know Pascal's Triangle, it is easy to compute, though.
 *
 * @param p A vector of 4 control points
 * @param t The current interpolation value. Must be between 0 and 1.
 * @return The point along the Bezier curve at time t.
 */
glm::vec3 cubic(const std::vector<glm::vec3> &p, float t) {
    float s = 1 - t;
    auto pos = (s * s * s * p[0]) + (3 * t * s * s * p[1]) + (3 * t * t * s * p[2]) + (t * t * t * p[3]);
    return pos;
}

/**
 * Calculate a transformation matrix based upon a Bezier cubic.
 *
 * @param bspline The Bezier curve to use.
 * @param now The time in seconds
 * @return A transformation matrix
 */
glm::mat4 animate(const bspline_t &bspline, float now) {
    // clamps between 0 and 10.
    double cycle = std::fmod(now, PERIOD);

    // [0, 5) is the first segment, [5, 10) is the second
    const std::vector<glm::vec3> &segment = cycle < PERIOD / 2.f ? bspline.s1 : bspline.s2;

    // clamps between 0 and 1
    float t = 2 * std::fmod(cycle, PERIOD/2) / PERIOD;

    // get the translation value
    auto tr = cubic(segment, (float)t);

    return glm::translate(glm::mat4(1), tr);
}

glm::vec3 oscillation_x(float scope, float now, float dt) {
    const float SPEED = scope;
    float step = SPEED*dt;
    // clamps between 0 and 10.
    float cycle = std::fmod(now, 2.0f);

    float shift = 1.0f - cycle;
    if (shift <= 0){
        step = 0;
    } else if (shift <= 0.5f && shift > 0){
        step = step;
    } else {
        step = -step;
    }

    return glm::vec3(step, 0, 0);
}

glm::vec3 oscillation_x_1(float scope, float now, float dt) {
    const float SPEED = scope;

    float step = SPEED*dt;
    // clamps between 0 and 10.
    float cycle = std::fmod(now, 2.0f);

    float shift = 1.0f - cycle;
    if (shift >= 0){
        step = 0;
    } else if (shift >= -0.5f && shift < 0){
        step = step;
    } else {
        step = -step;
    }

    return glm::vec3(step, 0, 0);
}



/**
 * Calculate a transformation matrix using a parameterised lemniscate.
 * Here mainly for comparison with the corresponding BSpline.
 * @param lemni A tag type to represent a lemniscate
 * @param now The time now in seconds
 * @return A transformation matrix
 */
glm::mat4 animate(const lemniscate_t &lemni, float now) {
    auto t = now;

    auto denom = 1 + std::sin(t) * std::sin(t);
    auto a = 7.5f;
    auto x = (a * a * std::cos(t)) / denom;
    auto y = 0;
    auto z = (a * a * std::sin(t) * std::cos(t)) / denom;

    return glm::translate(glm::mat4(1), glm::vec3{ x, y, z });
}


animator_t make_animator() {
    std::vector<glm::vec3> s1 = {
            { 0,    0, 0 },
            { 1.85, 0, 1.5 },
            { 1.85, 0, -1.5 },
            { 0,    0, 0 },
    };

    std::vector<glm::vec3> s2 = {
            { 0,     0, 0 },
            { -1.85, 0, 1.5 },
            { -1.85, 0, -1.5 },
            { 0,     0, 0 }
    };

    /*
     * This is a correction factor since the original control points assume a smaller curve
     * This is not the only way to do this; alternatively, one could calculate new control points.
     * For thr purposes of demonstration, this way is obviously quicker to show
     */
    float scale = 40.f;
    for (glm::vec3 &cp : s1) {
        cp *= scale;
    }

    for (glm::vec3 &cp : s2) {
        cp *= scale;
    }


    return animator_t{ bspline_t{ s1, s2 }, lemniscate_t{ }};
}

void animate_external(const animator_t &animator, node_t &scene, float now, float dt) {
    std::stack<node_t *> animatables{{ &scene }};

    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();
        
        switch (n->ani_kind) {
            case node_t::TANK:
                n->transform = animate(animator.spline_animation, now);
                break;
            case node_t::TANK2:
                n->transform = animate(animator.lemniscate_animation, now);
                break;
            default:
                break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}

void animate_room(const animator_t &animator, node_t &scene, float now, float dt) {
    std::stack<node_t *> animatables{{ &scene }};

    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();
        
        switch (n->ani_kind) {
            case node_t::SPINNING_FAN:
                n->rotation.y += glm::radians(-50.0f) * dt;
                break;
            case node_t::DECORATE_BALL_1:
                n->translation += oscillation_x(5.0f, now, dt);
                break;
            case node_t::DECORATE_BALL_2:
                n->translation += oscillation_x_1(5.0f, now, dt);
                break;
            default:
                break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}

void update_reflection(node_t &scene, node_t reflection, renderer_t renderer, framebuffer_t frame) {
    std::stack<node_t *> animatables{{ &scene }};

    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();
        
        switch (n->kind) {
            case node_t::REFLECTIVE_DYNAMIC:
                glDeleteTextures(1, &n->texture);
                n->texture = make_dynamic_room_skybox(glm::vec3(4.5,5,7.5), reflection, renderer, frame);
                break;
            default:
                break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}

