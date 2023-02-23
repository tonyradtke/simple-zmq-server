#ifndef CLIENT_H
#define CLIENT_H 

#include <zmq_addon.hpp>
#include <zmq.hpp>

#include <iostream>
#include <queue>
#include <thread>

class Client {

public:

  //we should prob put the ctx, sock, messageQueue, thread shit, etc into a 
  //ZmqContext class, then each of these has one of those...
  //
  zmq::context_t m_ctx;
  zmq::socket_t m_sock;

  ~Client();
  Client(bool); 
 
  std::mutex m_mutex;

  bool m_killed; 
  bool m_isLocalHost = false;
  std::queue<std::string> m_messageQueue;
  
  std::vector<std::thread> m_threadPool;

  void start_polling_thread();

  std::string pop_queue();
  std::string message_to_string(zmq::message_t& f_msg);
  std::string wait_for_response();
  
  bool has_message();

  void kill();
  void poll_response();
  void send_request(std::string); 
  void push_to_queue(std::string&);


  void foo();

};

#endif
