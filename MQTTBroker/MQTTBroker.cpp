// MQTTBroker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "asio.hpp"
#include <memory>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex global_stream_lock;

void WorkerThread( std::shared_ptr<asio::io_service> io_service )
{
   global_stream_lock.lock();
   std::cout << "[" << std::this_thread::get_id() << "] Thread Start" << std::endl;
   global_stream_lock.unlock();

   io_service->run();

   global_stream_lock.lock();
   std::cout << "[" << std::this_thread::get_id()
      << "] Thread Finish" << std::endl;
   global_stream_lock.unlock();
}

void PrintNum( int x )
{
   global_stream_lock.lock();
   std::cout << "[" << std::this_thread::get_id() << "] x: " << x << std::endl;
   global_stream_lock.unlock();
}

int main( int argc, char * argv[] )
{
   std::shared_ptr< asio::io_service > io_service(
      new asio::io_service
   );
   std::shared_ptr< asio::io_service::work > work(
      new asio::io_service::work( *io_service )
   );
   asio::io_service::strand strand( *io_service );

   global_stream_lock.lock();
   std::cout << "[" << std::this_thread::get_id() << "] The program will exit when all  work has finished." << std::endl;
   global_stream_lock.unlock();

   std::thread hey( &WorkerThread, io_service );
   std::thread hey2( &WorkerThread, io_service );

   io_service->post( strand.wrap( std::bind( &PrintNum, 5 ) ) );

   hey.join();
   hey2.join();

   return 0;
}


