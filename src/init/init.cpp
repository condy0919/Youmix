#include <cstdint>
#include <numeric>
#include <iterator>

#include "algorithm.hpp"
#include "console.hpp"
#include "string.hpp"
#include "backtrace.hpp"

#ifdef __cplusplus
extern "C"
#endif


int kern_entry() {
    //std::uint8_t *input = (std::uint8_t *)0xB8000;
    //std::uint8_t color = (0 << 4) | (15 & 0x0F);

    //const char *str = "Patchouli GO!";
    //for (const char *p = str; *p; ++p) {
    //    *input++ = *p;
    //    *input++ = color;
    //}

    using namespace console;
    stream cout;
    cout.clear();
    cout << green << "Patchouli GO!" << endl;
    cout << hex << 0x7fffffff << endl;
    cout << oct << 0xB8000 << endl;
    cout << setw(10) << "hello" << endl;
    cout << setw(10) << left << "world" << endl;
    cout << setw(20) << left << red << "Konpaku Youmu diski!" << endl;
    cout << "Kernel\n";

    std::uint8_t x = 10;
    cout << dec << x << endl;
    cout << dec << -1 << endl;

    cout.print("%d %c %s %x\n", -1, 'a', "hello", 255);

    char str[26];
    std::iota(std::begin(str), std::end(str), 'a');
    for (const auto& i : str)
        cout << i << " ";
    cout << endl;

    cout.print("%u\n", strlen("hello, world"));

    kernel_panic();

    return 0;
}
