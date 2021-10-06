#include "SpeedChecking.h"

std::string SpeedChecking::INTERNET_DNS = "8.8.8.8";

SpeedChecking::SpeedChecking(){
    m_client = new TCPClient();
    m_client->connectToHost("127.0.0.1", 2323);
}

SpeedChecking::~SpeedChecking() {
    if(nullptr != m_client) {
        m_client->closeConnection();
        delete m_client;
    }
}

void SpeedChecking::doCheck() {
    while (true) {
        printf("[SpeedChecking] doCheck..\n");
        m_client->startReadData();
        if (m_start == true) {
            m_start = false;
            std::string details, details2;
            bool result2 = this->checkPing( INTERNET_DNS, 1, details2);
            printf("Internet Connection = %s\r\n", result2 ? "OK" : "Fail");
            if (!result2) return;
            bool result = this->checkSpeed(details);
            printf("Check Speed = %s\r\n", result ? "OK" : "Fail");
        }
    }
}

void SpeedChecking::startCheckThread() {
    checkThread = std::thread(&SpeedChecking::doCheck, this);
}

void SpeedChecking::isChecked() {
    m_start = true;
}

bool SpeedChecking::checkSpeed(std::string &details) {
    // Format a command std::string
    std::string command;
    std::string output;
    command = "speedtest-cli --json";
    int code = commandExec( command, details );
    json j = json::parse(details);
    printf("==> Download: %f Mbps|| <== Upload: %f Mbps\r\n", j.value("download", 0.0) / (1024.0 * 1024.0), j.value("upload", 0.0) / (1024.0 * 1024.0));
    return (code == 0);
}

bool SpeedChecking::checkPing(const std::string &address, const int &max_attempts, std::string &details) {
    // Format a command std::string
    std::string command;
    std::string output;
    command = "ping -w 1 -c " + std::to_string(max_attempts) + " " + address + " 2>&1";
    int code = commandExec(command, details);
    return (code == 0);
}

int SpeedChecking::commandExec(  const std::string&  command,
                                    std::string&        output,
                                    const std::string&  mode) {
    // Create the std::stringstream
    std::stringstream sout;
    // Run Popen
    FILE *in;
    char buff[512];
    // Test output
    if(!(in = popen(command.c_str(), mode.c_str()))) return 1;
    // Parse output
    while(fgets(buff, sizeof(buff), in) != nullptr){
        sout << buff;
    }
    int exit_code = pclose(in);
    // set output
    output = sout.str();
    // Return exit code
    return exit_code;
}
