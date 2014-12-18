#include "../libs/memory"

namespace std {
template <typename T>
void swap(shared_ptr<T>& a, shared_ptr<T>& b) {
    a.swap(b);
}
}
