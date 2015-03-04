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
    vec2 camAngle = (0.0f, 0.0f); //vec2 to store x and y pos of camera angle.

    ///this function is responsible for moving the camera based on mouse position
    void move_camera(int x, int y, HWND *w)
    {
      static bool is_mouse_moving = true;

      if (is_mouse_moving){
        int vx, vy;
        get_viewport_size(vx, vy);
        float dx = x - vx * 0.5f;
        float dy = y - vy * 0.5f;

        //apply the deltaX and deltaY of the mouse to the camera angles.
        const float sensitivity = -0.5f;
        camAngle.x() += dx * sensitivity;
        camAngle.y() += dy * sensitivity;
        is_mouse_moving = false;

        //set the position of the mouse to the center of the window
        tagPOINT p;
        p.x = vx * 0.5f;
        p.y = vy * 0.5f;
        ClientToScreen(*w, &p);
        SetCursorPos(p.x, p.y);
      }
      else
      {
        is_mouse_moving = true;
      }
    }

  public:
    /// this is called when we construct the class before everything is initialised.
    ghost(int argc, char **argv) : app(argc, argv) {
    }
    ~ghost(){
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      //hide the cursor
      ShowCursor(false);
      //mouse_look_helper.init(this, 200.0f / 360.0f, false);
      fps_helper.init(this);
      //set gravity for the world
      //world->setGravity(btVector3(0.0f, 0.0f, 0.0f));

      app_scene = new visual_scene();

      
      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      dynarray<resource*> meshes;
      dict.find_all(meshes, atom_mesh);

      if (meshes.size()) {     
        mesh *player_ship = meshes[0]->get_mesh();
        mat4t location;
        location.translate(vec3(0, 200, 0));
        material *mat = new material(new image("assets/playerShip_uv.jpg"));
        app_scene->add_shape(location, player_ship, mat, true);
        player_node = app_scene->get_mesh_instance(0)->get_node();
      }

      //mesh_instance *mi = app_scene->add_shape(
      //   mat,


      //  )

      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->get_node()->translate(vec3(0, 4, 0));
      the_camera->set_far_plane(10000);

      inputs.init(this);

      material *red = new material(vec4(0, 1, 1, 1));
      mesh_box *box = new mesh_box(vec3(100, 1, 100));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, box, red));
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      scene_node *camera_node = the_camera->get_node();
      mat4t &camera_to_world = camera_node->access_nodeToParent();
      //mouse_look_helper.update(camera_to_world);

      camera_to_world.loadIdentity();
      camera_to_world.rotateY(camAngle.x());
      camera_to_world.rotateX(camAngle.y());

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
