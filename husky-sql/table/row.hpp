#pragma once

#include <string>
#include <vector>

namespace husky {
namespace sql {

class Row {
    public:
        using KeyT = int;
        using ValT = std::vector<std::string>;

        explicit Row(const KeyT& key) : key_(key) { }

        inline void set_row_data(const ValT & data) { data_ = data; }
        inline const KeyT& get_id() const { return key_; }
        inline const ValT& get_row_data() const { return data_; }

    private:
        KeyT key_; 
        ValT data_;
};

}  // namespace sql
}  // namespace husky