#pragma once
#include <string>
#include <cstring>
#include <atlstr.h>

std::wstring StringToWstring(const std::string& _src);

std::string WstringToString(const std::wstring& _src);
