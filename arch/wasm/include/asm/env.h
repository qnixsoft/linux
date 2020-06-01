#ifndef WASM_ENV
#define WASM_ENV

void flush(char *, unsigned long);
ssize_t js_emem(size_t, u32 *, size_t);
ssize_t evt_count(unsigned);
unsigned long umem(int, char *, char *, unsigned long);
unsigned long kmem(int, char *, char *, unsigned long);

#endif
