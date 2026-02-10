#include <reference_data/ref_data_defines.hpp>
#include <server/globals_singleton.hpp>
#include <utils/resource_paths.hpp>

#include <filesystem>
#include <vector>

namespace btrgb::paths {
std::string build_ref_data_file_path(const std::string &file_name) {
    namespace fs = std::filesystem;
    const fs::path app_root = GlobalsSingleton::get_instance()->app_root();
    const fs::path cwd = fs::current_path();

    const std::vector<fs::path> candidates = {
        app_root / REF_DATA_RELATIVE_DIR / file_name,
        app_root / FRONTEND_REF_DATA_RELATIVE_DIR / file_name,
        app_root.parent_path() / REF_DATA_RELATIVE_DIR / file_name,
        cwd / REF_DATA_RELATIVE_DIR / file_name,
    };

    for (const auto &candidate : candidates) {
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }

    return candidates[0].string();
}
} // namespace btrgb::paths
