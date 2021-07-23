#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <utility>

enum class FileStatus {
    load, updated, erased
};

class FileWatcher {
public:
    std::string path_to_watch;
    std::chrono::duration<int, std::milli> delay;
    std::stop_token token;

    FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay, std::stop_token &token)
            : path_to_watch{std::move(path_to_watch)}, delay{delay}, token{token} {}

    void start(const std::function<void(std::string, FileStatus)> &action) {
        while (!token.stop_requested()) {
            // Wait for "delay" milliseconds
            std::this_thread::sleep_for(delay);

            auto it = paths_.begin();
            while (it != paths_.end()) {
                if (!std::filesystem::exists(it->first)) {
                    action(it->first, FileStatus::erased);
                    it = paths_.erase(it);
                } else {
                    it++;
                }
            }

            for (auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
                auto current_file_last_write_time = std::filesystem::last_write_time(file);

                if (!contains(file.path().string())) {
                    paths_[file.path().string()] = current_file_last_write_time;
                    action(file.path().string(), FileStatus::load);
                } else {
                    if (paths_[file.path().string()] != current_file_last_write_time) {
                        paths_[file.path().string()] = current_file_last_write_time;
                        action(file.path().string(), FileStatus::updated);
                    }
                }
            }
        }
    }

private:
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

    bool contains(const std::string &key) {
        return paths_.contains(key);
    }
};