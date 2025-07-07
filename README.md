# Sistem Antrean Restoran - Multi-Item Per Pelanggan dengan Undo/Redo

## Deskripsi Singkat
Program ini adalah simulasi sistem pelayanan restoran berbasis struktur data, dengan fitur pesanan multi-menu per pelanggan, manajemen antrean FIFO, serta dukungan Undo/Redo pesanan pelanggan. Struktur data yang digunakan meliputi **AVL Tree** (menu), **Linked List** (pesanan pelanggan), **Queue** (antrean pelanggan), dan **Stack** (Undo/Redo).

## Fitur Utama
- **AVL Tree**: Menyimpan data menu secara terurut dan mendukung pencarian cepat.
- **Linked List**: Mencatat pesanan multi-item + quantity dalam satu transaksi pelanggan.
- **Queue**: Mengatur antrean pelanggan secara FIFO, satu node antrean = satu pelanggan.
- **Stack**: Menyimpan histori pembatalan pesanan (Undo) & mengembalikan pesanan (Redo).
- **Flowchart**: Diagram alur program tersedia di `docs/Flowchart.png`.

## Cara Menjalankan
1. Buka folder `src/`.
2. Compile file `restoran_per_pelanggan_redo.c` menggunakan compiler C.
3. Jalankan program, ikuti menu interaktif.
4. Coba fitur: pesan multi-item, lihat antrean, undo/redo, dan layani pelanggan.

## Struktur Project
Lihat file `docs/StrukturProject.txt` untuk detail struktur folder & penjelasan.

---

