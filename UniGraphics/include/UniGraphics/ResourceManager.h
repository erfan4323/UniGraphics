#include <functional>
#include <unordered_map>

template <typename T>
class ResourceManager {
   public:
    int Add(T* resource) {
        int id        = nextId++;
        resources[id] = resource;
        return id;
    }

    T* Get(int id) {
        auto it = resources.find(id);
        return it != resources.end() ? it->second : nullptr;
    }

    void Remove(int id) { resources.erase(id); }

    void Clear(std::function<void(T*)> deleter) {
        for (auto& [id, res] : resources) {
            deleter(res);
        }
        resources.clear();
    }

   private:
    int                         nextId = 1;
    std::unordered_map<int, T*> resources;
};
