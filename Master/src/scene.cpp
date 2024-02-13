#include <ass3/scene.hpp>
#include <ass3/cubemap.hpp>
#include <ass3/model.hpp>

const char *EXTERIOR_PATH = "res/obj/exterior/exterior.obj";
const char *EXTERIOR_PORTAL_PATH = "res/obj/exterior/exterior_portal.obj";
const char *TABLE_PATH = "res/obj/more/table/console_table.obj";
const char *INTERIOR_PATH = "res/obj/interior/interior.obj";
const char *INTERIOR_PORTAL_PATH = "res/obj/interior/interior_portal.obj";
const char* SKYBOX_BASE_PATH = "res/skybox/sky";
const char *CEILING_FAN_PATH = "res/obj/more/fan/Ceiling Fan.obj";
const char *TANK_PATH = "res/obj/more/tank/IS4.obj";
const char *BED_PATH = "res/obj/more/bed/bed.obj";
const char *WIFE_PATH = "res/obj/more/wife/Female.obj";
const char *TOY_PATH = "res/obj/more/toybear/teddy3.obj";
const char *GUITAR_PATH = "res/obj/more/guitar/10367_AcousticGuitar_v01_it2.obj";
const char *BLOOD_PATH = "res/obj/more/blood/blood.obj";
const char *CRUTH_PATH = "res/obj/more/crutch/Crutch.obj";
const char *BAG_PATH = "res/obj/more/bag/10130_BagPurse_v3_L3.obj";
const char *GUN_PATH = "res/obj/more/pistol/pistol_tauros.obj";
const char *DOOR_PATH = "res/obj/more/door/Door1obj.obj";



node_t external_tank ();
node_t external_tank_2 ();
node_t wooden_table ();
node_t make_ceiling_fan ();
node_t wooden_bed ();
node_t wife_body ();
node_t toy_bear ();
node_t make_guitar () ;
node_t make_blood () ;
node_t make_scene_room_reflec(GLuint window_texture);

node_t make_scene_interior(GLuint portal_texture) {
    node_t scene;

    node_t interior;
    interior.kind = node_t::STATIC_MESH;
    interior.obj = load_obj(INTERIOR_PATH);

    node_t fan;
    fan = external_tank();

    node_t interior_portal;
    interior_portal.kind = node_t::PORTAL;
    interior_portal.obj = load_obj(INTERIOR_PORTAL_PATH);
    interior_portal.texture = portal_texture;

    scene.children.push_back(interior);
    scene.children.push_back(interior_portal);
    scene.children.push_back(fan);

    return scene;
}

node_t make_scene_exterior(GLuint portal_texture) {
    node_t scene;

    node_t exterior;
    exterior.kind = node_t::STATIC_MESH;
    exterior.transform = glm::scale(glm::mat4(1.0), glm::vec3(2.0));

    exterior.obj = load_obj(EXTERIOR_PATH);

    node_t tank;
    tank = external_tank();
    tank.translation = 
        glm::vec3(0, 0, -25);
	tank.scale = glm::vec3(3.0f);
	
	node_t tank2;
    tank2 = external_tank_2();
    tank2.translation = 
        glm::vec3(100, 0, -50);
	tank2.scale = glm::vec3(3.0f);

	node_t tank4;
    tank4 = external_tank_2();
    tank4.translation = 
        glm::vec3(-45, 0, 35);
	tank4.scale = glm::vec3(3.0f);

	node_t tank5;
    tank5 = external_tank_2();
    tank5.translation = 
        glm::vec3(45, 0, -50);
	tank5.scale = glm::vec3(3.0f);

	node_t tank6;
    tank6 = external_tank_2();
    tank6.translation = 
        glm::vec3(45, 0, 35);
	tank6.scale = glm::vec3(3.0f);
	
	node_t tank3;
    tank3 = external_tank_2();
    tank3.translation = 
         glm::vec3(35, 0, 0);
	tank3.scale = glm::vec3(3.0f);

    node_t tank7;
    tank7 = external_tank();
    tank7.translation = 
        glm::vec3(75, 0, -25);
	tank7.scale = glm::vec3(3.0f);

    node_t tank8;
    tank8 = external_tank();
    tank8.translation = 
        glm::vec3(0, 0, 0);
	tank8.scale = glm::vec3(3.0f);

    node_t tank9;
    tank9 = external_tank();
    tank9.translation = 
        glm::vec3(25, 0, -25);
	tank9.scale = glm::vec3(3.0f);

    node_t tank10;
    tank10 = external_tank();
    tank10.translation = 
        glm::vec3(50, 0, 25);
	tank10.scale = glm::vec3(3.0f);

    scene.children.push_back(exterior);
    scene.children.push_back(tank);
	scene.children.push_back(tank2);
	scene.children.push_back(tank3);
	scene.children.push_back(tank4);
	scene.children.push_back(tank5);
	scene.children.push_back(tank6);
	scene.children.push_back(tank7);
	scene.children.push_back(tank8);
	scene.children.push_back(tank9);
	scene.children.push_back(tank10);

    return scene;
}

