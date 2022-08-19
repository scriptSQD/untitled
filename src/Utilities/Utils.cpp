#include "Utilities/Utils.hpp"

void Utils::PrintHelp() {
    std::cout
        << R"(untitled - a scriptSQD's C++ program to learn and test new things.

Available arguments and usage patterns:
Usage:
    1. untitled
    2. untitled -v
Arguments:
    --verbose, -v               Enable verbose mode (expanded output).
    --conn-string, --cs         PostgreSQL connection string (may be passed later in GUI).)"
        << std::endl;
}

wxBitmapBundle Utils::CreateBitmapBundle(std::string_view baseFilename,
                                         std::string_view extension,
                                         const wxSize &size) {

    auto *bmpNormal = new wxBitmap(
        fmt::format("{}-{}.{}", baseFilename, "normal", extension));
    auto *bmpMedium = new wxBitmap(
        fmt::format("{}-{}.{}", baseFilename, "medium", extension));
    auto *bmpHidpi =
        new wxBitmap(fmt::format("{}-{}.{}", baseFilename, "hidpi", extension));

    // Do all the resizing
    wxBitmap::Rescale(*bmpNormal, size);
    wxBitmap::Rescale(*bmpMedium, size);
    wxBitmap::Rescale(*bmpHidpi, size);

    // Vectorize the result
    auto bmpVec = wxVector<wxBitmap>();
    bmpVec.push_back(*bmpNormal);
    bmpVec.push_back(*bmpMedium);
    bmpVec.push_back(*bmpHidpi);

    const auto &ret = wxBitmapBundle::FromBitmaps(bmpVec);

    // Deallocate all leftovers
    delete bmpNormal;
    delete bmpMedium;
    delete bmpHidpi;

    return ret;
}
