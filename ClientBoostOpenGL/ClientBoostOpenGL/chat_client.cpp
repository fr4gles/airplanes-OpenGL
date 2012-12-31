//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <deque>
#include <iostream>
//#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

static double i=0.0;

double global_vec[4];
std::string cli_name;

class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
      socket_(io_service)
  {
    boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&chat_client::handle_connect, this,
          boost::asio::placeholders::error));
  }

  void write(const chat_message& msg)
  {
    io_service_.post(boost::bind(&chat_client::do_write, this, msg));
  }

  void close()
  {
    io_service_.post(boost::bind(&chat_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.data(), chat_message::header_length),
          boost::bind(&chat_client::handle_read_header, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_read_header(const boost::system::error_code& error)
  {
    if (!error && read_msg_.decode_header())
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
          boost::bind(&chat_client::handle_read_body, this,
            boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void handle_read_body(const boost::system::error_code& error)
  {
    if (!error)
    {
      /*std::cout.write(read_msg_.body(), read_msg_.body_length());
      std::cout << "\n";*/

      std::string tmp = read_msg_.body();//static_cast<std::string>()
      std::vector<std::string> strs;
      boost::split(strs, tmp, boost::is_any_of(","));

      if(strs.size()>4 && cli_name != strs[0])
      {
            try
            {
                global_vec[0] = boost::lexical_cast<double>(strs[1]);
                global_vec[1] = boost::lexical_cast<double>(strs[2]);
                global_vec[2] = boost::lexical_cast<double>(strs[3]);
                global_vec[3] = boost::lexical_cast<double>(strs[4]);
            }
            catch (boost::bad_lexical_cast &ex)
            {

            }

            std::cout << strs[0] << ',' << global_vec[0] << ',' << global_vec[1] << ',' << global_vec[2] << ',' << global_vec[3] << std::endl;
      }

      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.data(), chat_message::header_length),
          boost::bind(&chat_client::handle_read_header, this,
            boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void do_write(chat_message msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(write_msgs_.front().data(),
            write_msgs_.front().length()),
          boost::bind(&chat_client::handle_write, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      write_msgs_.pop_front();
      if (!write_msgs_.empty())
      {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
              write_msgs_.front().length()),
            boost::bind(&chat_client::handle_write, this,
              boost::asio::placeholders::error));
      }
    }
    else
    {
      do_close();
    }
  }

  void do_close()
  {
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};



int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      //return 1;
    }



    std::cout << "Podaj imie: " << std::endl;
    std::cin >> cli_name;
    double vec[4] = {0.0,0.0,0.0,0.0};

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query("192.168.1.105"/*argv[1]*/, "1234"/*argv[2]*/);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    chat_client c(io_service, iterator);

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    char line[chat_message::max_body_length + 1];
    /*while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      using namespace std; // For strlen and memcpy.
      chat_message msg;
      msg.body_length(strlen(line));
      memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }*/
    std::stringstream str;
    while (1)
    {



      vec[0]+=0.0001;
      vec[1]+=0.0005;
      vec[2]+=0.05;
      vec[3]+=0.2;

      str.str( std::string() );
      str << cli_name << ',' << vec[0] << ',' << vec[1] << ',' << vec[2] << ',' << vec[3];
      //if(i10000=0)
      using namespace std; // For strlen and memcpy.
      chat_message msg;
      msg.body_length(str.str().length());
      memcpy(msg.body(), str.str().c_str(), msg.body_length());
      msg.encode_header();
            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      c.write(msg);



    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
