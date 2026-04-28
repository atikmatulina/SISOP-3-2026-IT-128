#include "protocol.h"

Client clients[MAX_CLIENTS];
time_t start_time;

void init_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = 0;
        clients[i].is_admin = 0;
        clients[i].is_auth = 0;
        memset(clients[i].name, 0, 50);
    }
}

void write_log(const char *actor, const char *action) {
    FILE *f = fopen("history.log", "a");
    if (!f) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[25];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    fprintf(f, "[%s] [%s] [%s]\n", time_str, actor, action);
    fclose(f);
}

void disconnect_client(int index) {
    if (clients[index].fd != 0) {
        char logmsg[128];
        sprintf(logmsg, "User '%s' disconnected", clients[index].name);
        write_log("System", logmsg);
        close(clients[index].fd);
        clients[index].fd = 0;
        clients[index].is_auth = 0;
        clients[index].is_admin = 0;
    }
}

int main() {
    int server_fd, new_socket, max_sd, sd, activity;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    init_clients();
    start_time = time(NULL);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    write_log("System", "SERVER ONLINE");
    printf("The Wired terhubung pada port %d...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].fd;
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("Accept error");
                continue;
            }
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].fd == 0) {
                    clients[i].fd = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].fd;
            if (FD_ISSET(sd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int valread = read(sd, buffer, BUFFER_SIZE - 1);
                
                if (valread == 0) {
                    disconnect_client(i);
                } else {
                    buffer[valread] = '\0';
                    
                    if (strncmp(buffer, PREFIX_NAME, 5) == 0) {
                        char *name = buffer + 5;
                        int is_duplicate = 0;
                        for (int j = 0; j < MAX_CLIENTS; j++) {
                            if (clients[j].fd != 0 && clients[j].is_auth && strcmp(clients[j].name, name) == 0) {
                                is_duplicate = 1;
                                break;
                            }
                        }

                        if (is_duplicate) {
                            send(sd, RESP_ERR "The identity is already synchronized in The Wired.", 54, 0);
                        } else if (strcmp(name, ADMIN_NAME) == 0) {
                            strcpy(clients[i].name, name);
                            send(sd, RESP_PWD "Enter Password:", 15, 0);
                        } else {
                            strcpy(clients[i].name, name);
                            clients[i].is_auth = 1;
                            
                            char logmsg[128], reply[128];
                            sprintf(logmsg, "User '%s' connected", name);
                            write_log("System", logmsg);
                            
                            sprintf(reply, RESP_SYS "--- Welcome to The Wired, %s ---", name);
                            send(sd, reply, strlen(reply), 0);
                        }
                    } else if (strncmp(buffer, PREFIX_PASS, 5) == 0) {
                        char *pass = buffer + 5;
                        if (strcmp(pass, ADMIN_PASS) == 0) {
                            clients[i].is_auth = 1;
                            clients[i].is_admin = 1;
                            write_log("System", "User 'The Knights' connected");
                            
                            send(sd, RESP_AUTH "[System] Authentication Successful. Granted Admin privileges.", 61, 0);
                            usleep(10000); // Sinkronisasi socket kecil
                            send(sd, RESP_MENU "=== THE KNIGHTS CONSOLE ===\n1. Check Active Entities (Users)\n2. Check Server Uptime\n3. Execute Emergency Shutdown\n4. Disconnect\nCommand >> ", 141, 0);
                        } else {
                            send(sd, RESP_ERR "Incorrect password. Disconnecting...", 36, 0);
                            disconnect_client(i);
                        }
                    } else if (strncmp(buffer, PREFIX_MSG, 4) == 0) {
                        char *text = buffer + 4;
                        char logmsg[BUFFER_SIZE];
                        sprintf(logmsg, "[%s]: %s", clients[i].name, text);
                        write_log("User", logmsg);

                        char bcast[BUFFER_SIZE];
                        sprintf(bcast, RESP_CHAT "[%s]: %s", clients[i].name, text);
                        
                        // Broadcast ke seluruh client yang non-admin
                        for (int j = 0; j < MAX_CLIENTS; j++) {
                            if (clients[j].fd != 0 && clients[j].is_auth && !clients[j].is_admin && j != i) {
                                send(clients[j].fd, bcast, strlen(bcast), 0);
                            }
                        }
                    } else if (strncmp(buffer, PREFIX_CMD, 4) == 0) {
                        char *cmd = buffer + 4;
                        int c = atoi(cmd);
                        char reply[128];
                        
                        if (c == 1) {
                            int count = 0;
                            for (int j = 0; j < MAX_CLIENTS; j++) {
                                if (clients[j].fd != 0 && clients[j].is_auth && !clients[j].is_admin) count++;
                            }
                            write_log("Admin", "RPC_GET_USERS");
                            sprintf(reply, RESP_SYS "%d Active Entity(s)\nCommand >> ", count);
                            send(sd, reply, strlen(reply), 0);
                        } else if (c == 2) {
                            time_t now = time(NULL);
                            int diff = (int)difftime(now, start_time);
                            write_log("Admin", "RPC_GET_UPTIME");
                            sprintf(reply, RESP_SYS "Server Uptime: %d seconds\nCommand >> ", diff);
                            send(sd, reply, strlen(reply), 0);
                        } else if (c == 3) {
                            write_log("Admin", "RPC_SHUTDOWN");
                            write_log("System", "EMERGENCY SHUTDOWN INITIATED");
                            for (int j = 0; j < MAX_CLIENTS; j++) {
                                if (clients[j].fd != 0) {
                                    send(clients[j].fd, RESP_SYS "Server is shutting down...", 26, 0);
                                    close(clients[j].fd);
                                }
                            }
                            exit(0);
                        }
                    } else if (strncmp(buffer, PREFIX_EXIT, 5) == 0) {
                        disconnect_client(i);
                    }
                }
            }
        }
    }
    return 0;
}
