#include <stdio.h>
#include <stdlib.h>

struct Person { //Kiþi yapýsý 
    struct Person *prev, *next; //Kiþinin öncesini ve sonrasýný tutan kiþi yapýsý göstericisi deðiþkenleri  
    int page; //Kiþinin hash tablosundaki yer bilgisini tutan tam sayý deðiþkeni 
    char* idNumber; //Kiþinin kimlik numarasýný tutan karakter göstericisi deðiþkeni 
    char* nameSurname; //Kiþinin adýný ve soyadýný tutan karakter göstericisi deðiþkeni 
    int birthYear; //Kiþinin doðum yýlýmý tutan tam sayý deðiþkeni 
    char* address; //Kiþinin ikamet þehrini tutan karakter göstericisi deðiþkeni 
    int placeOnLL; //Kiþinin cache'deki yer bilgisini tutan tam sayý deðiþkeni 
};
 

struct Queue { //Kuyruk yapýsý 
    int count; //Kuyryk yapýsýndaki eleman sayýsýný tutan tam sayý deðiþkeni 
    int frames; //Kuruk yapýsýnýn sýnýr deðerini tutan tam sayý deðiþkeni 
    struct Person *front, *rear; //Kuyruk yapýsýnýn ilk ve son verisini tutan kiþi yapýsý göstericisi deðiþkenleri 
};
 
struct Hash { //Hash yapýsý 
    int capacity; //Hash kapasitesini tutan tam sayý deðiþkeni 
    struct Person** array; //Hash içerisindeki kiþi yapýlarýný tutan kiþi yapýsý göstericisini gösteren gösterici deðiþkeni 
};

int h1(int key, int m){ //Birinci hash fonksiyonunu ifade eden iþlev 
	int hr = key % m; //Verilen deðerin, hash kapasitesine göre modu alýnýr
	return hr;
}

int h2(int key, int m){ //Ýkinci hash fonksiyonunu ifade eden iþlev 
	int hr = key % (m-1); //Verilen deðerin, hash kapasitesinin bir eksiðine göre modu alýnýr
	return 1 + hr; 
}

int h(int key, int i, int m){ //Birinci ve ikinci hash fonksiyonlarýnýn kombinasyonunu gönderen iþlev 
	int hr1 = h1(key, m); //Birinci hash fonksiyonu 
	int hr2 = i*h2(key, m); //Ýkinci hash fonksiyonu 
	int hr = (hr1+ hr2)%m; 
	return hr;
}

int isprime(int number){ //Verilen sayýnýn asal sayý olup olmadýðýný ölçen iþlev 
	int k;
	if(number == 0 || number == 1) //Defult koþullar 
		return 0;
	if(number % 2 == 0) //2'ye bölüm
		return number == 2;
	if(number % 3 == 0) //3'e bölüm
		return number == 3;
	if(number % 5 == 0) //5'e bölüm
		return number == 5;
	for (k = 7; k * k <= number; k+= 2) //daha üst sayýlara bülüm
		if (number % k == 0)
			return 0;
	return 1; //Hiçbirine bölünmedi ise asaldýr.
}
 
struct Person* newPerson(struct Person* person) //Yeni kiþi yapýsý oluþturan iþlev 
{
    struct Person* temp = (struct Person*)malloc(sizeof(struct Person)); //Yeni kiþi için dinamik  bellek yönetimi 
    temp->page = person->page; //Hash'teki yeri atama 
    temp->address = person->address; //Adres atama
    temp->birthYear = person->birthYear; //Doðum yýlý atama 
    temp->idNumber = person->idNumber; //Kimlik no atama 
    temp->nameSurname = person->nameSurname; //Ad soyad atama 
    temp->placeOnLL = 0; //Cache'deki yeri atama 
    temp->prev = temp->next = NULL; //Kiþiden önce ve sonra gelen kiþilerin adreslerini atama 
    return temp; //Oluþtutulan kiþiyi döndür 
}
 
struct Queue* createQueue(int frames) //Yeni bir kuyruk yapýsý oluþturan iþlev 
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); //Kuyruk yapýsý için dinamik bellek yönetimi 
    queue->count = 0; //Eleman sayýsý atama 
    queue->front = queue->rear = NULL; //Kuyruðun baþ ve son elemanlarýný atama 
    queue->frames = frames; //Kapasite atama 
    return queue; //Oluþtutulan kuyruðu döndür 
}
 
struct Hash* createHash(int capacity) //Yeni bir hash yapýsý oluþturan iþlev 
{
    struct Hash* hash = (struct Hash*)malloc(sizeof(struct Hash)); //Hash yapýsý için dinamik bellek yönetimi 
    hash->capacity = capacity; //Kapasite atama 
    hash->array = (struct Person**)malloc(hash->capacity * sizeof(struct Person*)); //Hash'teki kiþi tablosu için dinamik bellek yönetimi 
    int i; //iterasyon 
    for (i = 0; i < hash->capacity; ++i) //Hash tablosunu ilklendiren döngü 
        hash->array[i] = NULL;
    return hash;
}
 
