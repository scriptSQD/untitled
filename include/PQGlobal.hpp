#pragma once

#include <UntitledPrecomp.hpp>

#include <functional>
#include <optional>
#include <pqxx/pqxx>

class PQGlobal {
  public:
  public:
    static std::pair<bool, std::string>
    MakePQConnection(const std::string &connString);

    static bool IsConnectionOpen() {
        if (!PQGlobal::s_PGconnection)
            return false;
        return PQGlobal::s_PGconnection->is_open();
    };

    static bool CloseConnection();

    static void OnConnected(const std::function<void()> &callback) {
        PQGlobal::s_SucceedCallbackList.push_back(callback);
    }

    static void OnConnectionFailed(const std::function<void()> &callback) {
        PQGlobal::s_FailedCallbackList.push_back(callback);
    }

    static std::vector<std::string>
    EnumerateColumns(std::string_view tableName);

    template <typename... CTs>
    static std::vector<std::tuple<CTs...>>
    GetQueryAsData(std::string_view query) {
        SQD_LOG("GetQueryAsData: Getting data from db query.");
        auto ret = std::vector<std::tuple<CTs...>>();

        const auto queryRes = PQGlobal::ProcessQuery(query);
        if (!queryRes.has_value()) {
            SQD_WARN("Database query returned no value!");
            return ret;
        }

        try {
            for (pqxx::row row : queryRes.value()) {
                ret.emplace_back(row.as<CTs...>());
            }
        } catch (...) {
            SQD_ERR(
                "Caught exception while trying to parse database response.");
            wxMessageBox("Failed to serialize data received from database. "
                         "\nThis is most likely problem on your end (wrong "
                         "database layout, check out the README.md on GitHub).",
                         "Data retrieval failed.");
            ExceptionHandler::HandleEptr(std::current_exception());
        }

        return ret;
    };

    static std::optional<pqxx::result> ProcessQuery(std::string_view query);

    static DatabaseTable ParseTable(std::string_view tableName);

  private:
    inline static std::shared_ptr<pqxx::work> CreateTransaction() {
        return std::make_shared<pqxx::work>(*PQGlobal::s_PGconnection);
    };

  private:
    inline static std::unique_ptr<pqxx::connection> s_PGconnection = nullptr;

    inline static std::vector<std::function<void()>> s_SucceedCallbackList;
    inline static std::vector<std::function<void()>> s_FailedCallbackList;
};