#ifndef __RHALGR_NETWORK_H__
#define __RHALGR_NETWORK_H__

#include <string>
#include <map>
#include <chrono>
#include <future>
#include <vector>
#include "PcapLiveDeviceList.h"
#include "SkillCodes.hpp"
#include "StatusEffectCodes.hpp"
#include "TcpLayer.h"
#include "DataConversion.hpp"
#include "RhalgrMemory.hpp" // needed to obtain certain info from game process
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

constexpr unsigned char game_event_code = 3;
constexpr unsigned int server_keepalive = 0x039b;
constexpr unsigned int client_keepalive = 0x023d;
constexpr unsigned int weapon_sheathe_toggle = 0x8b;
constexpr unsigned int movement_event = 0x212;
constexpr unsigned int camera_rotation = 0x1a3;
constexpr unsigned int crafting_event = 0x179;
constexpr unsigned int class_change = 0x1d7;
constexpr unsigned int remote_player_class_change = 0x199;
constexpr unsigned int cast_finish = 0x22a;
constexpr unsigned int friend_online_resp = 0x6e;
constexpr unsigned int talk_npc = 0x12d;
constexpr unsigned int say_chat_send = 0x2f4;
constexpr unsigned int perform_play_note = 0x15e;
constexpr unsigned int player_attack = 0x0102;
constexpr unsigned int skill_cast = 0x175;
constexpr unsigned int change_target = 0x02c0;
constexpr unsigned int request_playtime = 0x01a8;
constexpr unsigned int playtime_request_response = 0x02be;
constexpr unsigned int actor_cast = 0x02a7;
constexpr unsigned int ability_cast_8_man = 0x0345;
constexpr unsigned int remote_player_status_effect = 0x0151;
constexpr unsigned int client_update_search_info = 0x0219;

constexpr unsigned int send_tell = 0x0064;
constexpr unsigned int send_tell_message_offset = 51;
constexpr unsigned int send_tell_recipient_name_offset = 19;
constexpr unsigned int send_tell_failed = 0x0066;

constexpr unsigned int party_message = 0x0065;
constexpr unsigned int party_join_disband = 0x006e;

constexpr unsigned int attack_damage_offset = 48;

const std::string ffxiv_proc_name("ffxiv_dx11.exe");

enum connection_type
{
  LOBBY, ZONE, CHAT, UNKNOWN
};

enum perform_notes
{
  C = 0x2a, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B, C_HIGH
};
constexpr unsigned char perform_note_position = 1;


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
    void start_mem_read();
    void run_capture_loop();
    void run_mem_loop();
    void get_participant_info(pcpp::RawPacketVector& data);
    void parse_tcp_packet(pcpp::TcpLayer* layer);
    bool is_ffxiv_packet(uint8_t* p_payload);
    long long timestamp_from_payload(uint8_t* payload);
    unsigned int length_from_payload(uint8_t* payload);
    connection_type type_from_payload(uint8_t* payload);
    unsigned short count_from_payload(uint8_t* payload);
    bool is_payload_compressed(uint8_t* payload);
    void compressed_frame_to_payload_data(uint8_t* payload, unsigned int size, std::vector<char>& data);
    void set_data_from_payload(uint8_t* payload, unsigned int size, std::vector<char>& data);
    FFXIV_Body populate_frame_data(FFXIV_Frame& frame);
    void parse_frame_data(FFXIV_Body &body);
    void handle_job_swap(const char* swap_data, const uint32_t length);
    void handle_perform_note_play(char* perform_data);
    void parse_skill_info(const char* skill_data, const uint32_t data_len);
    void parse_cast_end(const char* cast_data, const uint32_t data_len);
    void handle_playtime_response(const char* playtime_data, const uint32_t data_len);
    void handle_send_tell(const char* tell_data, const uint32_t data_len);
    void handle_send_tell_failure(const char* tell_data, const uint32_t data_len);
    void handle_remote_player_status_effect(const char* attack_data, const uint32_t data_len);
    void handle_ability8_packet(const char* ability_data, const uint32_t data_len);
    void update_player_dps(const char* attack_data, const uint32_t data_len);
    std::string operation_mode = "Idle";
    std::map<std::string, double> participants;
    double parse_interval = 1.0; // How long before DPS calculation occurs.    
    bool running;
    bool interface_opened;
    bool mem_cap_running;
    std::mutex run_status_mutex;
    std::mutex interval_mutex;
    std::string if_name;
    std::string ip_addr;
    pcpp::PcapLiveDevice* network_interface;

    // Mem reading functions
    RhalgrMemory* mem_reader;
};

#endif
