#include "RhalgrNetwork.hpp"

#include <iostream>
#include <vector>
#include <thread>

#include "PlatformSpecificUtils.h"

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
    std::cout << "Cleaning up RhalgrNetwork" << std::endl;
    stop();
  }
}

bool RhalgrNetwork::start()
{
  bool status = false;
  status = start_interface(); 
  if (status)
  {
    start_capture();
  }
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
  operation_mode = "Halted";
  run_status_mutex.lock();
  running = false;
  run_status_mutex.unlock();
  if (interface_opened)
  {
    interface_opened = false;
  }
}

bool RhalgrNetwork::set_parse_interval(double seconds)
{
  bool status = seconds > 0;
  if (status)
  {
    interval_mutex.lock();
    this->parse_interval = seconds;
    interval_mutex.unlock();
  }
return status;
}

double RhalgrNetwork::get_interval()
{
  interval_mutex.lock();
  return this->parse_interval;
  interval_mutex.unlock();
}

std::string RhalgrNetwork::get_operation_mode()
{
  return this->operation_mode;
}

std::map<std::string, double> RhalgrNetwork::get_participants()
{
  return this->participants;
}

double RhalgrNetwork::get_participant_dps(std::string name, bool* found)
{
  double result = -1;
  std::map<std::string, double>::iterator it = participants.find(name);
  if (it != participants.end())
  {
    result = participants.at(name);
    if (nullptr != found)
    {
      *found = true;
    }
  }
  else
  {
    if (nullptr != found)
    {
      *found = false;
    }
  }
  return result;
}

std::string RhalgrNetwork::get_interface_ip_addr()
{
  return this->ip_addr;
}

/** BEGIN PRIVATE FUNCTIONS **/

bool RhalgrNetwork::setup_interface()
{
  bool status = false;
  if (!if_name.empty())
  {
    pcpp::PcapLiveDevice* temp_device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(if_name);
    if (temp_device != nullptr)
    {
      network_interface = temp_device;
      status = true;
    }
  }
  return status;
}

bool RhalgrNetwork::start_interface()
{
  if (setup_interface())
  {
    interface_opened = network_interface->open();
    if (interface_opened)
    {
      ip_addr = network_interface->getIPv4Address().toString();
    }
    else
    {
      ip_addr = "255.255.255.255";
    }
  }
  return interface_opened;
}

void RhalgrNetwork::start_capture()
{
  if (interface_opened && !running)
  {
    operation_mode = "Capturing";
    std::thread capture_thread(&RhalgrNetwork::run_capture_loop, this);
    capture_thread.detach();
  }
  else
  {
    std::cerr << "WARNING: Trying to start capture when one is already running. Ignoring request." << std::endl;
  }
}

void RhalgrNetwork::run_capture_loop()
{
  run_status_mutex.lock();
  running = interface_opened;
  while(running)
  {
    run_status_mutex.unlock();
    std::cout << "TICK" << std::endl;
    PCAP_SLEEP(1);
    run_status_mutex.lock();
  }
  run_status_mutex.unlock();
}
