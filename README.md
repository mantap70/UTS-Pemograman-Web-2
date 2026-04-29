# 🚀 WebAssembly vs JavaScript — Benchmark Experiment

Repositori ini berisi kode eksperimen sederhana untuk membandingkan performa **WebAssembly (Wasm)** dengan **JavaScript** dalam operasi komputasi berat, sebagai bahan penelitian untuk artikel ilmiah populer bertopik WebAssembly.

---

## 📋 Deskripsi Eksperimen

Eksperimen ini mengukur perbedaan waktu eksekusi antara JavaScript dan WebAssembly ketika melakukan operasi **penjumlahan angka dari 1 hingga 100 juta** (loop sebanyak 100.000.000 iterasi).

Tujuannya adalah membuktikan secara langsung klaim bahwa WebAssembly dapat berjalan lebih cepat dari JavaScript untuk operasi komputasi intensif.

---

## 📁 Struktur File

```
wasm-experiment/
├── sum.c          # Kode sumber C yang dikompilasi ke WebAssembly
├── sum.js         # Glue code hasil kompilasi Emscripten (auto-generated)
├── sum.wasm       # File biner WebAssembly (auto-generated)
└── index.html     # Halaman benchmark yang dijalankan di browser
```

---

## ⚙️ Cara Menjalankan

### Prasyarat

- [Git](https://git-scm.com/)
- [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)
- Python 3 (untuk local server) atau ekstensi Live Server di VS Code

---

### Langkah 1 — Clone repositori

```bash
git clone https://github.com/username/wasm-experiment.git
cd wasm-experiment
```

---

### Langkah 2 — Aktifkan Emscripten

> Gunakan **Command Prompt (cmd)**, bukan PowerShell.

**Windows:**
```cmd
cd C:\emsdk\emsdk-main
emsdk_env.bat
```

**macOS / Linux:**
```bash
cd ~/emsdk
source ./emsdk_env.sh
```

Verifikasi instalasi:
```bash
emcc --version
```

---

### Langkah 3 — Kompilasi kode C ke WebAssembly

Pindah ke folder proyek, lalu jalankan:

```cmd
emcc sum.c -o sum.js -sEXPORTED_FUNCTIONS=_hitung_jumlah -sEXPORTED_RUNTIME_METHODS=ccall -sALLOW_MEMORY_GROWTH
```

Perintah ini akan menghasilkan dua file:
- `sum.wasm` — file biner WebAssembly
- `sum.js` — glue code untuk memuat Wasm di browser

---

### Langkah 4 — Jalankan local server

> File `.wasm` tidak bisa dibuka langsung via `file://` karena kebijakan keamanan browser (CORS).

**Menggunakan Python:**
```bash
python -m http.server 8080
```

**Menggunakan VS Code:** klik kanan `index.html` → *Open with Live Server*

---

### Langkah 5 — Buka di browser

```
http://localhost:8080
```

Klik tombol **"Mulai Benchmark"** dan lihat hasilnya.

---

## 📊 Hasil Eksperimen

Dijalankan di browser Chrome, laptop Windows, prosesor AMD Ryzen 5 7000 Series.

| Percobaan | JavaScript | WebAssembly | Keterangan |
|---|---|---|---|
| Percobaan 1 | 105.40 ms | 121.10 ms | **Wasm 5x lebih cepat** |
| Percobaan 2 | 719.40 ms | 121.30 ms | JS lebih cepat (JIT warm-up) |

### Analisis

Pada **percobaan pertama**, WebAssembly terbukti **5x lebih cepat** dari JavaScript. Namun pada percobaan kedua, hasilnya berbalik — JavaScript justru lebih cepat.

Fenomena ini terjadi karena browser modern memiliki mekanisme **JIT (Just-In-Time) compilation**. Setelah kode JavaScript dijalankan sekali, browser "mengingat" pola eksekusinya dan mengoptimalkan run berikutnya secara otomatis. Efek ini dikenal sebagai **JIT warm-up**.

> **Kesimpulan:** WebAssembly paling unggul pada kondisi *cold-start* — yaitu saat kode dijalankan pertama kali sebelum browser sempat melakukan optimasi JIT. Untuk operasi yang berulang, browser modern mampu "mengejar" performa WebAssembly melalui optimasi JIT.

---

## 🧠 Penjelasan Kode

### `sum.c`

```c
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
long long hitung_jumlah(int n) {
    long long total = 0;
    for (int i = 1; i <= n; i++) {
        total += i;
    }
    return total;
}
```

- `EMSCRIPTEN_KEEPALIVE` — mencegah fungsi dihapus saat optimasi kompilasi
- Fungsi ini menjumlahkan angka 1 sampai `n` menggunakan loop sederhana

### `index.html`

Halaman HTML yang:
1. Memuat file `sum.js` (yang secara otomatis memuat `sum.wasm`)
2. Menjalankan fungsi JavaScript native untuk operasi yang sama
3. Mengukur waktu keduanya menggunakan `performance.now()`
4. Menampilkan perbandingan hasilnya

---

## 🔗 Referensi

- [WebAssembly Official Site](https://webassembly.org)
- [MDN — WebAssembly Concepts](https://developer.mozilla.org/en-US/docs/WebAssembly/Concepts)
- [Emscripten Documentation](https://emscripten.org/docs/getting_started/)
- Haas, A., et al. (2017). *Bringing the Web up to Speed with WebAssembly*. ACM SIGPLAN Notices, 52(6), 185–200.

---

## 📄 Lisensi

MIT License — bebas digunakan untuk keperluan belajar dan penelitian.

---

*Dibuat sebagai bagian dari tugas UTS Pemrograman Web — Artikel Ilmiah Populer*
