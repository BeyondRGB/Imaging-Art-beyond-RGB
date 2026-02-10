#ifndef RESOURCE_PATHS_H
#define RESOURCE_PATHS_H

#include <filesystem>
#include <server/globals_singleton.hpp>
#include <string>
#include <vector>

namespace btrgb::paths {
inline std::string build_ref_data_file_path(const std::string &file_name) {
    namespace fs = std::filesystem;
    const fs::path app_root = GlobalsSingleton::get_instance()->app_root();
    const fs::path cwd = fs::current_path();

    const std::vector<fs::path> candidates = {
        app_root / "res" / "ref_data" / file_name,
        app_root / "frontend" / "res" / "ref_data" / file_name,
        app_root.parent_path() / "res" / "ref_data" / file_name,
        cwd / "res" / "ref_data" / file_name,
    };

    for (const auto &candidate : candidates) {
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }

    return candidates[0].string();
}
} // namespace btrgb::paths

#endif // RESOURCE_PATHS_H
