#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define M 997
#define R 31
#define MAX 50000

// Kelimelerin gectigi dokumanlari tutmak icin kullandigim linkli liste
struct node{
	char name[16]; // dokuman ismini tuttugum degisken
	struct node* next; // sonraki nodu gosteren pointer
};

// Hashtable
struct hash{
	int key; // key i tuttugum degisken
	char kelime[100]; // value yani kelimeyi tuttugum degisken
	struct node* head; // kelimenin gectigi dokumanlari tutan linkli listenin
	// ilk dugumunu gosteren pointer
};


// Kolaylik olmasi acisindan typedef ile tanimladim
typedef struct node NODE; 
typedef struct hash HASH;


void dokumanOku(); // dokuman dosyasini okudugum fonksiyon
void kelimeSorgu(); // istenen kelimeyi sorgulayan fonksiyon
void hashIt(char*, char**); // kelimeyi hashleyen fonksyion
NODE* createNode(char*); // dokuman tutmak icin node olusturdugum fonksiyon
int varMi(char*); // kelimenin hashTable'de olup olmadigini bulan fonksiyon
int h1(int); // 1. hash fonksiyonu
int h2(int); // 2.hash fonksiyonu
int h(int, int); // hash fonksiyonu
int strToInt(char*); // Horner method ile string i int e cevirdigim kisim
void toLower(char**); // kelimeyi lower case yaptigim fonksiyon
void degerleriOku(); // program baslangicinda eski degerleri dosyadan okudugum fonk.
void lf_hashGuncelle(); // hash ve loadfactor u tuttugum dosyayi guncelledigim fonk.

HASH* hashTable; // hastable'i tutacagim degisken
static double LoadFactor;  // loadFactor 'u tuttugum degisken
static double factorArtis = 1 / (double)M; /* her seferinde tabloyu kontrol etmemek icin
loadFactor'un artis oranini tuttugum degisken 
her kelime eklendiginde bu deger kadar artiyor
*/
static int uyari; // loadFactor un dolulugunu kontrol ederken kullandigim degisken
static FILE* lf_hash; // loadFactor ve hashTable'i tuttugum dosyanin pointeri
static int counter; // kelimeyi ararken kac adimda buldugumu tuttugum degisken

int main(){
	int flag = 1; // programin bir dongu halinde olmasi icin kullandigim dongu degiskeni
	int input; // kullancinin yapacagi isleme gore girdigi inputu tuttugum degisken
	hashTable = (HASH*) calloc(M,sizeof(HASH)); // M boyutunda hashTable olusturdum
	degerleriOku(); // son degerleri dosyadan cektim
	while(flag){ // ana program dongsu
		printf("Dokuman eklemek icin 1 giriniz\n");
		printf("Kelime sorgulamak icin 2 giriniz\n");
		printf("Cikis icin -1 giriniz\n");
		printf("-> ");
		scanf("%d", &input);
		// kullanicidan yapmak istedigi islemi aldim
		if(input == 1){		
			// 1. islem dokuman ekleme islemi
			dokumanOku(); // dokumani okuyup hashTable'e yerlestirdigim fonksiyon
			char *name = "17011004.txt"; // loadFactor ve hashTable' i tuttugum dosyanin adi
			lf_hash = fopen(name,"w"); // her seferinde guncelledigim icin "w" modda aciyorum.
			lf_hashGuncelle(); // degerleri guncelledigim fonksiyon
			fclose(lf_hash); // islem bittiginde dosyayi kapattim
		}
		else if(input == 2){
			// 2. islem bir kelimenin hangi dokumanlarda gectiginin sorgulanmasi
			kelimeSorgu(); // islemin yapildigi fonksiyon
		}			
		else if(input == -1){
			// -1 girildiginde flag degiskeni 0 oluyor ve program sonlaniyor
			flag = 0;
		}			
		else{
			// istenilen disinda bir deger girilirse uyari yazdiriyor
			printf("Hatali giris!!!");
		}
			
		
	}
	printf("Iyi gunler :)");

	return 0;
}

