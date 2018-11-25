// MQTTBroker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "asio.hpp"
#include "AsioService.h"
#include "AsioServer.h"
#include "ServerIOStream.h"
#include <memory>
#include <iostream>
#include <mutex>
#include <thread>
//
//std::mutex global_stream_lock;
//
//void WorkerThread( std::shared_ptr<asio::io_service> io_service )
//{
//   global_stream_lock.lock();
//   std::cout << "[" << std::this_thread::get_id() << "] Thread Start" << std::endl;
//   global_stream_lock.unlock();
//
//   io_service->run();
//
//   global_stream_lock.lock();
//   std::cout << "[" << std::this_thread::get_id()
//      << "] Thread Finish" << std::endl;
//   global_stream_lock.unlock();
//}
//
//void PrintNum( int x )
//{
//   global_stream_lock.lock();
//   std::cout << "[" << std::this_thread::get_id() << "] x: " << x << std::endl;
//   global_stream_lock.unlock();
//}
//
//void OnAccept( const asio::error_code& ec, std::shared_ptr< asio::ip::tcp::socket > sock )
//{
//   if( ec )
//   {
//      global_stream_lock.lock();
//      std::cout << "[" << std::this_thread::get_id()
//         << "] Error: " << ec << std::endl;
//      global_stream_lock.unlock();
//   }
//   else
//   {
//      global_stream_lock.lock();
//      std::cout << "[" << std::this_thread::get_id()
//         << "] Accepted!" << std::endl;
//      global_stream_lock.unlock();
//
//      char* buffer = new char[500];
//      auto buf = asio::mutable_buffer( buffer, 500 );
//      sock->receive( buf );
//
//      global_stream_lock.lock();
//      std::cout << "[" << std::this_thread::get_id()
//         << "] Received! " << buffer << std::endl;
//      global_stream_lock.unlock();
//   }
//}
//
void write_handler(
   const asio::error_code& ec,
   std::size_t bytes_transferred )
{
   std::cout << "[" << std::this_thread::get_id()
      << "] Bytes sent! (" << bytes_transferred << ")"  << std::endl;
}
//
//int main2( int argc, char * argv[] )
//{
//   std::shared_ptr< asio::io_service > io_service(
//      new asio::io_service
//   );
//   std::shared_ptr< asio::io_service::work > work(
//      new asio::io_service::work( *io_service )
//   );
//   asio::io_service::strand strand( *io_service );
//
//   global_stream_lock.lock();
//   std::cout << "[" << std::this_thread::get_id() << "] The program will exit when all  work has finished." << std::endl;
//   global_stream_lock.unlock();
//
//   std::thread hey( &WorkerThread, io_service );
//   std::thread hey2( &WorkerThread, io_service );
//
//
//
//
//   std::shared_ptr< asio::ip::tcp::acceptor > acceptor(
//      new asio::ip::tcp::acceptor( *io_service )
//   );
//   std::shared_ptr< asio::ip::tcp::socket > sock(
//      new asio::ip::tcp::socket( *io_service )
//   );
//
//   try
//   {
//      asio::ip::tcp::resolver resolver( *io_service );
//      asio::ip::tcp::resolver::query query(
//         "127.0.0.1",
//         "8000"
//      );
//      asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
//      acceptor->open( endpoint.protocol() );
//      acceptor->set_option( asio::ip::tcp::acceptor::reuse_address( false ) );
//      acceptor->bind( endpoint );
//      acceptor->listen( asio::socket_base::max_connections );
//      acceptor->async_accept( *sock, std::bind( OnAccept, std::placeholders::_1, sock ) );
//
//      global_stream_lock.lock();
//      std::cout << "Listening on: " << endpoint << std::endl;
//      global_stream_lock.unlock();
//   }
//   catch( std::exception & ex )
//   {
//      global_stream_lock.lock();
//      std::cout << "[" << std::this_thread::get_id()
//         << "] Exception: " << ex.what() << std::endl;
//      global_stream_lock.unlock();
//   }
//
//
//   std::shared_ptr< asio::ip::tcp::socket > sock2(
//      new asio::ip::tcp::socket( *io_service )
//   );
//
//
//   asio::error_code ec;
//   size_t transferred = 0;
//   try
//   {
//
//      asio::ip::tcp::resolver resolver( *io_service );
//      asio::ip::tcp::resolver::query query(
//         "127.0.0.1",
//         "8000"
//      );
//      asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
//      sock2->connect( endpoint, ec );
//
//      global_stream_lock.lock();
//      std::cout << "Connected to: " << endpoint << std::endl;
//      global_stream_lock.unlock();
//      auto buf = asio::const_buffer( "What", 5 );
//      sock2->async_send( buf , write_handler);
//   }
//   catch( std::exception & ex )
//   {
//      global_stream_lock.lock();
//      std::cout << "[" << std::this_thread::get_id()
//         << "] Exception: " << ex.what() << std::endl;
//      global_stream_lock.unlock();
//   }
//
//   std::cin.get();
//
//   asio::error_code ec2;
//   acceptor->close( ec2 );
//
//   sock->shutdown( asio::ip::tcp::socket::shutdown_both, ec2 );
//   sock->close( ec2 );
//
//   hey.join();
//   hey2.join();
//
//   return 0;
//}

//#include "RingBuffer.h"
//
//int main( int argc, char * argv[] )
//{
//   //RingBuffer r(128);
//   //auto s = std::string( 127, 'a' );
//   //r.Push( s.data(), 127 );
//   //auto s1 = std::string( 4, 'b' );
//   //r.Push( s1.data(), 4 );
//
//   //char buf[128] = {0};
//   //r.Read( buf, 128 );
//
//   return 0;
//}
#include "MQTTConnectionFactory.h"
#include "Broker\Broadcaster.h"
using namespace me;
int main()
{
   std::shared_ptr<AsioService> pService( new AsioService( 4 ) );

   std::shared_ptr<ServerIOStream> pIOStream( new ServerIOStream() );
   std::shared_ptr<Broadcaster> pBroadcaster( new Broadcaster( pService ) );
   std::shared_ptr<AsioConnectionFactory> pFactory( new MQTTConnectionFactory( pIOStream, pBroadcaster ) );
   std::shared_ptr<AsioServer> pServer( new AsioServer( pService, pFactory, pIOStream ) );

   pServer->StartListener("127.0.0.1", "8000");

   std::cin.get();

   return 0;
}