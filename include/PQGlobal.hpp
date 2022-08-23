#pragma once

#include <UntitledPrecomp.hpp>

#include <functional>
#include <optional>
#include <pqxx/pqxx>

class PQGlobal {
  public:
    static std::pair<int, std::string>
    AddConnection(const std::string &connectionString,
                  const std::string &displayName);

    static std::pair<bool, std::string>
    TestConnection(const std::string &connectionString);

    static bool IsConnectionOpen(int identifier) {
        try {
            return PQGlobal::s_ActiveConnections.at(identifier)->is_open();
        } catch (...) {
            ExceptionHandler::HandleEptr(std::current_exception());
            return false;
        }
    };

    static bool HasConnectionsOpen() {
        try {
            bool ret = false;

            for (const auto &connPair : s_ActiveConnections) {
                const auto &[id, conn] = connPair;
                ret = ret || conn->is_open();
            }

            return ret;
        } catch (...) {
            ExceptionHandler::HandleEptr(std::current_exception());
            return false;
        }
    }

    static bool CloseConnection(int identifier);
    static bool CloseAllConnections();

    static void
    OnConnected(const std::function<void(int, const std::string &)> &callback) {
        s_SucceedCallbackList.emplace_back(callback);
    };

    static void OnConnectionFailed(const std::function<void()> &callback) {
        s_FailedCallbackList.emplace_back(callback);
    };

    static void OnConnectionClose(const std::function<void(int)> &callback) {
        s_ConnectionCloseCallbacks.emplace_back(callback);
    };
    static void OnConnectionClosed(const std::function<void(int)> &callback) {
        s_ConnectionClosedCallbacks.emplace_back(callback);
    };

    /**
     * If there are open connections, invokes OnConnected callbacks again.
     */
    static void RecheckConnectionsOpen();

    static DatabaseTable ParseTable(int connectionIdentifier,
                                    std::string_view queryRes,
                                    std::string_view tableName);

    static DatabaseMetadata IntrospectDatabase(int connectionIdentifier);

    static int GetLastConnectionId() { return s_LastConnectionIdentifier; }

    static std::string GetConnectionName(int identifier) {
        try {
            return s_ConnectionsInfoMap.at(identifier);
        } catch (...) {
            ExceptionHandler::HandleEptr(std::current_exception());
            return {};
        }
    }

  private:
    inline static std::optional<std::shared_ptr<pqxx::work>>
    CreateTransaction(int connectionIdentifier) {
        try {
            return std::make_shared<pqxx::work>(
                *(PQGlobal::s_ActiveConnections.at(connectionIdentifier)));
        } catch (...) {
            ExceptionHandler::HandleEptr(std::current_exception());
            return {};
        }
    };

    static std::optional<pqxx::result> ProcessQuery(int connectionIdentifier,
                                                    std::string_view query);

    static std::vector<std::string>
    EnumerateColumns(int connectionIdentifier, std::string_view tableName);

    static std::vector<std::string> GetSchemas(int connectionIdentifier);

    static std::vector<std::string> GetTablesForSchema(int connectionIdentifier,
                                                       std::string_view name);

  private:
    inline static std::vector<std::function<void(int, const std::string &)>>
        s_SucceedCallbackList;
    inline static std::vector<std::function<void()>> s_FailedCallbackList;

    inline static std::vector<std::function<void(int)>>
        s_ConnectionCloseCallbacks;
    inline static std::vector<std::function<void(int)>>
        s_ConnectionClosedCallbacks;

    inline static std::unordered_map<
        int /* Unique identifier */,
        std::unique_ptr<pqxx::connection> /* Connection pointer */
        >
        s_ActiveConnections;

    inline static std::unordered_map<int /* Unique identifier */,
                                     std::string /* Display name */>
        s_ConnectionsInfoMap;

    inline static int s_LastConnectionIdentifier = 0;
};