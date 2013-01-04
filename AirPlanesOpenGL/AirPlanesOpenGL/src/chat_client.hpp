//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <deque>
#include <iostream>
#include <utility>

//#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

double global_vec[4];
std::string cli_name;

//static double			myPos[4]; // moja pozycja
//static double			opPos[4]; // pozycja przeiwnika

static std::vector<double> tmp_Me(6,0.0f);
std::vector<double> tmp_OP(6,0.0f);

typedef std::vector<std::pair<std::string,std::vector<double>>> Player;
extern Player players;

std::vector<std::pair<std::string,bool>> czyPapa;


class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
	  tcp::resolver::iterator endpoint_iterator)
	: io_service_(io_service),
	  socket_(io_service),
	  endpoint_iterator(endpoint_iterator)
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

  void *operator new(std::size_t sz) {
	void *out = ::malloc(sz);
	if (!out) throw new std::bad_alloc();
	return out;
}

static void operator delete(void* wsk) {
	  delete wsk;
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

	  // papa
	  
	  if(strs.size() > 1 )  
		  if(strs[1] == "papa")
		  {
			  if(czyPapa.empty())
				  czyPapa.push_back(std::make_pair(strs[0],true));
			  else
			  {
				  bool is = false;
				  for(int i=0;i<czyPapa.size();++i)
				  {
					  if(strs[0] == czyPapa[i].first)
						  is = czyPapa[i].second = true;
				  }
				  if(is == true)
					czyPapa.push_back(std::make_pair(strs[0],true));
			  }
		  }

	// play
	  if(strs.size()>7 && cli_name != strs[0])
	  {
			try
			{
				tmp_OP[0] = boost::lexical_cast<double>(strs[1]);
				tmp_OP[1] = boost::lexical_cast<double>(strs[2]);
				tmp_OP[2] = boost::lexical_cast<double>(strs[3]);
				tmp_OP[3] = boost::lexical_cast<double>(strs[4]);
				tmp_OP[4] = boost::lexical_cast<double>(strs[5]);
				tmp_OP[5] = boost::lexical_cast<double>(strs[6]);
				//tmp_OP[6] = boost::lexical_cast<double>(strs[7]);
			}
			catch (boost::bad_lexical_cast &ex)
			{

			}
		
		if(players.empty())
			players.push_back(std::make_pair(strs[0],tmp_OP));
		else
		{
			for(int i=0;i<players.size();++i)
			{
				if(players[i].first == strs[0])
					players[i] = std::make_pair(strs[0],tmp_OP);
			}
		}

		for(int i=0;i<players.size();++i)
		{
			for(int j=0;j<czyPapa.size();++j)
			{
				if(players[i].first == czyPapa[j].first)
					czyPapa[j].second = false;
			}
		}

	  }
	  //if(strs.size()>4 && cli_name != strs[0])
	  //{
			//try
			//{
			//	opPos[0] = boost::lexical_cast<double>(strs[1]);
			//	opPos[1] = boost::lexical_cast<double>(strs[2]);
			//	opPos[2] = boost::lexical_cast<double>(strs[3]);
			//	opPos[3] = boost::lexical_cast<double>(strs[4]);
			//}
			//catch (boost::bad_lexical_cast &ex)
			//{

			//}

			//std::cout << strs[0] << ',' << opPos[0] << ',' << opPos[1] << ',' << opPos[2] << ',' << opPos[3] << std::endl;
	  //}

	 //std::cout << strs[0] << ',' << opPos[0] << ',' << opPos[1] << ',' << opPos[2] << ',' << opPos[3] << std::endl;

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
  tcp::resolver::iterator endpoint_iterator;
};



