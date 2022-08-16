#pragma once

#include <UntitledPrecomp.hpp>

#include <functional>
#include <optional>
#include <pqxx/pqxx>

class PQGlobal {
  public:
    static std::pair<bool, std::string>
    MakePQConnection(const std::string &connString);

    static bool IsConnectionOpen();

    static bool CloseConnection();

    static void OnConnected(const std::function<void()> &callback);
    static void OnConnectionFailed(const std::function<void()> &callback);

    static std::map<int, std::string>
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

  private:
    inline static std::shared_ptr<pqxx::work> CreateTransaction();

  private:
    inline static std::unique_ptr<pqxx::connection> s_PGconnection = nullptr;

    inline static std::vector<std::function<void()>> s_SucceedCallbackList;
    inline static std::vector<std::function<void()>> s_FailedCallbackList;
};