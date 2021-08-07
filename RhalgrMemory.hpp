
#ifndef __H_RHALGR_MEMORY__
#define __H_RHALGR_MEMORY__

/**
 * This Class handles reading values from FFXIV
 * memory process.
 * Currently it only supports Linux, but should
 * eventually be ported to windows and mac.
 */

#include <linux/ptrace.h>
#include <string>
#include <map>
#include <mutex>
#include <fstream>

class RhalgrMemory {
public:
    RhalgrMemory();
    explicit RhalgrMemory(int process_id);
    explicit RhalgrMemory(std::string process_name);
    bool set_output_id_map(std::map<unsigned int, std::string>* id_map);
    bool attach_to_process(std::string proc_name);
    bool attach_to_process(int proc_id);
    bool process_found();
    std::string id_to_name(unsigned int character_id);
    ~RhalgrMemory();
    void lock_data();
    void unlock_data();
private:
    int pid;
    std::mutex id_lock;
    bool mem_opened;
    bool id_map_valid;
    bool process_list_populated;
    bool proc_file_open;
    std::map<unsigned int, std::string>* id_map;
    std::map<std::string, unsigned int> process_map;
    std::ifstream proc_stream;

    // Functions
    void populate_process_map();
    int find_process_id(std::string proc_name);
    int scan_proc_dir_for_name(std::string proc_name);
};


#endif