NODE* createNode(char* name){
	// dokumanlarin ismini tuttugum node'lerin olustugu fonksiyon
	NODE* newNode; // NODE* tipinde bir degisken olusturdum
	newNode = (NODE*) malloc(sizeof(NODE)); // degiskene bellekte yer actim
	newNode->next = NULL; // linkli liste ozelligi olarak next node'una NULL atadim
	strcpy(newNode->name, name); // dokumanin ismini node'e kaydettim
	// string oldugu icin direkt atayamiyoruz o yuzden strcpy kullandim
	return newNode; // olusan node u returnledim
}

int varMi(char* word){
	// bir kelimenin hashTable'de olup olmadigini bulan fonksiyon
	int i; /* collusion olustuyda double hashing fonksiyonundaki i indexini
	arttirarak elemani baska bir indexe ekliyoruz. hash fonksiyonuna yollamak icin
	tuttugum i degiskeni
	*/
	int index; // hash fonksiyonundan donen index i tuttugum degisken
	int key = strToInt(word); // Horner Methodu kullanarak stringten elde ettigim degeri key olarak kullanmak icin tuttugum degisken
	i = 0; // index degerini bulmak icin i yi ilklendirdim
	index = h(key,i); // hash fonksiyonundan donen index degerini tuttum
	counter = 1; // kelime sorgularken kac adimda buldugunu hesaplamak icin counteri 1 olarak baslatiyorum
	while(i < M && hashTable[index].head != NULL && strcmp(hashTable[index].kelime, word)){
		// i < M ile tabloyu 1 kere dondukten sonra bulamazsak dongunun bitmesini sagliyorum
		// != NULL kontrolu ile o indexin bos olup olmadigini kontrol ediyorum
		// != key kontrolu ile aradigim key ile hashTable'in o indexinde keyin esit olup olmadigini kontrol ediyorum
			counter++; // adim sayisini her dongude arttiriyorum
			i++; // o index doluysa ve aradigim key orada degilse i yi arttirip
			// tekrar key i hash fonksiyonuna sokuyorum
			// bu durum bize orada bir collusion olustugunu anlatiyor
			index = h(key,i);
	}
	if(hashTable[index].head != NULL && !strcmp(hashTable[index].kelime, word)){
		// dongu bittiginde aradigimi bulduysam
		// indexi donduruyorum
		return index;
	}
	else{
		// kelime table'de bulunmuyorsa -1 donduruyorum
		return -1;
	}
}

void hashIt(char* docName, char** word){
	// kelimeyi hashTable ' e yerlestirdigim fonksiyon
	toLower(word); // once gelen kelimeyi lowerCase 'e ceviriyorum
	int result = varMi(*word); // kelimenin tablede daha onceden olup olmadigini kontrol ediyorum
	
	if(result != -1){
		// kelime tablede bulunuyorsa
		NODE* temp = hashTable[result].head;
		// temp degiskeninde o kelimenin gectigi dokumanlari tutan listenin head'ini tutuyorum
		while(temp->next != NULL && strcmp(temp->name,docName)){
			// listeyi dolasarak o an okudugum dokumanin eklenip eklenmedigini kontrol ediyorum
			// bu kontrolu ayni dokumanda bir kelime birden fazla gectiginde
			// ayni dokuman adini tekrar eklememek icin yapiyorum
			temp = temp->next;
		}
		if(strcmp(temp->name,docName)){
			// dokuman daha onceden eklenmemisse
			// dokuman adini iceren bir node olusturup listenin sonuna ekliyorum
			NODE* doc = createNode(docName);
			temp->next = doc;
		}
		
	}
	else{
		// kelime Tablede bulunmuyorsa
		int key = strToInt(*word); // Horner method ile key i buluyrum
		int index; // key i kullanarak hash fonksiyonu ile bulacagim index i tutmak icin degisken
		int i = 0; // hash fonksiyonunda kullanilan index degiskeni icin degisken
		while(i < M && hashTable[h(key, i)].head != NULL){
			// i < M kontrolu tabloyu 1 kez dolasinca dongu bitmesi cin
			// diger kontrol o indexin bos olup olmadigini kontrol etmek icin
			i++; // o index doluysa hash fonksiyon indexini arttiriyorum
		}
		if(i < M){
			// bos bir indexe ulasildiginda
			index = h(key, i); // hash fonksiyonundan donen indexi degiskene atiyorum
			hashTable[index].head = createNode(docName); 
			// dokumanin adinin gectigi node olusturarak hashTableye ekledim
			strcpy(hashTable[index].kelime,*word); // gelen kelimeyi o indexteki struct a ekledim
			hashTable[index].key = key; // key degerini ekledim
			LoadFactor += factorArtis; // her eklenen kelimede loadFactoru arttýrdým
			printf("\nLoadFactor ---> %lf\n", LoadFactor); // Her eklenen deger icin loadFactor'u yazdiriyoruz.
			if(uyari == 2 && LoadFactor >= 0.8 && LoadFactor < 1){
					// loadfactor 0.8 ulasmis ve uyari degiskeni 2 ise program uyari veriyor
					// uyari degiskeninin 2 olmasi daha once 0.8 uyarisi verilmedigi anlamina geliyor
					printf("LoadFactor 0.8 degerini gecti\nTablonun en az %c80'i dolu!!!\n",'%');
			}
			else if(LoadFactor == 1){
				uyari = 1; // Loadfactor 1 e ulastiginda uyari degiskeni 1 oluyor daha fazla 0.8 uyarisi vermiyor
			}
		}
	}
}

