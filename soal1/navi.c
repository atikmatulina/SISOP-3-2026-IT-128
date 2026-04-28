#include "protocol.h"

int sock = 0;
int is_admin = 0;
int is_running = 1;

// Thread untuk mendengarkan transmisi dari server (The Wired) secara asinkron
void *listen_thread(void *arg) {
    char buffer[BUFFER_SIZE];
    while (is_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        
        if (valread <= 0) {
            if (is_running) {
                printf("\r\033[K[System] Connection lost from The Wired.\n");
            }
            is_running = 0;
            exit(0);
        }
        buffer[valread] = '\0';

        // \r\033[K menghapus baris prompt "> " agar pesan tidak berantakan
        if (strncmp(buffer, RESP_CHAT, 5) == 0) {
            printf("\r\033[K%s\n> ", buffer + 5); 
        } 
        else if (strncmp(buffer, RESP_MENU, 5) == 0) {
            printf("\r\033[K\n%s", buffer + 5);
        } 
        else if (strncmp(buffer, RESP_SYS, 4) == 0) {
            if (is_admin) printf("\r\033[K\n%s", buffer + 4);
            else printf("\r\033[K%s\n> ", buffer + 4);
        }
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    char input[MAX_INPUT];
    char send_buf[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed. Is The Wired online?\n");
        return -1;
    }

    // --- FASE OTENTIKASI ---
    while (1) {
        printf("Enter your name: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        snprintf(send_buf, sizeof(send_buf), PREFIX_NAME "%s", input);
        send(sock, send_buf, strlen(send_buf), 0);

        memset(send_buf, 0, BUFFER_SIZE);
        int valread = read(sock, send_buf, BUFFER_SIZE - 1);
        send_buf[valread] = '\0';

        if (strncmp(send_buf, RESP_ERR, 4) == 0) {
            printf("%s\n", send_buf + 4);
        } else if (strncmp(send_buf, RESP_PWD, 4) == 0) {
            printf("Enter Password: ");
            if (!fgets(input, sizeof(input), stdin)) break;
            input[strcspn(input, "\n")] = 0;

            snprintf(send_buf, sizeof(send_buf), PREFIX_PASS "%s", input);
            send(sock, send_buf, strlen(send_buf), 0);

            memset(send_buf, 0, BUFFER_SIZE);
            valread = read(sock, send_buf, BUFFER_SIZE - 1);
            send_buf[valread] = '\0';

            if (strncmp(send_buf, RESP_AUTH, 8) == 0) {
                printf("%s\n", send_buf + 8);
                is_admin = 1;
                break;
            } else {
                printf("%s\n", send_buf + 4);
                close(sock);
                return 0;
            }
        } else if (strncmp(send_buf, RESP_SYS, 4) == 0) {
            printf("%s\n", send_buf + 4);
            break;
        }
    }

    // Menjalankan thread pendengar (ASINKRON - Tanpa fork)
    pthread_t tid;
    pthread_create(&tid, NULL, listen_thread, NULL);

    // --- FASE INTERAKSI UTAMA ---
    while (is_running) {
        if (!is_admin) {
            printf("> ");
            fflush(stdout);
        }
        
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        // Penanganan Exit
        if (strcmp(input, "/exit") == 0 || (is_admin && strcmp(input, "4") == 0)) {
            send(sock, PREFIX_EXIT, 5, 0);
            is_running = 0;
            printf("\r\033[K[System] Disconnecting from The Wired...\n");
            break;
        }

        if (strlen(input) > 0) {
            char final_buf[BUFFER_SIZE];
            if (is_admin) {
                snprintf(final_buf, sizeof(final_buf), PREFIX_CMD "%s", input);
            } else {
                snprintf(final_buf, sizeof(final_buf), PREFIX_MSG "%s", input);
            }
            send(sock, final_buf, strlen(final_buf), 0);
        }
    }

    close(sock);
    return 0;
}
