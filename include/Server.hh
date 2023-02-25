#ifndef SERVER_H
#define SERVER_H 

#include <zmq_addon.hpp>
#include <zmq.hpp>

#include <iostream>
#include <queue>
#include <thread>

class Server {

public:

  zmq::context_t m_ctx;
  zmq::socket_t m_sock;

  std::mutex m_mutex;
  
  bool m_killed {false}; 
  bool m_isLocalHost {false};

  std::queue<std::string> m_messageQueue;
  std::vector<std::thread> m_threadPool; 

  ~Server(); 
  Server(bool = true); 
  Server(std::string, std::string, std::string);

  bool has_message();

  std::string pop_queue();
  std::string wait_for_request();
  std::string message_to_string(zmq::message_t&);

  void kill();
  void poll_request();
  void start_polling_thread();
  void send_response(std::string); 
  void push_to_queue(std::string&);

};

#endif
