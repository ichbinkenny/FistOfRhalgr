#ifndef __RHALGR_NETWORK_H__
#define __RHALGR_NETWORK_H__

#include <string>
#include <map>
#include <chrono>
#include <future>
#include <vector>
#include "PcapLiveDeviceList.h"
#include "TcpLayer.h"
#include <mutex>

constexpr unsigned char FFXIV_MAGIC[8] = {0x52, 0x52, 0xA0, 0x41, 0xFF, 0x5D, 0x46, 0xE2};
constexpr char header_size = 40;
constexpr char ffxiv_body_len = 32;

constexpr unsigned char timestamp_offset = 16;
constexpr unsigned char length_offset = 24;
constexpr unsigned char type_offset = 28;
constexpr unsigned char count_offset = 30;
constexpr unsigned char compression_offset = 32;

constexpr unsigned char body_len_offset = 0;
constexpr unsigned char performer_offset = 4;
constexpr unsigned char target_offset = 8;
constexpr unsigned char segment_offset = 12;

constexpr unsigned char op_code_offset = 18;
constexpr unsigned char server_id_offset = 22;
constexpr unsigned char body_timestamp_offset = 24;

enum connection_type
{
  LOBBY, ZONE, CHAT, UNKNOWN
};

struct FFXIV_Frame
{
  unsigned char magic[8] = {0x52, 0x52, 0xA0, 0x41, 0xFF, 0x5D, 0x46, 0xE2};
  long long timestamp_ms;
  unsigned int length;
  connection_type type;
  unsigned short count;
  char fudge_factor_1;
  bool is_compressed;
  unsigned int fudge_factor_2;
  unsigned short fudge_factor_3;
  std::vector<char> data;
};

struct FFXIV_Body_Header
{
  unsigned int length;
  unsigned int action_performer_id;
  unsigned int action_target_id;
  unsigned short segment_type;
};

struct FFXIV_Body
{
  FFXIV_Body_Header header;
  unsigned short op_code;
  unsigned short server_id;
  unsigned long long timestamp;
  char* data;
};

class RhalgrNetwork
{
  public:
    RhalgrNetwork();
    RhalgrNetwork(std::string if_name);
    RhalgrNetwork(std::string if_name, bool auto_start);
    ~RhalgrNetwork();
    bool start();
    bool start(std::string if_name);
    bool set_parse_interval(double seconds);
    bool is_active();
    void stop();
    double get_interval();
    std::string get_operation_mode();
    std::map<std::string, double> get_participants();
    double get_participant_dps(std::string player_name, bool* found);
    std::string get_interface_ip_addr();
  private:
    bool setup_interface();
    bool start_interface();
    void start_capture();
    void run_capture_loop();
    void get_participant_info(pcpp::RawPacketVector& data);
    void parse_tcp_packet(pcpp::TcpLayer* layer);
    bool is_ffxiv_packet(uint8_t* p_payload);
    long long timestamp_from_payload(uint8_t* payload);
    unsigned int length_from_payload(uint8_t* payload);
    connection_type type_from_payload(uint8_t* payload);
    unsigned short count_from_payload(uint8_t* payload);
    bool is_payload_compressed(uint8_t* payload);
    void set_data_from_payload(uint8_t* payload, unsigned int size, std::vector<char>& data);
    FFXIV_Body populate_frame_data(FFXIV_Frame& frame);
    std::string operation_mode = "Idle";
    std::map<std::string, double> participants;
    double parse_interval = 1.0; // How long before DPS calculation occurs.    
    bool running;
    bool interface_opened;
    std::mutex run_status_mutex;
    std::mutex interval_mutex;
    std::string if_name;
    std::string ip_addr;
    pcpp::PcapLiveDevice* network_interface;
};

#endif
