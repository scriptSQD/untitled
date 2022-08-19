#pragma once

#include <wx/bitmap.h>
#include <wx/bmpbndl.h>
#include <wx/vector.h>

#include <functional>
#include <iostream>
#include <optional>
#include <type_traits>

class Utils {
  public:
    static void PrintHelp();

    template <typename OT>
    inline static void HandleOptional(
        OT optional,
        std::function<void(const typename OT::value_type &)> functor) {
        if (!optional.has_value()) {
            SQD_WARN("HandleOptional: Optional had no value!");
            return;
        }

        return std::invoke(functor, optional.value());
    };

    static wxBitmapBundle CreateBitmapBundle(std::string_view baseFilename,
                                             std::string_view extension,
                                             const wxSize &size = wxSize(28,
                                                                         28));
};

struct HashDatabaseTableLocation
    : public std::function<size_t(const DatabaseMetadata::TableLocation &)> {
    size_t operator()(const DatabaseMetadata::TableLocation &key) const {
        const auto &[schema, table] = key;
        return std::hash<std::string>{}(schema) ^
               (std::hash<std::string>{}(table) << 1);
    }
};