node_t make_scene_exterior_nowall(GLuint portal_texture) {
    node_t scene;

    node_t wall_group;

    node_t wall_front;
    wall_front.kind = node_t::STATIC_MESH;
    wall_front.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_front.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, -10));
    wall_group.children.push_back(wall_front);

    node_t wall_back;
    wall_back.kind = node_t::STATIC_MESH;
    wall_back.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_back.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, 10));
    wall_group.children.push_back(wall_back);
    
    node_t wall_left;
    wall_left.kind = node_t::STATIC_MESH;
    wall_left.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_left.transform =
            glm::translate(glm::mat4(1), glm::vec3(-10, 5, 0));
    wall_group.children.push_back(wall_left);
    
    node_t wall_right;
    wall_right.kind = node_t::STATIC_MESH;
    wall_right.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_right.transform =
            glm::translate(glm::mat4(1), glm::vec3(10, 5, 0));
    wall_group.children.push_back(wall_right);

    node_t floor;
    floor.kind = node_t::STATIC_MESH;
    floor.obj = make_volume(10,1,10,"res/textures/room/wood_floor.jpg");
    floor.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    wall_group.children.push_back(floor);

    node_t ceiling;
    ceiling.kind = node_t::STATIC_MESH;
    ceiling.obj = make_volume(10,1,10,"res/textures/room/ceiling.jpg");
    ceiling.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 10, 0));
    wall_group.children.push_back(ceiling);

	node_t bed = wooden_bed();
    bed.transform =
        glm::translate(glm::mat4(1), glm::vec3(-2, 1.0, -2))*glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
    wall_group.children.push_back(bed);
    
    node_t table = wooden_table();
    table.transform =
        glm::translate(glm::mat4(1), glm::vec3(5, 0, 8))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    floor.children.push_back(table);

	node_t wife = wife_body();
    wife.transform =
        glm::translate(glm::mat4(1), glm::vec3(5, 1.5, 3))*glm::scale(glm::mat4(1.0), glm::vec3(0.028f));
	wife.rotation.x = glm::radians(90.0f);
    wall_group.children.push_back(wife);

	node_t blood = make_blood();
    blood.transform =
        glm::translate(glm::mat4(1), glm::vec3(5, 1.2, 5))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    scene.children.push_back(blood);

	node_t gun;
    gun.kind = node_t::STATIC_MESH;
    gun.obj = load_obj(GUN_PATH);
	gun.transform =
        glm::translate(glm::mat4(1), glm::vec3(3, 1.0, 3))*glm::scale(glm::mat4(1.0), glm::vec3(0.03f));
	gun.rotation.x = glm::radians(90.0f);
	scene.children.push_back(gun);




    node_t window;
    window.kind = node_t::STATIC_MESH;
    window.obj = make_volume(1.00,2,2,"res/textures/room/window.jpg");
    window.transform =
        glm::translate(glm::mat4(1), glm::vec3(9.9, 5, 2));
    wall_group.children.push_back(window);


    node_t window_withLight;
    window_withLight.kind = node_t::STATIC_MESH;
    window_withLight.obj = make_volume(1.00,2,2,"res/textures/room/window.jpg");
    window_withLight.transform =
        glm::translate(glm::mat4(1), glm::vec3(-9.9, 5, 2));
    wall_group.children.push_back(window_withLight);

    node_t ceiling_fan = make_ceiling_fan();
    ceiling_fan.ani_kind = node_t::SPINNING_FAN;
    ceiling_fan.transform =
        glm::translate(glm::mat4(1), glm::vec3(0, 0, 0))*glm::scale(glm::mat4(1.0), glm::vec3(0.01f));
    wall_group.children.push_back(ceiling_fan);

    node_t decorate_metal_ball_1;
    decorate_metal_ball_1.kind = node_t::REFLECTIVE;
    //decorate_metal_ball_1.ani_kind = node_t::DECORATE_BALL;
    decorate_metal_ball_1.obj = generate_sphere();
    decorate_metal_ball_1.transform =
            glm::translate(glm::mat4(1), glm::vec3(-10, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_1.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_2;
    decorate_metal_ball_2.kind = node_t::REFLECTIVE;
    decorate_metal_ball_2.obj = generate_sphere();
    decorate_metal_ball_2.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_2.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_3;
    decorate_metal_ball_3.kind = node_t::REFLECTIVE;
    decorate_metal_ball_3.obj = generate_sphere();
    //decorate_metal_ball_3.obj = make_volume(1,1,1,"res/textures/room/wood_wall.jpg");
    decorate_metal_ball_3.transform =
            glm::translate(glm::mat4(1), glm::vec3(5, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_3.texture = make_cubemap(SKYBOX_BASE_PATH);
    //decorate_metal_ball_3.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_4;
    decorate_metal_ball_4.kind = node_t::REFLECTIVE;
    //decorate_metal_ball_4.ani_kind = node_t::DECORATE_BALL;
    decorate_metal_ball_4.obj = generate_sphere();
    decorate_metal_ball_4.transform =
            glm::translate(glm::mat4(1), glm::vec3(20, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_4.texture = make_cubemap(SKYBOX_BASE_PATH);


    table.children.push_back(decorate_metal_ball_1);
    table.children.push_back(decorate_metal_ball_2);
    table.children.push_back(decorate_metal_ball_3);
    table.children.push_back(decorate_metal_ball_4);



    scene.children.push_back(wall_group);
    scene.children.push_back(table);
    return scene;
}

node_t make_scene_room(GLuint window_texture, node_t room, framebuffer_t frame, GLuint window_withLight_texture) {
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.001, 1000.0));
    node_t scene;

    node_t wall_group;

    node_t wall_front;
    wall_front.kind = node_t::STATIC_MESH;
    wall_front.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_front.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, -10));
    wall_group.children.push_back(wall_front);

    node_t wall_back;
    wall_back.kind = node_t::STATIC_MESH;
    wall_back.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_back.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, 10));
    wall_group.children.push_back(wall_back);
    
    node_t wall_left;
    wall_left.kind = node_t::STATIC_MESH;
    wall_left.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_left.transform =
            glm::translate(glm::mat4(1), glm::vec3(-10, 5, 0));
    wall_group.children.push_back(wall_left);
    
    node_t wall_right;
    wall_right.kind = node_t::STATIC_MESH;
    wall_right.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_right.transform =
            glm::translate(glm::mat4(1), glm::vec3(10, 5, 0));
    wall_group.children.push_back(wall_right);

    node_t floor;
    floor.kind = node_t::STATIC_MESH;
    floor.obj = make_volume(10,1,10,"res/textures/room/wood_floor.jpg");
    floor.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    wall_group.children.push_back(floor);

    node_t ceiling;
    ceiling.kind = node_t::STATIC_MESH;
    ceiling.obj = make_volume(10,1,10,"res/textures/room/ceiling.jpg");
    ceiling.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 10, 0));
    wall_group.children.push_back(ceiling);

	node_t bed = wooden_bed();
    bed.transform =
        glm::translate(glm::mat4(1), glm::vec3(-2, 1.0, -2))*glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
    wall_group.children.push_back(bed);
    
    node_t table = wooden_table();
    table.transform =
        glm::translate(glm::mat4(1), glm::vec3(5, 0, 8))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    floor.children.push_back(table);

	node_t wife = wife_body();
    wife.transform =
        glm::translate(glm::mat4(1), glm::vec3(-2, 2.5, -2))*glm::scale(glm::mat4(1.0), glm::vec3(0.028f));
	wife.rotation.x = glm::radians(-90.0f);
    wall_group.children.push_back(wife);

    node_t window;
    window.kind = node_t::PORTAL;
    window.obj = make_volume(1.00,2,2,"res/textures/room/wood_wall.jpg");
    window.transform =
        glm::translate(glm::mat4(1), glm::vec3(9.9, 5, 2));
    window.texture = window_texture;
    wall_group.children.push_back(window);


    node_t window_withLight;
    window_withLight.kind = node_t::PORTAL;
    window_withLight.obj = make_volume(1.00,2,2,"res/textures/room/wood_wall.jpg");
    window_withLight.transform =
        glm::translate(glm::mat4(1), glm::vec3(-9.9, 5, 2));
    window_withLight.texture = window_withLight_texture;
    wall_group.children.push_back(window_withLight);

    node_t ceiling_fan = make_ceiling_fan();
    ceiling_fan.ani_kind = node_t::SPINNING_FAN;
    ceiling_fan.transform =
        glm::translate(glm::mat4(1), glm::vec3(0, 0, 0))*glm::scale(glm::mat4(1.0), glm::vec3(0.01f));
    wall_group.children.push_back(ceiling_fan);

    node_t decorate_metal_ball_1;
    decorate_metal_ball_1.kind = node_t::REFLECTIVE;
    //decorate_metal_ball_1.ani_kind = node_t::DECORATE_BALL;
    decorate_metal_ball_1.obj = generate_sphere();
    decorate_metal_ball_1.transform =
            glm::translate(glm::mat4(1), glm::vec3(-10, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_1.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_2;
    decorate_metal_ball_2.kind = node_t::REFLECTIVE;
    decorate_metal_ball_2.obj = generate_sphere();
    decorate_metal_ball_2.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_2.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_3;
    decorate_metal_ball_3.kind = node_t::REFLECTIVE_DYNAMIC;
    decorate_metal_ball_3.obj = generate_sphere();
    //decorate_metal_ball_3.obj = make_volume(1,1,1,"res/textures/room/wood_wall.jpg");
    decorate_metal_ball_3.transform =
            glm::translate(glm::mat4(1), glm::vec3(5, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_3.texture = make_dynamic_room_skybox(glm::vec3(4.5,5,7.5), room, renderer, frame);
    //decorate_metal_ball_3.texture = make_cubemap(SKYBOX_BASE_PATH);

    node_t decorate_metal_ball_4;
    decorate_metal_ball_4.kind = node_t::REFLECTIVE;
    //decorate_metal_ball_4.ani_kind = node_t::DECORATE_BALL;
    decorate_metal_ball_4.obj = generate_sphere();
    decorate_metal_ball_4.transform =
            glm::translate(glm::mat4(1), glm::vec3(20, 35, 0))*glm::scale(glm::mat4(1.0), glm::vec3(2.7f));
    decorate_metal_ball_4.texture = make_cubemap(SKYBOX_BASE_PATH);


    table.children.push_back(decorate_metal_ball_1);
    table.children.push_back(decorate_metal_ball_2);
    table.children.push_back(decorate_metal_ball_3);
    table.children.push_back(decorate_metal_ball_4);



    scene.children.push_back(wall_group);
    scene.children.push_back(table);
    return scene;
}

node_t make_scene_room_reflec(GLuint window_texture) {
    node_t scene;

    node_t wall_group;

    node_t wall_front;
    wall_front.kind = node_t::STATIC_MESH;
    wall_front.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_front.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, -10));
    wall_group.children.push_back(wall_front);

    node_t wall_back;
    wall_back.kind = node_t::STATIC_MESH;
    wall_back.obj = make_volume(10,5,1,"res/textures/room/wood_wall.jpg");
    wall_back.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, 10));
    wall_group.children.push_back(wall_back);
    
    node_t wall_left;
    wall_left.kind = node_t::STATIC_MESH;
    wall_left.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_left.transform =
            glm::translate(glm::mat4(1), glm::vec3(-10, 5, 0));
    wall_group.children.push_back(wall_left);
    
    node_t wall_right;
    wall_right.kind = node_t::STATIC_MESH;
    wall_right.obj = make_volume(1,5,10,"res/textures/room/wood_wall.jpg");
    wall_right.transform =
            glm::translate(glm::mat4(1), glm::vec3(10, 5, 0));
    wall_group.children.push_back(wall_right);

    node_t floor;
    floor.kind = node_t::STATIC_MESH;
    floor.obj = make_volume(10,1,10,"res/textures/room/wood_floor.jpg");
    floor.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    wall_group.children.push_back(floor);

    node_t ceiling;
    ceiling.kind = node_t::STATIC_MESH;
    ceiling.obj = make_volume(10,1,10,"res/textures/room/ceiling.jpg");
    ceiling.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 10, 0));
    wall_group.children.push_back(ceiling);
    
    node_t table = wooden_table();
    table.transform =
        glm::translate(glm::mat4(1), glm::vec3(5, 0, 8))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

    floor.children.push_back(table);

    node_t window;
    window.kind = node_t::PORTAL;
    window.obj = make_volume(1.00,2,2,"res/textures/room/wood_wall.jpg");
    window.transform =
        glm::translate(glm::mat4(1), glm::vec3(9.9, 5, 2));
    window.texture = window_texture;
    wall_group.children.push_back(window);

    node_t ceiling_fan = make_ceiling_fan();
    ceiling_fan.ani_kind = node_t::SPINNING_FAN;
    ceiling_fan.transform =
        glm::translate(glm::mat4(1), glm::vec3(0, 0, 0))*glm::scale(glm::mat4(1.0), glm::vec3(0.01f));
    wall_group.children.push_back(ceiling_fan);

    scene.children.push_back(wall_group);
    scene.children.push_back(table);
    return scene;
}


