#include <stdio.h>
#define MAX_COUNT 5

typedef struct People {
    char name[14];
    unsigned short int age;
    float height;
    float weight;
} Person;

int AddFriend(Person *p_friend, int count)
{
    if(count < MAX_COUNT) {
        p_friend = p_friend + count;
        printf("\n새로운 친구 정보를 입력하세요\n");
        printf("1. 이름: ");
        scanf("%s", p_friend->name);
        printf("2. age: ");
        scanf("%hu", &p_friend-> age);
        printf("3. height: ");
        scanf("%f", &p_friend->weight);
        printf("input done. \n\n");
        return 1;
    } else {
        printf("최대인원 초과하여 입력할 수 없습니다. \n");

    } return 0;
}

void ShowFriendList(Person *p_friend, int count) {
    int i;
    if(count > 0) {
        printf("\n Friend List");
        printf("====================\n");
        
        for(i = 0; i < count; i++){
            printf("%-14s, %3d, %6.2f, %6.2f\n", p_friend->name, p_friend->age, p_friend->height, p_friend->weight);
            p_friend++;
        }

        printf("=====================\n\n");
    } else {
        printf("\n no friend \n\n");
    }
}

void main() {
    Person friends[MAX_COUNT];
    int count = 0, num;

    while(1) {
        printf(" [Menu] \n");
        
    }
}