int h1(int key){
	// 1. hash fonksiyonu
	return key % M;
}

int h2(int key){
	// 2. hash fonksiyonu
	return 1 + (key % (M-1));
}

int h(int key, int i){
	// ana hash fonksiyonu
	return (h1(key) + i*h2(key)) % M;
}

int strToInt(char* word){
	// Horner's Method ile string i integer a cevirdigim fonksiyon
	int length = strlen(word); // stringin uzunlugunu tuttum
	unsigned long long int hash = 0; // donecek degeri tuttugum degisken 0 olarak ilklendirdim
	// ancak boyle yaptigimda sigiyordu
	int i; // for dongu degiskeni
	// Derste Elif hocanin gosterdigi kod ile degeri hesapladim
	for(i = 0; i < length; i++){
		hash = (R * hash + (word[i] - 'a' + 1)) % M;
	}

	return hash % M; // integer a sigmadigi icin dondururken modunu aldim
}

void dokumanOku(){
	FILE* fptr; // okunacak dokumani tutmak icin FILE pointeri tuttum.
	char line[MAX]; // dokumandaki her bir satiri okumak icin tuttugum buffer
	char *word; // satirlari kelimelere ayirirken kelimeri tuttugum degisken
	char docName[16]; // dokumanin adini tutmak icin degisken
	int result; // kelime var mi fonksiyonundan donen degeri tutmak icin degisken
	printf("\nDokumanin adini giriniz : ");
	scanf("%s",docName);
	// kullanicidan dokuman adini input aldim
	fptr = fopen(docName,"r");
	// dokumanda degisiklik yapmayacagim icin "r" modda actim
	if(!fptr){
		// dosya acilamadiysa uyari verip programi sonlandirdim
		printf("Dosya acilamadi.\n");
		exit(0);
	}
	while(fgets(line, MAX, fptr)){
		// dosyanin sonuna kadar her bir satiri fgets ile aldim
		word = strtok(line, "\n"); // satirin sonundaki \n karakterini sildim
		word = strtok(line, " "); // strtok ile satirdaki kelimeleri ayirip word degiskenine atiyorum
		while(word){ 
		// satirdaki kelime bitene kadar yani stryok fonksiyonu NULL donene kadar dongu ile donup kelimeleri hashliyorum
			if(LoadFactor < 1){
				hashIt(docName,&word); // tablo daha dolmadigi icin kelimeyi hashlemek icin dokuman adiyla beraber hash fonksiyonuna yolluyorum
			}
			else{
				
				// tabloda dolduysa
				if(uyari){
					// uyari degiskeni 1 ise yani tablo doldu uyarisi daha verilmediyse
					printf("LoadFactor 1 degerine ulasti\nTablo tamamen dolu.\nEklenemeyen kelimeler ekrana yazdirilacak.\n");
					uyari = 0;
					// tablo doldu uyarisi verip uyari degiskenini 0 yapiyorum
				}
				// Tablo doldugu icin yeni gelen kelimeleri ekrana yazdiriyorum
				int result = varMi(word);
				if(result != -1){
					// bu kisim hashIt fonksiyonundaki kisimla ayni
					// tabloda kelime varsa dokuman adini ekledim sadece
					NODE* temp = hashTable[result].head;
					while(temp->next != NULL && strcmp(temp->name,docName)){
						temp = temp->next;
					}
					if(strcmp(temp->name,docName)){
						NODE* doc = createNode(docName);
						temp->next = doc;
					}
				}
				else{
					// kelime tabloda yoksa yer olmadigi icin eklenemedigini belirttim
					printf("Eklenemedi ---> %s\n",word);
				}
			}
			word = strtok(NULL, " "); // kalan kelimeleri strtok ile ayiriyorum
		}
	}
	fclose(fptr); // dokuman bitince dosyayi kapatiyorum
}

