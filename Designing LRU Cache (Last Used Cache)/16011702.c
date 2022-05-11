#include <stdio.h>
#include <stdlib.h>

struct Person { //Ki�i yap�s� 
    struct Person *prev, *next; //Ki�inin �ncesini ve sonras�n� tutan ki�i yap�s� g�stericisi de�i�kenleri  
    int page; //Ki�inin hash tablosundaki yer bilgisini tutan tam say� de�i�keni 
    char* idNumber; //Ki�inin kimlik numaras�n� tutan karakter g�stericisi de�i�keni 
    char* nameSurname; //Ki�inin ad�n� ve soyad�n� tutan karakter g�stericisi de�i�keni 
    int birthYear; //Ki�inin do�um y�l�m� tutan tam say� de�i�keni 
    char* address; //Ki�inin ikamet �ehrini tutan karakter g�stericisi de�i�keni 
    int placeOnLL; //Ki�inin cache'deki yer bilgisini tutan tam say� de�i�keni 
};
 

struct Queue { //Kuyruk yap�s� 
    int count; //Kuyryk yap�s�ndaki eleman say�s�n� tutan tam say� de�i�keni 
    int frames; //Kuruk yap�s�n�n s�n�r de�erini tutan tam say� de�i�keni 
    struct Person *front, *rear; //Kuyruk yap�s�n�n ilk ve son verisini tutan ki�i yap�s� g�stericisi de�i�kenleri 
};
 
struct Hash { //Hash yap�s� 
    int capacity; //Hash kapasitesini tutan tam say� de�i�keni 
    struct Person** array; //Hash i�erisindeki ki�i yap�lar�n� tutan ki�i yap�s� g�stericisini g�steren g�sterici de�i�keni 
};

int h1(int key, int m){ //Birinci hash fonksiyonunu ifade eden i�lev 
	int hr = key % m; //Verilen de�erin, hash kapasitesine g�re modu al�n�r
	return hr;
}

int h2(int key, int m){ //�kinci hash fonksiyonunu ifade eden i�lev 
	int hr = key % (m-1); //Verilen de�erin, hash kapasitesinin bir eksi�ine g�re modu al�n�r
	return 1 + hr; 
}

int h(int key, int i, int m){ //Birinci ve ikinci hash fonksiyonlar�n�n kombinasyonunu g�nderen i�lev 
	int hr1 = h1(key, m); //Birinci hash fonksiyonu 
	int hr2 = i*h2(key, m); //�kinci hash fonksiyonu 
	int hr = (hr1+ hr2)%m; 
	return hr;
}

int isprime(int number){ //Verilen say�n�n asal say� olup olmad���n� �l�en i�lev 
	int k;
	if(number == 0 || number == 1) //Defult ko�ullar 
		return 0;
	if(number % 2 == 0) //2'ye b�l�m
		return number == 2;
	if(number % 3 == 0) //3'e b�l�m
		return number == 3;
	if(number % 5 == 0) //5'e b�l�m
		return number == 5;
	for (k = 7; k * k <= number; k+= 2) //daha �st say�lara b�l�m
		if (number % k == 0)
			return 0;
	return 1; //Hi�birine b�l�nmedi ise asald�r.
}
 
struct Person* newPerson(struct Person* person) //Yeni ki�i yap�s� olu�turan i�lev 
{
    struct Person* temp = (struct Person*)malloc(sizeof(struct Person)); //Yeni ki�i i�in dinamik  bellek y�netimi 
    temp->page = person->page; //Hash'teki yeri atama 
    temp->address = person->address; //Adres atama
    temp->birthYear = person->birthYear; //Do�um y�l� atama 
    temp->idNumber = person->idNumber; //Kimlik no atama 
    temp->nameSurname = person->nameSurname; //Ad soyad atama 
    temp->placeOnLL = 0; //Cache'deki yeri atama 
    temp->prev = temp->next = NULL; //Ki�iden �nce ve sonra gelen ki�ilerin adreslerini atama 
    return temp; //Olu�tutulan ki�iyi d�nd�r 
}
 
