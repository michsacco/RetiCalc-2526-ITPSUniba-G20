/*
    MICHELE SACCO
    MATRICOLA 825051
    PROGETTO RETI DI CALCOLATORI - 2° ANNO 1 SEM
    CLIENT UDP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    // LETTURA NOME SERVER
    char hostname[256];
    printf("Inserisci nome server: ");
    scanf("%s", hostname);

    // RISOLUZIONE NOME SERVER
    struct hostent *he = gethostbyname(hostname);
    if (!he) { perror("ERRORE HOST! "); exit(1); }

    // CREAZIONE SOCKET UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);
    // INSTAURO CONNESSIONE CON SERVER
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4500);
    memcpy(&serv_addr.sin_addr, he->h_addr, he->h_length);

    // PRIMO PACCHETTO PER AVVIARE LA COMUNICAZIONE
    sendto(sockfd, "START", 5, 0, (struct sockaddr *)&serv_addr, serv_len);

    char buffer[256];
    memset(buffer, 0, 256);

    // RICEZIONE E STAMPA PRIMA RISPOSTA
    recvfrom(sockfd, buffer, 255, 0,
             (struct sockaddr *)&serv_addr, &serv_len);
    printf("%s\n", buffer);

    // LETTURA LETTERA OPERAZIONE
    printf("Inserisci lettera operazione: ");
    char op;
    scanf(" %c", &op);

    // INVIO OPERAZIONE
    sendto(sockfd, &op, 1, 0, (struct sockaddr *)&serv_addr, serv_len);

    memset(buffer, 0, 256);

    // RICEZIONE TIPO OPERAZIONE
    recvfrom(sockfd, buffer, 255, 0,
             (struct sockaddr *)&serv_addr, &serv_len);
    printf("%s\n", buffer);
    if (strcmp(buffer, "TERMINE PROCESSO CLIENT") == 0) {
        close(sockfd);
        return 0;
    }

    // INVIO 2 INTERI
    int nums[2];
    printf("Inserisci due interi: ");
    scanf("%d %d", &nums[0], &nums[1]);
    sendto(sockfd, nums, sizeof(nums), 0, (struct sockaddr *)&serv_addr, serv_len);

    // RICEZIONE E STAMPA RISULTATO
    int result;
    recvfrom(sockfd, &result, sizeof(result), 0,
             (struct sockaddr *)&serv_addr, &serv_len);
    printf("Risultato: %d\n", result);

    close(sockfd);
    return 0;
}
