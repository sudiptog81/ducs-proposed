// Client Application - Sudipto Ghosh

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
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
  struct sockaddr_in server;

  int status;
  string ipAddress;
  int serverSocket, choice, size, port;
  char buf[BUFFER_SIZE], command[BUFFER_SIZE],
      fileName[BUFFER_SIZE], *f;

  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0)
  {
    cerr << "Error: Socket creation failed" << endl;
    exit(1);
  }

  cout << "Enter Server Address: ";
  cin >> ipAddress;
  cout << "Enter Port Number: ";
  cin >> port;

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_aton(ipAddress.c_str(), &server.sin_addr);

  if (connect(serverSocket, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
  {
    cerr << "Error: Connection failed" << endl;
    exit(1);
  }

  cout << "Connected to Server..." << endl;

  while (true)
  {
    cout << "> ";
    cin >> command;

    if (!strcmp(command, "ping"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      cout << buf << endl;
    }
    else if (!strcmp(command, "pwd"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      cout << buf << endl;
    }
    else if (!strcmp(command, "ls"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      recv(serverSocket, &size, BUFFER_SIZE, 0);
      f = (char *)malloc(size);
      recv(serverSocket, f, size, 0);
      cout << f;
      free(f);
    }
    else if (!strcmp(command, "rm"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      cout << "Enter Filename: ";
      cin >> fileName;
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
        cout << "File Removed Successfully" << endl;
      else
        cout << "File Removal Failed" << endl;
    }
    else if (!strcmp(command, "mkdir"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      cout << "Enter Directory Name: ";
      cin >> fileName;
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
        cout << "Directory Created Successfully" << endl;
      else
        cout << "Directory Creation Failed" << endl;
    }
    else if (!strcmp(command, "rmdir"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      cout << "Enter Directory Name: ";
      cin >> fileName;
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
        cout << "Directory Removed Successfully" << endl;
      else
        cout << "Directory Removal Failed" << endl;
    }
    else if (!strcmp(command, "cd"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      cout << "Enter Directory Name: ";
      cin >> fileName;
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
        cout << "Directory Changed Successfully" << endl;
      else
        cout << "Directory Change Failed" << endl;
    }
    else if (!strcmp(command, "get"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      cout << "Enter Filename: ";
      cin >> fileName;
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
      {
        recv(serverSocket, &size, BUFFER_SIZE, 0);
        f = (char *)malloc(size);
        recv(serverSocket, f, size, 0);
        int fileHandle = creat(fileName, 0666);
        if (write(fileHandle, f, size) < 0)
          cout << "File could not be saved" << endl;
        else
        {
          cout << "File Saved Successfully" << endl;
          close(fileHandle);
        }
        free(f);
      }
      else
        cout << "File Retrieval Failed" << endl;
    }
    else if (!strcmp(command, "put"))
    {
      cout << "Enter Filename: ";
      cin >> fileName;
      int fileHandle = open(fileName, O_RDONLY);
      if (fileHandle < 0)
      {
        cout << "File doesn't exist" << endl;
        break;
      }
      send(serverSocket, command, BUFFER_SIZE, 0);
      send(serverSocket, fileName, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "success"))
      {
        stat(fileName, &obj);
        size = obj.st_size;
        send(serverSocket, &size, BUFFER_SIZE, 0);
        sendfile(serverSocket, fileHandle, NULL, size);
        recv(serverSocket, buf, BUFFER_SIZE, 0);
        if (!strcmp(buf, "success"))
          cout << "File Uploaded Successfully" << endl;
        else
          cout << "File Upload Failed" << endl;
      }
      else
        cout << "File Upload Failed" << endl;
      close(fileHandle);
    }
    else if (!strcmp(command, "exit"))
    {
      send(serverSocket, command, BUFFER_SIZE, 0);
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      if (!strcmp(buf, "bye"))
      {
        cout << "Disconnected from Server..." << endl;
        exit(0);
      }
      else
        cout << "Error: Disconnection failed" << endl;
    }
    else
    {
      cout << "Supported Commands:" << endl
           << "ls - list files in remote machine" << endl
           << "pwd - current directory on the remote" << endl
           << "put - upload a local file to the remote" << endl
           << "get - retrieve a remote file" << endl
           << "rm - remove a remote file" << endl
           << "cd - change directory on the remote" << endl
           << "mkdir - create a directory on the remote" << endl
           << "rmdir - remove a directory on the remote" << endl
           << "exit - disconnect from the session" << endl
           << "ping - expect pong" << endl;
    }
  }
}
