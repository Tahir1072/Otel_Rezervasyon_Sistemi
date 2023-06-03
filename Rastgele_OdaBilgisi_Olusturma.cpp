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
        room.personOfNumber = 1 + rand() % 4;  // 1-4 arasýnda rastgele kiþi sayýsý
        room.roomNumber = i + 1;  // Oda numarasý
        room.roomState = rand() % 2;  // Rastgele oda durumu (dolu/boþ)
        room.price = (rand() % 500) + 50;  // Rastgele oda fiyatý 50-550 arasý
        room.entryDate.day = rand() % 30 + 1;  // Rastgele giriþ tarihi
        room.entryDate.month = rand() % 12 + 1;
        room.entryDate.year = 2023;
        room.exitDate.day = rand() % 30 + 1;  // Rastgele çýkýþ tarihi
        room.exitDate.month = rand() % 12 + 1;
        room.exitDate.year = 2023;
        rooms[i] = room;
    }

    FILE *file = fopen("RoomInfoData.bin", "wb");
    if (file != NULL) {
        fwrite(rooms, sizeof(struct RoomInfo), 100, file);
        fclose(file);
        printf("Dosya baþarýyla oluþturuldu!\n");
    } else {
        printf("Dosya oluþturulamadý!\n");
    }

    return 0;
}

