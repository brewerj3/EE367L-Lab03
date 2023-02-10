/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define PORT "3502" // the port client will be connecting to

#define DEBUG

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd, numbytes, firstTime = 1;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc == 1) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }
    if (argc > 2) {
        fprintf(stderr, "usage: too many arguments\n");
        exit(1);
    }

    while (1) {
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }

        // loop through all the results and connect to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("client: socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                perror("client: connect");
                close(sockfd);
                continue;
            }
            break;
        }

        if (p == NULL) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;
        }


        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s, sizeof s);
        if(firstTime == 1) {
            printf("client: connecting to %s\n", s);
            firstTime = 0;
        }

        freeaddrinfo(servinfo); // all done with this structure

        char *message;
        size_t len = 0;
        // Enter While loop of asking for user input and sending it to the server

        // Empty message of any data
        memset(&message, 0, sizeof(message));

        // Client prompts user for a command
        printf("Command(enter 'h' for help) :");
        if (getline(&message, &len, stdin) == -1) {
            fprintf(stderr, "usage: invalid\n");
            exit(1);
        }

#ifdef DEBUG
        //This is debug use
        printf("message is: %s", message);
#endif

        // if else decision tree for processing user input
        // Check for quit command
        if (strcmp(message, "quit\n") == 0) {
            // Quit the client by ending the loop
            printf("Quiting client\n");
            break;
        }
            // Check for ls command
        else if (strcmp(message, "ls\n") == 0) {
#ifdef DEBUG
            printf("sending ls command\n");
#endif
            // 'encode' ls to L
            strcpy(message, "L\n");
        }
            // Check for check command
        else if (strncmp(message, "check\n", 4) == 0) {
            // Change message from starting with check to C
            for (int j = 0; j < 4; j++) {
                for (int i = 1; i < MAXDATASIZE; i++) {
                    message[i - 1] = message[i];
                }
            }
            message[0] = 'C';

            //strcpy(message, message);
        }
            // check for help command then print helpful list of commands then restart loop
        else if (strcmp(message, "h\n") == 0) {
            printf("quit    - quit this client program and exit to the console.\n");
            printf("check   - check if file exists in current directory.\n");
            printf("ls      - print the contents of the current directory to the current console\n");
            printf("display - this attempts to display the contents of a file\n");
            printf("h       - prints this help page\n");
            continue;
        }
            // Print this if command is not recognized then restart loop
        else {
            printf("Command not recognized\n");
            close(sockfd);
            continue;
        }

#ifdef DEBUG
        // Print the message being sent to server for debugging purposes
        printf(" message to send %s\n", message);
#endif

        pid_t child_pd, parent_pid;
        int status = 0;
        // Child process sends message to server
        if((child_pd = fork()) == 0) {
            // Send message to server
            if (message != NULL) {
                send(sockfd, message, strlen(message), 0);
            }
            memset(&message, 0, sizeof(message));

            // Receive message from server
            if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
                perror("recv");
                exit(1);
            }

            buf[numbytes] = '\0';   // This terminates the string
            printf("client: received '%s'\n", buf);
            close(sockfd);
            exit(0);
        }
        // Parent waits for child process to finish
        while((parent_pid = wait(&status)) > 0);
        close(sockfd);
    }
    close(sockfd);
    return 0;
}

