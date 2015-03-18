////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Ship controller class
//

#ifndef SHIP_CONTROL_H_INCLUDED
#define SHIP_CONTROL_H_INCLUDED
namespace octet {

  ///Class to create a vehicle using hinge constraints.
  ///Class will create a vehicle out of rigid bodies. The vehicle consists of a chassis, 4 axils and 4 wheels.
  ///They are attached using hinge constraints. There are 2 hinge constraint relationships, Chassis-Axils and Axils-Wheels.
  class ship_controls : public resource {

    app *the_app;
    visual_scene *vs;
    float angle_iteration = 0.03f;
    float acceleration = 0.0f;
    float power = 2.0f;

  public:
    ship_controls(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->vs = vs;
    }

    //called every frame to update the player physics
    void update(scene_node *player_node, scene_node *camera_node){
      player_node->set_damping(0.5f, 1.0f);
      player_node->clamp_linear_velocity(10);

      float friction = 0.0f;
      if (the_app->is_key_down('A')) {
        rotate(player_node, angle_iteration);
      }
      else if (the_app->is_key_down('D')) {
        rotate(player_node, -angle_iteration);
      }
      if (the_app->is_key_down('W')) {
        if (acceleration < 10.0f){
          acceleration += power;
        }
        accelerate(player_node, acceleration);
      }
      else if (the_app->is_key_down('S')) {
        if (acceleration > -10.0f){
          acceleration -= power;
        }
        accelerate(player_node, acceleration);
      }
      else if (the_app->is_key_down('Q')) {
        if (acceleration > -10.0f){
          acceleration -= power;
        }
        player_node->activate();
        player_node->apply_central_force(player_node->get_x() * (-acceleration));
      }
      else if (the_app->is_key_down('E')) {
        if (acceleration > -10.0f){
          acceleration -= power;
        }
        player_node->activate();
        player_node->apply_central_force(player_node->get_x() * (acceleration));
      }
      if (the_app->is_key_going_down(' ')){
        fire_projectile(player_node);
      }

      else {
        friction = 1.0f;
      }
      player_node->set_friction(friction);

      player_node->add_child(camera_node);
      camera_node->loadIdentity();
      camera_node->translate(vec3(0.0f, 40.0f, -50.0f));
      camera_node->access_nodeToParent().rotateY(180);
      camera_node->access_nodeToParent().rotateX(-40);

      if (the_app->is_key_down(key_esc)){
        exit(1);
      }
    }

    void fire_projectile(scene_node *ship_node){
      vec3 forward_vec = ship_node->get_position();
      material *blue = new material(vec4(0, 0, 1, 1));
      mesh_sphere *sphere = new mesh_sphere(vec3(0, 0, 0), 0.2f);
      mat4t location;
      forward_vec.z() += 20.0f;
      location.translate(forward_vec);
      //location.rotateY(forward_vec.z());
      vs->add_shape(location, sphere, blue, true);
    }

    void rotate(scene_node *ship_node, float angle){
      ship_node->activate();
      btTransform trans = ship_node->get_rigid_body()->getCenterOfMassTransform();
      btQuaternion transrot = trans.getRotation();
      btQuaternion rotquat;
      rotquat = rotquat.getIdentity();
      rotquat.setY(angle);

      transrot = rotquat * transrot;
      trans.setRotation(transrot);
      ship_node->get_rigid_body()->setCenterOfMassTransform(trans);
    }


    //used to control the player but could also be used to control the AI ship movements
    void accelerate(scene_node *ship_node, float accel){
      ship_node->activate();
      ship_node->apply_central_force(ship_node->get_z() * (accel));
    }
    //check if we need deconstructor
    ~ship_controls() {}
  };
}
#endif
