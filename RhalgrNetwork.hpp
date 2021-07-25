#ifndef __RHALGR_NETWORK_H__
#define __RHALGR_NETWORK_H__

#include <string>
#include <map>
#include <chrono>
#include <future>
#include "PcapLiveDeviceList.h"
#include <mutex>

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
    std::string operation_mode = "Idle";
    std::map<std::string, double> participants;
    double parse_interval = 1.0; // How long before DPS calculation occurs.    
    bool running;
    bool interface_opened;
    std::string if_name;
    pcpp::PcapLiveDevice* network_interface;
    bool setup_interface();
    std::string ip_addr;
    bool start_interface();
    void start_capture();
    void run_capture_loop();
    std::mutex run_status_mutex;
    std::mutex interval_mutex;
};

#endif
