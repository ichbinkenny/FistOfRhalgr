//
// Created by kenny on 8/6/21.
//

#include "RhalgrMemory.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <iostream>

RhalgrMemory::RhalgrMemory()
{
  RhalgrMemory(-1);
}

RhalgrMemory::RhalgrMemory(int process_id)
{
  this->pid = process_id;
  attach_to_process(process_id);
}

RhalgrMemory::RhalgrMemory(std::string process_name)
{
  this->pid = find_process_id(process_name);
  attach_to_process(process_name);
}

RhalgrMemory::~RhalgrMemory()
{
  if (proc_file_open)
  {
    // TODO Clean up memory use
    proc_stream.close();
  }
  this->id_map_valid = false;
}

bool RhalgrMemory::set_output_id_map(std::map<unsigned int, std::string>* character_id_map)
{
  this->id_map = character_id_map;
  return id_map == nullptr;
}

bool RhalgrMemory::attach_to_process(std::string proc_name)
{
  bool status = false;
  // Try to find the process ID
  int proc_id = find_process_id(proc_name);
  if (proc_id != -1)
  {
    std::cout << "FOUND PROCESS ID: " << std::to_string(proc_id) << std::endl;
    status = attach_to_process(proc_id);
  }
  return status;
}

bool RhalgrMemory::attach_to_process(int proc_id)
{
  // open proc ifstream
  std::stringstream ss;
  ss << "/proc/" << proc_id;
  proc_stream.open(ss.str().c_str());
  if (proc_stream.is_open())
  {
    proc_file_open = true;
    pid = proc_id;
  }
  else
  {
    proc_file_open = false;
  }
  return proc_file_open;
}

bool RhalgrMemory::process_found()
{
  return proc_file_open;
}

/**
 * Scans a total list of processes and places them
 * in an internal map of ids to name.
 */
void RhalgrMemory::populate_process_map()
{

}

/**
 * Searches for a process id from a provided name
 * @param proc_name The process name to look for.
 * @return the process id if found, or -1.
 */
int RhalgrMemory::find_process_id(std::string proc_name)
{
  int proc_id = -1;
  if (process_list_populated)
  {
    // We've previously scanned proc.
    // Look for the id in the map first
    // to save time.
    auto proc_info = process_map.find(proc_name);
    if (proc_info != process_map.end())
    {
      proc_id = proc_info->second;
    }
  }
  if (proc_id == -1)
  {
    // either the process map didn't contain this proc_name,
    // or we are still unpopulated in process. We won't scan
    // to populate processes, but will look until we find this
    // proc_name.
    proc_id = scan_proc_dir_for_name(proc_name);
  }

  return proc_id;
}

int RhalgrMemory::scan_proc_dir_for_name(std::string proc_name)
{
  int proc_id = -1;
  DIR* dp = opendir("/proc");
  if (nullptr != dp)
  {
    dirent* p_dir_ent;
    while (proc_id < 0 && (p_dir_ent = readdir(dp)))
    {
      int id = atoi(p_dir_ent->d_name);
      if (id > 0)
      {
        std::stringstream ss;
        ss << std::string("/proc/") << p_dir_ent->d_name << "/comm";
        std::string path(ss.str());
        std::ifstream proc_cmd_file(path.c_str());
        if (!proc_cmd_file.is_open())
        {
          continue;
        }
        std::string line;
        std::getline(proc_cmd_file, line);
        if (!line.empty())
        {
          if (line == proc_name)
          {
            proc_id = id;
          }
        }
      }
    }
  }
  closedir(dp);
  return proc_id;
}

std::string RhalgrMemory::id_to_name(unsigned int character_id)
{
  std::string character_name("Unknown Player");
  if (nullptr != id_map)
  {
    auto item = id_map->find(character_id);
    if (item != id_map->end())
    {
      character_name = item->second;
    }
  }
  return character_name;
}

void RhalgrMemory::lock_data()
{
  id_lock.lock();
}

void RhalgrMemory::unlock_data()
{
  id_lock.unlock();
}
