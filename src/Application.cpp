#include "Utilities/Fonts.hpp"
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
            SQD_LOG("Enabled verbose logger.");
        }

        if (!connectionString.empty()) {
            SQD_LOG("Connecting to PostgreSQL via connection string upon app"
                    "initialization: " +
                    connectionString);
            PQGlobal::AddConnection(connectionString, "Base connection");
        }

        delete parser;
    }

    // Set this so X11 can target this application
    // By default is 'untitled' and can cause confusion
    this->SetAppName("sqd.untitled.manager");

    wxInitAllImageHandlers();

    auto *frame = new UntitledFrame(
        nullptr, wxID_ANY, "Untitled - a scriptSQD's database manager.");
    frame->Show();

    return true;
}
