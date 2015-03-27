
////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
// This class just creates a player mesh and returns the player node -> could probably merge with other enemy mesh creations....
//
#ifndef AI_BEHAVIOURS_H_INCLUDED
#define AI_BEHAVIOURS_H_INCLUDED

namespace octet {

  class ai_behaviours : public resource {

    // helper for drawing text
    ref<text_overlay> overlay;

    // text mesh object for overlay.
    ref<mesh_text> text;

    //variables for wandering
    float wandertheta = 0.0f;

    //for wandering
    float currentShipAngle = 0.0f;

    //counters for AI
    int numOfMercs = 0;
    int numOfCivilians = 0;
    int numOfBosses = 0;
    
    int deadMercs = 0;
    int deadCivilians = 0;
    int deadBoss = 0;

  public:
    ai_behaviours(){}

    //predefined to pass back random numbers bettwen -0.15f and 0.15f
    float random_float() {
      float rand_num = 0;
      rand_num = rand() % 10 - 5; //gives me a number between -10 and 10;
      rand_num = rand_num / 100;

      float r3 = -0.15f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.15f - -0.15f)));
      return r3;
    }

    //figure out the arctangent of a vector
    //return the degrees to rotate object by
    float angle_to_target(float x, float z){
      float Angle = atan2(x, z);
      float degreesToRotate = Angle - currentShipAngle;
      currentShipAngle = Angle;
      return degreesToRotate;
    }

    ///map_values allows the mapping of two values (used to set the acceleration depending on position)
    float map_values(float x, float in_min, float in_max, float out_min, float out_max){
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    //adds to the counters
    void bossDead(){
      deadBoss++;
    }
    void civilianDead(){
      deadCivilians++;
    }
    void mercsDead(){
      deadMercs++;
    }

    void init(){
      srand(static_cast <unsigned> (time(0)));

      // create the overlay
      overlay = new text_overlay();

      // get the defualt font.
      bitmap_font *font = overlay->get_default_font();

      // create a box containing text (in pixels)
      aabb bb(vec3(-550.5f, 400.0f, 0.0f), vec3(256, 64, 0));
      text = new mesh_text(font, "", &bb);

      // add the mesh to the overlay.
      overlay->add_mesh_text(text);
    }

    void number_of_agents(int civilians, int mercs, int bosses){
      numOfCivilians = civilians;
      numOfMercs = mercs;
      numOfBosses = bosses;
    }

    void update_text(int vx, int vy, int deadCivilians, int deadMercs, int deadBoss, int freedCivilians){
      text->clear();
      text->format("Number of Civilian Ships: %i (Alive: %i Dead: %i Freed by Player: %i) \n" 
      "Number of Merc Ships: %i (Alive: %i Dead: %i) \n" 
      "Number of Boss Ships: %i (Alive: %i Dead: %i)", numOfCivilians, numOfCivilians - deadCivilians - freedCivilians, deadCivilians, freedCivilians,
      numOfMercs, numOfMercs - deadMercs, deadMercs, numOfBosses, numOfBosses - deadBoss, deadBoss);
      // convert it to a mesh.
      text->update();
      // draw the text overlay
      overlay->render(vx, vy);
    }

    //Basic wandering behaviours
    void wander(scene_node *ship_node, float speed = 4.0f){
      vec3 target = (0.0f, 0.0f, 0.0f);
      //basic "keep within the boundaries" wandering
      float ship_x = ship_node->get_position().x();
      float ship_z = ship_node->get_position().z();
      if (ship_x > 1000.0f || ship_z > 1000.0f || ship_x < -1000.0f || ship_z < -1000.0f){
        target.x() = -ship_x;
        target.z() = -ship_z;
      }
      else{
        float wander_radius = 5.0f;         // radius for our "wander circle"
        float wander_distance = 17.0f;         // distance for our "wander circle
        wandertheta += random_float(); //get a random float back between 

        vec3 circleloc = ship_node->get_z(); //get forward vector
        circleloc.normalize();
        circleloc *= wander_distance; //multiply by distance 
        circleloc += ship_node->get_position(); //make it relative to the ship nodes location

        vec3 forwardVec = ship_node->get_z(); //get forward vector again

        float heading = atan2(forwardVec.x(), forwardVec.z()); //need to know heading to offset wandertheta
        vec3 circleOffSet = (wander_radius * cos(wandertheta + heading), 0.0f, wander_radius * sin(wandertheta + heading));
        target = circleloc + circleOffSet; //our final target to aim for
      }

      vec3 facingVector = target - ship_node->get_position();
      float angle = angle_to_target(facingVector.x(), facingVector.z());

      rotate(ship_node, angle);
      accelerate(ship_node, speed);
    }

    //Basic seek behaviours (change to arrive)
    void seek(scene_node *ship_node, scene_node *target_ship, float speed = 4.0f){
      //get the distance vector to the target
      vec3 distanceVector = target_ship->get_position() - ship_node->get_position();
      //rotate to angle
      float angle = angle_to_target(distanceVector.x(), distanceVector.z());
      rotate(ship_node, angle);
      accelerate(ship_node, speed);
    }

    //Basic flee behaviours, the opposite of seek
    void flee(scene_node *ship_node, scene_node *enemy){

      vec3 oppositeVec = ship_node->get_position() - enemy->get_position();
      float angle = angle_to_target(oppositeVec.x(), oppositeVec.z());
      rotate(ship_node, angle);
      accelerate(ship_node, 5.0f);
    }

    //flock to a target -- civilians will flock to the player ship (proceded by leader following...to add later)
    void flock(scene_node *ship_node, scene_node *flock_target){

      vec3 thetaVelocity = flock_target->get_z() * -1.0f; //inverse the velocity vector
      thetaVelocity = normalize(thetaVelocity) * 10.0f; // normalize a scale by distance float
      vec3 followBehind = thetaVelocity + flock_target->get_position(); //make it relative (add to character pos)
      vec3 distanceVector = followBehind - ship_node->get_position();

      float distance = distanceVector.length();
      float acceleration = map_values(distance, 0.0f, 20.0f, 0.0f, 7.0f);
      acceleration = acceleration < 0.3f ? 0.0f : acceleration;
     
      float angle = angle_to_target(distanceVector.x(), distanceVector.z());
      rotate(ship_node, angle);
      accelerate(ship_node, acceleration);
    }

    void rotate(scene_node *ship_node, float angle){
      ship_node->activate();
      btTransform trans = ship_node->get_rigid_body()->getCenterOfMassTransform();
      btQuaternion transrot = trans.getRotation();
      //create a new quaternion with an angle on y
      btQuaternion rotquat(btVector3(0, 1, 0), angle);
      transrot = transrot * rotquat;
      trans.setRotation(transrot);
      ship_node->get_rigid_body()->setCenterOfMassTransform(trans);
    }

    //used to control the player but could also be used to control the AI ship movements
    void accelerate(scene_node *ship_node, float accel){
      ship_node->activate();
      ship_node->apply_central_force(ship_node->get_z() * (accel));
    }

    ~ai_behaviours() {
    }
  };
}

#endif
