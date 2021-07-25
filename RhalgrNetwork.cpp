#include "RhalgrNetwork.hpp"

#include <vector>

RhalgrNetwork::RhalgrNetwork()
{
  network_interface = nullptr;
  participants.clear();
  this->running = false;
}

RhalgrNetwork::RhalgrNetwork(std::string if_name) : RhalgrNetwork()
{
  this->if_name = if_name;
}

RhalgrNetwork::RhalgrNetwork(std::string if_name, bool auto_start) : RhalgrNetwork(if_name)
{
  this->running = auto_start ? start(if_name) : false;
}

RhalgrNetwork::~RhalgrNetwork()
{
  if(running)
  {
    stop();
  }
}

bool RhalgrNetwork::start()
{
  bool status = false;

  return status;
}

bool RhalgrNetwork::start(std::string if_name)
{
  this->if_name = if_name;
  return start();
}

bool RhalgrNetwork::is_active()
{
  return this->running;
}

void RhalgrNetwork::stop()
{
  this->operation_mode = "Halted";
  this->running = false;
}

bool RhalgrNetwork::set_parse_interval(double seconds)
{
  bool status = seconds != 0;
  if (status)
  {
    this->parse_interval = seconds;
  }
  return status;
}

double RhalgrNetwork::get_interval()
{
  return this->parse_interval;
}

std::string RhalgrNetwork::get_operation_mode()
{
  return this->operation_mode;
}

std::map<std::string, double> RhalgrNetwork::get_participants()
{
  return this->participants;
}

double RhalgrNetwork::get_participant_dps(std::string name, bool& found)
{
  double result = -1;
  std::map<std::string, double>::iterator it = participants.find(name);
  if (it != participants.end())
  {
    result = participants.at(name);
  }
  return result;
}