void kelimeSorgu(){
	// girilen kelimenin hangi dokumanlarda gectigini buldugum fonksiyon
	char word[100]; // kelimeyi tutmak icin degisken;
	char **lowerCase; // varMi fonksiyonuna word degiskenini yollayamadigim icin 
	// kelimenin lowerCase halini tututtugum degisken
	int result; // varMi fonksiyonundan doner degeri tuttugum fonksiyon
	NODE* temp; // dokumanlarin tutuldugu linkli listenin headini tutmak icin degisken
	printf("Sorgulamak istediginiz kelimeyi giriniz : ");
	scanf("%s",word);
	// kullanicidan kelimeyi input aldim
	strcpy(*lowerCase, word); // kelimeyi lowerCase degiskenine kopyaladim
	toLower(lowerCase); // kelimeyi lowerCase e cevirdim
	result = varMi(*lowerCase); // varMi fonksiyonu ile kelienin gecip gecmedigini kontrol ettim
	if(result != -1){
		
		// kelime tablede bulunuyorsa
		printf("\nKelime %d adimda bulundu.\n", counter);
		printf("%s'nin gectigi dokumanlar : \n",word);
		for(temp = hashTable[result].head; temp != NULL; temp = temp->next){
			// linkli listedeki her node'de bulunan dokuman isimlerini ekrana yazdirdim
			printf("%s\n",temp->name);
		}
		printf("----------------------------------\n");
	}
	else{
		// varMi fonksiyonundan -1 donduyse kelime hicbir dokumanda gecmiyor demektir
		printf("\n%d adim gidildi ve kelime bulunamadi.\n", counter);
		printf("%s hicbir dokumanda gecmemektedir.\n", word);
	}
}

void toLower(char** str){
	// kelimeyi lowerCase e cevirdigim fonksiyon
	int size = strlen(*str); // size degiskeninde kelimenin boyutunu tutuyorum
	int i; // for dongu degiskeni
	for(i = 0; i < size; i++){
		// karakterin degeri 97 ' den kucukse yani buyuk harfse 32 ekleyip kucuk harf haline ceviriyorum
		if( (*str)[i] < 97)
			(*str)[i] += 32;
	}
	
}

