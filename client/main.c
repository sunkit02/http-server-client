#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "../lib/base64.h"
#include "../lib/blackJack.h"

#define DEFAULT_PORT 9001
#define LOCALHOST "127.0.0.1"


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

void sendPlayer(Player player, int choice){
    

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

        printf("\nPress enter to continue...");
        getchar();
    }
}

