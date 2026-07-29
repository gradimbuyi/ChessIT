#pragma once

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

class EngineProcess {
public:
    explicit EngineProcess(const std::string &expath);
    ~EngineProcess();

    bool Start();
    void Stop();
    bool IsRunning() const;

    void SendCommand(const std::string &cmd);
    bool WaitFor(const std::string &expected);
    std::string ReadLine();

private:    
    std::string m_expath;
    
    int   m_stdinfd  = -1;
    int   m_stdoutfd = -1;
    pid_t m_pid      = -1; 
};