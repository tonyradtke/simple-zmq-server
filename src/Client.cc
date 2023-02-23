#include "Client.hh"

#define kSLEEP_MS 1

Client::Client(std::string f_ip, std::string f_port) {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pair);

  const std::string addr = "tcp://" + f_ip + ":" + f_port;
  m_sock.connect(addr);

  std::cout << "created a client at : " << addr << "\n";
  start_polling_thread();
}


Client::Client() {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pair);
  m_sock.connect("tcp://127.0.0.1:8000");

  std::cout << "created a localhost client \n";

  start_polling_thread();
};


void Client::start_polling_thread() {
  m_threadPool.push_back(std::thread([&](){ poll_response(); })); 
}


std::string Client::wait_for_response() {
  std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
  while (true) {
    if (not has_message()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
      continue;
    }   
    return pop_queue(); 
  }
}


void Client::poll_response() {

  while (true) {

    if (m_killed) {
      std::cout << "client polling thread has been killed\n";
      return;
    }

    zmq::message_t message;

    auto ret = m_sock.recv(message, zmq::recv_flags::dontwait);

    if (not ret) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); //dont shread the cpu polling :)
      continue;
    }

    std::string msg_str = message_to_string(message);

    push_to_queue(msg_str);
  }
}


std::string Client::pop_queue() {
    m_mutex.lock();
    std::string j = m_messageQueue.front();
    m_messageQueue.pop();
    m_mutex.unlock();
    return j;
}


void Client::push_to_queue(std::string& f_m) { 
  m_mutex.lock();
  m_messageQueue.push((f_m));
  m_mutex.unlock();
};


void Client::send_request(std::string f_response) {
  zmq_send(m_sock, strdup(f_response.c_str()), strlen(f_response.c_str()), 0);
}


std::string Client::message_to_string(zmq::message_t& f_msg) {
  return std::string(static_cast<char*>(f_msg.data()), f_msg.size());
}


bool Client::has_message() {
  return not m_messageQueue.empty();
}


void Client::kill() {
  m_killed = true;
}


Client::~Client() {
  kill();
  for (auto& thread : m_threadPool) {
    thread.join();
  }
}
