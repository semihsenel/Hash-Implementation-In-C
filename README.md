\documentclass[letterpaper,12pt]{article}
\usepackage{tabularx} % extra features for tabular environment
\usepackage{amsmath}  % improve math presentation
\usepackage{graphicx} % takes care of graphic including machinery
\usepackage[margin=1in,letterpaper]{geometry} % decreases margins
\usepackage{cite} % takes care of citations
\usepackage[unicode]{hyperref} % adds hyper links inside the generated pdf file
\usepackage{minted}
\hypersetup{
	unicode=false
}
\usepackage{blindtext}
%++++++++++++++++++++++++++++++++++++++++


\begin{document}

\title{\textbf{Algoritma Analizi}}
\author{Semih Şenel\\
17011004 \\
semihsenel99@gmail.com}
\date{}
\maketitle

\section{Ödevin Konusu}
    Sistem 3 alt bölümden oluşuyor:
    \subsection*{Dökümanı Sözlüğe Ekleme}
        Bir döküman geldiğinde dökümanda bulunan kelimeleri hash fonksiyonundan geçirerek eğer tabloda yoksa hem kelimeyi hem de döküman adını tabloya ekliyoruz, kelime tabloda varsa sadece dökümanın adını bu kelimenin bulunduğu adrese ekliyoruz.
        
        Hash tablosunu bir dosyada saklıyoruz. Program yeniden çalıştırıldığında mevcut dosyayı kullanarak, yeni bilgileri ekliyoruz.
    \subsection*{Kelime Sorgulama}
        Bir kelime sorgulandığından hash fonksiyonundan geçirerek kelimeyi tabloda arıyoruz. Eğer tabloda varsa geçtiği fökümanların isimlerini ekrana yazdırıyoruz.
        
    \subsection*{Hash Tablosu Oluşturma}
        \begin{itemize}
            \item
                Hash tablosunu oluştururken \textit{openaddress}, çakışma problemini çözmek için \textit{double hashing} yöntemleri kullanıyoruz. Buna göre:\\
                \\
                \textbf{h(key, i) = [h1(key) + i*h2(key)] mod M}\\
                \textbf{h1(key) = key mod M}\\
                \textbf{h2(key) = 1 + (key mod MM)}
             \item
                Dosyaların içinde sadece kelimeler olduğu, özel karakterlerin olmadığı varsayılmıştır. Kelimeler küçük-büyük harf karışık olabilir. Örneğin "Araba" ve "araba" aynı kelimelerdir. Kelimeleri sayıya çevirirken Horner Metodu kullanılacaktır.
        
            \item
                Tablonun uzunluğu gösteren \textbf{M} değerini 1000 değerine en yakın Asal sayı olarak alıyoruz. (997)
        \end{itemize}
        
    \subsection*{Önemli Notlar}
        \begin{itemize}
            \item h2 fonksiyonunda MM = M - 1 olarak aldık.
            \item Loadfactor her yeni eklenen kelime için ekrana yazdırılacak. 
            LoadFactor 0.8’i geçtiğinde kullanıcıya UYARI MESAJI gösterilecek. 
            LoadFactor 1.0 olduğunda EKLEME işlemini DURDURALACAK ve dosyada EKLENEMEYEN kelimeler EKRANA yazdırılacak.
            \item LoadFactor'ün son değeri dosyada saklanıyor. Yeni eklenen kelmeler için güncelleniyor.
        \end{itemize}
        
        
        


