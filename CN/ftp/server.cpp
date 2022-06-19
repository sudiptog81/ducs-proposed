// Server Application - Sudipto Ghosh

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 100

using namespace std;

int main()
{
  struct stat obj;
  struct sockaddr_in server, client;

  int size, port, fileHandle,
      serverSocket, clientSocket;

  char buf[BUFFER_SIZE], command[BUFFER_SIZE],
      fileName[BUFFER_SIZE], path[BUFFER_SIZE];

  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0)
  {
    cerr << "Error: Socket creation failed" << endl;
    exit(1);
  }

  cout << "Enter Port Number: ";
  cin >> port;

  server.sin_port = htons(port);
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;

  if (bind(serverSocket, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
  {
    cerr << "Error: Bind failed" << endl;
    exit(1);
  }

  if (listen(serverSocket, 1) < 0)
  {
    cerr << "Error: Listen failed" << endl;
    exit(1);
  }

  cout << "Server started..." << endl;
  cout << "Listening on 0.0.0.0:" << port << endl;

  int len = sizeof(client);
  clientSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t *)&len);

  cout << "Client connected..." << endl;

  while (true)
  {

    recv(clientSocket, buf, 100, 0);
    sscanf(buf, "%s", command);

    if (!strcmp(command, "ls"))
    {
      system("ls -l > ls.tmp");
      stat("ls.tmp", &obj);
      size = obj.st_size;
      send(clientSocket, &size, BUFFER_SIZE, 0);
      fileHandle = open("ls.tmp", O_RDONLY);
      sendfile(clientSocket, fileHandle, NULL, size);
      remove("ls.tmp");
    }
    else if (!strcmp(command, "cd"))
    {
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      if (chdir(buf) < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
    }
    else if (!strcmp(command, "pwd"))
    {
      getcwd(path, BUFFER_SIZE);
      send(clientSocket, (const void *)path, BUFFER_SIZE, 0);
    }
    else if (!strcmp(command, "mkdir"))
    {
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      if (mkdir(buf, 0777) < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
    }
    else if (!strcmp(command, "rmdir"))
    {
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      if (rmdir(buf) < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
    }
    else if (!strcmp(command, "rm"))
    {
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      if (remove(buf) < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
    }
    else if (!strcmp(command, "get"))
    {
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      stat(buf, &obj);
      fileHandle = open(buf, O_RDONLY);
      size = obj.st_size;
      if (fileHandle < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
      {
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
        send(clientSocket, (const void *)&size, BUFFER_SIZE, 0);
        sendfile(clientSocket, fileHandle, NULL, size);
        close(fileHandle);
      }
    }
    else if (!strcmp(command, "put"))
    {
      int c;
      char *f;
      recv(clientSocket, buf, BUFFER_SIZE, 0);
      fileHandle = open(buf, O_WRONLY | O_CREAT, 0777);
      if (fileHandle < 0)
        send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
      else
      {
        send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
        recv(clientSocket, &size, BUFFER_SIZE, 0);
        f = (char *)malloc(sizeof(char) * size);
        recv(clientSocket, f, size, 0);
        if (write(fileHandle, f, size) < 0)
          send(clientSocket, (const void *)"fail", BUFFER_SIZE, 0);
        else
          send(clientSocket, (const void *)"success", BUFFER_SIZE, 0);
        close(fileHandle);
      }
    }
    else if (!strcmp(command, "exit"))
    {
      send(clientSocket, (const void *)"bye", BUFFER_SIZE, 0);
      cout << "Client disconnected..." << endl;
      cout << "Server exiting..." << endl;
      break;
    }
    else if (!strcmp(command, "ping"))
    {
      send(clientSocket, (const void *)"pong", BUFFER_SIZE, 0);
    }
  }

  return 0;
}
