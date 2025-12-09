/*
    MICHELE SACCO
    MATRICOLA 825051
    PROGETTO RETI DI CALCOLATORI - 2° ANNO 1 SEM
    SERVER UDP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // DICHIARAZIONE VARIABILI
    int sockudp;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // CREAZIONE SOCKET UDP
    sockudp = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockudp < 0) { perror("socket"); exit(1); }

    // INIZIALIZZAZIONE PROCESSO SERVER
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(4500);

    // BIND DELLA SOCKET ALLA PORTA
    if (bind(sockudp, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); exit(1);
    }

    printf("STATUS: Server in ascolto sulla porta 4500...\n");

    while (1) {
        char buffer[256];
        memset(buffer, 0, 256);

        // RICEZIONE PACCHETTO DAL CLIENT
        recvfrom(sockudp, buffer, 255, 0, (struct sockaddr *)&cli_addr, &cli_len);

        // INVIO RISPOSTA DI INIZIO COMUNICAZIONE
        sendto(sockudp, "connessione avvenuta", 21, 0, (struct sockaddr *)&cli_addr, cli_len);

        // RICEZIONE LETTERA OPERAZIONE
        recvfrom(sockudp, buffer, 255, 0, (struct sockaddr *)&cli_addr, &cli_len);

        char op = buffer[0];
        char msg[32];

        // SELEZIONE OPERAZIONE
        if (op == 'A' || op == 'a') strcpy(msg, "ADDIZIONE");
        else if (op == 'S' || op == 's') strcpy(msg, "SOTTRAZIONE");
        else if (op == 'M' || op == 'm') strcpy(msg, "MOLTIPLICAZIONE");
        else if (op == 'D' || op == 'd') strcpy(msg, "DIVISIONE");
        else strcpy(msg, "TERMINE PROCESSO CLIENT");

        // INVIO RISPOSTA
        sendto(sockudp, msg, strlen(msg), 0, (struct sockaddr *)&cli_addr, cli_len);

        // RICEZIONE 2 INTERI
        if (strcmp(msg, "TERMINE PROCESSO CLIENT") != 0) {
            int nums[2];
            recvfrom(sockudp, nums, sizeof(nums), 0, (struct sockaddr *)&cli_addr, &cli_len);

            int a = nums[0], b = nums[1], result = 0;

            // ESECUZIONE OPERAZIONE
            if (op == 'A' || op == 'a') result = a + b;
            if (op == 'S' || op == 's') result = a - b;
            if (op == 'M' || op == 'm') result = a * b;
            if (op == 'D' || op == 'd') result = (b != 0) ? a / b : 0;

            // INVIO RISULTATO AL CLIENT
            sendto(sockudp, &result, sizeof(result), 0, (struct sockaddr *)&cli_addr, cli_len);
        }
    }

    close(sockudp); // CHIUDO SOCKET
    return 0;
}
