#ifndef CLIENT_H
#define CLIENT_H 

#include <zmq_addon.hpp>
#include <zmq.hpp>

#include <iostream>
#include <queue>
#include <thread>

class Client {

public:

  zmq::context_t m_ctx;
  zmq::socket_t m_sock;
  
  std::string m_topic {""};

  Client(); 
  Client(std::string,std::string, std::string); 

  std::string message_to_string(zmq::message_t&);

  void send_request(std::string); 

};

#endif
