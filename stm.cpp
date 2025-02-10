#include "sock.hpp"
#include <iostream>
#include <cmath>

using namespace std;

int main() {
  const int MAX_RPM = 9000;
  int16_t  rpm = 0;
  net::UDPSender sender("192.168.111.111", 11111);

  while (1) {
    cout << "Input rpm: ";
    cin >> rpm;

    if (abs(rpm) > MAX_RPM) {
      cout << "[ERROR] Over value!" << endl;
    }

    sender.sendData<int16_t>(rpm);
  }

  return 0;
}