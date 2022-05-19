/**
 * @file Test.cpp
 * @author Ilan Gold and Haim Or Hadad
 * @brief tests 
 * @version 0.1
 * @date 2022-05-09
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <iostream>

using namespace std;

#define PORT 6060
char buf[1024];
char rbuf[1024];

void send_msg(int socket, string txt){
    memset(buf,0,sizeof(buf));
    strcpy(buf,txt.c_str());
    send(socket, buf , sizeof(buf), 0);
}
string check_the_response(int socket, char expected[1024]){
    memset(rbuf,0,sizeof(rbuf));
    recv(socket, rbuf, sizeof(rbuf), 0);
    assert(!strcmp(expected,rbuf) == 0);
}

int main()
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Socket failed\n");
        exit(1);
    }
    
    if (connect(sock,(struct sockaddr*)&server_address, sizeof(server_address))!= 0)
    {
        perror("Connect failed");
        exit(1);
    }
    cout << "We are connected! starting the tests..";
    
// ================================
    // Basic stack operation check
// ================================

    send_msg(sock, "PUSH Hey");
    check_the_response(sock, "Hey");
    send_msg(sock, "TOP");
    check_the_response(sock, "Last string in stack is: Hey");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: Hey");

// ===================
    // Special cases:
// ===================

    // Multiple pushes
    send_msg(sock, "PUSH do you want to hear a joke?");
    check_the_response(sock, "Data pushed: do you want to hear a joke?");
    send_msg(sock, "PUSH I will tell you any way");
    check_the_response(sock, "Data pushed: I will tell you any way");
    send_msg(sock, "PUSH why do java prograammers have to wear glasses?");
    check_the_response(sock, "Data pushed: why do java prograammers have to wear glasses?");
    send_msg(sock, "PUSH Because they can't C#");
    check_the_response(sock, "Data pushed: Because they can't C#");
    cout << '\n\n';

    // Multiple pops
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: do you want to hea a joke?");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: I will tell you any way");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: why do java prograammers have to wear glasses?");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: Because they can't C#");
    cout << '\n\n';

    // POP/TOP from empty stack
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: Non stack is empty");
    send_msg(sock, "TOP");
    check_the_response(sock, "Last string in stack is: Non stack is empty");
    cout << '\n\n';

    // Invalid commends opper/lower case
    send_msg(sock, "BLABLA");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "JHJHJ");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "blabla");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "jhjhj");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "push aaa");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "push AAA");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "top");
    check_the_response(sock, "This commend is not suported! please read the instuction..");
    send_msg(sock, "pop");
    check_the_response(sock, "This commend is not suported! please read the instuction..");

}
