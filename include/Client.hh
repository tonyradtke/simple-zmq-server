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

  std::mutex m_mutex;

  bool m_killed {false};

  std::queue<std::string> m_messageQueue;
  std::vector<std::thread> m_threadPool;

  ~Client();
  Client(); 
  Client(std::string,std::string); 

  bool has_message();

  std::string pop_queue();
  std::string wait_for_response();
  std::string message_to_string(zmq::message_t&);

  void kill();
  void poll_response();
  void start_polling_thread();
  void send_request(std::string); 
  void push_to_queue(std::string&);

};

#endif
