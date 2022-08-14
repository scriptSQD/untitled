#include <ExceptionHandler.hpp>

std::string ExceptionHandler::HandleEptr(std::exception_ptr eptr) {
    try {
        std::rethrow_exception(eptr);
    } catch (std::exception &e) {
        SQD_ERR("Handled exception: " + std::string(e.what()));

        return e.what();
    }
}
