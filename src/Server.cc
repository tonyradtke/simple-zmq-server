#include "Server.hh"

#define kSLEEP_MS 1 


//pair
Server::Server(std::string f_ip, std::string f_port, std::string f_topic) {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::sub);
  const std::string addr = "tcp://" + f_ip + ":" + f_port;
  m_sock.connect(addr);
  
  std::cout << "created a server that can only read from : " << addr << "\n";

  m_sock.set(zmq::sockopt::subscribe, f_topic);
  start_polling_thread();
}


Server::Server(bool f_localhost) {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::sub);

  if (f_localhost) {
    m_sock.connect("tcp://127.0.0.1:8000");
    std::cout << "created a localhost server\n";
  }
  else {
    m_sock.bind("tcp://*:8000");
    std::cout << "created an open server\n";
  }

  m_sock.set(zmq::sockopt::subscribe, "A");
  start_polling_thread();
};


std::string Server::wait_for_request() {
  std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
  while (true) {
    if (not has_message()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
      continue;
    }
    return pop_queue(); 
  }
}


void Server::start_polling_thread() {
  m_threadPool.push_back(std::thread([&](){ poll_request(); }));
}


void Server::poll_request() {
  while (true) {

    if (m_killed) {
      std::cout << "server polling thread has been killed\n";
      return;
    }
  
    //[0] is topic
    //[1] is message
    std::vector<zmq::message_t> messages;

    auto ret =  zmq::recv_multipart(m_sock, std::back_inserter(messages), zmq::recv_flags::dontwait);

    if (not ret) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); //dont shread the cpu polling :)
      continue;
    }

    if (messages.size() != 2) {
      continue; 
    }

    std::string msg_str = message_to_string(messages.back());

    push_to_queue(msg_str);
  }
}


std::string Server::pop_queue() {
  m_mutex.lock();
  std::string j = m_messageQueue.front();
  m_messageQueue.pop();
  m_mutex.unlock();
  return j;
}


void Server::push_to_queue(std::string& f_m) { 
  m_mutex.lock(); 
  m_messageQueue.push((f_m));
  m_mutex.unlock();
};


std::string Server::message_to_string(zmq::message_t& f_msg) { //comes through as void*
  return std::string(static_cast<char*>(f_msg.data()), f_msg.size());
}


bool Server::has_message() {
  return not m_messageQueue.empty();
}


void Server::kill() {
  m_killed = true;
}


Server::~Server() {
  kill();
  for (auto& thread : m_threadPool) {
    thread.join();
  }
}




