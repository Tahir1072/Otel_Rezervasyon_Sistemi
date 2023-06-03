#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

#define MAX_ODA 1000

struct CustomerInfo{
	unsigned int id;
	char customerName[50];
	char customerSurname[50];
	unsigned int roomNumber;
};

struct DateTime{
	unsigned int day;
	unsigned int month;
	unsigned int year;
};

struct RoomInfo{
	unsigned int id;
	unsigned int personOfNumber;
	unsigned int roomNumber;
	bool roomState;
	float price;
	struct DateTime entryDate;
	struct DateTime exitDate;
};

struct Personel{
	char personelName[50];
    char password[50];
};

enum{
	customer,
	personel
};

RoomInfo* GetRooms(int** size);
RoomInfo* GetRooms();
int CheckRoomDetails(RoomInfo *rooms,int roomNumber,int size);
CustomerInfo FindCustomer(int id);
void ReservationClear(RoomInfo *rooms,CustomerInfo customer);
int CalculatePrice(RoomInfo *rooms,int roomNumber);
void MenuCustomer();

//Boyutu bilinen bir diziye ekleme iþlemi yapma.
void Add(struct RoomInfo room,struct RoomInfo** array,int size){
	if(size != 0 && size % 4 == 0){
		*array = (struct RoomInfo*)realloc(*array,sizeof(struct RoomInfo) * (size + 4));
		(*array)[size] = room;
	}
	else{
		(*array)[size] = room;
	}
}


void MenuPersonel() {
	
	int temp = 0;
	printf("1-Yeni musteri girisi\n2-Dolu oda bilgileri goruntule\n3-Odalardan cikis yapma islemi\n");
	scanf("%d", &temp);

	// Case etiketleri arasýnda deðiþken tanýmlamak için blok tanýmlýyoruz
	switch(temp) {
		case 1: {
			// Case 1 için lokal deðiþkenler
			FILE *fp;
			FILE *fp2;
			RoomInfo *rooms;
			int j;
			int dng;

			fp = fopen("customerData.bin", "w+b");
			if(fp == NULL) {
				printf("\nFile can not open");
				break;
			} else {
				do {
					struct CustomerInfo temp1;
					printf("id :");
					scanf("%d",&temp1.id);
					printf("\nName :");
					scanf("%s", &temp1.customerName);
					printf("\nSurname :");
					scanf("%s", &temp1.customerSurname);
					printf("\nOda Numarasý :");
					scanf("%d", &(temp1.roomNumber));
					fp2 = fopen("RoomInfoData.bin", "w+b");

					if(fp2 == NULL) {
						printf("\nFile can not open!");
						break;
					} else {
						j = 1;
						rooms = (RoomInfo*)calloc(4, sizeof(RoomInfo));
						while(fread(rooms, sizeof(RoomInfo), 1, fp2) == 1) {
							j++;
							if(j % 4 == 0) {
								rooms = (RoomInfo*)realloc(rooms, j * sizeof(RoomInfo));
							}
						}
						fclose(fp2);
					}

					for(int i = 0; rooms[i].id != '\0'; i++) {
						if(rooms[i].roomNumber == temp1.roomNumber && rooms[i].roomState == false) {
							fwrite(&temp1, sizeof(CustomerInfo), 1, fp);
						} else {
							printf("\nÝlgili oda dolu");
						}
					}
					printf("\nCikmak icin 1'e basin :");
					scanf("%d", &dng);
				} while(dng != 1);

				fclose(fp);
			}
			break;
		}
		case 2: {
			// Case 2 için lokal deðiþkenler
			int roomNumber;
			int *size = (int*)malloc(sizeof(int*));
			printf("\nKontrol etmek istediginiz oda numarasini girin :");
			scanf("%d", &roomNumber);
			RoomInfo* rooms = GetRooms(&size);
			int sonuc = CheckRoomDetails(rooms, roomNumber,(*size));
            if(sonuc == 0) printf("\nOda Bos");
			else if(sonuc == -1) printf("\nHatali bilgi!");
			else {
				printf("\nOda Dolu");
			}
			free(rooms);
			break;
		}
		case 3:{
			int id;
			printf("Cýkýs yapilacak musteri no'sunu giriniz :");
			scanf("%d",&id);
			ReservationClear(GetRooms(), FindCustomer(id));
			break;
		}
		default:
			break;
	}
}


CustomerInfo FindCustomer(int id){
	FILE *fp = fopen("customerData.bin","r");
	if(fp == NULL){
		printf("\nFile can not open!");
	}
	CustomerInfo customer;
	while(fread(&customer,sizeof(customer),1,fp) == 1){
		if(customer.id == id) break;
	}
	fclose(fp);
	return customer;
}

