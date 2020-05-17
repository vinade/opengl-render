#ifndef APP_UTILS_HPP
#define APP_UTILS_HPP

class AppUtils
{
public:
    template <typename T>
    static bool add_once(std::vector<T> &item_vector, const T &item);

    template <typename T>
    static bool has_item(const std::vector<T> &item_vector, const T &item);
};

template <typename T>
bool AppUtils::has_item(const std::vector<T> &item_vector, const T &item)
{
    auto it = std::find(item_vector.begin(), item_vector.end(), item);
    return (it != item_vector.end());
}

template <typename T>
bool AppUtils::add_once(std::vector<T> &item_vector, const T &item)
{
    if (AppUtils::has_item(item_vector, item))
    {
        return false;
    }

    item_vector.push_back(item);
    return true;
}

#endif