#include "RhalgrNetwork.hpp"

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <sstream>
#include <zlib.h>

#include "PlatformSpecificUtils.h"

RhalgrNetwork::RhalgrNetwork()
{
  network_interface = nullptr;
  participants.clear();
  this->running = false;
  mem_reader = new RhalgrMemory(ffxiv_proc_name);
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
  delete mem_reader;
}

bool RhalgrNetwork::start()
{
  bool status = false;
  status = start_interface(); 
  if (status)
  {
    start_mem_read();
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
  mem_reader->lock_data();
  mem_cap_running = false;
  mem_reader->unlock_data();
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
  auto it = participants.find(name);
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
  pcpp::RawPacketVector dataVector;
  network_interface->startCapture(dataVector);
  run_status_mutex.lock();
  running = interface_opened;
  while(running)
  {
    run_status_mutex.unlock();
    interval_mutex.lock();
    PCAP_SLEEP(parse_interval);
    interval_mutex.unlock();
    get_participant_info(dataVector);
    run_status_mutex.lock();
  }
  run_status_mutex.unlock();
  network_interface->stopCapture();
}

void RhalgrNetwork::start_mem_read()
{
  if (mem_reader->process_found())
  {
    std::thread mem_read_thread(&RhalgrNetwork::run_mem_loop, this);
    mem_read_thread.detach();
  }
}

void RhalgrNetwork::run_mem_loop()
{
  mem_cap_running = true;
  mem_reader->lock_data();
  while (mem_cap_running)
  {
    mem_reader->unlock_data();

    mem_reader->lock_data();
  }
}

void RhalgrNetwork::get_participant_info(pcpp::RawPacketVector& data)
{
  for(pcpp::RawPacketVector::ConstVectorIterator it = data.begin(); it != data.end(); it++)
  {
    pcpp::Packet packet(*it);
    pcpp::TcpLayer* tcp_layer = packet.getLayerOfType<pcpp::TcpLayer>();
    if (nullptr != tcp_layer)
    {
      parse_tcp_packet(tcp_layer); 
    }
  }
  data.clear();
}

void RhalgrNetwork::parse_tcp_packet(pcpp::TcpLayer* layer)
{
  auto payload = layer->getLayerPayload();
  size_t payload_size = layer->getLayerPayloadSize();
  if (payload_size >= 40)
  {
    if (is_ffxiv_packet(payload))
    {
      FFXIV_Frame frame;
      frame.timestamp_ms = timestamp_from_payload(payload);
      frame.length = length_from_payload(payload);
      frame.type = type_from_payload(payload);
      frame.count = count_from_payload(payload);
      frame.is_compressed = is_payload_compressed(payload);
      if (frame.is_compressed)
      {
        // We need to decompress this frame
        // Everything I have seen uses zlib for this purpose
        compressed_frame_to_payload_data(payload, frame.length, frame.data);
      }
      set_data_from_payload(payload, frame.length, frame.data);
      FFXIV_Body body = populate_frame_data(frame);
      std::cout << "Layer size: " << std::to_string(payload_size) << " Body Size: " << body.header.length << std::endl;
      // TODO Multiple op_code can be in one common message
      if (body.header.segment_type == game_event_code)
      {
        parse_frame_data(body);
      }
    }
  }
}

bool RhalgrNetwork::is_ffxiv_packet(uint8_t* p_payload)
{
  bool valid = false;
  for (int i = 0; i < 8; i++)
  {
    valid = p_payload[i] == FFXIV_MAGIC[i];
    if (!valid)
    {
      break;
    }
  }
  return valid;
}

long long RhalgrNetwork::timestamp_from_payload(uint8_t* payload)
{
  long long time = *reinterpret_cast<long long*>(&payload[timestamp_offset]);
  return time;
}

unsigned int RhalgrNetwork::length_from_payload(uint8_t* payload)
{
  unsigned int len = *reinterpret_cast<unsigned int*>(&payload[length_offset]);
  return len;
}

connection_type RhalgrNetwork::type_from_payload(uint8_t* payload)
{
  connection_type type = connection_type(*reinterpret_cast<unsigned short*>(&payload[type_offset]));
  return type;
}

unsigned short RhalgrNetwork::count_from_payload(uint8_t* payload)
{
  unsigned short count = *reinterpret_cast<unsigned short*>(&payload[type_offset]); 
  return count;
}

bool RhalgrNetwork::is_payload_compressed(uint8_t* payload)
{
  bool compressed = payload[compression_offset] == true;
  return compressed;
}

void RhalgrNetwork::compressed_frame_to_payload_data(uint8_t* payload, unsigned int size, std::vector<char>& data)
{
  // Setup inflation state
  uLong compressed_size = size;
}

void RhalgrNetwork::set_data_from_payload(uint8_t* payload, unsigned int size, std::vector<char>& data)
{
  if (size != 0)
  {
    for (unsigned int i = 0; i != size; ++i)
    {
      data.push_back(payload[i]);
    }
    //std::reverse(data.begin(), data.end());
  }
}

FFXIV_Body RhalgrNetwork::populate_frame_data(FFXIV_Frame &frame)
{
  FFXIV_Body body;
  char* data = &frame.data[header_size];
  body.header.length = *reinterpret_cast<unsigned int*>(&data[0]);
  body.header.action_performer_id = *reinterpret_cast<unsigned int*>(&data[performer_offset]);
  body.header.action_target_id = *reinterpret_cast<unsigned int*>(&data[target_offset]);
  body.header.segment_type = *reinterpret_cast<unsigned short*>(&data[segment_offset]);
  body.op_code = *reinterpret_cast<unsigned short*>(&data[op_code_offset]);
  body.server_id = *reinterpret_cast<unsigned short*>(&data[server_id_offset]);
  body.timestamp = *reinterpret_cast<unsigned long long*>(&data[body_timestamp_offset]);
  body.data = &data[ffxiv_body_len];
  return body;
}

void RhalgrNetwork::parse_frame_data(FFXIV_Body &body)
{
  if (body.op_code != server_keepalive && body.op_code != client_keepalive && body.op_code != 612 && body.op_code != 528 && body.op_code != 0xfc && body.op_code != weapon_sheathe_toggle && body.op_code != movement_event && body.op_code != 0x314 && body.op_code != camera_rotation && body.op_code != 0x3a3 && body.op_code != 0x2cb)
  {
    if (body.op_code == crafting_event)
    {
    }
    else if (body.op_code == class_change || body.op_code == remote_player_class_change)
    {
      std::cout << "Action performer: " << std::hex << body.header.action_performer_id << std::endl;
      std::cout << "Action recipient: " << std::hex << body.header.action_target_id << std::endl;
      handle_job_swap(body.data, body.header.length);
    }
    else if (body.op_code == cast_finish)
    {
      parse_cast_end(body.data, body.header.length);
    }
    else if (body.op_code == talk_npc)
    {

    }
    else if (body.op_code == say_chat_send)
    {
    
    }
    else if (body.op_code == perform_play_note)
    {
      handle_perform_note_play(body.data);
    }
    else if (body.op_code == player_attack)
    {
      update_player_dps(body.data, body.header.length);
    }
    else if (body.op_code == skill_cast)
    {
      parse_skill_info(body.data, body.header.length);
    }
    else if (body.op_code == change_target)
    {

    }
    else if (body.op_code == request_playtime)
    {
      // Probably can ignore the request
    }
    else if (body.op_code == playtime_request_response)
    {
      handle_playtime_response(body.data, body.header.length);
    }
    else if (body.op_code == send_tell)
    {
      handle_send_tell(body.data, body.header.length);
    }
    else if (body.op_code == send_tell_failed)
    {
      handle_send_tell_failure(body.data, body.header.length);
    }
    else if (body.op_code == remote_player_status_effect)
    {
      handle_remote_player_status_effect(body.data, body.header.length);
    }
    else if (body.op_code == ability_cast_8_man)
    {
      handle_ability8_packet(body.data, body.header.length);
    }
    else if (body.op_code != server_keepalive && body.op_code != client_keepalive && body.op_code != 0x127)
    {
      std::cout << "OP CODE: " << std::hex << body.op_code << " unhandled" << std::endl;
    }
    else
    { //std::cout << std::hex << "OP CODE: " << body.op_code << std::endl;  
    }
  }
  
}

// All data handling functions below here

void RhalgrNetwork::handle_job_swap(const char* swap_data, const uint32_t length)
{
  for (unsigned int i = 0; i < length; i++)
  {
    std::cout << std::hex << static_cast<uint8_t>(swap_data[i]) << " ";
  }
  std::cout << std::endl;
}

void RhalgrNetwork::handle_perform_note_play(char* p_data)
{
  switch (perform_notes(p_data[perform_note_position]))
  {
    case C:
      std::cout << "C" << std::endl;
      break;
    case Db:
      std::cout << "C#" << std::endl;
      break;
    case D:
      std::cout << "D" << std::endl;
      break;
    case Eb:
      std::cout << "Eb" << std::endl;
      break;
    case E:
      std::cout << "E" << std::endl;
      break;
    case F:
      std::cout << "F" << std::endl;
      break;
    case Gb:
      std::cout << "F#" << std::endl;
      break;
    case G:
      std::cout << "G" << std::endl;
      break;
    case Ab:
      std::cout << "Ab" << std::endl;
      break;
    case A:
      std::cout << "A" << std::endl;
      break;
    case Bb:
      std::cout << "Bb" << std::endl;
      break;
    case B:
      std::cout << "B" << std::endl;
      break;
    case C_HIGH:
      std::cout << "C 8va" << std::endl;
      break;
  }
}

void RhalgrNetwork::parse_skill_info(const char* skill_data, const uint32_t length)
{
  // According to the Sapphire project and wireshark
  // the skill ID is at bytes 5-8
  // stored in little endian order
  unsigned int skill_id = le_bytes_to_u32(skill_data[4], skill_data[5], skill_data[6], skill_data[7]); 
  // Check for the value in the skill map 
  auto skill_info = skill_map.find(skill_id);
  if (skill_info != skill_map.end())
  {
    std::cout << "Skill Cast: " << skill_info->second << std::endl;
  }
  else
  {
    std::cout << "Unable to find skill name for entry: " << skill_id << std::endl;
  }
}

void RhalgrNetwork::parse_cast_end(const char* cast_data, const uint32_t data_len)
{
  std::cout << "Cast finished" << std::endl;
  for (uint32_t i = 0; i != data_len; ++i)
  {
    std::cout << std::to_string(cast_data[i]) << " ";
  }
  std::cout << std::endl;
}

void RhalgrNetwork::handle_playtime_response(const char* playtime_data, const uint32_t data_len)
{
  // Prints the player's playtime to the console
  uint32_t playtime = le_bytes_to_u32(playtime_data[0], playtime_data[1], playtime_data[2], playtime_data[3]);
  std::cout << "Playtime: " << std::hex << playtime << std::endl;
}

void RhalgrNetwork::handle_send_tell(const char* tell_data, const uint32_t data_len)
{
  std::stringstream ss;
  for (uint32_t i = send_tell_recipient_name_offset; i != send_tell_message_offset; i++)
  {
    ss << tell_data[i];
  }
  std::string recipient = ss.str();
  ss.str(std::string());
  for (uint32_t i = send_tell_message_offset; i != data_len; i++)
  {
    ss << tell_data[i];
  }
  std::string message = ss.str();
  std::cout << "Sending tell to: " << recipient << std::endl;
  std::cout << "Message: " << message << std::endl;
}

void RhalgrNetwork::handle_send_tell_failure(const char* tell_data, const uint32_t data_len)
{
  std::cout << "Failed to send tell. Recipient likely does not exist." << std::endl;
}

void RhalgrNetwork::handle_remote_player_status_effect(const char* effect_data, const uint32_t data_len)
{
  uint32_t player_id = le_bytes_to_u32(effect_data[4], effect_data[5], effect_data[6], effect_data[7]);
  uint32_t target_id = le_bytes_to_u32(effect_data[36], effect_data[37], effect_data[38], effect_data[39]);
  uint32_t player_current_hp = le_bytes_to_u32(effect_data[8], effect_data[9], effect_data[10], effect_data[11]);
  uint32_t player_max_hp = le_bytes_to_u32(effect_data[12], effect_data[13], effect_data[14], effect_data[15]);
  uint16_t player_current_mp = le_bytes_to_u16(effect_data[16], effect_data[17]);
  uint16_t player_current_tp = le_bytes_to_u16(effect_data[18], effect_data[19]);
  uint32_t status_id = le_bytes_to_u32(effect_data[26], effect_data[27], effect_data[28], effect_data[29]);
  std::cout << "Player: " << std::hex << player_id << " afflicts " << target_id << " with: ";
  auto status_effect_info = status_effect_map.find(status_id);
  if (status_effect_info != status_effect_map.end())
  {
    std::cout << status_effect_info->second << std::endl;
  }
  else
  {
    std::cout << "Unknown attack entry: " << status_id << std::endl;
  }
}

void RhalgrNetwork::handle_ability8_packet(const char *ability_data, const uint32_t data_len)
{
  
}

void RhalgrNetwork::update_player_dps(const char *attack_data, const uint32_t data_len)
{
  std::string player = "UNKNOWN PLAYER";
  unsigned short damage = le_bytes_to_u16(attack_data[attack_damage_offset], attack_data[attack_damage_offset + 1]);
  std::cout << "Player damage: " << std::to_string(damage) << std::endl;
}