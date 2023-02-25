#include "Client.hh"

#define kSLEEP_MS 1

Client::Client(std::string f_ip, std::string f_port, std::string f_topic) {
  
  m_topic = f_topic; 
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pub);

  const std::string addr = "tcp://" + f_ip + ":" + f_port;
  m_sock.bind(addr);
  std::cout << "created a client at : " << addr << "\n";
}


Client::Client() {
  m_topic = "A";
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pub);
  m_sock.bind("tcp://127.0.0.1:8000");
  std::cout << "created a localhost client \n";
};


void Client::send_request(std::string f_response) {
  zmq_send(m_sock, strdup(m_topic.c_str()), size_t(1),  ZMQ_SNDMORE);
  zmq_send(m_sock, strdup(f_response.c_str()), strlen(f_response.c_str()), 0);
}


std::string Client::message_to_string(zmq::message_t& f_msg) {
  return std::string(static_cast<char*>(f_msg.data()), f_msg.size());
}

