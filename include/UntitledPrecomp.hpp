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

enum {
    None = 0,
    IdChangeDb,
    IdConnect,
    IdErrorDetails,
    IdReloadSchoolList,

    IdProviderWizard,
    IdProviderRaw,

    IdToggleDatabaseTree,

    IdDatabaseTreeAdd,
    IdDatabaseTreeDelete,
    IdDatabaseTreeSync

};
