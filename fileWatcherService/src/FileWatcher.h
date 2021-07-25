#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <utility>
#include <iostream>

enum class FileStatus {
    load, updated, erased
};

using namespace ::std::literals;

class FileWatcher {
public:
    FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay)
            : _path_to_watch{std::move(path_to_watch)}, _delay{delay} {}

    ~FileWatcher() {
        std::cout << "~FileWatcher \n";
    }

    void start(const std::function<void(std::string, FileStatus)> &action) {
        _action = action;
        _thread = std::jthread([this](std::stop_token stopToken) {
            while (!stopToken.stop_requested()) {
                // Wait for "delay" milliseconds
                std::this_thread::sleep_for(_delay);

                auto it = paths_.begin();
                while (it != paths_.end()) {
                    if (!std::filesystem::exists(it->first)) {
                        _action(it->first, FileStatus::erased);
                        it = paths_.erase(it);
                    } else {
                        it++;
                    }
                }

                for (auto &file : std::filesystem::recursive_directory_iterator(_path_to_watch)) {
                    auto current_file_last_write_time = std::filesystem::last_write_time(file);

                    if (!contains(file.path().string())) {
                        paths_[file.path().string()] = current_file_last_write_time;
                        _action(file.path().string(), FileStatus::load);
                    } else {
                        if (paths_[file.path().string()] != current_file_last_write_time) {
                            paths_[file.path().string()] = current_file_last_write_time;
                            _action(file.path().string(), FileStatus::updated);
                        }
                    }
                }
            }
        });
    }

private:
    [[maybe_unused]] std::jthread _thread;
    std::function<void(std::string, FileStatus)> _action;
    std::string _path_to_watch;
    std::chrono::duration<int, std::milli> _delay;
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

    bool contains(const std::string &key) {
        return paths_.contains(key);
    }
};

class FileWatcherContainer {
public:

    FileWatcherContainer(const std::string &path_to_watch, int delay_in_ms) {
        _fw = std::make_unique<FileWatcher>(path_to_watch, std::chrono::milliseconds(delay_in_ms));
    }

    ~FileWatcherContainer() {
        std::cout << "~FileWatcherContainer \n";
    }

    void start() {
        _fw->start([](const std::string &file, FileStatus status) -> void {

            if (!std::filesystem::is_regular_file(std::filesystem::path(file)) &&
                status != FileStatus::erased) {
                return;
            }

            switch (status) {
                case FileStatus::load:
                    std::cout << "File load: " << file << '\n';
                    break;
                case FileStatus::updated:
                    std::cout << "File updated: " << file << '\n';
                    break;
                case FileStatus::erased:
                    std::cout << "File erased: " << file << '\n';
                    break;
                default:
                    std::cout << "Error! Unknown file status.\n";
            }
        });
    }

private:
    std::unique_ptr<FileWatcher> _fw;
};