void lf_hashGuncelle(){
	// loadFactor ve hash tablosunu tuttugum dosyayi guncelledigim fonksiyon
	NODE* temp; // dokumanlarin tutuldugu linkli listenin headini tutmak icin degisken
	char kelime[100]; // kelimeleri tuttugum degisken
	int key; // key'leri tuttugum degisken
	int index; // indexleri tuttugum degisken
	fprintf(lf_hash,"%lf",LoadFactor); // ilk satira loadfactorun degerini yazdim
	for(index = 0; index < M; index++){
		// tablodaki her bir index icin
		if(hashTable[index].head != NULL){
			// index doluysa
			key = hashTable[index].key; // indexdeki key degerini degiskene atadim
			strcpy(kelime, hashTable[index].kelime); // indexdeki kelimeyi kelime degiskenine kopyaladim
			fprintf(lf_hash,"\n%d %d %s",index, key, kelime); // satira index bosluk key bosluk kelime olacak sekilde yazdirdim
			
			temp = hashTable[index].head; // listenin headini degiskene atadim
			while(temp != NULL){
				// listenin sonuna kadar her bir dokumanin adini bir bosluk birakarak ayný satirin devamina yazdirdim
				fprintf(lf_hash," %s",temp->name);
				temp = temp->next;
			}
		}
	}
	// dosyanin ilk satiri loadfactor
	// diger satirlar index bosluk key bosluk kelime bosluk 1.dokumanAdi bosluk 2.dokumanAdi .. olacak sekilde ekledim
	
}

void degerleriOku(){
	// program baslarken dosyadan degerleri okudugum fonksiyon
	char *name = "17011004.txt"; // tabloyu ve loadfactor ' u tuttugum dosyanin adini tuttugum degisken
	FILE* fp; // file islemi icn olusturdugum pointer
	char buffer[MAX]; // satirlari tuttugum buffer
	char *docName; // dokumanlarin isimlerini tuttugum degisken
	char word[100]; // kelimeleri tuttugum degisken
	int index; // indexleri tuttugum degisken
	int key; // keyleyi tuttugum degisken
	NODE* temp; // dokuman listesi icin tuttugum pointer
	fp = fopen(name, "r"); // dosyadan sadece okuma yapacagim icin "r" modda actim
	if(!fp){
		// dosya acilamadiysa
		// ya bir hata olusmustur ya da program ilk kez calistirildigi icin dosya olusmamistir
		// bu durumda loadfactoru 0 olarak baslatip bos bir table ile programi baslatiyorum
		LoadFactor = 0.0;
		uyari = 2;
		return;
	}
	// dosya acildiysa
	// ilk satirdaki degeri loadFactor 'a atiyorum
	fscanf(fp,"%lf\n",&LoadFactor);
	
	if(LoadFactor< 0.8){
		// okunan loadfactor degeri 0.8'den dusuk ise uyari degiskenini 2 yapiyorum 
		// 0.8 e ulastiginde uyari yazdirabilmek icin
		uyari = 2;
	}
	else if(LoadFactor < 1){
		// loadfactor 0.8 den buyuk ama 1den kucukse 0.8 uyarisini verip
		// uyari degerini 1 olarak atiyorum ki tekrar uyari vermesin
		printf("LoadFactor 0.8 degerine ulasmis\nTablonun en az %c80'i dolu!!!",'%');
		uyari = 1;
	}
	else{
		// loadfactor degeri 1 e ulasmis ise uyari degerini 0 yapiyorum ki tekrar uyari vermesin
		uyari = 0;
		printf("Tabloda yer yok!!!\nEklenemeyen kelimeler ekrana yazdirilacak.\n");
	}
	
	while(!feof(fp)){
		// dosyanin sonuna ulasana kadar
		fscanf(fp,"%d %d %s ",&index, &key, word);
		// satirdaki ilk 3 degeri alip index key ve word degiskenlerine atiyorum
		// degiskenklere atadigim degerleri hashTable'e yerlestiriyorum
		hashTable[index].key = key;
		strcpy(hashTable[index].kelime, word);
		// index key ve word degerlerini okuduktan sonra satirin kalanini fgets ile aliyorum
		fgets(buffer,MAX,fp);
		docName = strtok(buffer,"\n");
		docName = strtok(buffer," ");
		// satirin kalanindaki her bir kelime o kelimenin gectigi dokumanlarin isimleri
		while(docName){
			// her bir ismi o indexdeki linkli listeye node olarak ekliyorum
			temp = createNode(docName);
			temp->next = hashTable[index].head;
			hashTable[index].head = temp;
			docName = strtok(NULL, " ");
		}
	}
	// dosyadan okuma islemi bittiginde dosyayi kapatiyorum
	fclose(fp);
}
