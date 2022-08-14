#include <ExceptionHandler.hpp>

std::string ExceptionHandler::HandleEptr(std::exception_ptr eptr) {
    try {
        std::rethrow_exception(eptr);
    } catch (std::exception &e) {
        SQD::Logger::Log("Handled exception: " + std::string(e.what()),
                         SQD::Logger::LEVEL_ERROR);

        return e.what();
    }
}
