#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>

#include "reference_data/ref_data_defines.hpp"
#include "server/globals_singleton.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "utils/csv_parser.hpp"
#include "utils/general_utils.hpp"
#include "utils/resource_paths.hpp"

namespace {
void reset_globals() {
    auto *globals = GlobalsSingleton::get_instance();
    globals->set_is_test(false);
    globals->set_app_root("./");
    globals->set_port(9002);
}

struct ScopedCurrentPath {
    explicit ScopedCurrentPath(const std::filesystem::path &path)
        : original(std::filesystem::current_path()) {
        std::filesystem::current_path(path);
    }

    ~ScopedCurrentPath() { std::filesystem::current_path(original); }

    std::filesystem::path original;
};
} // namespace

TEST_CASE("CSVParser counts items with default and custom delimiters") {
    CSVParser parser;

    REQUIRE(parser.count_line_items("a,b,c") == 3);
    REQUIRE(parser.count_line_items("art|target|flat|dark", "|") == 4);
}

TEST_CASE("CSVParser reads and parses CSV files without advancing on peek") {
    const auto csv_path =
        std::filesystem::temp_directory_path() / "btrgb-unit-utils.csv";

    {
        std::ofstream csv(csv_path);
        REQUIRE(csv.is_open());
        csv << "name,value\r\n";
        csv << "sample,42\r\n";
        csv << "other,84\r\n";
    }

    CSVParser parser;
    REQUIRE(parser.open_file(csv_path.string()));
    REQUIRE(parser.get_line_count() == 3);
    REQUIRE(parser.peek() == "name,value");
    REQUIRE(parser.get_next_line() == "name,value");

    std::string line = parser.get_next_line();
    REQUIRE(parser.get_next<std::string>(line) == "sample");
    REQUIRE(parser.get_next<int>(line) == 42);
    REQUIRE(parser.has_next_line());
    REQUIRE(parser.get_next_line() == "other,84");

    parser.close_file();
    std::filesystem::remove(csv_path);
}

TEST_CASE("CSVParser reports failed conversions with token context") {
    CSVParser parser;
    std::string line = "sample,not-a-number";

    REQUIRE(parser.get_next<std::string>(line) == "sample");

    try {
        static_cast<void>(parser.get_next<int>(line));
        FAIL("Expected get_next<int> to throw for a non-numeric token");
    } catch (const std::runtime_error &e) {
        const std::string message = e.what();
        REQUIRE(message.find("token: not-a-number") != std::string::npos);
    }
}

TEST_CASE("CSVParser returns false when opening a missing file") {
    CSVParser parser;
    const auto missing_path =
        std::filesystem::temp_directory_path() / "btrgb-missing-file.csv";

    REQUIRE_FALSE(parser.open_file(missing_path.string()));
}

TEST_CASE("CMDArgManager updates runtime globals from optional args") {
    reset_globals();

    char program[] = "beyond-rgb-backend";
    char test_flag[] = "--test_run=TrUe";
    char port_flag[] = "--port=9015";
    char root_flag[] = "--app_root=/tmp/btrgb";
    char *argv[] = {program, test_flag, port_flag, root_flag};

    CMDArgManager::process_args(4, argv);

    auto *globals = GlobalsSingleton::get_instance();
    REQUIRE(globals->is_test());
    REQUIRE(globals->get_port() == 9015);
    REQUIRE(globals->app_root() == "/tmp/btrgb");

    reset_globals();
}

TEST_CASE("CMDArgManager ignores unknown options and prints usage text") {
    reset_globals();

    char program[] = "beyond-rgb-backend";
    char unknown_flag[] = "--unknown=value";
    char usage_flag[] = "-u";
    char *argv[] = {program, unknown_flag, usage_flag};

    std::ostringstream captured;
    auto *original = std::cout.rdbuf(captured.rdbuf());

    CMDArgManager::process_args(3, argv);

    std::cout.rdbuf(original);

    auto *globals = GlobalsSingleton::get_instance();
    REQUIRE_FALSE(globals->is_test());
    REQUIRE(globals->get_port() == 9002);
    REQUIRE(globals->app_root() == "./");
    REQUIRE(captured.str().find("usage: [options]") != std::string::npos);
}

