#include "sock.hpp"

#include <iostream>

using namespace std;

int main() {
  net::UDPSender sender(50000);
  int count = 0;

  while (1) {
    sender.sendData<int>(count);

    cout << "send: " << count << endl;
    count++;

    sleep(1);
  }

  return 0;
}