//boost::asio::io_service io_service;
//
//tcp::resolver resolver(io_service);
//tcp::resolver::query query("192.168.0.100", "1234");
//tcp::resolver::iterator iterator = resolver.resolve(query);
//
//chat_client			_c(io_service, iterator);
//boost::thread		_t(boost::bind(&boost::asio::io_service::run, &io_service));
//
// //Definicja
//class Connection
// {
//  public:
//		void Init(const std::string &imie,const std::string &ip = "192.168.0.100",const std::string &port = "1234")
//		{
//			_ip = ip;
//			_port = port;
//			cli_name =_imie = imie;
//
//			for(int i=0;i<3;++i)
//				myPos[i] = opPos[i] = 0.0;
//
//		}
//
//		void Start()
//		{
//
//			//while (1)
//			////{
//			// -----------------------------
//			// uzywac globali myPos i opPos
//			//  vec[0]+=0.0001;
//			//  vec[1]+=0.0005;
//			//  vec[2]+=0.05;
//			//  vec[3]+=0.2;
//			// -----------------------------
//
//			_sstr.str( std::string() );
//			_sstr << cli_name << ',' << myPos[0] << ',' << myPos[1] << ',' << myPos[2] << ',' << myPos[3];
//
//			//chat_message msg;
//			_msg.body_length(_sstr.str().length());
//			std::memcpy(_msg.body(), _sstr.str().c_str(), _msg.body_length());
//			_msg.encode_header();
//				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
//			_c.write(_msg);
//			
//			//}
//		}
//
//		void Stop()
//		{
//			_c.close();
//			_t.join();
//		}
//
//		static Connection& getInstance()
//		{
//		  static Connection instance;
//		  return instance;
//		}
//
//private:
//		Connection() {}
//		Connection(const Connection &);
//		Connection& operator=(const Connection&);
//
//		//boost::asio::io_service			io_service;
//		//tcp::resolver					resolver;//(io_service);
//		//tcp::resolver::query			query;//(ip/*argv[1]*/, port/*argv[2]*/);
//		//tcp::resolver::iterator			iterator;// = resolver.resolve(query);
//
//		///*static*/ chat_client			_c;
//		///*static*/ boost::thread		_t;
//
//
//		/*static*/ std::string			_ip;
//		/*static*/ std::string			_port;
//		/*static*/ std::string			_imie;
//		/*static*/ std::stringstream	_sstr;
//		/*static*/ chat_message			_msg;
// };
//
//  //Uzycie
// //std::cout << singleton::getInstance().method();

 //Definicja
class Connection
 {
  public:
		void Init(std::string imie, std::string ip = "192.168.1.105",std::string port = "1234")
		{
			_ip = ip;
			_port = port;
			cli_name =_imie = imie;

			//for(int i=0;i<3;++i)
			//	myPos[i] = opPos[i] = 0.0;

			tcp::resolver resolver(io_service);

			tcp::resolver::query query(ip, port);
			tcp::resolver::iterator iterator = resolver.resolve(query);

			//chat_client c(io_service, iterator);

			_c = new chat_client(io_service, iterator);
			_t = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
		}

		void Start()
		{

			//while (1)
			////{
			// -----------------------------
			// uzywac globali myPos i opPos
			//  vec[0]+=0.0001;
			//  vec[1]+=0.0005;
			//  vec[2]+=0.05;
			//  vec[3]+=0.2;
			// -----------------------------

			_sstr.str( std::string() );
			_sstr << cli_name << ',' << 
				tmp_Me[0] << ',' << tmp_Me[1] << ',' << tmp_Me[2] << ',' << 
				tmp_Me[3] << ',' << tmp_Me[4] << ',' << tmp_Me[5];

			//std::string tttt = _sstr.str();

			//chat_message msg;
			_msg.body_length(_sstr.str().length());
			std::memcpy(_msg.body(), _sstr.str().c_str(), _msg.body_length());
			_msg.encode_header();
			//	boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			_c->write(_msg);
			
			//}
		}

		void sendByebye()
		{
			_sstr.str( std::string() );
			_sstr << cli_name << ',' << "papa";

			//chat_message msg;
			_msg.body_length(_sstr.str().length());
			std::memcpy(_msg.body(), _sstr.str().c_str(), _msg.body_length());
			_msg.encode_header();
			//	boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			_c->write(_msg);
		}

		void Stop()
		{
			_c->close();
			_t->join();
		}

		static Connection& getInstance()
		{
		  static Connection instance;
		  return instance;
		}

private:
		Connection() {}
		Connection(const Connection &);
		Connection& operator=(const Connection&);

		boost::asio::io_service				io_service;

		chat_client*						_c;
		boost::thread*						_t;


		std::string			_ip;
		std::string			_port;
		std::string			_imie;
		std::stringstream	_sstr;
		chat_message			_msg;
 };

  //Uzycie
 //std::cout << singleton::getInstance().method();

 #endif 