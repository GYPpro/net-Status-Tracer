#include<..\include\headers.hpp>
#include<..\include\platform.hpp>
#include<..\include\ANSI.hpp>
#pragma once

int ping_once(std::string ip, int ttl) {
    std::stringstream command;
    command << commandPrefix << ttl << " " << ip << " " << commandRedirect;
	// cerr << command.str() << endl;

    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        std::cerr << RED << "[Error] unExpected error, add an Issue on github repo" << RESET << std::endl;
        return -1;
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    std::regex regex_time(REGEX);
    std::smatch match;
	// cerr << result << endl;
    if (regex_search(result, match, regex_time)) {
        float time_ms = stof(match[1].str());
        return static_cast<int>(time_ms);
    } else {
        return -1;
    }
}

std::pair<int,int> ping(const string &ip, int ttl,int thread_num = 3) {
    std::vector<std::thread> threads;
    std::vector<int> latencies;
    for (int i = 0; i < thread_num; i++) {
        threads.push_back(std::thread([&latencies, ip, ttl] {
            int latency = ping_once(ip, ttl);
            latencies.push_back(latency);
        }));
    }
    for (auto &thread : threads) {
        thread.join();
    }
    int count = 0;
    int latency = 0;
    for (auto &latency_ : latencies) {
        if (latency_ != -1) {
            count++;
            latency += latency_;
        }
    }
    if (count == 0) {
        return {0, -1};
    }
    return {count, latency / count};
} //[ ac count , latency ]
