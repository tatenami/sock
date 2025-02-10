#include "sock.hpp"

#include <iostream>

using namespace std;

int main() {
  net::UDPReceiver reciever(50000);
  int buf = 0;

  while (1) {
    reciever.receiveData<int>(buf);

    cout << "data: " << buf << " IP: " << 
    reciever.getSrcAddr() << " PORT: " << reciever.getSrcPort() << endl;
  }

  return 0;
}