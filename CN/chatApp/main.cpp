#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#define BUFFER_SIZE 100
using namespace std;

struct message
{
  string nick;
  string message;
};

void *serverCode(void *param);

int main()
{
  socklen_t len1, len2;
  char buf[BUFFER_SIZE];
  struct sockaddr_in server, client;
  int c, size, port, serverSocket, clientSocket;

  cout << "     MENU\n==============\n";
  cout << "(1) Create Room\n";
  cout << "(2) Join Room\n\n";
  cout << "Enter Choice: ";
  cin >> c;
  cout << endl;

  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0)
  {
    cerr << "Error: Socket creation failed" << endl;
    exit(1);
  }

  if (c == 1)
  {
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if (bind(serverSocket, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
    {
      cerr << "Error: Bind failed" << endl;
      exit(1);
    }

    if (listen(serverSocket, 5) < 0)
    {
      cerr << "Error: Listen failed" << endl;
      exit(1);
    }

    struct sockaddr_in sin;
    len1 = sizeof(sin);
    getsockname(serverSocket, (struct sockaddr *)&sin, &len1);

    cout
        << "Chat Room Started..." << endl;
    cout << "Joining Code: " << ntohs(sin.sin_port)
         << endl
         << endl;

    len2 = sizeof(client);

    vector<message> messages;

    while (true)
    {
      clientSocket = accept(serverSocket, (struct sockaddr *)&client, &len2);

      char nick[BUFFER_SIZE];

      recv(clientSocket, nick, BUFFER_SIZE, 0);
      cout << nick << " connected..." << endl;

      while (true)
      {
        char temp[BUFFER_SIZE];
        recv(clientSocket, buf, BUFFER_SIZE, 0);

        if (!strcmp(buf, "exit"))
        {
          cout << nick << " disconnected..." << endl;
          exit(0);
        }
        else
        {
          message m;
          m.nick = nick;
          m.message = string(buf);
          messages.push_back(m);
          cout << "[" << nick << "]: " << buf << endl;

          string s;
          cout << "[Server]: ";
          cin >> s;

          message m2;
          m2.nick = string("Server");
          m2.message = string(s);
          messages.push_back(m2);
          send(clientSocket, (const void *)s.c_str(), BUFFER_SIZE, 0);
        }
      }
    }
  }
  else if (c == 2)
  {
    string ipAddress;
    cout << "Enter Chat Server Address: ";
    cin >> ipAddress;
    cout << "Enter Joining Code: ";
    cin >> port;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_aton(ipAddress.c_str(), &server.sin_addr);

    if (connect(serverSocket, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
    {
      cerr << "Error: Connection failed" << endl;
      exit(1);
    }

    string nick;
    cout << "Enter Nickname: ";
    cin >> nick;
    cout << "Connected to Chat Room " << port << "..."
         << endl
         << endl;

    vector<message> messages;
    send(serverSocket, (const void *)nick.c_str(), BUFFER_SIZE, 0);

    while (true)
    {
      system("clear");

      cout << "Connected to Room " << port << "...\n\n";

      for (int i = 0; i < messages.size(); i++)
        cout << messages[i].nick << ": " << messages[i].message << endl;

      string s;
      cout << "\n[" << nick << "]: ";
      cin >> s;

      send(serverSocket, s.c_str(), BUFFER_SIZE, 0);

      if (!strcmp(s.c_str(), "exit"))
        exit(0);

      message m;
      m.nick = nick;
      m.message = s.c_str();
      messages.push_back(m);

      message m2;
      recv(serverSocket, buf, BUFFER_SIZE, 0);
      m2.nick = string("Server");
      m2.message = string(buf);
      messages.push_back(m2);
    }
  }
}
