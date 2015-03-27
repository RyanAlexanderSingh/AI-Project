
////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
// This class just creates a player mesh and returns the player node -> could probably merge with other enemy mesh creations....
//
#ifndef PLAYER_CONTROLLER_H_INCLUDED
#define PLAYER_CONTROLLER_H_INCLUDED
namespace octet {

  class player_controller : public resource {

    ships playerSpaceShip;

    app *the_app;
    visual_scene *app_scene;

    ref<scene_node> player_node;
    //camera instance
    ref<scene_node> the_camera;

    float ship_orientation = 0.0f;
    const float acceleration = 10.0f;
    const float angle_iteration = 0.03f;
    bool alternate_camera = true;

  public:
    player_controller(){}

    //return the scene_node of the player
    scene_node *return_player_node(){
      return player_node;
    }

    //get the players orientation -> used for the drawing OpenGL things
    float get_orientation(){
      return ship_orientation;
    }

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      playerSpaceShip.init(the_app, app_scene);
      //create the player mesh and scene node
      player_node = playerSpaceShip.create_player();
      the_camera = app_scene->get_camera_instance(0)->get_node();
    }

    void update(){

      update_camera();
      player_node->set_damping(0.5f, 0.0f);
      player_node->clamp_linear_velocity(30);

      float friction = 0.0f;
      if (the_app->is_key_down('A')) {
        rotate_player(player_node, angle_iteration);
      }
      else if (the_app->is_key_down('D')) {
        rotate_player(player_node, -angle_iteration);
      }
      if (the_app->is_key_down('W')) {
        player_node->apply_central_force(player_node->get_z() * (acceleration));
      }
      else if (the_app->is_key_down('S')) {
        player_node->apply_central_force(player_node->get_z() * (-acceleration));
      }
      else if (the_app->is_key_down('Q')) {
        player_node->activate();
        player_node->apply_central_force(player_node->get_x() * (-acceleration));
      }
      else if (the_app->is_key_down('E')) {
        player_node->activate();
        player_node->apply_central_force(player_node->get_x() * (acceleration));
      }
      if (the_app->is_key_going_down(' ')){
      }
      else {
        friction = 1.0f;
      }
      player_node->set_friction(friction);

      if (the_app->is_key_down(key_esc)){
        exit(1);
      }

      playerSpaceShip.statusCircle(1.0f, 10.0f);
    }

    //update the camera
    void update_camera(){

      player_node->add_child(the_camera);
      the_camera->loadIdentity();

      //change the camera type (behind ship or above ship)
      if (the_app->is_key_going_down('P')){
        alternate_camera = !alternate_camera;
      }

      //behind ship
      if (alternate_camera){
        the_camera->translate(vec3(0.0f, 80.0f, -130.0f));
        the_camera->access_nodeToParent().rotateY(180);
        the_camera->access_nodeToParent().rotateX(-20);
      }
      //above ship
      else{
        the_camera->translate(vec3(0.0f, 300.0f, 0.0f));
        the_camera->access_nodeToParent().rotateY(180.0f);
        the_camera->access_nodeToParent().rotateX(-90.0f);
      }
    }

    void rotate_player(scene_node *ship_node, float angle){
      ship_node->activate();
      btTransform trans = ship_node->get_rigid_body()->getCenterOfMassTransform();
      btQuaternion transrot = trans.getRotation();
      //create a new quaternion with an angle on y
      btQuaternion rotquat(btVector3(0, 1, 0), angle);
      ship_orientation = (ship_orientation + angle);
      if (ship_orientation > PI) ship_orientation -= 2 * PI;
      if (ship_orientation < 0) ship_orientation += 2 * PI;
      transrot = transrot * rotquat;
      trans.setRotation(transrot);
      ship_node->get_rigid_body()->setCenterOfMassTransform(trans);
    }


    ~player_controller() {
    }
  };
}

#endif
