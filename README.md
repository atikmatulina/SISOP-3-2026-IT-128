## LAPORAN PRAKTIKUM SISTEM OPERASI 

Nama : ATIK PUTRI MATULINA 

NRP : 5027251128

KELAS : SISTEM OPERASI (A)


## SOAL_1

DESKRIPSI 

Di dunia nyata sudah tidak memadai bagi Lain Iwakura. Di kamarnya banyak hardware NAVi yang menumpuk, 
tapi dia masih ngerasa ada yang kurang dan ternyata yang kurang adalah protokol buat nyambungin semua orang ke dalam satu kesadaran kolektif yang di sebut The Wired. 


LANGKAH LANGKAH 

1. Buat folder soal1 menggunkan mkdir-p
2. Masukan kode/codingan ke dalam wired, protocol.h, dan navi
3. Lalu cek kode/codingan tersebut menggunkan gcc ...c -o ..
4. Dan jalankan menggunakan ./wired dan ./navi




<img width="992" height="144" alt="WhatsApp Image 2026-05-03 at 5 45 50 PM" src="https://github.com/user-attachments/assets/80eabc17-146a-45c7-b05c-b5e5f508fd29" />
Di sini ketika ./ wired dimasukkan maka port dari wired untuk navi ini sudah terhubung dan disini akan menyimpan percakapan dari ./navi



<img width="956" height="264" alt="WhatsApp Image 2026-05-03 at 5 45 49 PM (1)" src="https://github.com/user-attachments/assets/0ee107fe-4d95-4a6f-8738-7f0af6a4acdc" />
Ketika ./navi pada terminal satu di pakai maka akan keluar perintah untuk memasukan nama untuk user satu.



<img width="1020" height="252" alt="WhatsApp Image 2026-05-03 at 5 45 49 PM" src="https://github.com/user-attachments/assets/a4e7bb39-5b18-425a-b990-192a2a7b6f75" />
Disini lalu membuat ./navi di terminal dua untuk membuat lawan percakapan atau user dua, supaya bisa bertukar pesan. Nanti ketika membuat terminal baru lagi dan memasukan nama maka akan terhubung ketiga user tersebut dalam satu percakapan atau satu waktu yang akan terlihat dari history.log ketika dijalankan.



<img width="1166" height="694" alt="WhatsApp Image 2026-05-04 at 10 05 15 PM" src="https://github.com/user-attachments/assets/26844741-e786-435f-b4ac-20901d03775c" />
.
<img width="874" height="140" alt="WhatsApp Image 2026-05-04 at 9 37 43 PM (1)" src="https://github.com/user-attachments/assets/85c3c1a6-e96c-402f-9f88-670e195963bb" />

Lalu disini ketika kita memasukan angka 1 maka akan keluar 
aktivitas user itu ngapain aja, dan saat memasukan nomor 2 maka akan terlihat waktu dimana user melakukan aktivitas, jika memasukan angka 3 maka output nya adalah server mengeluarkan notifikasi emergency maka sambungan terputus (lost) pada user yg sedang melakukan aktivitas, dan yang keempat jika memasukannya 
kembali yang terjadi adalah sambungan ter disconnect atau istilahnya seperti putus sambungan permanen dan tidak mengeluarkan emergency.


<img width="1600" height="1075" alt="WhatsApp Image 2026-05-04 at 9 37 43 PM (2)" src="https://github.com/user-attachments/assets/aae44ed8-3cb4-4d01-9ae1-f3b1dac42f4e" />
Dan yang terakhir ini melihat history.log atau seperti histori aktivitas apa saja dan waktu aktivitasnya kapan yang dilakukan user maka akan masuk atau terditeksi dan terlihat oleh history.log dari aktivitas yang baru sampai yang lama. 

<img width="1014" height="250" alt="WhatsApp Image 2026-05-03 at 5 45 49 PM (2)" src="https://github.com/user-attachments/assets/ce08f0b1-ff22-4c44-a4f4-d20207241439" />

Ini apabila ingin mengecek apa saja yang ada di dalam folder soal1 tree dan ls.

<img width="1142" height="90" alt="WhatsApp Image 2026-05-04 at 9 54 49 PM" src="https://github.com/user-attachments/assets/1bcaec1f-fa61-4195-8924-029fb8deb92f" />
Apabila keika di ls dan tree seperti ini, maka cukup 'rm history.log navi wired' dan hasil nya akan seperti contoh yang di atas.


<img width="1086" height="106" alt="WhatsApp Image 2026-05-04 at 9 37 43 PM" src="https://github.com/user-attachments/assets/a0c3f8e7-68d3-44b5-a049-254482e963a9" />
Ini terjadi apabila sambungan dari wired ke navi terputus sementara (lost) atau disconnect, karena tidak ada user atau aktivitas yang terditeksi atau sedang melakukan sambungan