int AreAllFramesFull(struct Queue* queue) //Kuyruk yapýsýnýn doluluðunu kontrol eden iþlev 
{
    return queue->count == queue->frames; //Kuyrukta bulunan elaman sayýsýnýn, kapasiteye eriþip eriþmediðinin kontrolü 
}
 
int isQueueEmpty(struct Queue* queue) //Kuyuk yapýsýnýn boþ olma durumunu kontrol eden iþlev 
{
    return queue->rear == NULL; //Kuyruðun eðer son elemaný yoksa kuyruk boþ demektir
}
 
void deQueue(struct Queue* queue) //Kuyruk yapýsýndan eleman çýkarma iþlevi 
{
    if (isQueueEmpty(queue)) //Kuyruðun boþ olup olmadýðý kontrol edilir 
        return;
    if (queue->front == queue->rear) //Kuyrukta tek elaman olup olmadýðý kontrol edilir 
        queue->front = NULL; //Tek eleman varsa kuyruk sýfýrlanýr 
    struct Person* temp = queue->rear; //Kuyruk yapýsýndaki son kiþinin bilgisi alýnýr 
    printf("%s CIKARILDI\n\n", temp->nameSurname); //Son kiþinin çýkarýlacaðý bilgisi verilir 
    queue->rear = queue->rear->prev; // Son kiþiden bir önceki kiþi, artýk son kiþidir 
    if (queue->rear) // Kuyruk yapýsýnýn son kiþisi varsa 
        queue->rear->next = NULL; // Sonra gelen hiçbir kiþi yoktur
    free(temp); // Bellekte alan açýlýr silinen kiþi için 
    queue->count--; // Kuyruk yapýsý elaman sayýsý azalýr 
}
 
void Enqueue(struct Queue* queue, struct Hash* hash, struct Person* person) //Kuyruk yapýsýna elaman ekleme iþlevi 
{
    if (AreAllFramesFull(queue)) { //Eðer ki kuyruk yapýsý dolu ise 
        hash->array[queue->rear->page] = NULL; //Kuyruðun sonundaki kiþi hash'ten silinir 
        deQueue(queue); //Kuyruktan da silinir 
    }
    struct Person* temp = newPerson(person); //Yeni bir kiþi yapýsý oluþturulur 
	temp->next = queue->front; // Bu yeni kiþi, kuyruk yapýsýnda baþa alýnýr 
    if (isQueueEmpty(queue)) //Kuyruk yapýsýnýn boþ olup olmadýðý kontrol edilir 
        queue->rear = queue->front = temp; //Boþsa, oluþturulan elaman eklenir 
    else //Kuyruk boþ deðilse 
    {
        queue->front->prev = temp; // Yeni kiþi, kuyruk yaýsýnda baþa alýnýr 
        queue->front = temp; 
    }
    hash->array[person->page] = temp; //Hash tablosuna yeni kiþi eklenir 
    queue->count++; // Kuyruktaki elaman sayýsý arttýrýlýr 
    // KUYRUKTAKÝ GÜNCEL ELEMANLARI GÖSTEREN DÖNGÜ 
    struct Person* tmp = queue->front; 
    int i=1;
    while(tmp!=NULL){
    	tmp->placeOnLL = i;
    	printf("%s(cache:%d, hash:%d)\n", tmp->nameSurname, tmp->placeOnLL, tmp->page);
    	i++;
    	tmp = tmp->next;
	}
	printf("\n");
}

int horner(char* id, int charLength, int frames) //Horner parabol bölümü yapan iþlev 
{ 
	int i; //iterasyon 
    int result = id[0]-48; //Sonuç ilklendirilir  
    for (i=1; i<charLength; i++) //Karakter dizisi boyunca 
        result = result*frames + id[i]-48; //Her karakterin sayýsal deðeri, basamak deðerine göre sonuca eklenir 
    return result; 
} 

