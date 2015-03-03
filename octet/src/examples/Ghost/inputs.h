////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Inputs class
//

#ifndef INPUTS_H_INCLUDED
#define INPUTS_H_INCLUDED
namespace octet {

  ///Class to create a vehicle using hinge constraints.
  ///Class will create a vehicle out of rigid bodies. The vehicle consists of a chassis, 4 axils and 4 wheels.
  ///They are attached using hinge constraints. There are 2 hinge constraint relationships, Chassis-Axils and Axils-Wheels.
  class inputs : public resource {

  public:

    inputs()
    {
    }

    app *the_app;
    void init(app *app){
      this->the_app = app;
    }

    void update(mat4t &camera){
      //check for keyboard inputs
      keyboard_inputs();
      //mouse inputs
      mouse_inputs();
    }

    void mouse_inputs(){
      
    }

    void keyboard_inputs(){
      //exit the program
      if (the_app->is_key_down(key_esc)){
        exit(1);
      }
    }


    //check if we need deconstructor
    ~inputs() {
    }
  };
}
#endif
