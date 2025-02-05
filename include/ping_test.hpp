#include "../include/headers.hpp"
#include "../include/platform.hpp"
#include "../include/ANSI.hpp"
#pragma once

int ping_once(std::string ip, int ttl) {
    std::stringstream command;
    command << commandPrefix << ttl << " " << ip << " " << commandRedirect;
	// cerr << command.str() << endl;

#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping_once> : new pipe" << std::endl;
#endif

    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        std::cerr << RED << "[Error] unExpected error, add an Issue on github repo" << RESET << std::endl;
        return -1;
    }

#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping_once> : geting pipe result" << std::endl;
#endif

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping_once> : closed pipe" << std::endl;
#endif

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
    std::vector<int> latencies(thread_num,-1);

#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping> : prepare to new thread" << std::endl;  
#endif

    for (int i = 0; i < thread_num; i++) {
    threads.push_back(std::thread([&latencies, ip, ttl, i] {
        latencies[i] = ping_once(ip, ttl);
    }));
}
    
#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping> : waiting for thread join" << std::endl;
#endif
    for (auto &thread : threads) {
        thread.join();
#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping> : thread " << (&thread) << "recv." << std::endl;
#endif
    }

#ifdef __IF_LOG__
    std::cerr << "[DEBUG] : function <ping> : got all thread result" << std::endl;
#endif
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
