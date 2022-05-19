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
#include <unistd.h>

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
    int rv = recv(socket, rbuf, sizeof(rbuf), 0);
    if (rv == -1)
    {
        perror("Sec recv prob!");
    }
    cout << "expected: " << expected;
    cout << "\nGot: " << rbuf << '\n';
    fflush(stdout);
    assert(!(strcmp(expected,rbuf) == 0));
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
    memset(rbuf,0,sizeof(rbuf));
    int rv = recv(sock, rbuf, sizeof(rbuf), 0);
    if (rv == -1)
    {
        perror("First recv prob!");
    }
    
    cout << "Begin: " << rbuf << '\n';
    cout << "We are connected! starting the tests..\n";
    
// ================================
    // Basic stack operation check
// ================================

    send_msg(sock, "PUSH Hey");
    check_the_response(sock, "Data pushed: Hey\n");
    send_msg(sock, "TOP");
    check_the_response(sock, "Last string in stack is: Hey\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: Hey\n");

// ===================
    // Special cases:
// ===================

    // Multiple pushes
    // send_msg(sock, "PUSH do you want to hear a joke?");
    // check_the_response(sock, "Data pushed: do you want to hear a joke?\n");
    send_msg(sock, "PUSH I");
    check_the_response(sock, "Data pushed: I\n");
    send_msg(sock, "PUSH am");
    check_the_response(sock, "Data pushed: am\n");
    send_msg(sock, "PUSH working on");
    check_the_response(sock, "Data pushed: working on\n");
    send_msg(sock, "PUSH this");
    check_the_response(sock, "Data pushed: this\n");
    send_msg(sock, "PUSH assignment");
    check_the_response(sock, "Data pushed: assignment\n");
    send_msg(sock, "PUSH far");
    check_the_response(sock, "Data pushed: far\n");
    send_msg(sock, "PUSH too long");
    check_the_response(sock, "Data pushed: too long\n");
    // send_msg(sock, "PUSH i will tell you any way");
    // check_the_response(sock, "Data pushed: i will tell you any way\n");
    // send_msg(sock, "PUSH why do java prograammers have to wear glasses?");
    // check_the_response(sock, "Data pushed: why do java prograammers have to wear glasses?\n");
    // send_msg(sock, "PUSH Because they can't C#");
    // check_the_response(sock, "Data pushed: Because they can't C#\n");
    cout << "\n\n";

    // Multiple pops
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: I\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: am\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: working on\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: this\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: assignment\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: far\n");
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: too long\n");
    cout << "\n\n";

    // POP/TOP from empty stack
    send_msg(sock, "POP");
    check_the_response(sock, "Element poped: Non stack is empty\n");
    send_msg(sock, "TOP");
    check_the_response(sock, "Last string in stack is: Non stack is empty\n");
    cout << "\n\n";

    // Invalid commends opper/lower case
    send_msg(sock, "BLABLA");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "JHJHJ");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "blabla");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "jhjhj");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "push aaa");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "push AAA");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "top");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");
    send_msg(sock, "pop");
    check_the_response(sock, "This commend is not suported! please read the instuction..\n");

    cout << "\nThe test was a success!!!\n"; 

}
