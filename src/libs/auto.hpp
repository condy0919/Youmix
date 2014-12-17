#ifndef _YOUMIX_AUTO_HPP_
#define _YOUMIX_AUTO_HPP_

template <typename Lambda>
class AtScopeExit {
public:
    AtScopeExit(Lambda &l) : lambda(l) {}
    ~AtScopeExit() { lambda(); }
private:
    Lambda &lambda;
};

#define TOKEN_PASTEx(x, y) x ## y
#define TOKEN_PASTE(x, y) TOKEN_PASTEx(x, y)

#define Auto_INTERNAL1(lname, aname, ...) \
    auto lname = [&]() { __VA_ARGS__; }; \
    AtScopeExit<decltype(lname)> aname(lname);

#define Auto_INTERNAL2(ctr, ...) \
    Auto_INTERNAL1(TOKEN_PASTE(Auto_func_, ctr), \
                   TOKEN_PASTE(Auto_instance_, ctr), __VA_ARGS__)

#define Auto(...) Auto_INTERNAL2(__COUNTER__, __VA_ARGS__)

#endif
