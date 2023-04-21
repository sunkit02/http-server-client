#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "../lib/game.h"
#include "../lib/base64.h"

#define DEFAULT_PORT 9001
#define LOCALHOST "127.0.0.1"


void getHomePage(HttpClient *client) {

    HttpRequest *request = constructHttpRequest(GET, "/", NULL, NULL);

    HttpResponse *response = sendHttpRequest(client, request);

    if (response == NULL) {
        puts("Failed to send request");
        return;
    }

    printf("%s\n", response->body);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void resetHomePageCount(HttpClient *client) {

    HttpRequest *request = constructHttpRequest(GET, "/reset", NULL, NULL);

    HttpResponse *response = sendHttpRequest(client, request);

    if (response == NULL) {
        puts("Failed to send request");
        return;
    }

    printf("%s\n", response->body);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void sendPlayer(HttpClient *client) {

    int id = 0;
    int hit = 0;

    printf("\nEnter id and hit: ");
    scanf("%d %d", &id, &hit);
    getchar(); // clear '\n' from stdin

    Player player = { id, hit };

    char buffer[1024];
    sprintf(buffer, "Player(id=%d, hit=%s)", 
            player.id, player.hit == true ? "true" : "false");
    printf("\nYou Entered:\n%s\n", buffer);

    size_t objSize = sizeof(Player);

    char *encodedBody =
        base64_encode((unsigned char *)&player, objSize, &objSize);

    HttpRequest *request =
        constructHttpRequest(POST, "/player", NULL, encodedBody);

    HttpResponse *response = sendHttpRequest(client, request);

    if (response == NULL) {
        puts("Failed to send request");
        return;
    }

    objSize = strlen(response->body);

    Player *decodedPlayer = base64_decode(response->body, objSize, &objSize);

    sprintf(buffer, "Player(id=%d, hit=%s)", 
            decodedPlayer->id,
            decodedPlayer->hit == true ? "true" : "false");
    printf("\nServer Response:\n%s\n", buffer);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void addPlayer(HttpClient *client) {

    int id = 0;
    int hit = 0;

    printf("\nEnter id and hit: ");
    scanf("%d %d", &id, &hit);
    getchar(); // clear '\n' from stdin

    Player player = { id, hit };

    char buffer[1024];
    sprintf(buffer, "Player(id=%d, hit=%s)", 
            player.id, player.hit == true ? "true" : "false");
    printf("\nYou Entered:\n%s\n", buffer);

    size_t objSize = sizeof(Player);

    char *encodedBody =
        base64_encode((unsigned char *)&player, objSize, &objSize);

    HttpRequest *request =
        constructHttpRequest(POST, "/players/add", NULL, encodedBody);

    HttpResponse *response = sendHttpRequest(client, request);

    if (response == NULL) {
        puts("Failed to send request");
        return;
    }

    printf("\nServer Response: %s\n", response->body);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void getPlayers(HttpClient *client) {
    HttpRequest *request = constructHttpRequest(GET, "/players/get", NULL, NULL);

    HttpResponse *response = sendHttpRequest(client, request);

    if (response == NULL) {
        puts("Failed to send request");
        return;
    }

    printf("\nServer Response:\n%s\n", response->body);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}


// Assigns server ip address and port number based on command line arguments.
// @param argc - number of commandline arguments.
// @param argv - array of commandline arguments.
// @param serverIpAddress - pointer to string containing the server ip address.
// @param port - pointer to int containing target port number.
void assignServerIp(int argc, char *argv[], char **serverIpAddress, int *port) {

    // Use localhost:9001 as default server ip address
    *serverIpAddress = LOCALHOST;
    *port = DEFAULT_PORT;

    if (argc >= 2) *serverIpAddress = argv[1];
    if (argc >= 3) *port = atoi(argv[2]);

    if (port <= 0) {
        perror("Port number must be greater than 0.");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[]) {
    char *serverIpAddress;
    int port;

    assignServerIp(argc, argv, &serverIpAddress, &port);

    HttpClient *client = constructHttpClient(serverIpAddress, port);

    if (client == NULL) {
        puts("Failed to construct client");
        return 1;
    }

    bool run = true;
    while (run) {

        puts("\n\n");
        puts("Choose an action:\n"
             "1. GET home page\n"
             "2. Reset home page count\n"
             "3. Send player\n"
             "4. Add player\n"
             "5. Get players\n"
             "6. Quit\n");

        printf("Enter your choice [1-6]: ");
        size_t input = 0;
        scanf("%zu", &input);
        getchar();
        puts("");

        switch (input) {
            case 1:
                getHomePage(client);
                break;
            case 2:
                resetHomePageCount(client);
                break;
            case 3:
                sendPlayer(client);
                break;
            case 4:
                addPlayer(client);
                break;
            case 5:
                getPlayers(client);
                break;
            case 6:
                puts("Quitting...");
                run = false;
                break;
            default:
                printf("Input: %zu is not a valid choice. Try again.", input);
                continue;
        }

        printf("\nPress enter to continue...");
        getchar();
    }
}

