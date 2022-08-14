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
            SQD_LOG("Enabled logger from --verbose flag.");
        }

        if (!connectionString.empty()) {
            SQD_LOG("Connecting to PostgreSQL via connection string: " +
                    connectionString);
            if (!PQGlobal::MakePQConnection(connectionString).first) {
                SQD_WARN("Connection to PGSQL failed.");
            }
        }

        delete parser;
    }

    // Set this so X11 can target this application
    // By default is 'untitled' and can cause mess
    this->SetAppName("sqd.untitled.manager");

    auto *frame = new UntitledFrame(
        nullptr, wxID_ANY, "Untitled - a scriptSQD's student manager.");
    frame->Show();

    return true;
}
