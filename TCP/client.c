/*
    MICHELE SACCO
    MATRICOLA 825051
    PROGETTO RETI DI CALCOLATORI - 2° ANNO 1 SEM
    CLIENT TCP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


int main() {
    // INSERIMENTO NOME HOST
    char hostname[256];
    printf("Inserisci nome server: ");
    scanf("%s", hostname);

    // CHECK VALIDITA' NOME HOST
    struct hostent *he = gethostbyname(hostname);
    if (!he) { 
        perror("ERRORE HOST!"); exit(1); 
    }

    // CREAZIONE SOCKET
    int socktcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socktcp < 0) { 
        perror("ERRORE SOCKET!"); exit(1); 
    }

    // INSTAURO LA CONNESSIONE (+ CHECK)
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4500);
    memcpy(&serv_addr.sin_addr, he->h_addr, he->h_length);
    if (connect(socktcp, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERRORE CONNESSIONE!"); exit(1);
    }

    // STAMPO MESSAGGIO RICEVUTO DAL SERVER
    char buffer[256];
    memset(buffer, 0, 256);
    read(socktcp, buffer, 255);
    printf("%s\n", buffer);

    // RICHIESTA LETTERA OPERAZIONE
    printf("Inserisci lettera operazione: ");
    char op;
    scanf(" %c", &op);
    write(socktcp, &op, 1);

    // STAMPO MESSAGGIO
    memset(buffer, 0, 256);
    read(socktcp, buffer, 255);
    printf("%s\n", buffer);
    if (strcmp(buffer, "TERMINE PROCESSO CLIENT") == 0) {
        close(socktcp); // CHIUSURA SOCKET
        return 0;
    }

    // RICHIESTA NUMERI INTERI
    int nums[2];
    printf("Inserisci due interi: ");
    scanf("%d %d", &nums[0], &nums[1]);
    write(socktcp, nums, sizeof(nums));

    // STAMPA RISULTATO OPERAZIONE
    int result;
    read(socktcp, &result, sizeof(result));
    printf("Risultato: %d\n", result);

    // CHIUSURA SOCKET
    close(socktcp);
    return 0;
}