struct Queue* createQueue(int frames) //Yeni bir kuyruk yap�s� olu�turan i�lev 
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); //Kuyruk yap�s� i�in dinamik bellek y�netimi 
    queue->count = 0; //Eleman say�s� atama 
    queue->front = queue->rear = NULL; //Kuyru�un ba� ve son elemanlar�n� atama 
    queue->frames = frames; //Kapasite atama 
    return queue; //Olu�tutulan kuyru�u d�nd�r 
}
 
struct Hash* createHash(int capacity) //Yeni bir hash yap�s� olu�turan i�lev 
{
    struct Hash* hash = (struct Hash*)malloc(sizeof(struct Hash)); //Hash yap�s� i�in dinamik bellek y�netimi 
    hash->capacity = capacity; //Kapasite atama 
    hash->array = (struct Person**)malloc(hash->capacity * sizeof(struct Person*)); //Hash'teki ki�i tablosu i�in dinamik bellek y�netimi 
    int i; //iterasyon 
    for (i = 0; i < hash->capacity; ++i) //Hash tablosunu ilklendiren d�ng� 
        hash->array[i] = NULL;
    return hash;
}
 
int AreAllFramesFull(struct Queue* queue) //Kuyruk yap�s�n�n dolulu�unu kontrol eden i�lev 
{
    return queue->count == queue->frames; //Kuyrukta bulunan elaman say�s�n�n, kapasiteye eri�ip eri�medi�inin kontrol� 
}
 
int isQueueEmpty(struct Queue* queue) //Kuyuk yap�s�n�n bo� olma durumunu kontrol eden i�lev 
{
    return queue->rear == NULL; //Kuyru�un e�er son eleman� yoksa kuyruk bo� demektir
}
 
void deQueue(struct Queue* queue) //Kuyruk yap�s�ndan eleman ��karma i�levi 
{
    if (isQueueEmpty(queue)) //Kuyru�un bo� olup olmad��� kontrol edilir 
        return;
    if (queue->front == queue->rear) //Kuyrukta tek elaman olup olmad��� kontrol edilir 
        queue->front = NULL; //Tek eleman varsa kuyruk s�f�rlan�r 
    struct Person* temp = queue->rear; //Kuyruk yap�s�ndaki son ki�inin bilgisi al�n�r 
    printf("%s CIKARILDI\n\n", temp->nameSurname); //Son ki�inin ��kar�laca�� bilgisi verilir 
    queue->rear = queue->rear->prev; // Son ki�iden bir �nceki ki�i, art�k son ki�idir 
    if (queue->rear) // Kuyruk yap�s�n�n son ki�isi varsa 
        queue->rear->next = NULL; // Sonra gelen hi�bir ki�i yoktur
    free(temp); // Bellekte alan a��l�r silinen ki�i i�in 
    queue->count--; // Kuyruk yap�s� elaman say�s� azal�r 
}
 
