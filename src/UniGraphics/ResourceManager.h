#include <functional>
#include <unordered_map>

template <typename T>
struct ResourceManager {
    std::unordered_map<unsigned int, T*> map;
    unsigned int                         nextId = 1;

    unsigned int Add(T* resource) {
        unsigned int id = nextId++;
        map[id]         = resource;
        return id;
    }

    void Remove(unsigned int id) {
        auto it = map.find(id);
        if (it != map.end()) {
            map.erase(it);
        }
    }

    void Clear(std::function<void(T*)> deleter) {
        for (auto& [id, res] : map) {
            deleter(res);
        }
        map.clear();
        nextId = 1;  // reset ID counter
    }

    T* Get(unsigned int id) {
        auto it = map.find(id);
        return it != map.end() ? it->second : nullptr;
    }
};
