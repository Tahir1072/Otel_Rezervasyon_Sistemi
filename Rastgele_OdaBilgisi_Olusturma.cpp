#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct DateTime {
    int day;
    int month;
    int year;
};

struct RoomInfo {
    unsigned int id;
    unsigned int personOfNumber;
    unsigned int roomNumber;
    bool roomState;
    float price;
    struct DateTime entryDate;
    struct DateTime exitDate;
};

int main() {
    struct RoomInfo rooms[100];

    for (int i = 0; i < 100; i++) {
        struct RoomInfo room;
        room.id = i;
        room.personOfNumber = 1 + rand() % 4;  // 1-4 aras�nda rastgele ki�i say�s�
        room.roomNumber = i + 1;  // Oda numaras�
        room.roomState = rand() % 2;  // Rastgele oda durumu (dolu/bo�)
        room.price = (rand() % 500) + 50;  // Rastgele oda fiyat� 50-550 aras�
        room.entryDate.day = rand() % 30 + 1;  // Rastgele giri� tarihi
        room.entryDate.month = rand() % 12 + 1;
        room.entryDate.year = 2023;
        room.exitDate.day = rand() % 30 + 1;  // Rastgele ��k�� tarihi
        room.exitDate.month = rand() % 12 + 1;
        room.exitDate.year = 2023;
        rooms[i] = room;
    }

    FILE *file = fopen("RoomInfoData.bin", "wb");
    if (file != NULL) {
        fwrite(rooms, sizeof(struct RoomInfo), 100, file);
        fclose(file);
        printf("Dosya ba�ar�yla olu�turuldu!\n");
    } else {
        printf("Dosya olu�turulamad�!\n");
    }

    return 0;
}

