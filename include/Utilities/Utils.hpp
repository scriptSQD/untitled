#pragma once

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
};
