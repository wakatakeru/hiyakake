#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define LISTEN_PORT_NUMBER 6789
#define TEST_STRING_LENGTH 128

void error(char *msg);
void show_hiyakake_splash(int listen_port_number);

int main(int argc, char *argv[])
{
  int listen_port_number;
  int socket_listen;
  int socket_fd;

  char read_character;
  char test_string[TEST_STRING_LENGTH] = "TESTSTRING";
  char test_character = 'T';

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  socklen_t lenght_client_address;

  lenght_client_address = sizeof(client_address);
  listen_port_number = LISTEN_PORT_NUMBER; /* TODO Get number by config file */

  show_hiyakake_splash(listen_port_number);

  if ((socket_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    error("[Failed] Open datastream socket");
  }

  bzero((char *)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(listen_port_number);

  if (bind(socket_listen, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
  {
    close(socket_listen);
    error("[Failed] Bind local address");
  }

  if (listen(socket_listen, 5) == -1)
  {
    close(socket_listen);
    error("[Failed] Listen");
  }

  if ((socket_fd = accept(socket_listen, (struct sockaddr *)&client_address, &lenght_client_address)) < 0)
  {
    close(socket_listen);
    close(socket_fd);
    error("[Failed] Accept");
  }

  fcntl(socket_fd, F_SETFL, O_NONBLOCK);

  while (true)
  {
    if (read(socket_fd, &read_character, sizeof(read_character)) == -1)
    {
      break;
    }

    putchar(read_character);
  }

  write(socket_fd, "HTTP1.1 200 OK", 14);

  close(socket_fd);
  close(socket_listen);

  return 0;
}

void show_hiyakake_splash(int listen_port_number)
{
  puts("   __ ___           __        __      ");
  puts("  / // (_)_ _____ _/ /_____ _/ /_____ ");
  puts(" / _  / / // / _ `/  '_/ _ `/  '_/ -_)");
  puts("/_//_/_/\\_, /\\_,_/_/\\_\\_,_/_/\\_\\__/ ");
  puts("       /___/                          ");
  puts("");
  puts("Hiyakake HTTP Server");
  printf("Listen Port: %d\n", listen_port_number);

  return;
}

void error(char *msg)
{
  puts(msg);

  exit(1);
}