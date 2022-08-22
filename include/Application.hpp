#pragma once

#include <UntitledPrecomp.hpp>

#include "Utilities/Utils.hpp"
#include <ArgParser.hpp>

#include <MainFrame.hpp>

class UntitledApp : public wxApp {
  public:
    bool OnInit() override;
    ~UntitledApp() override {
        SQD::Logger::Log("Destructing main application object.");
        try {
            // Other stuff should go in here for cleanup process
            if (PQGlobal::HasConnectionsOpen()) {
                PQGlobal::CloseAllConnections();
            }
        } catch (...) {
            SQD::Logger::Log(
                "Caught exception while destructing main application object.");
            ExceptionHandler::HandleEptr(std::current_exception());
        }
    }
};
