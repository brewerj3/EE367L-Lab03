/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define PORT "3502"  ///< The port users will be connecting to

//#define DEBUG         ///< Uncomment to print debug information during execution

#define MAXDATASIZE 100 ///< Maximum data size

#define BACKLOG 10     ///< How many pending connections queue will hold

/// Handles sigchild
void sigchld_handler(int s) {
    (void) s; ///< quiet unused variable warning

    /// waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


/// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(void) {
    int sockfd, new_fd, numbytes;  ///< listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; ///< connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    char buff[MAXDATASIZE];

    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; ///< use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    /// Loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); ///< all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {  ///< main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);  ///< Print where the connection is from

        if (!fork()) { ///< this is the child process
            close(sockfd); ///< child doesn't need the listener
            if ((numbytes = recv(new_fd, buff, MAXDATASIZE - 1, 0)) == -1) {
                perror("recv");
                exit(1);
            }
            buff[numbytes] = '\0';                  // Null terminate string

            printf("Server: received %s\n", buff);

            // Create a string to hold the message to send
            char msgToSend[MAXDATASIZE];
            for (int i = 0; i < MAXDATASIZE; i++) {
                msgToSend[i] = 0;
            }

            // Case of ls listing command to the server
            if (strncmp(buff, "L", 1) == 0) {      // This checks if command is 'encoded' as L
                FILE *fp;
                execl(".", "ls", NULL);
                fp = popen("ls", "r");
                while (fgets(buff, sizeof(buff), fp) != NULL) {
                    strncat(msgToSend, buff, sizeof(msgToSend) - strlen(msgToSend) - 1);
                    printf("%s", buff);
                }
#ifdef DEBUG
                printf("closing FILE\n");
#endif
                pclose(fp);
            }
                // Case of check command 'encoded' as C
            else if (strncmp(buff, "C ", 2) == 0) {
#ifdef DEBUG
                printf("Attempt to check if %s exists \n", buff);
#endif
                // Create temp string to hold message
                char tmpMsg[MAXDATASIZE];
                for (int i = 0; i < MAXDATASIZE; i++) {
                    tmpMsg[i] = 0;
                }
                // Check rest of command
                for (int i = 2; i < MAXDATASIZE; i++) {
                    tmpMsg[i - 2] = buff[i];
                }

                // Null terminate string
                tmpMsg[strlen(tmpMsg) - 1] = '\0';
#ifdef DEBUG
                printf("buff = %s\n", buff);
                printf("temp msg = %s\n", tmpMsg);
                printf("length of tmpMsg = %lu \n", strlen(tmpMsg));
#endif
                int tmp = access(tmpMsg, F_OK);
                if ((tmp) == -1) {
                    strcpy(msgToSend, "File not found\0");
                } else {
                    strcpy(msgToSend, "File exists\0");
                }
            }
                // Case of check command with no entry
            else if (strncmp(buff, "C\n", 2) == 0) {
                strcpy(msgToSend, "check command with no argument\0");
            }
                // Case of display command 'encoded' as P
            else if (strncmp(buff, "P ", 2) == 0 || strncmp(buff, "D ", 2) == 0) {

#ifdef DEBUG
                // Print buff when debugging
                printf("Attempt to display %s \n", buff);
#endif

                // Create temp string to hold message
                char tmpMsg[MAXDATASIZE];
                for (int i = 0; i < MAXDATASIZE; i++) {
                    tmpMsg[i] = 0;
                }

                // shift buff into tmpMsg
                for (int i = 2; i < MAXDATASIZE; i++) {
                    tmpMsg[i - 2] = buff[i];
                }

                // Null terminate string
                tmpMsg[strlen(tmpMsg) - 1] = '\0';

#ifdef DEBUG    // Show current strings and their lengths when debugging
                printf("buff = %s\n", buff);
                printf("temp msg = %s\n", tmpMsg);
                printf("length of tmpMsg = %lu \n", strlen(tmpMsg));
#endif
                // Now determine if the file exists
                int tmp = access(tmpMsg, F_OK);
                if ((tmp) == -1) {
                    strcpy(msgToSend, "File not Found\0");
                }

                    // The file exists, so now attempt to send the contents to the user
                else {
#ifdef DEBUG
                    printf("now trying to read File ");
#endif

                    char tmpMsg2[MAXDATASIZE] = "cat \0";

                    strcat(tmpMsg2, tmpMsg);
#ifdef DEBUG
                    printf("tmpMsg2 = %s \n", tmpMsg2);
#endif

                    FILE *fp;
                    fp = popen(tmpMsg2, "r");
                    while (fgets(buff, sizeof(buff), fp) != NULL) {
                        strcpy(msgToSend, buff);

#ifdef DEBUG
                        printf("msgToSend = %s\n", msgToSend);
#endif

                        if (send(new_fd, msgToSend, sizeof msgToSend, 0) == -1) {
                            perror("send");
                        }
                        printf("%s", buff);
                    }

#ifdef DEBUG
                    printf("closing FILE\n");
#endif
                    pclose(fp);
                    close(new_fd);
                    exit(0);
                }

            }
                /// Case of display command with no entry
            else if (strncmp(buff, "P\n", 2) == 0) {
                strcpy(msgToSend, "display command with no argument\0");
            } else if (strncmp(buff, "D\n", 2) == 0) {
                strcpy(msgToSend, "download command with no argument\0");
            }
                /// If command is not recognized tell client
            else {
                strcpy(msgToSend, "command not recognized by server\0");
            }

#ifdef DEBUG
            printf("sending message to client: %s\n", msgToSend);
#endif

            // This sends the message to the client and prints an error if it fails
            if (send(new_fd, msgToSend, sizeof msgToSend, 0) == -1) {
                perror("send");
            }

#ifdef DEBUG
            printf("now closing listener and exiting fork\n");
#endif

            close(new_fd);     ///< close listener
            exit(0);        ///< exit fork
        }
        close(new_fd);  ///< parent doesn't need this
    }

    return 0;
}