TEST_CASE("general utils return zero-padded date and time strings") {
    const std::string date = btrgb::get_date("-");
    const std::string time = btrgb::get_time(btrgb::MILITARY, ".");
    const std::string timestamp = btrgb::get_time_stamp();

    REQUIRE(std::regex_match(date, std::regex(R"(\d{4}-\d{2}-\d{2})")));
    REQUIRE(std::regex_match(time, std::regex(R"(\d{2}\.\d{2}\.\d{2})")));
    REQUIRE(std::regex_match(timestamp, std::regex(R"(\d+)")));
}

TEST_CASE("general utils honor custom delimiters") {
    REQUIRE(std::regex_match(btrgb::get_date("_"),
                             std::regex(R"(\d{4}_\d{2}_\d{2})")));
    REQUIRE(std::regex_match(btrgb::get_time(btrgb::STANDARD, "-"),
                             std::regex(R"(\d{2}-\d{2}-\d{2})")));
}

TEST_CASE("resource path builder prefers app root before fallback locations") {
    reset_globals();

    const auto temp_root =
        std::filesystem::temp_directory_path() / "btrgb-resource-priority";
    const auto app_root = temp_root / "app";
    const auto direct_ref_dir = app_root / REF_DATA_RELATIVE_DIR;
    const auto frontend_ref_dir = app_root / FRONTEND_REF_DATA_RELATIVE_DIR;
    const auto parent_ref_dir = temp_root / REF_DATA_RELATIVE_DIR;
    const auto cwd_root = temp_root / "cwd";
    const auto cwd_ref_dir = cwd_root / REF_DATA_RELATIVE_DIR;
    const std::string file_name = "priority.csv";

    std::filesystem::remove_all(temp_root);
    std::filesystem::create_directories(direct_ref_dir);
    std::filesystem::create_directories(frontend_ref_dir);
    std::filesystem::create_directories(parent_ref_dir);
    std::filesystem::create_directories(cwd_ref_dir);

    std::ofstream(direct_ref_dir / file_name) << "direct";
    std::ofstream(frontend_ref_dir / file_name) << "frontend";
    std::ofstream(parent_ref_dir / file_name) << "parent";
    std::ofstream(cwd_ref_dir / file_name) << "cwd";

    GlobalsSingleton::get_instance()->set_app_root(app_root.string());
    ScopedCurrentPath scoped_cwd(cwd_root);

    REQUIRE(btrgb::paths::build_ref_data_file_path(file_name) ==
            (direct_ref_dir / file_name).string());

    std::filesystem::remove_all(temp_root);
    reset_globals();
}

TEST_CASE("resource path builder falls back through frontend parent and cwd") {
    reset_globals();

    const auto temp_root =
        std::filesystem::temp_directory_path() / "btrgb-resource-fallbacks";
    const auto app_root = temp_root / "app";
    const auto frontend_ref_dir = app_root / FRONTEND_REF_DATA_RELATIVE_DIR;
    const auto parent_ref_dir = temp_root / REF_DATA_RELATIVE_DIR;
    const auto cwd_root = temp_root / "cwd";
    const auto cwd_ref_dir = cwd_root / REF_DATA_RELATIVE_DIR;

    std::filesystem::remove_all(temp_root);
    std::filesystem::create_directories(frontend_ref_dir);
    std::filesystem::create_directories(parent_ref_dir);
    std::filesystem::create_directories(cwd_ref_dir);

    GlobalsSingleton::get_instance()->set_app_root(app_root.string());
    ScopedCurrentPath scoped_cwd(cwd_root);

    const std::string frontend_file = "frontend.csv";
    std::ofstream(frontend_ref_dir / frontend_file) << "frontend";
    REQUIRE(btrgb::paths::build_ref_data_file_path(frontend_file) ==
            (frontend_ref_dir / frontend_file).string());

    std::filesystem::remove(frontend_ref_dir / frontend_file);

    const std::string parent_file = "parent.csv";
    std::ofstream(parent_ref_dir / parent_file) << "parent";
    REQUIRE(btrgb::paths::build_ref_data_file_path(parent_file) ==
            (parent_ref_dir / parent_file).string());

    std::filesystem::remove(parent_ref_dir / parent_file);

    const std::string cwd_file = "cwd.csv";
    std::ofstream(cwd_ref_dir / cwd_file) << "cwd";
    REQUIRE(btrgb::paths::build_ref_data_file_path(cwd_file) ==
            (cwd_ref_dir / cwd_file).string());

    const std::string missing_file = "missing.csv";
    REQUIRE(btrgb::paths::build_ref_data_file_path(missing_file) ==
            (app_root / REF_DATA_RELATIVE_DIR / missing_file).string());

    std::filesystem::remove_all(temp_root);
    reset_globals();
}