void Enqueue(struct Queue* queue, struct Hash* hash, struct Person* person) //Kuyruk yap�s�na elaman ekleme i�levi 
{
    if (AreAllFramesFull(queue)) { //E�er ki kuyruk yap�s� dolu ise 
        hash->array[queue->rear->page] = NULL; //Kuyru�un sonundaki ki�i hash'ten silinir 
        deQueue(queue); //Kuyruktan da silinir 
    }
    struct Person* temp = newPerson(person); //Yeni bir ki�i yap�s� olu�turulur 
	temp->next = queue->front; // Bu yeni ki�i, kuyruk yap�s�nda ba�a al�n�r 
    if (isQueueEmpty(queue)) //Kuyruk yap�s�n�n bo� olup olmad��� kontrol edilir 
        queue->rear = queue->front = temp; //Bo�sa, olu�turulan elaman eklenir 
    else //Kuyruk bo� de�ilse 
    {
        queue->front->prev = temp; // Yeni ki�i, kuyruk ya�s�nda ba�a al�n�r 
        queue->front = temp; 
    }
    hash->array[person->page] = temp; //Hash tablosuna yeni ki�i eklenir 
    queue->count++; // Kuyruktaki elaman say�s� artt�r�l�r 
    // KUYRUKTAK� G�NCEL ELEMANLARI G�STEREN D�NG� 
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

int horner(char* id, int charLength, int frames) //Horner parabol b�l�m� yapan i�lev 
{ 
	int i; //iterasyon 
    int result = id[0]-48; //Sonu� ilklendirilir  
    for (i=1; i<charLength; i++) //Karakter dizisi boyunca 
        result = result*frames + id[i]-48; //Her karakterin say�sal de�eri, basamak de�erine g�re sonuca eklenir 
    return result; 
} 

void Refer(struct Queue* queue, struct Hash* hash, struct Person* person) //Ki�i yap�s�n�n hash tablosunda yaz�laca�� yeri ayarlayan i�lev 
{
	int i=0; //�terasyon 
	int n=0; //Kimlik no uzunlu�unu tutan tam say� de�i�keni 
	int index = 0; //Hash fonksiyonu i�in kullan�lacak iterasyon 
	while(person->idNumber[i]!='\0'){ // Kimlik no uzunlu�unu hesaplayan d�ng� 
		i++;
		n++;
	}
	int hornerNumber = horner(person->idNumber, n, queue->frames); //Kimlik numaras�na g�re hesaplanan Horner de�erini tutan tam say� de�i�keni 
	person->page = h(hornerNumber, index, hash->capacity); //Atanacak hash sayfas�n�n ilklendirilmesi 
	index++; 
	while(1){ //S�rekli d�nmeye olanak veren d�ng� 
    	struct Person* ref = hash->array[person->page]; //Atanacak hash sayfas�n�n referans al�nmas� 
    	//ATANACAK YERDE ELEMAN YOKSA 
    	if (ref == NULL){ //E�er ki referans ki�i yap�s� bo� ise 
    		struct Person* tmp = queue->front; // Kuyru�un ba�lang�� de�eri, ge�ici ki�i yap�s�nda tutulur 
    		//HASH TABLOSUNDA ATANACAK K��� VAR MI, YOK MU D�YE KONTROL ED�L�R 
    		while(tmp!=NULL){ //Ge�ici ki�i yap�s� bo� olmad��� s�rece 
    			if(strcmp(tmp->idNumber, person->idNumber)==0){ //E�er atanacak ki�i, hash tablosunda mevcut ise 
    				person->page = tmp->page; // Atanacak sayfa ayarlan�r 
    				ref = hash->array[person->page]; // Atama yap�l�r 
    				if (ref != queue->front) { // E�er ki atanan ki�i, kuyruk ba�lang�c� de�ilse 
        				ref->prev->next = ref->next; //Atanan ki�i, bir �nceki konumundan silinir 
        				if (ref->next) // Atanan ki�iden sonras� mevcut ise 
            				ref->next->prev = ref->prev; //Gerekli ayarlamalar yap�l�r 
        				if (ref == queue->rear) { //E�er ki referans ki�isi, kuyruk sonu ise 
            				queue->rear = ref->prev; // Referanstan �nceki ki�i, sona gelir 
            				queue->rear->next = NULL; //Referans sonras� ki�i mevcut olmaz 
        				}
        				ref->next = queue->front; //Referans sonras�, kuyruk ba�lang�c� olur 
        				ref->prev = NULL; //Referans �ncesi yok olur 
        				ref->next->prev = ref; 
        				queue->front = ref; //Referensa, kuyruk yap�s� ba�� getirilir 
        				tmp = queue->front; //Kuyruk yap�s� ba��na da, ge�ici ki�i getirilir 
        				//G�NCEL CACHE'� EKRANA YAZDIRMA 
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
    		Enqueue(queue, hash, person); //�lgili ki�i kuyru�a da eklenir 
    		return;
		}
		//E�ER K� ATANACAK ALAN DOLU �SE HORNER Y�NTEM� �LE DE�ER SE��LEREK ATAMA YAPILIR 
		else if(strcmp(ref->idNumber,person->idNumber)!=0){ 
			person->page = h(hornerNumber, index, hash->capacity);
			index++;
		}		
    	else if (ref != queue->front) {//E�er ki mevcut olan ki�i, kuyruk yap�s�n�n ba��nda de�ilse gerekli ayarlar yap�l�r 
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
    			//G�NCEL CACHE'� EKRANA YAZDIRMA
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

int detectPersonCount(FILE* f, char* file_name){ //Kay�t say�s�n� saptayan i�lev 
	int ch; //Okunan karakteri tutan karakter de�i�keni 
    int personCount=0; //Kay�t say�s� ilklendirme 
	if ((f = fopen(file_name, "r")) == NULL) { 
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	while ((ch = fgetc(f)) != EOF){ //Dosya sonuna gelene kadar 
		if(ch==10){ //E�er ki sat�r sonuna gelindiyse 
			personCount++; //Kay�t say�s�n� bir artt�r 
		}
	}
	personCount++;
	fclose(f);
	return personCount;
}

struct Person* readPersons(int personCount, FILE* f){ //Kay�tlar� okuyan i�lev 
	int i; //�terasyon 
	struct Person* persons = (struct Person*)malloc(personCount*sizeof(struct Person)); //Ki�ileri tutan ki�i yap�s� g�stericisi 
	for(i=0; i<personCount;i++){ // Ki�ileri ilklendirme ve dinamik bellek y�netimi 
		persons[i].address = (char*)malloc(20);
		persons[i].birthYear = 0;
		persons[i].idNumber = (char*)malloc(12);
		persons[i].nameSurname = (char*)malloc(40);
		persons[i].next = NULL;
		persons[i].page = 0;
		persons[i].placeOnLL = 0;
		persons[i].prev = NULL;
	}
	int ch; //Okunacak karakteri tutan karakter de�i�keni 
	i=0; //�terasyon 
	int j; //�terasyon 
	if ((f = fopen("test.txt", "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	ch=fgetc(f); //Dosyadan karakter okuma 
	while (ch != EOF){ //Dosya sonuna gelene kadar 
		j=0; //�terasyon ilklendirme 
		while(ch!=32){ //Bo�lu�a gelene kadar 
			persons[i].idNumber[j] = ch; //Kimlik numaras� atama 
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
			persons[i].birthYear = (persons[i].birthYear*10)+(ch-48); //Do�um y�l� atama 
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
	int i; //�terasyon 
	int n; //Kuyruk kapasitesi 
	printf("Kuyruk kapasitesini girin : ");
	scanf("%d",&n);
    struct Queue* q = createQueue(n); //Yeni kuyruk yap�s� olu�turma 
    int tableLength = n/0.6; //Hash boyutunu ayarlama 
    while(isprime(tableLength)==0){ //Hash boyut fonksiyonuna g�re en b�y�k asal say�y� bulma
    	tableLength++;
	}
    struct Hash* hash = createHash(tableLength); //Yeni hash yap�s� olu�turma 
    FILE *f; //Dosya g�stericisi 
    char file_name[256]; //Dosya ad� 
    printf("Yazdirilacak dosyanin ismi : ");
	scanf("%s",&file_name); 
	printf("\n"); 
	int personCount = detectPersonCount(f, "test.txt"); //Kay�t say�s� 
	printf("%d kayit var\n\n", personCount); 
	struct Person* persons = readPersons(personCount, f); //Kay�tlar dizisi 
	for (i=0;i<personCount;i++){ //KAYITLARI EKRANA YAZDIRMA 
		printf("%s %s %d %s\n", persons[i].idNumber, persons[i].nameSurname, persons[i].birthYear, persons[i].address);
	}
	printf("\n");
	
	for (i=0;i<personCount;i++){
		Refer(q, hash, &persons[i]); //Kay�tlar� hash yap�s�na ekleme 
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
