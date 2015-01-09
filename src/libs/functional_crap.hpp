#ifndef _YOUMIX_FUNCTIONAL_HPP_
#define _YOUMIX_FUNCTIONAL_HPP_

#include "utility"
#include "memory"
#include "algorithm"
#include "type_traits"

namespace std {
template<typename...> class function_impl;

template<typename R>
class function_impl<R()> {
public:
    template<typename T>
    function_impl(const T& fn) : pfn(new T(fn)), ref_cnt(new int(1)) {
        invoke = helper<T>::invoke;
        del = helper<T>::del;
    }

    function_impl(R (*fn)(void)) : pfn(fn), ref_cnt(nullptr) {
        invoke = helper<R(void)>::invoke;
        del = helper<R(void)>::nop;
    }

    function_impl(const function_impl& rhs)
        : pfn(rhs.pfn), ref_cnt(rhs.ref_cnt), invoke(rhs.invoke), del(rhs.del) {
        ++*ref_cnt;
    }

    ~function_impl() { decr_ref(); }

    function_impl& operator=(function_impl rhs) {
        rhs.swap(*this);
        return *this;
    }

    void swap(function_impl& rhs) {
        using std::swap;
        swap(pfn, rhs.pfn);
        swap(ref_cnt, rhs.ref_cnt);
        swap(invoke, rhs.invoke);
        swap(del, rhs.del);
    }

    R operator()() const {
        return invoke(pfn);
    }

private:
    void *pfn;
    int *ref_cnt;

    R (*invoke)(void*);
    void (*del)(void*);

    template<typename T>
    struct helper {
        static R invoke(void *p) { return (*(T*)p)(); }
        static void del(void *p) { delete (T*)p; }
        static void nop(void *) { }
    };

    void decr_ref() {
        if (--*ref_cnt == 0) {
            delete ref_cnt;
            del(pfn);
        }
    }
};

template<typename R, typename... Args>
class function_impl<R(Args...)> {
public:
    template<typename T>
    function_impl(const T& fn) : pfn(new T(fn)), ref_cnt(new int(1)) {
        invoke = helper<T>::invoke;
        del = helper<T>::del;
    }

    function_impl(R (*fn)(Args...)) : pfn(reinterpret_cast<void*>(fn)), ref_cnt(nullptr) {
        invoke = helper<R(Args...)>::invoke;
        del = helper<R(Args...)>::nop;
    }

    function_impl(const function_impl& rhs)
        : pfn(rhs.pfn), ref_cnt(rhs.ref_cnt), invoke(rhs.invoke), del(rhs.del) {
        ++*ref_cnt;
    }

    ~function_impl() { decr_ref(); }

    function_impl& operator=(function_impl rhs) {
        rhs.swap(*this);
        return *this;
    }

    void swap(function_impl& rhs) {
        using std::swap;
        swap(pfn, rhs.pfn);
        swap(ref_cnt, rhs.ref_cnt);
        swap(invoke, rhs.invoke);
        swap(del, rhs.del);
    }

    R operator()(Args&&... args) const {
        return invoke(pfn, std::forward<Args>(args)...);
    }

private:
    void *pfn;
    int *ref_cnt;

    R (*invoke)(void*, Args&&...);
    void (*del)(void*);

    template<typename T>
    struct helper {
        static R invoke(void *p, Args&&... args) {
            return (*(T *)p)(std::forward<Args>(args)...);
        }
        static void del(void *p) { delete (T*)p; }
        static void nop(void *) { }
    };

    void decr_ref() {
        if (--*ref_cnt == 0) {
            delete ref_cnt;
            del(pfn);
        }
    }
};

template<typename...> class function;

template<typename R>
class function<R> : public function_impl<typename std::function_type<R>::type> {
    typedef function_impl<typename std::function_type<R>::type> inherited;
public:
    using inherited::inherited;
};

template<typename R, typename... Args>
class function<R(Args...)>
    : public function_impl<typename std::function_type<R(Args...)>::type> {
    typedef function_impl<typename std::function_type<R(Args...)>::type> inherited;
public:
    using inherited::inherited;
};

}

#endif
