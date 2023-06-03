#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
int CheckRoomDetails(RoomInfo *rooms,int roomNumber);
RoomInfo* GetRooms();
CustomerInfo FindCustomer(int id);
void ReservationClear(RoomInfo *rooms,CustomerInfo customer);
int CalculatePrice(RoomInfo *rooms,int roomNumber);
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

			fp = fopen("customerData.exe", "w+b");
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
					fp2 = fopen("RoomInfoData.exe", "w+b");

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
			FILE *fp1;
			RoomInfo *rooms1;
			int kontrol, k;

			printf("\nKontrol etmek istediginiz oda numarasini girin :");
			scanf("%d", &kontrol);

			fp1 = fopen("RoomInfoData.exe", "rb");
			rooms1 = (RoomInfo*)calloc(4, sizeof(RoomInfo));
			k = 1;

			if(fp1 == NULL) {
				printf("File can not open!");
				break;
			}

			while(fread(rooms1, sizeof(RoomInfo), 1, fp1) == 1) {
				k++;
				if(k % 4 == 0) {
					rooms1 = (RoomInfo*)realloc(rooms1, k * sizeof(RoomInfo));
				}
			}
			fclose(fp1);

			CheckRoomDetails(rooms1, kontrol);
			free(rooms1);
			break;
		}
		case 3:
			//Simülasyon yapýlacak.
			ReservationClear(GetRooms(), FindCustomer(2));
			break;

		default:
			break;
	}
}


CustomerInfo FindCustomer(int id){
	FILE *fp = fopen("customerData.exe","r");
	CustomerInfo customer;
	while(fread(&customer,sizeof(customer),1,fp) == 1){
		if(customer.id == id) break;
	}
	fclose(fp);
	return customer;
}

RoomInfo* GetRooms(){
	FILE *fp1 = fopen("RoomInfoData.exe","rb");
		RoomInfo *rooms = (RoomInfo*)calloc(4,sizeof(RoomInfo));
		int k = 4;
		if(fp1 == NULL){
			printf("File can not open!");
		}
		while(fread(rooms,sizeof(RoomInfo),1,fp1)==1){
			k++;
			if(k % 4 == 0){
				rooms = (RoomInfo*)realloc(rooms,k * sizeof(RoomInfo));
			}
		}
		fclose(fp1);
	return rooms;
}

void MenuCustomer(){
	int secim;
	printf("1-Oda bilgisi sorgulama\n2-Fiyat bilgisi goruntuleme");
	switch(secim){
		case 1:
			//simüle edilecek
			break;
		case 2:
			int temp;
			printf("\nOda numarasý girin :");
			scanf("%d",&temp);
			printf("Fiyatý : %d",CalculatePrice(GetRooms(),temp));
			break;
		default:
			break;
	}
	
}

int computeDayDiff(DateTime enter,DateTime exit){
    
    int diff = (enter.year - exit.year) * 365 + (enter.month - exit.month) * 30 + (enter.day - exit.day);
    
	return diff;
}

int CalculatePrice(RoomInfo *rooms, int roomNumber) {
    // NULL kontrolü yerine 0 kontrolü yapacaðýz.
    for(int i = 0; rooms[i].id != 0; i++) {
        if(rooms[i].roomNumber == roomNumber) {
            return rooms[i].price;
        }
    }
    printf("Yanlis bir oda numarasi girdiniz.");
    return -1;
}


int CheckRoomDetails(RoomInfo *rooms,int roomNumber){
	
	for(int i = 0; rooms[i].id != NULL; i++){
		if(rooms[i].roomNumber == roomNumber){
			return rooms[i].roomState;
		}
	}
	printf("\nHatalý oda numarasý");
	return -1;
}

void ReservationClear(RoomInfo *rooms,CustomerInfo customer){
    FILE *fp;
    int i = 0;
	for(i = 0; rooms[i].id != NULL;i++){
		if(rooms[i].roomNumber == customer.roomNumber){
			rooms[i].roomState = 0;
			//Exit time kodlanacak.
			fp = fopen("RoomInfoData.exe","w+b");
			if(fp == NULL){
				printf("File can not open!");
			}
			RoomInfo temp;
			fseek(fp,(rooms[i].id -1) * sizeof(RoomInfo),SEEK_SET);
			fread(&temp,sizeof(RoomInfo),1,fp);
			if(temp.id == rooms[i].id){
				fwrite(&rooms[i],sizeof(RoomInfo),1,fp);
			}
			fclose(fp);
            break;
		}
	}
}
int main(){
	while(true){
		printf("Musteri islemleri icin 0'a,personel islemleri icin 1'e bas :");
	    int temp; scanf("%d",&temp);
	    if(temp == 1){
	    	char *tempDizi = (char*)calloc(100,sizeof(char));
		    printf("Lutfen sifreninizi giriniz :");
		    scanf("%s",tempDizi);
		    FILE *fp;
		    fp = fopen("PersonelInfo.txt","r");
		    if(fp == NULL){
		    	printf("File can not open!");
		    	return -1;
			}
			struct Personel tempPer;
		    while(fscanf(fp,"%s %s",tempPer.personelName , tempPer.password) == 2){
			    printf("password : %s\n",tempPer.password);
			    if(!strcmp(tempPer.password,tempDizi)){
				   fclose(fp);
				   free(tempDizi);
				   MenuPersonel();
			    }
			}
			if(fp != NULL){
				fclose(fp);
			    free(tempDizi);
			    printf("Sifre yanlis");
			}
			continue;
		}
	    else{
		    MenuCustomer();
    	}
	}
	
	return 0;
}
