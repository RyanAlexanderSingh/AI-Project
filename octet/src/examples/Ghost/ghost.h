////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#ifndef GHOST_H_INCLUDED
#define GHOST_H_INCLUDED

namespace octet {
  /// Scene containing a box with octet.
  class ghost : public app {
    // scene for drawing box

    inputs inputs;
    mouse_look mouse_look_helper;
    helper_fps_controller fps_helper;

    ref<scene_node> player_node;

    //camera instance
    ref<camera_instance> the_camera;

    collada_builder loader;



    // scene for drawing box
    ref<visual_scene> app_scene;

    /* PROJECTION */
    double dim = 25;
    int th = 340;
    int ph = 30;
    int fov = 25;
    double asp = 1;
    double ecX = 0;
    double ecY = 0;
    double ecZ = 0;

    dynarray<material*> skybox;

    

  public:
    /// this is called when we construct the class before everything is initialised.
    ghost(int argc, char **argv) : app(argc, argv) {
    }

    void initSkybox(void)
    {
      /*
      SKY_FRONT 0
      SKY_RIGHT 1
      SKY_LEFT 2
      SKY_BACK 3
      SKY_UP 4
      SKY_DOWN 5
      */
      //skybox[0] = new material(new image("assets/skybox/skybox_front.jpg"));
      //skybox[1] = new material(new image("assets/skybox/skybox_right.jpg"));
      //skybox[2] = new material(new image("assets/skybox/skybox_left.jpg"));
      //skybox[3] = new material(new image("assets/skybox/skybox_back.jpg"));
      //skybox[4] = new material(new image("assets/skybox/skybox_up.jpg"));
      //skybox[5] = new material(new image("assets/skybox/skybox_down.jpg"));
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      //hide the cursor
      ShowCursor(false);
      mouse_look_helper.init(this, 200.0f / 360.0f, false);
      fps_helper.init(this);

      app_scene = new visual_scene();
      app_scene->set_world_gravity(btVector3(0, 0, 0));

      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      // note that this call will dump the code below to log.txt
      dict.dump_assets(log(""));

      mesh *player_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/playerShip_uv.jpg"));
      mat4t location;
      location.translate(vec3(0, 100, 0));
      app_scene->add_shape(location, player_mesh, mat, false);
      player_node = app_scene->get_mesh_instance(0)->get_node();

      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->set_far_plane(10000);
      the_camera->get_node()->translate(vec3(0, 4, 0));

      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera.rotateZ(180);
      camera.loadIdentity();
      

      inputs.init(this);

      material *red = new material(vec4(0, 1, 1, 1));
      mesh_box *box = new mesh_box(vec3(100, 1, 100));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      location.loadIdentity();
      app_scene->add_shape(location, box, red, true, 0.0f);

      material *blue = new material(vec4(0, 0, 1, 1));
      mesh_sphere *sphere = new mesh_sphere(vec3(0, 0, 0), 0.2f);
      for (int i = -10; i <= 10; ++i) {
        for (int j = -10; j <= 10; ++j) {
          mat4t location;
          location.translate((float)i, 70, (float)j);
          app_scene->add_shape(location, sphere, blue, true);
        }
      }
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      scene_node *camera_node = the_camera->get_node();
      mat4t &camera_to_world = camera_node->access_nodeToParent();
      mouse_look_helper.update(camera_to_world);

      fps_helper.update(player_node, camera_node);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);
    
      inputs.update();

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
  }

#endif
