# Konu
<p>Bir döküman geldiğinde dökümanda bulunan kelimeleri hash fonksiyonundan geçirerek eğer tabloda yoksa hem kelimeyi hem de döküman adını tabloya ekliyoruz, kelime tabloda varsa sadece dökümanın adını bu kelimenin bulunduğu adrese ekliyoruz.</p>
<p>Hash tablosunu bir dosyada saklıyoruz. Program yeniden çalıştırıldığında mevcut dosyayı kullanarak, yeni bilgileri ekliyoruz.</p>

## Kelime Sorgulama
<p>Bir kelime sorgulandığından hash fonksiyonundan geçirerek kelimeyi tabloda arıyoruz. Eğer tabloda varsa geçtiği fökümanların isimlerini ekrana yazdırıyoruz.</p>

## Hash Tablosunu Oluşturma
<ul>
<li>Hash tablosunu oluştururken \textit{openaddress}, çakışma problemini çözmek için \textit{double hashing} yöntemleri kullanıyoruz. Buna göre:</li>

<li><b>h(key, i) = [h1(key) + i*h2(key)] mod M</b></li>
<li><b>h1(key) = key mod M</b></li>
<li><b>h2(key) = 1 + (key mod MM)</b></li>
<br>
<li>Dosyaların içinde sadece kelimeler olduğu, özel karakterlerin olmadığı varsayılmıştır. Kelimeler küçük-büyük harf karışık olabilir. Örneğin "Araba" ve "araba" aynı kelimelerdir. Kelimeleri sayıya çevirirken Horner Metodu kullanılacaktır.</li>
<li>Tablonun uzunluğu gösteren **M** değerini 1000 değerine en yakın Asal sayı olarak alıyoruz. (997)</li>
</ul>
## Önemli Notlar
<ul>
<li>h2 fonksiyonunda MM = M - 1 olarak aldık.</li>
<li>Loadfactor her yeni eklenen kelime için ekrana yazdırılacak. LoadFactor 0.8’i geçtiğinde kullanıcıya UYARI MESAJI gösterilecek. LoadFactor 1.0 olduğunda EKLEME işlemini DURDURALACAK ve dosyada EKLENEMEYEN kelimeler EKRANA yazdırılacak.</li>
<li>LoadFactor'ün son değeri dosyada saklanıyor. Yeni eklenen kelmeler için güncelleniyor.</li>
</ul>
