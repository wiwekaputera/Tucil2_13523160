# Tucil2_13523160

## a. Penjelasan Program

Program ini merupakan aplikasi CLI kompresi gambar yang menggunakan algoritma divide and conquer dengan pendekatan quadtree. Program memuat sebuah gambar, mengkonversinya ke format 24-bit RGB, dan kemudian membangun quadtree berdasarkan metode perhitungan error yang dipilih oleh pengguna (Variance, MAD, Max Pixel Difference, atau Entropy). Hasil dari proses tersebut adalah gambar terkompres yang disimpan ke file output, di mana tiap blok direpresentasikan oleh nilai rata-rata warnanya.

## b. Requirement & Instalasi

- **Compiler:** g++
- **Library Eksternal:** FreeImage
  - **Linux:** install via `sudo apt install libfreeimage-dev`.
  - **Windows:** install [`FreeImage`](https://sourceforge.net/projects/freeimage/files/Binary%20Distribution/3.18.0/FreeImage3180Win32Win64.zip/download?use_mirror=onboardcloud) dan tempatkan folder `include` di `C:/FreeImage/include` dan folder `lib` di `C:/FreeImage/lib`. Selain itu, pastikan file `FreeImage.dll` ada di folder `bin` program ini.
- **Sistem Operasi:** Windows atau Linux

## c. Cara Kompilasi
**Linux:**
1. Buka direktori root proyek.
2. Jalankan perintah:
    ```bash
    mkdir build && cd build
    ```
    ```bash
    cmake ..
    ```
    ```bash
    cmake --build .
    ```
**Windows:**
1. Buka direktori root proyek (tempat Makefile berada) pada terminal Unix-like.
2. Jalankan perintah:
   ```bash
   make
   ```
   Perintah tersebut akan mengkompilasi seluruh source code dan menghasilkan executable di folder `bin`.

## d. Cara Menjalankan dan Menggunakan Program
**Linux:**
1. Pada direktori build, jalankan program dengan perintah:
    ```bash
   ./tucil2_13523160
   ```
**Windows:**
1. Pastikan file `FreeImage.dll` ada di folder `bin` program ini.
2. Jalankan program dengan perintah:
   ```bash
   make run
   ```
   atau secara langsung:
   ```bash
   ./bin/tucil2_13523160
   ```

## Identitas

I Made Wiweka Putera - 13523160
