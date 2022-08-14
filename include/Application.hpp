#pragma once

#include <UntitledPrecomp.hpp>

#include <ArgParser.hpp>
#include <Utils.hpp>

#include <MainFrame.hpp>

class UntitledApp : public wxApp {
  public:
    bool OnInit() override;
    ~UntitledApp() override {
        SQD::Logger::Log("Destructing main application object.");
        try {
            // Other stuff should go in here for cleanup process
            if (PQGlobal::IsConnectionOpen()) {
                PQGlobal::CloseConnection();
            }
        } catch (...) {
            SQD::Logger::Log(
                "Caught exception while destructing main application object.");
            ExceptionHandler::HandleEptr(std::current_exception());
        }
    }
};
