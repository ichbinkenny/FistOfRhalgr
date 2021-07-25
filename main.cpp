#include <iostream>
#include <thread>

/**
* Begin includes for App functionality
**/
#include "RhalgrGui.hpp"
#include "RhalgrNetwork.hpp"

int main()
{
  RhalgrNetwork network_control("enp3s0", false);
  Rhalgr::Gui main_window;
  main_window.Show();
  return EXIT_SUCCESS;
}
