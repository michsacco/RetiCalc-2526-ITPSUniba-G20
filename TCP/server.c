/*
    MICHELE SACCO
    MATRICOLA 825051
    PROGETTO RETI DI CALCOLATORI - 2° ANNO 1 SEM
    SERVER TCP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main() {
    // DICHIARAZIONE VARIABILI
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // CREAZIONE SOCKET
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    // INIZIALIZZO PROCESSO SERVER
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(4500);
    // BIND PORTA + CHECK ERRORI EVENTUALI BIND
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERRORE BIND!"); exit(1);
    }

    // ASCOLTO MESSAGGI IN ARRIVO SUL SOCKET
    listen(sockfd, 5);

    while (1) {
        // ACCETTAZIONE NUOVE CONNESSIONI, INVIO MESSAGGIO DI CONFERMA
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (newsockfd < 0) { 
            perror("accept"); continue; 
        }
        char buffer[256];
        write(newsockfd, "connessione avvenuta", 22);

        // LEGGO OPERAZIONE DA SVOLGERE
        memset(buffer, 0, 256);
        read(newsockfd, buffer, 255);
        
        // CHECK OPERAZIONE IN CORSO DI SVOLGIMENTO
        char op = buffer[0];
        char msg[32];
        if (op == 'A' || op == 'a') strcpy(msg, "ADDIZIONE");
        else if (op == 'S' || op == 's') strcpy(msg, "SOTTRAZIONE");
        else if (op == 'M' || op == 'm') strcpy(msg, "MOLTIPLICAZIONE");
        else if (op == 'D' || op == 'd') strcpy(msg, "DIVISIONE");
        else strcpy(msg, "TERMINE PROCESSO CLIENT");
        // INVIO MESSAGGIO (tipo operazione) VIA SOCKET
        write(newsockfd, msg, strlen(msg));

        if (strcmp(msg, "TERMINE PROCESSO CLIENT") != 0) {
            // SVOLGIMENTO OPERAZIONE
            int nums[2];
            read(newsockfd, nums, sizeof(nums));
            int a = nums[0], b = nums[1], result = 0;
            if (op == 'A' || op == 'a') result = a + b;
            if (op == 'S' || op == 's') result = a - b;
            if (op == 'M' || op == 'm') result = a * b;
            if (op == 'D' || op == 'd') result = (b != 0) ? a / b : 0;
            write(newsockfd, &result, sizeof(result));
        }

        close(newsockfd); // CHIUDO SOCKET
    }

    close(sockfd); // CHIUDO SOCKET
    return 0;
}