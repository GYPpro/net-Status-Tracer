#include <..\include\headers.hpp>
#include <..\include\platform.hpp>
#include <..\include\ANSI.hpp>
#include <..\include\ping_test.hpp>

using namespace std;
// initailize parameters

const string ini_ip = "1.1.1.1";
const int ini_ttl = 1;
const int ini_reclen = 40;
const int ini_thread_num = 3;


int main(int args,char * argv []) {
    string ip = ini_ip;
	if(args >=2 ) ip = string(argv[1]);
	
	int ttl = ini_ttl;
	if(args >=3 ) ttl = stoi(argv[2]);

	int reclen = ini_reclen;
	if(args >=4 ) reclen = stoi(argv[3]);

	const int loopTime = 1000 * ttl + 200;

    auto [status,latency] = ping(ip, ttl);
	// #define now chrono::steady_clock::now
	using clock = chrono::steady_clock;
	using ms = chrono::milliseconds;
	auto start_tic = clock::now();
	
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

	// Main loop
	while(1){
		if(!first_loop_flag){// thread sleep
			auto cur_tic = clock::now();
			auto elapsed_time = chrono::duration_cast< ms >(cur_tic-start_tic).count();
			int sleep_time = loopTime-(int)elapsed_time;
			sleep_time = max(0,sleep_time);
			this_thread::sleep_for(ms(sleep_time));
			start_tic = clock::now();
		}

		auto [status,latency] = ping(ip,ttl);  
//		cin.get();
		if(!first_loop_flag) cout << KHOME << KUP << KDEL << KUP << KDEL << KUP << KDEL;
	//	cin.get();
		cout << RESET << "[" << change_statuses[chd_status] << "] ";
		chd_status ++;
		chd_status %= (change_statuses.size());
		if (latency != -1) {
    	    cout <<"[status] " <<  GREEN << "NORMAL " << RESET << "   connected " << status << "/" << ini_thread_num << endl
				<< RESET << "[info] ping commend ended in " << latency << " ms" << RESET << endl;
    		lst.push_back(status);
		} else {
    	    cout <<"[status] " <<  RED << "ABORTED" << RESET << "   connected " << status << "/" << ini_thread_num << endl
				<< RESET <<  "[info] ping to " << ip << " time limited exceded" << RESET << endl;
   			lst.push_back(-1);
		}
		lst.pop_front();
		for(auto x:lst){
			if(x == 0) cout << RESET << historyTic;
			else if(x == ini_thread_num) cout << GREEN << historyTic;
			else if(x > 0) cout << YELLOW << historyTic;
			else if(x == -1)cout << RED   << historyTic;
		}cout << RESET << endl;
///		cin.get();
		first_loop_flag = 0;
	}
    return 0;
}
