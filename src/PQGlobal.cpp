#include <PQGlobal.hpp>

std::pair<bool, std::string>
PQGlobal::MakePQConnection(const std::string &connString) {
    try {
        SQD::Logger::Log("Attempting to connect to PostgreSQL at '" +
                         connString + "'.");
        PQGlobal::m_PQconnection =
            std::make_unique<pqxx::connection>(connString);

        SQD::Logger::Log(
            "Connected successfully! Processing associated callbacks.");
        for (const auto &cb : PQGlobal::m_SucceedCallbackList) {
            std::invoke(cb);
        }

        return {true, ""};
    } catch (...) {
        SQD::Logger::Log("Caught exception while trying to connect to PGSQL.",
                         SQD::Logger::LEVEL_ERROR);
        std::string what =
            ExceptionHandler::HandleEptr(std::current_exception());

        SQD::Logger::Log("Processing callbacks on failure.");
        for (const auto &cb : PQGlobal::m_FailedCallbackList) {
            std::invoke(cb);
        }

        return {false, what};
    }
}
bool PQGlobal::CloseConnection() {
    if (!PQGlobal::m_PQconnection)
        return false;

    try {
        PQGlobal::m_PQconnection->close();
        return true;
    } catch (...) {
        ExceptionHandler::HandleEptr(std::current_exception());
        return false;
    }
}
bool PQGlobal::IsConnectionOpen() {
    if (!PQGlobal::m_PQconnection)
        return false;
    return PQGlobal::m_PQconnection->is_open();
}

void PQGlobal::OnConnected(const std::function<void()> &callback) {
    PQGlobal::m_SucceedCallbackList.push_back(callback);
}

void PQGlobal::OnConnectionFailed(const std::function<void()> &callback) {
    PQGlobal::m_FailedCallbackList.push_back(callback);
}
