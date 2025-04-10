# # Tucil2_13523160

## a. Penjelasan Program

Program ini merupakan aplikasi CLI kompresi gambar yang menggunakan algoritma divide and conquer dengan pendekatan quadtree. Program memuat sebuah gambar, mengkonversinya ke format 24-bit RGB, dan kemudian membangun quadtree berdasarkan metode perhitungan error yang dipilih oleh pengguna (Variance, MAD, Max Pixel Difference, atau Entropy). Hasil dari proses tersebut adalah gambar terkompres yang disimpan ke file output, di mana tiap blok direpresentasikan oleh nilai rata-rata warnanya.

## b. Requirement & Instalasi

- **Compiler:** g++ (atau compiler C++ yang kompatibel)
- **Library Eksternal:** [`FreeImage`](http://freeimage.sourceforge.net/)
  - Linux: install via `sudo apt install libfreeimage-dev`
  - Windows: pastikan `FreeImage.dll` tersedia di folder `bin`
- **Sistem Operasi:** Windows atau Linux

## c. Cara Kompilasi

1. Buka direktori root proyek (tempat Makefile berada) pada terminal Unix-like.
2. Jalankan perintah:
   ```bash
   make
   ```
   Perintah tersebut akan mengkompilasi seluruh source code dan menghasilkan executable di folder `bin`.

## d. Cara Menjalankan dan Menggunakan Program

1. Jalankan program dengan perintah:
   ```bash
   make run
   ```
   atau secara langsung:
   ./bin/tucil2_13523160

## Identitas

I Made Wiweka Putera - 13523160