\section{Analiz}
    \subsection*{Dökümanı Sözlüğe Ekleme}
        Kodun bu bölümünde asıl işin yapıldığı kısım kısaca şu şekilde özetlenebilir :
        \begin{minted}[fontsize=\scriptsize]{C}
           word = strtok(line, " "); 
           while(word){ 
			if(LoadFactor < 1){
				hashIt(docName,&word); 
			}
			else{
				int result = varMi(word);
				if(result != -1){
					NODE* temp = hashTable[result].head;
					while(temp->next != NULL && strcmp(temp->name,docName)){
						temp = temp->next;
					}
					if(strcmp(temp->name,docName)){
						NODE* doc = createNode(docName);
						temp->next = doc;
					}
				}
				else{printf("Eklenemedi ---> %s\n",word);}
			}
			word = strtok(NULL, " "); 
		}
        \end{minted}
        N : Kelime Sayısı\\
        Operasyon : Hashleme\\
        M : Tablo Boyutu\\
        f(N) : Hash fonksiyonu olarak alırsak ;
         \begin{center}
             C(N) = $\sum_{i=1}^{N}{f(N)}$
        \end{center}
        şeklinde bir formül çıkıyor.\\
        Best Case için bakarsak yani bu da Hash Table'ın boş olduğu durum oluyor. Hiçbir collusion oluşmayacağından f(N) = 1 alabiliriz. Buna göre:
        \begin{center}
            $C_{best}(N) = \sum_{i=1}^{N}{1} \Rightarrow C_{best}(N) \in O(n) $
            diyebiliriz.
        \end{center}
        Worst Case için bakarsak yani bu da Hash Table'ın tamamen dolu olduğu durum oluyor. Kontrol edilen her yer dolu olacağından ve kontrol etme işlemi $i < M$ olduğu sürece devam edeceğinden bütün Hash Table dolaşılmış olacak. Buna göre:
        \begin{center}
            $C_{worst}(N) = \sum_{i=1}^{N}{M} \Rightarrow C_{worst}(N) \in O(M*N) $\\
            M ve N çok yakın sayılar oldukları için\\
            $\Rightarrow C_{worst}(N) \in O(N^2)$
            diyebiliriz.
        \end{center}
        
    \subsection*{Sorgulanan Kelimeyi Arama}
        Kodun bu bölümünde asıl işin yapıldığı kısım kısaca şu şekilde özetlenebilir :
        \begin{minted}[fontsize=\scriptsize]{C}
    int varMi(char* word)
    {
    	int i;
    	int index;
    	int key = strToInt(word);
    	i = 0; 
    	index = h(key,i);
    	counter = 1;
    	while(i < M && hashTable[index].head != NULL && strcmp(hashTable[index].kelime, word))
    	{ 
    			counter++; 
    			i++; 
    			index = h(key,i);
    	}
    	if(hashTable[index].head != NULL && !strcmp(hashTable[index].kelime, word))
    	{
    		return index;
    	}
    	else
    	{
    		return -1;
    	}
    }
        \end{minted}
        Operasyon : Kelimenin aradığımız kelimeye eşit olup olmadığı kontrolü\\
        M : Tablo Boyutu\\
        f(N) : Eşitlik Kontrolü ;\\
        
        Best Case için bakarsak yani bu da collusion oluşmamış bir tabloda arama yapmak oluyor. Aradığımız kelime $hash(key,0)$ değerine karşılık gelen indexte olacaktır ve arama karmaşıklığımız O(1) olacaktır.\\
        
        Worst Case için bakarsak yani çok fazla collusion oluşmuş olacaktır ve aradığımız kelime tabloda $hash(key, M-1)$ değerine karşılık gelen indexte olacaktır ya da aradığımız kelime tabloda yoktur. Bu durumda:
        
        
        \begin{center}
            $C_{worst}(M) = \sum_{i=1}^{M}{1} \Rightarrow C_{worst}(M) \in O(M) $
            diyebiliriz.\\
            $M \cong N$ olduğu için\\
            $C_{worst}(M)  \in O(N)$
        \end{center}
        
        \newpage
        
        \subsection*{Program Çıktıları}
        
        \begin{figure}[ht]
            \includegraphics{1.PNG}
            \caption{Programın Giriş Kısmı}
            \label{fig:my_label}
        \end{figure}
        
        \begin{figure}[ht]
          \centering
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{2_1.PNG}
            \caption{1. Döküman Eklenirken}
          \end{minipage}
          \hfill
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{2_2.PNG}
            \caption{1. Döküman Eklenmesinin Bitişi}
          \end{minipage}
        \end{figure}
        
        \begin{figure}[ht]
          \centering
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{3.PNG}
            \caption{2. Döküman Eklenirken}
          \end{minipage}
          \hfill
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{4.PNG}
            \caption{3. Döküman Eklenirken}
          \end{minipage}
        \end{figure}
        
        \begin{figure}[ht]
          \centering
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{5.PNG}
            \caption{LoadFactor 0.8'e ulaştığında}
          \end{minipage}
          \hfill
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{6.PNG}
            \caption{LoadFactor 1'e ulaştığında}
          \end{minipage}
        \end{figure}
        
        \begin{figure}[ht]
          \centering
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{7.PNG}
            \caption{Kelime Arama Case 1}
          \end{minipage}
          \hfill
          \begin{minipage}[b]{0.4\textwidth}
            \includegraphics[width=\textwidth]{8.PNG}
            \caption{Kelime Arama Case 2}
          \end{minipage}
        \end{figure}
        
        \begin{figure}[ht]
            \centering
            \includegraphics{10.PNG}
            \caption{Kelime Arama Case 3}
            \label{fig:my_label}
        \end{figure}
        
        


\end{document}
