#include "EngineProcess.hpp"

EngineProcess::EngineProcess(const std::string &expath): m_expath(expath){}
EngineProcess::~EngineProcess() { Stop(); }

bool EngineProcess::Start() {
    int to_engine[2]; 
    int from_engine[2];

    if(pipe(to_engine) == -1) return false;
    
    if(pipe(from_engine) == -1) {
        close(to_engine[0]);
        close(to_engine[1]);
        
        return false;
    }

    m_pid = fork();

    if(m_pid < 0) return false;

    if(m_pid == 0) {
        dup2(to_engine[0],   STDIN_FILENO);
        dup2(from_engine[1], STDOUT_FILENO);

        close(to_engine[0]);
        close(to_engine[1]);
        close(from_engine[0]);
        close(from_engine[1]);

        execl(m_expath.c_str(), m_expath.c_str(), nullptr);

        std::exit(EXIT_FAILURE);
    }

    m_stdinfd  = to_engine[1];
    m_stdoutfd = from_engine[0];

    close(to_engine[0]);
    close(from_engine[1]);

    return true;
}

void EngineProcess::Stop() {
    if(m_pid == -1) return ;

    SendCommand("quit");
    waitpid(m_pid, nullptr, 0);

    close(m_stdinfd);
    close(m_stdoutfd);

    m_stdinfd  = -1;
    m_stdoutfd = -1;
    m_pid      = -1;
}

bool EngineProcess::IsRunning() const {
    if(m_pid == -1) return false;

    int     status;
    pid_t   result = waitpid(m_pid, &status, WNOHANG);
    return  result == 0;
}

void EngineProcess::SendCommand(const std::string &cmd) {
    if(!IsRunning()) return;

    std::string msg = cmd + "\n";
    write(m_stdinfd, msg.c_str(), msg.size());
}

bool EngineProcess::WaitFor(const std::string &expected) {
    while(IsRunning()) {
        std::string line = ReadLine();
        
        if(line.empty())     return false;
        if(line == expected) return true;
    }

    return false;
}

std::string EngineProcess::ReadLine() {
    std::string line;
         char   c;

    while(true) {
        ssize_t bytes_read = read(m_stdoutfd, &c, 1);

        if(bytes_read <= 0) return "";
        if(c == '\n') break;
        if(c != '\r') line += c;
    }

    return line;
}