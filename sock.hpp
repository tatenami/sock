#ifndef SOCK_H
#define SOCK_H

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <string>
#include <iostream>

namespace net {

  extern const std::string LOOPBACK_ADDR;
  extern const std::string BROADCAST_ADDR;


  class BaseSock {
   protected:
    bool enable_{false};
    int sock_;
    uint16_t    port_;
    std::string ip_addr_;
    sockaddr_in addr_;
    socklen_t   addr_len_;

    void setSock(int sock_type, std::string ip_address, uint16_t port_number);

   public:
    BaseSock(int sock_type, std::string ip_address, uint16_t port_number);
    ~BaseSock();
    bool enable();
  };



  class UDPSock: public BaseSock {
   public:
    UDPSock(std::string ip_address, uint16_t port_number);
  };


  class UDPSender: public UDPSock {
   public:
    UDPSender(std::string ip_address, uint16_t port_number);

    template <typename T>
    bool sendData(T& data) const {
      if (!enable_) return false; 

      int rsize = sendto(sock_, &data, sizeof(T), 0, (sockaddr *)&addr_, addr_len_);
      if (rsize < 0) {
        return false;
      }
      return true;
    }
  };


  class UDPReceiver: public UDPSock {
   private:
    sockaddr_in src_addr_;
    bool use_nonblocking_{false};
    void bindSock();

   public:
    UDPReceiver(std::string ip_address, uint16_t port_number);
    bool enableNonBlocking();
    bool recieveData(void* buf, int size);
    std::string getSrcAddr();
    uint16_t getSrcPort();

    template <typename T>
    bool receiveData(T& buf) {
      if (!enable_) return false;

      socklen_t len = sizeof(src_addr_);
      int ret = recvfrom(sock_, &buf, sizeof(T), 0, (sockaddr *)&src_addr_, &len);
      if (ret < 0) {
        return false;
      }
      return true;
    }
  };

  // class UDPSocket {
  //  protected:
  //   int sock_;
  //   sockaddr_in addr_;

  //  public:
  //   UDPSocket();
  //   void closeSocket();
  //   ~UDPSocket();
  // };


  // class UDPServer: public UDPSocket {
  //  private:
  //   sockaddr_in src_addr_;

  //  public:
  //   UDPServer(std::string address, uint16_t port);
  //   UDPServer(uint16_t port);

  //   template <typename T>
  //     int receive(T* buf) {
  //     socklen_t addr_len = static_cast<socklen_t>(sizeof(src_addr_));
  //     return recvfrom(this->sock_, buf, sizeof(T), 0, (sockaddr*)&this->src_addr_, &addr_len);
  //   }

  //   template <typename T>
  //   int receive(T* buf, int size) {
  //     socklen_t addr_len = static_cast<socklen_t>(sizeof(src_addr_));
  //     return recvfrom(this->sock_, buf, size, 0, (sockaddr*)&this->src_addr_, &addr_len);
  //   }
  // };

}

#endif // XXX_H