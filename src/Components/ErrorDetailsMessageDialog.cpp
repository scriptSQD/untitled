#include "Components/ErrorDetailsMessageDialog.hpp"

ErrorDetailsMessageDialog::ErrorDetailsMessageDialog(
    const std::string &title, const std::string &description,
    const std::string &error)
    : wxMessageDialog(nullptr, fmt::format("{}:\n{}", description, error),
                      title) {
    this->ShowModal();
}
