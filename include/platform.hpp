#include"../include/headers.hpp"
#pragma once

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
#elif __APPLE__
const string commandRedirect = "2>/dev/null";
const string historyTic = "■";
const string REGEX = "time=([0-9]+\\.?[0-9]*) ms";
const string commandPrefix = "ping -c 1 -W ";
#endif
