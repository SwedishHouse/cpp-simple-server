#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#include <string>
#include <unistd.h>

const std::string pid_info = "pid_info";

class PIDManager {
public:
    void writePIDToFile(const std::string& filename, int value);
    pid_t readPIDFromFile(const std::string& filename);
};

#endif // PID_MANAGER_H