#pragma once

#include <UntitledPrecomp.hpp>

#include <functional>
#include <pqxx/pqxx>

class PQGlobal {
  public:
    static std::pair<bool, std::string>
    MakePQConnection(const std::string &connString);

    static bool IsConnectionOpen();

    static bool CloseConnection();

    static void OnConnected(const std::function<void()> &callback);
    static void OnConnectionFailed(const std::function<void()> &callback);

  private:
    inline static std::unique_ptr<pqxx::connection> m_PQconnection = nullptr;

    inline static std::vector<std::function<void()>> m_SucceedCallbackList;
    inline static std::vector<std::function<void()>> m_FailedCallbackList;
};