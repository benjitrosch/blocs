#pragma once

#include <filesystem>
#include <vector>

#include <blocs/common.h>

namespace blocs
{
    namespace platform
    {
        namespace filesystem
        {
            inline bool find_file(cstr path)
            {
                return std::filesystem::is_regular_file(path);
            }

            inline bool find_file(const str& path)
            {
                return find_file(path.c_str());
            }

            inline bool delete_file(cstr path)
            {
                return std::filesystem::remove(path);
            }

            inline bool delete_file(const str& path)
            {
                return delete_file(path.c_str());
            }

            inline str get_file_name(
                const str& path, bool with_extension = false
            )
            {
                auto file = static_cast<std::filesystem::path>(path).filename();
                return with_extension ? file.string()
                                      : file.replace_extension().string();
            }

            inline str get_file_extension(const str& path)
            {
                return static_cast<std::filesystem::path>(path)
                    .extension()
                    .string();
            }

            inline bool new_folder(cstr path)
            {
                return std::filesystem::create_directories(path);
            }

            inline bool new_folder(const str& path)
            {
                return new_folder(path.c_str());
            }

            inline bool find_dir(cstr path)
            {
                return std::filesystem::is_directory(path);
            }

            inline bool find_dir(const str& path)
            {
                return find_dir(path.c_str());
            }

            inline bool delete_dir(cstr path)
            {
                return std::filesystem::remove_all(path) > 0;
            }

            inline bool delete_dir(const str& path)
            {
                return delete_dir(path.c_str());
            }

            inline void all_files_in_dir(
                std::vector<str>& list, cstr path, bool recursive = true
            )
            {
                if (std::filesystem::is_directory(path))
                {
                    if (recursive)
                    {
                        for (auto& p :
                             std::filesystem::recursive_directory_iterator(path
                             ))
                            list.emplace_back(p.path().string());
                    }
                    else
                    {
                        for (auto& p :
                             std::filesystem::directory_iterator(path))
                            list.emplace_back(p.path().string());
                    }
                }
            }

            inline void all_files_in_dir(
                std::vector<str>& list, const str& path, bool recursive = true
            )
            {
                all_files_in_dir(list, path.c_str(), recursive);
            }
        }
    }
}
