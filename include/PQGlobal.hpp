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

    static std::optional<pqxx::result> ProcessQuery(std::string_view query);

    static DatabaseTable ParseTable(std::string_view queryRes,
                                    std::string_view tableName);

    static DatabaseMetadata IntrospectDatabase();

  private:
    inline static std::shared_ptr<pqxx::work> CreateTransaction() {
        return std::make_shared<pqxx::work>(*PQGlobal::s_PGconnection);
    };

    static std::vector<std::string> GetSchemas();
    static std::vector<std::string> GetTablesForSchema(std::string_view name);

  private:
    inline static std::unique_ptr<pqxx::connection> s_PGconnection = nullptr;

    inline static std::vector<std::function<void()>> s_SucceedCallbackList;
    inline static std::vector<std::function<void()>> s_FailedCallbackList;
};