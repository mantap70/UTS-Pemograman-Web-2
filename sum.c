#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
long long hitung_jumlah(int n) {
    long long total = 0;
    for (int i = 1; i <= n; i++) {
        total += i;
    }
    return total;
}

emcc sum.c -o sum.js -s EXPORTED_FUNCTIONS='["_hitung_jumlah"]' -s EXPORTED_RUNTIME_METHODS='["ccall"]' -s ALLOW_MEMORY_GROWTH=1