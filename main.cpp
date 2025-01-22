//#include <bits/stdc++.h>
//using namespace std;
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <regex>
#include <chrono>
#include <thread>

#include <deque>

using namespace std;

#ifdef __linux__
const string commandRedirect = "2>/dev/null";
const string historyTic = "■";
const string REGEX = "time=([0-9]+\\.?[0-9]*) ms";
const string commandPrefix = "ping -c 1 -W ";
#elif _WIN32
const string commandRedirect = "";
const string historyTic = "=";
const string REGEX = "=([0-9]*)ms";
const string commandPrefix = "ping -n 1 -w ";
#endif


// color ANSI code
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED =  "\033[31m";

// control ANSI code
const string KUP = "\033[1A";
const string KDOWN = "\033[1B";
const string DEL = "\033[2K";
const string KHOME = "\r";

// initailize parameters

const string ini_ip = "1.1.1.1";
const int ini_ttl = 1;
const int ini_reclen = 40;


int ping(const string &ip, int ttl) {
    stringstream command;
    command << commandPrefix << ttl << " " << ip << " " << commandRedirect;
	// cerr << command.str() << endl;

    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        cerr << RED << "[Error] unExpected error, check your parameter" << RESET << endl;
        return -1;
    }

    char buffer[128];
    string result = "";

    // 读取命令输出
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    regex regex_time(REGEX);
    smatch match;
	// cerr << result << endl;
    if (regex_search(result, match, regex_time)) {
        float time_ms = stof(match[1].str());
        return static_cast<int>(time_ms);
    } else {
        return -1;
    }
}

int main(int args,char * argv []) {
    string ip = ini_ip;
	if(args >=2 ) ip = string(argv[1]);
	
	int ttl = ini_ttl;
	if(args >=3 ) ttl = stoi(argv[2]);

	int reclen = ini_reclen;
	if(args >=4 ) reclen = stoi(argv[3]);

	const int loopTime = 1000 * ttl + 200;

    int latency = ping(ip, ttl);
#define now chrono::steady_clock::now
	using ms = chrono::milliseconds;
	auto start_tic = now();
	
/*
 * auto start = chrono::steady_clock::now();
    int latency = ping(ip, timeout_ms);
    auto end = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
&*/

	string change_statuses = "-\\|/";
	int chd_status = 0;

	int first_loop_flag = 1;

	deque<int> lst;
	for(int i = 0;i < reclen;i ++) lst.push_back(0); 

	while(1){
		if(!first_loop_flag){// thread sleep
			auto cur_tic = now();
			auto elapsed_time = chrono::duration_cast< ms >(cur_tic-start_tic).count();
			int sleep_time = loopTime-(int)elapsed_time;
			sleep_time = max(0,sleep_time);
			this_thread::sleep_for(ms(sleep_time));
			start_tic = now();
		}

		int latency = ping(ip,ttl);
//		cin.get();
		if(!first_loop_flag) cout << KHOME << KUP << DEL << KUP << DEL << KUP << DEL;
	//	cin.get();
		cout << RESET << "[" << change_statuses[chd_status] << "] ";
		chd_status ++;
		chd_status %= (change_statuses.size());
		if (latency != -1) {
    	    cout << GREEN << "connection is NORMAL" << endl
				<< RESET << "[info] ping commend ended in " << latency << " ms" << RESET << endl;
    		lst.push_back(1);
		} else {
    	    cout << RED << "Disconnect to internet" << endl
				<< RESET <<  "[info] ping to " << ip << " time limited exceded" << RESET << endl;
   			lst.push_back(-1);
		}
		lst.pop_front();
		for(auto x:lst){
			if(x == 0) cout << RESET << historyTic;
			if(x == 1) cout << GREEN << historyTic;
			if(x == -1)cout << RED   << historyTic;
		}cout << RESET << endl;
///		cin.get();
		first_loop_flag = 0;
	}
    return 0;
}

