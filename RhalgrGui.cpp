#include "RhalgrGui.hpp"

using namespace Rhalgr;

Gui::Gui()
{
  if(glfwInit())
  {
    window = glfwCreateWindow(640, 480, "Fist of Rhalgr", nullptr, nullptr);
    if (window)
    {
      glfw_initialized = true;
    }
  }
}

Gui::~Gui()
{
  if(window)
  {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

void Gui::Show()
{
  glfwMakeContextCurrent(window);
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
  glfwSwapInterval(1);
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
