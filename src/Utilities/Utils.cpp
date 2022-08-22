#include "Utilities/Utils.hpp"

void Utils::PrintHelp() {
    std::cout
        << R"(untitled - a scriptSQD's C++ program to learn and test new things.

untitled now is a Database Management System (kind of...). It connects to databases, displays tables with variable data and will soon be able to create ones!

Available arguments and usage patterns:
Usage:
    1. untitled
    2. untitled -v
Arguments:
    --verbose, -v               Enable verbose mode (verbose output).
    --conn-string, --cs         PostgreSQL connection string (may be passed later in GUI).)"
        << std::endl;
}

wxBitmapBundle Utils::CreateBitmapBundle(std::string_view baseFilename,
                                         std::string_view extension,
                                         wxBitmapType type,
                                         const wxSize &size) {
    // Create images
    wxImage imgNormal(fmt::format("{}-{}.{}", baseFilename, "hidpi", extension),
                      type);
    wxImage imgMedium(
        fmt::format("{}-{}.{}", baseFilename, "medium", extension), type);
    wxImage imgHidpi(fmt::format("{}-{}.{}", baseFilename, "normal", extension),
                     type);

    // Convert
    wxBitmap bmpNormal(imgNormal);
    wxBitmap bmpMedium(imgMedium);
    wxBitmap bmpHidpi(imgHidpi);

    wxBitmap::Rescale(bmpNormal, size);
    wxBitmap::Rescale(bmpMedium, size);
    wxBitmap::Rescale(bmpHidpi, size);

    // Vectorize the result
    auto bmpVec = wxVector<wxBitmap>();
    bmpVec.push_back(bmpNormal);
    bmpVec.push_back(bmpMedium);
    bmpVec.push_back(bmpHidpi);

    auto ret = wxBitmapBundle::FromBitmaps(bmpVec);

    return ret;
}

wxBitmap Utils::CreateBitmap(const std::string &filename, wxBitmapType type,
                             const wxSize &size) {
    // Create images
    wxImage img(filename, type);

    // Convert
    wxBitmap bmp(img);
    wxBitmap::Rescale(bmp, size);

    return bmp;
}
