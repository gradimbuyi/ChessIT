#include "EngineProcess.hpp"

int main() {
    EngineProcess engine("./build/Release/engine");

    if(!engine.Start()) {
        std::cout << "Failed to start engine" << std::endl;
        return 1;
    }

    engine.SendCommand("uci");
    std::cout << engine.WaitFor("uciok") << std::endl;

    engine.SendCommand("isready");
    std::cout << engine.WaitFor("readyok") << std::endl;

    std::string dummy;
    std::getline(std::cin, dummy);

    engine.SendCommand("quit");
    engine.Stop();

    return 0;
}