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

    template <typename RT, typename OT>
    inline static RT
    HandleOptional(OT optional,
                   std::function<RT(const typename OT::value_type &)> functor) {
        if (!optional.has_value()) {
            SQD_LOG("HandleOptional: Optional had no value!");
            return RT();
        }

        return std::invoke(functor, optional.value());
    };

    static wxBitmapBundle
    CreateBitmapBundle(std::string_view baseFilename,
                       std::string_view extension,
                       wxBitmapType type = wxBITMAP_TYPE_PNG,
                       const wxSize &size = wxSize(28, 28));

    static wxBitmap CreateBitmap(const std::string &filename,
                                 wxBitmapType type = wxBITMAP_TYPE_PNG,
                                 const wxSize &size = wxSize(28, 28));
};

struct HashDatabaseTableLocation
    : public std::function<size_t(const DatabaseMetadata::TableLocation &)> {
    size_t operator()(const DatabaseMetadata::TableLocation &key) const {
        const auto &[id, schema, table] = key;
        return id ^ std::hash<std::string>{}(schema) ^
               (std::hash<std::string>{}(table) << 1);
    }
};