lightScene_t make_scene_forest() {
    lightScene_t lightScene;
    node_t scene;

    node_t floor;
    floor.kind = node_t::STATIC_MESH;
    floor.obj = make_volume(1000,1,1000,"res/textures/room/wood_floor.jpg");
    floor.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    scene.children.push_back(floor);

	node_t toybear = toy_bear();
    toybear.transform =
        glm::translate(glm::mat4(1), glm::vec3(-20, 1, -10))*glm::scale(glm::mat4(1.0), glm::vec3(0.03f));
	toybear.rotation.x = glm::radians(-90.0f);
    scene.children.push_back(toybear);

	node_t guitar = make_guitar();
    guitar.transform =
        glm::translate(glm::mat4(1), glm::vec3(-40, 2, -35))*glm::scale(glm::mat4(1.0), glm::vec3(0.005f));
	guitar.rotation.x = glm::radians(-30.0f);
    scene.children.push_back(guitar);

	node_t blood = make_blood();
    blood.transform =
        glm::translate(glm::mat4(1), glm::vec3(-80, 1.2, -60))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    scene.children.push_back(blood);

	node_t crutch;
    crutch.kind = node_t::STATIC_MESH;
    crutch.obj = load_obj(CRUTH_PATH);
	crutch.transform =
        glm::translate(glm::mat4(1), glm::vec3(-78, 1.5, -58))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    crutch.rotation.x = glm::radians(-90.0f);
	scene.children.push_back(crutch);

	node_t bag;
    bag.kind = node_t::STATIC_MESH;
    bag.obj = load_obj(BAG_PATH);
	bag.transform =
        glm::translate(glm::mat4(1), glm::vec3(-82, 1.5, -62))*glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    bag.rotation.x = glm::radians(-90.0f);
	bag.rotation.z = glm::radians(90.0f);
	scene.children.push_back(bag);

	node_t gun;
    gun.kind = node_t::STATIC_MESH;
    gun.obj = load_obj(GUN_PATH);
	gun.transform =
        glm::translate(glm::mat4(1), glm::vec3(-120, 2.0, -100))*glm::scale(glm::mat4(1.0), glm::vec3(0.08f));
	gun.rotation.z = glm::radians(-60.0f);
	scene.children.push_back(gun);

	node_t door;
    door.kind = node_t::STATIC_MESH;
    door.obj = load_obj(DOOR_PATH);
	door.transform =
        glm::translate(glm::mat4(1), glm::vec3(-200, 5.5, -200))*glm::scale(glm::mat4(1.0), glm::vec3(0.7f));
	door.rotation.x = glm::radians(-90.0f);
	scene.children.push_back(door);


    lightScene.node = scene;

    return lightScene;
}


