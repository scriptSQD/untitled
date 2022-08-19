#pragma once

// Main wx stuff
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Structures/DatabaseMetadata.hpp>
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

#include "Utilities/Utils.hpp"

#include "Utilities/ExceptionHandler.hpp"
#include "Utilities/Fonts.hpp"
#include <Logger.hpp>
#include <PQGlobal.hpp>

#include <fmt/core.h>

enum { None = 0, IdChangeDb, IdConnect, IdErrorDetails, IdReloadSchoolList };

struct HashDatabaseTableLocation
    : public std::function<size_t(const DatabaseMetadata::TableLocation &)> {
    size_t operator()(const DatabaseMetadata::TableLocation &key) const {
        const auto &[schema, table] = key;
        return std::hash<std::string>{}(schema) ^
               (std::hash<std::string>{}(table) << 1);
    }
};