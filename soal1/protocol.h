#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>

// Konfigurasi Koneksi
#define PORT 8080
#define IP "127.0.0.1"
#define MAX_CLIENTS 100

// Manajemen Memori (Kunci untuk cegah wformat-overflow)
#define BUFFER_SIZE 2048
#define MAX_INPUT (BUFFER_SIZE - 20) // Sisakan space untuk prefix

// Kredensial Admin
#define ADMIN_NAME "The Knights"
#define ADMIN_PASS "protocol7"

// Protokol Client -> Server
#define PREFIX_NAME "NAME:"
#define PREFIX_PASS "PASS:"
#define PREFIX_MSG  "MSG:"
#define PREFIX_CMD  "CMD:"
#define PREFIX_EXIT "EXIT:"

// Protokol Server -> Client
#define RESP_SYS    "SYS:"
#define RESP_ERR    "ERR:"
#define RESP_PWD    "PWD:"
#define RESP_AUTH   "AUTH_OK:"
#define RESP_MENU   "MENU:"
#define RESP_CHAT   "CHAT:"

typedef struct {
    int fd;
    char name[50];
    int is_admin;
    int is_auth;
} Client;

#endif