void Refer(struct Queue* queue, struct Hash* hash, struct Person* person) //Kiþi yapýsýnýn hash tablosunda yazýlacaðý yeri ayarlayan iþlev 
{
	int i=0; //Ýterasyon 
	int n=0; //Kimlik no uzunluðunu tutan tam sayý deðiþkeni 
	int index = 0; //Hash fonksiyonu için kullanýlacak iterasyon 
	while(person->idNumber[i]!='\0'){ // Kimlik no uzunluðunu hesaplayan döngü 
		i++;
		n++;
	}
	int hornerNumber = horner(person->idNumber, n, queue->frames); //Kimlik numarasýna göre hesaplanan Horner deðerini tutan tam sayý deðiþkeni 
	person->page = h(hornerNumber, index, hash->capacity); //Atanacak hash sayfasýnýn ilklendirilmesi 
	index++; 
	while(1){ //Sürekli dönmeye olanak veren döngü 
    	struct Person* ref = hash->array[person->page]; //Atanacak hash sayfasýnýn referans alýnmasý 
    	//ATANACAK YERDE ELEMAN YOKSA 
    	if (ref == NULL){ //Eðer ki referans kiþi yapýsý boþ ise 
    		struct Person* tmp = queue->front; // Kuyruðun baþlangýç deðeri, geçici kiþi yapýsýnda tutulur 
    		//HASH TABLOSUNDA ATANACAK KÝÞÝ VAR MI, YOK MU DÝYE KONTROL EDÝLÝR 
    		while(tmp!=NULL){ //Geçici kiþi yapýsý boþ olmadýðý sürece 
    			if(strcmp(tmp->idNumber, person->idNumber)==0){ //Eðer atanacak kiþi, hash tablosunda mevcut ise 
    				person->page = tmp->page; // Atanacak sayfa ayarlanýr 
    				ref = hash->array[person->page]; // Atama yapýlýr 
    				if (ref != queue->front) { // Eðer ki atanan kiþi, kuyruk baþlangýcý deðilse 
        				ref->prev->next = ref->next; //Atanan kiþi, bir önceki konumundan silinir 
        				if (ref->next) // Atanan kiþiden sonrasý mevcut ise 
            				ref->next->prev = ref->prev; //Gerekli ayarlamalar yapýlýr 
        				if (ref == queue->rear) { //Eðer ki referans kiþisi, kuyruk sonu ise 
            				queue->rear = ref->prev; // Referanstan önceki kiþi, sona gelir 
            				queue->rear->next = NULL; //Referans sonrasý kiþi mevcut olmaz 
        				}
        				ref->next = queue->front; //Referans sonrasý, kuyruk baþlangýcý olur 
        				ref->prev = NULL; //Referans öncesi yok olur 
        				ref->next->prev = ref; 
        				queue->front = ref; //Referensa, kuyruk yapýsý baþý getirilir 
        				tmp = queue->front; //Kuyruk yapýsý baþýna da, geçici kiþi getirilir 
        				//GÜNCEL CACHE'Ý EKRANA YAZDIRMA 
    					i=1; 
    					while(tmp!=NULL){
    						tmp->placeOnLL = i;
    						printf("%s(cache:%d, hash:%d)\n", tmp->nameSurname, tmp->placeOnLL, tmp->page);
    						i++;
    						tmp = tmp->next;
						}
						printf("\n");
        				return;
        			}
				}
				tmp = tmp->next;
			}
    		Enqueue(queue, hash, person); //Ýlgili kiþi kuyruða da eklenir 
    		return;
		}
		//EÐER KÝ ATANACAK ALAN DOLU ÝSE HORNER YÖNTEMÝ ÝLE DEÐER SEÇÝLEREK ATAMA YAPILIR 
		else if(strcmp(ref->idNumber,person->idNumber)!=0){ 
			person->page = h(hornerNumber, index, hash->capacity);
			index++;
		}		
    	else if (ref != queue->front) {//Eðer ki mevcut olan kiþi, kuyruk yapýsýnýn baþýnda deðilse gerekli ayarlar yapýlýr 
        	ref->prev->next = ref->next;
        	if (ref->next)
            	ref->next->prev = ref->prev;
        	if (ref == queue->rear) {
            	queue->rear = ref->prev;
            	queue->rear->next = NULL;
        	}
        	ref->next = queue->front;
        	ref->prev = NULL;
        	ref->next->prev = ref;
        	queue->front = ref;
        	struct Person* tmp = queue->front;
    			int i=1;
    			//GÜNCEL CACHE'Ý EKRANA YAZDIRMA
    			while(tmp!=NULL){
    				tmp->placeOnLL = i;
    				printf("%s(cache:%d, hash:%d)\n", tmp->nameSurname, tmp->placeOnLL, tmp->page);
    				i++;
    				tmp = tmp->next;
				}
				printf("\n");
        	return;
        }
	}
}

