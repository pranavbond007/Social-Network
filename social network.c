#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50

typedef struct User {
    char name[NAME_LENGTH];
    struct User *next;
    struct Friend *friends;
} User;

typedef struct Friend {
    User *user;
    struct Friend *next;
} Friend;

User *users = NULL;

void addUser(const char *name) {
    User *temp = users;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("User '%s' already exists.\n", name);
            return;
        }
        temp = temp->next;
    }

    User *newUser = (User *)malloc(sizeof(User));
    strcpy(newUser->name, name);
    newUser->next = users;
    newUser->friends = NULL;
    users = newUser;

    printf("User '%s' added successfully.\n", name);
}

User *findUser(const char *name) {
    User *temp = users;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void addConnection(const char *name1, const char *name2) {
    User *user1 = findUser(name1);
    User *user2 = findUser(name2);

    if (user1 == NULL || user2 == NULL) {
        printf("One or both users do not exist.\n");
        return;
    }

    Friend *newFriend1 = (Friend *)malloc(sizeof(Friend));
    newFriend1->user = user2;
    newFriend1->next = user1->friends;
    user1->friends = newFriend1;

    Friend *newFriend2 = (Friend *)malloc(sizeof(Friend));
    newFriend2->user = user1;
    newFriend2->next = user2->friends;
    user2->friends = newFriend2;

    printf("Connection added between '%s' and '%s'.\n", name1, name2);
}

void displayNetwork() {
    if (users == NULL) {
        printf("No users in the network.\n");
        return;
    }

    User *temp = users;
    while (temp != NULL) {
        printf("%s's connections: ", temp->name);

        Friend *friendTemp = temp->friends;
        if (friendTemp == NULL) {
            printf("No connections.");
        } else {
            while (friendTemp != NULL) {
                printf("%s ", friendTemp->user->name);
                friendTemp = friendTemp->next;
            }
        }

        printf("\n");
        temp = temp->next;
    }
}

void deleteUser(const char *name) {
    User *temp = users;
    User *prev = NULL;

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("User '%s' does not exist.\n");
        return;
    }

    User *curr = users;
    while (curr != NULL) {
        Friend *friendTemp = curr->friends;
        Friend *friendPrev = NULL;

        while (friendTemp != NULL) {
            if (friendTemp->user == temp) {
                if (friendPrev == NULL) {
                    curr->friends = friendTemp->next;
                } else {
                    friendPrev->next = friendTemp->next;
                }
                free(friendTemp);
                break;
            }
            friendPrev = friendTemp;
            friendTemp = friendTemp->next;
        }

        curr = curr->next;
    }

    if (prev == NULL) {
        users = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("User '%s' deleted successfully.\n", name);
}

int main() {
    int choice;
    char name1[NAME_LENGTH], name2[NAME_LENGTH];

    printf("Welcome to the Social Network System!\n");

    do {
        printf("\nMenu:\n");
        printf("1. Add User\n");
        printf("2. Add Connection\n");
        printf("3. Display Network\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter the name of the user to add: ");
            scanf("%s", name1);
            addUser(name1);
            break;

        case 2:
            printf("Enter the names of the two users to connect: ");
            scanf("%s %s", name1, name2);
            addConnection(name1, name2);
            break;

        case 3:
            displayNetwork();
            break;

        case 4:
            printf("Enter the name of the user to delete: ");
            scanf("%s", name1);
            deleteUser(name1);
            break;

        case 5:
            printf("Exiting the program. Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
