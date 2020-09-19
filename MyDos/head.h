#pragma once
#ifndef head.h
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <future>
#include <io.h>
#include <Shlwapi.h>
#include <map>
#include <sstream>
#include <filesystem>
#include <set>
#include <stack>
namespace fs = std::filesystem;
using namespace std;
vector<string> split(string s, string c = " ");
long long count_files(string path);
string normalize_windows_path(string path);
#endif // !head.h
