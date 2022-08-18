#include "Utilities/ExceptionHandler.hpp"
#include <utility>

std::string ExceptionHandler::HandleEptr(std::exception_ptr eptr) {
    try {
        std::rethrow_exception(std::move(eptr));
    } catch (std::exception &e) {
        SQD_ERR(fmt::format("Handled exception: {}", e.what()));

        return e.what();
    }
}