node_t wooden_table () {
    node_t table;
    table.kind = node_t::STATIC_MESH;
    table.obj = load_obj(TABLE_PATH);

    return table;
}

node_t wooden_bed () {
    node_t bed;
    bed.kind = node_t::STATIC_MESH;
    bed.obj = load_obj(BED_PATH);

    return bed;
}

node_t wife_body () {
    node_t wife;
    wife.kind = node_t::STATIC_MESH;
    wife.obj = load_obj(WIFE_PATH);

    return wife;
}

node_t toy_bear () {
    node_t toy;
    toy.kind = node_t::TOY;
    toy.obj = load_obj(TOY_PATH);

    return toy;
}

node_t make_guitar () {
    node_t guitar;
    guitar.kind = node_t::STATIC_MESH;
    guitar.obj = load_obj(GUITAR_PATH);

    return guitar;
}

node_t make_blood () {
    node_t blood;
    blood.kind = node_t::STATIC_MESH;
    blood.obj = load_obj(BLOOD_PATH);

    return blood;
}

node_t make_ceiling_fan () {
    node_t ceiling_fan;
    ceiling_fan.kind = node_t::FAN;
    ceiling_fan.obj = load_obj(CEILING_FAN_PATH);

    return ceiling_fan;

}
node_t external_tank () {
    node_t tank;
    tank.ani_kind = node_t::TANK;
    tank.obj = load_obj(TANK_PATH);

    return tank;
}

node_t external_tank_2 () {
    node_t tank;
    tank.ani_kind = node_t::TANK2;
    tank.obj = load_obj(TANK_PATH);

    return tank;
}