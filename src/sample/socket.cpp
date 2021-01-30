/*
    C socket server example, handles multiple clients using threads
    Compile
    gcc server.c -lpthread -o server
*/

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread
#include <glm/glm.hpp>

union FloatData
{
    float f;
    char b[4];
};

glm::vec3 *space_angle_pointer;

//the thread function
void *connection_handler(void *);

int create_server(glm::vec3 *angle)
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    space_angle_pointer = angle;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1973);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection accepted");

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    FloatData ax, ay, az;
    // FloatData ax, ay, az;
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char *message;
    char client_message[2000];

    //Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        //end of string marker
        // client_message[read_size] = '\0';

        ax.b[0] = client_message[0];
        ax.b[1] = client_message[1];
        ax.b[2] = client_message[2];
        ax.b[3] = client_message[3];

        ay.b[0] = client_message[4];
        ay.b[1] = client_message[5];
        ay.b[2] = client_message[6];
        ay.b[3] = client_message[7];

        az.b[0] = client_message[8];
        az.b[1] = client_message[9];
        az.b[2] = client_message[10];
        az.b[3] = client_message[11];

        (*space_angle_pointer).x = ax.f; // -
        (*space_angle_pointer).y = ay.f; // -
        (*space_angle_pointer).z = az.f; //

        //Send the message back to client
        // printf("%.2f\t%.2f\t%.2f\n", (*space_angle_pointer).x, (*space_angle_pointer).y, (*space_angle_pointer).z);
        // write(sock, client_message, strlen(client_message));

        //clear the message buffer
        // memset(client_message, 0, 2000);
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}