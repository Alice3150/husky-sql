#pragma once

#include <string>
#include <vector>
#include <memory>

namespace husky {
namespace sql {

class RowKV {
    public:
        using KeyT = std::string;
        using ValT = std::vector<std::string>;

        explicit RowKV(const KeyT& key) : key_(key) { }
        RowKV(KeyT&& key, ValT&& data) : key_(key), data_(std::move(data)) { }

        inline const KeyT& id() const { return key_; }
        inline void set_data(ValT&& data) { data_ = std::move(data); }
        inline void set_data(const ValT& data) { data_ = data; }
        inline const ValT& get_data() const { return data_; }

    private:
        KeyT key_; 
        ValT data_;
};

}  // namespace sql
}  // namespace husky