int detectPersonCount(FILE* f, char* file_name){ //Kayýt sayýsýný saptayan iþlev 
	int ch; //Okunan karakteri tutan karakter deðiþkeni 
    int personCount=0; //Kayýt sayýsý ilklendirme 
	if ((f = fopen(file_name, "r")) == NULL) { 
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	while ((ch = fgetc(f)) != EOF){ //Dosya sonuna gelene kadar 
		if(ch==10){ //Eðer ki satýr sonuna gelindiyse 
			personCount++; //Kayýt sayýsýný bir arttýr 
		}
	}
	personCount++;
	fclose(f);
	return personCount;
}

struct Person* readPersons(int personCount, FILE* f){ //Kayýtlarý okuyan iþlev 
	int i; //Ýterasyon 
	struct Person* persons = (struct Person*)malloc(personCount*sizeof(struct Person)); //Kiþileri tutan kiþi yapýsý göstericisi 
	for(i=0; i<personCount;i++){ // Kiþileri ilklendirme ve dinamik bellek yönetimi 
		persons[i].address = (char*)malloc(20);
		persons[i].birthYear = 0;
		persons[i].idNumber = (char*)malloc(12);
		persons[i].nameSurname = (char*)malloc(40);
		persons[i].next = NULL;
		persons[i].page = 0;
		persons[i].placeOnLL = 0;
		persons[i].prev = NULL;
	}
	int ch; //Okunacak karakteri tutan karakter deðiþkeni 
	i=0; //Ýterasyon 
	int j; //Ýterasyon 
	if ((f = fopen("test.txt", "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	ch=fgetc(f); //Dosyadan karakter okuma 
	while (ch != EOF){ //Dosya sonuna gelene kadar 
		j=0; //Ýterasyon ilklendirme 
		while(ch!=32){ //Boþluða gelene kadar 
			persons[i].idNumber[j] = ch; //Kimlik numarasý atama 
			j++;
			ch=fgetc(f);
		}
		persons[i].idNumber[j] = '\0';
		j=0;
		ch=fgetc(f);
		while(ch!=32){
			persons[i].nameSurname[j] = ch; //Ad ve soyad atama
			j++;
			ch=fgetc(f);
		}
		persons[i].nameSurname[j] = ch;
		j++;
		ch=fgetc(f);
		while(ch!=32){
			persons[i].nameSurname[j] = ch;
			j++;
			ch=fgetc(f);
		}
		persons[i].nameSurname[j] = '\0';
		j=0;
		ch=fgetc(f);
		while(ch!=32){
			persons[i].birthYear = (persons[i].birthYear*10)+(ch-48); //Doðum yýlý atama 
			j++;
			ch=fgetc(f);
		}
		j=0;
		ch=fgetc(f);
		while(ch!=10 && ch!=EOF){
			persons[i].address[j] = ch; //Adres bilgisi atama 
			j++;
			ch=fgetc(f);
		}
		persons[i].address[j] = '\0';
		i++;
		ch=fgetc(f);
	}
	fclose(f);
	return persons;
}

int main()
{
	int i; //Ýterasyon 
	int n; //Kuyruk kapasitesi 
	printf("Kuyruk kapasitesini girin : ");
	scanf("%d",&n);
    struct Queue* q = createQueue(n); //Yeni kuyruk yapýsý oluþturma 
    int tableLength = n/0.6; //Hash boyutunu ayarlama 
    while(isprime(tableLength)==0){ //Hash boyut fonksiyonuna göre en büyük asal sayýyý bulma
    	tableLength++;
	}
    struct Hash* hash = createHash(tableLength); //Yeni hash yapýsý oluþturma 
    FILE *f; //Dosya göstericisi 
    char file_name[256]; //Dosya adý 
    printf("Yazdirilacak dosyanin ismi : ");
	scanf("%s",&file_name); 
	printf("\n"); 
	int personCount = detectPersonCount(f, "test.txt"); //Kayýt sayýsý 
	printf("%d kayit var\n\n", personCount); 
	struct Person* persons = readPersons(personCount, f); //Kayýtlar dizisi 
	for (i=0;i<personCount;i++){ //KAYITLARI EKRANA YAZDIRMA 
		printf("%s %s %d %s\n", persons[i].idNumber, persons[i].nameSurname, persons[i].birthYear, persons[i].address);
	}
	printf("\n");
	
	for (i=0;i<personCount;i++){
		Refer(q, hash, &persons[i]); //Kayýtlarý hash yapýsýna ekleme 
	}
	printf("\n");
	//PRINT
    printf("KISI LISTESININ SON HALI : \n"); 
    printf("\n");
    for(i=0;i<hash->capacity;i++){
    	if(hash->array[i]!=NULL){
    		printf("Kimlik no : %s\n", hash->array[i]->idNumber);
    		printf("Ad soyad : %s\n", hash->array[i]->nameSurname);
    		printf("Dogum tarihi : %d\n", hash->array[i]->birthYear);
    		printf("Adres : %s\n", hash->array[i]->address);
    		printf("Hash'teki yeri : %d\n", hash->array[i]->page);
    		printf("Cache'deki yeri : %d\n", hash->array[i]->placeOnLL);
    		printf("\n");
		}
	}
    return 0;
}