RoomInfo* GetRooms(int** size){
	FILE *fp1 = fopen("RoomInfoData.bin","rb");
		RoomInfo *rooms = (RoomInfo*)calloc(4,sizeof(RoomInfo));
		RoomInfo room;
		int i = 0;
		if(fp1 == NULL){
			printf("File can not open!");
		}
		for(i = 0; fread(&room,sizeof(RoomInfo),1,fp1)==1;i++){
			Add(room,&rooms,i);
		}
		
		fclose(fp1);
	*(*size) = i;
	return rooms;
}
RoomInfo* GetRooms(){
	FILE *fp1 = fopen("RoomInfoData.bin","rb");
		RoomInfo *rooms = (RoomInfo*)calloc(4,sizeof(RoomInfo));
		RoomInfo room;
		
		if(fp1 == NULL){
			printf("File can not open!");
		}
		for(int i = 0; fread(&room,sizeof(RoomInfo),1,fp1)==1;i++){
			Add(room,&rooms,i);
		}
		
		fclose(fp1);

	return rooms;
}
int CalculatePrice(RoomInfo *rooms, int roomNumber) {
    
    int i =0;
	for(i = 0; rooms[i].roomNumber != roomNumber && i < MAX_ODA && rooms[i].id != NULL; i++);
	
    if(rooms[i].roomNumber == roomNumber) return rooms[i].price;
    
	printf("Yanlis bir oda numarasi girdiniz.");
    return -1;
}
void MenuCustomer(){
	int secim;
	printf("1-Oda bilgisi sorgulama\n2-Fiyat bilgisi goruntuleme :");
	scanf("%d",&secim);
	switch(secim){
		case 1:{
			int roomNumber;
			printf("Oda No'nuzu giriniz :");
			scanf("%d",&roomNumber);
			RoomInfo* rooms = GetRooms();
			int i = 0;
			for(i = 0;rooms[i].roomNumber != roomNumber;i++);
			printf("Room Number : %d\nRoom Price : %f\nRoom State: %d",rooms[i].roomNumber,rooms[i].price,rooms[i].roomState);
			free(rooms);
			break;
		}
		case 2:{
			int temp;
			printf("\nOda numarasý girin :");
			scanf("%d",&temp);
			RoomInfo* rooms = GetRooms();
			printf("Fiyatý : %d",CalculatePrice(rooms,temp));
			free(rooms);
			break;
		}
		default:
			break;
	}
	
}

int computeDayDiff(DateTime enter,DateTime exit){
    
    int diff = (enter.year - exit.year) * 365 + (enter.month - exit.month) * 30 + (enter.day - exit.day);
    
	return diff;
}

int CheckRoomDetails(RoomInfo *rooms,int roomNumber,int size){

	int i = 0;
	for(i = 0; rooms[i].roomNumber != roomNumber && i < size; i++);
    if(rooms[i].roomNumber == roomNumber) return rooms[i].roomState;

    return -1;
}

void ReservationClear(RoomInfo *rooms,CustomerInfo customer){
    FILE *fp;
    int i = 0;
	for(i = 0; rooms[i].roomNumber != customer.roomNumber;i++);
		if(rooms[i].roomNumber == customer.roomNumber){
			rooms[i].roomState = 0;
			//Exit time kodlanacak.
			fp = fopen("RoomInfoData.bin","w+b");
			if(fp == NULL){
				printf("File can not open!");
			}
			RoomInfo temp;
			fseek(fp,(rooms[i].id - 1) * sizeof(RoomInfo),SEEK_SET);
			fread(&temp,sizeof(RoomInfo),1,fp);
			if(temp.id == rooms[i].id){
				fwrite(&rooms[i],sizeof(RoomInfo),1,fp);
			}
			else{
				printf("HATA KODU : 404");
			}
			fclose(fp);
		}
}
int main(){
	while(true){
		printf("\nMusteri islemleri icin 0'a,personel islemleri icin 1'e bas :");
	    int temp; scanf("%d",&temp);
	    if(temp == 1){
	    	char *tempDizi = (char*)calloc(100,sizeof(char));
		    printf("Lutfen sifreninizi giriniz :");
		    scanf("%s",tempDizi);
		    FILE *fp;
		    fp = fopen("Personels.txt","r");
		    if(fp == NULL){
		    	printf("File can not open!");
		    	return -1;
			}
			struct Personel tempPer;
		    while(fscanf(fp,"%s %s",tempPer.personelName , tempPer.password) == 2){
			    printf("password : %s\n",tempPer.password);
			    if(!strcmp(tempPer.password,tempDizi)){
				   fclose(fp);
				   fp = NULL;
				   free(tempDizi);
				   MenuPersonel();
			    }
			}
			if(fp != NULL){
				fclose(fp);
			    free(tempDizi);
			    printf("\nSifre yanlis");
			}
			continue;
		}
	    else{
		    MenuCustomer();
    	}
	}
	
	return 0;
}
