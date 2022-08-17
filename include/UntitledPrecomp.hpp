#pragma once

// Main wx stuff
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Structures/DatabaseTable.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>

#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <ExceptionHandler.hpp>
#include <Fonts.hpp>
#include <Logger.hpp>
#include <PQGlobal.hpp>

enum { None = 0, IdChangeDb, IdConnect, IdErrorDetails, IdReloadSchoolList };