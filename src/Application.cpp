#include "Fonts.hpp"
#include <Application.hpp>

bool UntitledApp::OnInit() {
    // Parse cmd line args
    {
        bool help = false;
        bool verbose = false;
        std::string connectionString;

        auto parser = new SQD::ArgParser(this->argc, this->argv);

        parser->IfFlagsSet({"--verbose", "-v"})
            ->GetFlagValue(verbose)
            ->IfFlagsSet({"--help", "-h"})
            ->GetFlagValue(help)
            ->IfFlagsSet({"--conn-string", "--cs"})
            ->GetFlagValue(connectionString);

        if (help) {
            Utils::PrintHelp();
            exit(0);
        }

        if (verbose) {
            SQD::Logger::EnableLogger();
            SQD::Logger::Log("Enabled logger from --verbose flag.");
        }

        if (!connectionString.empty()) {
            SQD::Logger::Log(
                "Connecting to PostgreSQL via connection string: " +
                connectionString);
            if (!PQGlobal::MakePQConnection(connectionString).first) {
                SQD::Logger::Log("Connection to PGSQL failed.",
                                 SQD::Logger::LEVEL_WARNING);
            }
        }

        delete parser;
    }

    auto *frame = new UntitledFrame(
        nullptr, wxID_ANY, "Untitled - a scriptSQD's student manager.");
    frame->Show();

    return true;
}
