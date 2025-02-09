#include "common.h"

namespace editor {

class file_dialog {
public:
    enum class SelectMode {
        FilesOnly,      // Files only
        FoldersOnly,    // Folders only
        Any            // Any item
    };

    file_dialog() {
        current_path = std::filesystem::current_path();
        selected_path = "";
        filter = "";
        select_mode = SelectMode::FilesOnly;
    }

    bool show(const char* title, bool* p_open);

    const std::string& get_selected_path() const { return selected_path; }
    void clear_selection() { selected_path.clear(); }
    void set_current_path(const std::filesystem::path& path) { current_path = path; }
    void set_select_mode(SelectMode mode) { select_mode = mode; }
    
    // New methods for extension filters
    void add_extension_filter(const std::string& ext) { extension_filters.push_back(ext); }
    void clear_extension_filters() { extension_filters.clear(); }

private:
    std::filesystem::path current_path;
    std::string selected_path;
    std::string filter;
    std::vector<std::string> extension_filters;  // Changed to vector
    SelectMode select_mode;
};
}

namespace edt = editor;