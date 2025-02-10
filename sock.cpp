#include "sock.hpp"

namespace net {

  const std::string LOOPBACK_ADDR  = "127.0.0.1";
  const std::string BROADCAST_ADDR = "255.255.255.255";

  namespace {
    const int valid_flag = 1;
    const int invalid_flag = 0;
  }

  void BaseSock::setSock(int sock_type, std::string ip_address, uint16_t port_number) {
    ip_addr_ = ip_address;
    port_ = port_number;

    sock_ = socket(AF_INET, sock_type, 0);
    if (sock_ < 0) {
      return;
    }
    else {
      enable_ = true;
    }

    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip_addr_.c_str());
    addr_.sin_port = htons(port_);
    addr_len_ = static_cast<socklen_t>(sizeof(addr_));
  }

  BaseSock::BaseSock(int sock_type, std::string ip_address, uint16_t port_number) {
    setSock(sock_type, ip_address, port_number);
  }

  BaseSock::~BaseSock() {
    close(sock_);
  }

  bool BaseSock::enable() {
    return enable_;
  }


  UDPSock::UDPSock(std::string ip_address, uint16_t port_number):
    BaseSock(SOCK_DGRAM, ip_address, port_number)
  {

  }


  UDPSender::UDPSender(std::string ip_address, uint16_t port_number):
    UDPSock(ip_address, port_number)
  { 
    if (ip_address == BROADCAST_ADDR) {
      int ret = setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, &valid_flag, sizeof(valid_flag));
      if (ret < 0) enable_ = false;
    }
  }


  UDPReceiver::UDPReceiver(std::string ip_addr, uint16_t port_number):
    UDPSock(ip_addr, port_number)
  {
    bindSock();
  }

  void UDPReceiver::bindSock() {
    int ret = bind(sock_, (sockaddr *)&addr_, addr_len_);
    if (ret < 0) {
      enable_ = false;
    }
  }

  bool UDPReceiver::enableNonBlocking() {
    int ret = ioctl(sock_, FIONBIO, &valid_flag);
    if (ret < 0) {
      enable_ = false;
      return false;
    }

    use_nonblocking_ = true;
    return true;
  }

  bool UDPReceiver::recieveData(void* buf, int size) {
    if (!enable_) return false;

    socklen_t len = sizeof(src_addr_);
    int ret = recvfrom(sock_, buf, size, 0, (sockaddr *)&src_addr_, &len);

    if (ret < 0) {
      return false;
    }
    return true;
  }

  std::string UDPReceiver::getSrcAddr() {
    return std::string(inet_ntoa(src_addr_.sin_addr));
  }

  uint16_t UDPReceiver::getSrcPort() {
    return ntohs(src_addr_.sin_port);
  }


}