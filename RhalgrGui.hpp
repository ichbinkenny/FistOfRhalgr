#ifndef __RHALGR_GUI_H__
#define __RHALGR_GUI_H__

#include <glad/glad.h>  
#include <GLFW/glfw3.h>

namespace Rhalgr
{
  class Gui
  {
    public:
      Gui();
      ~Gui();
      void Show();
    private:
      bool glfw_initialized = false;
      GLFWwindow* window;
  };
};

#endif
