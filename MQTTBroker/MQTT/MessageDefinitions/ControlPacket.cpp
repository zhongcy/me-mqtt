#include "stdafx.h"
#include "ControlPacket.h"
#include "MalformedPacket.h"

namespace me
{
ControlPacket::ControlPacket( unsigned char aPacketType, unsigned char aiReserved = 0x00 )
{
   // Constructor of derived class SHOULD set length
   setType( aPacketType );

   if( aiReserved > 0xF )
   {
      throw MalformedPacket();
   }
   m_iReserved = aiReserved;
}


ControlPacket::~ControlPacket()
{
}



ControlPacket::PacketTypes
ControlPacket::GetType() const
{
   return m_iPacketType;
}

std::string
ControlPacket::Serialize() const
{
   std::string szRetval;
   szRetval.append( 1, m_iPacketType << 4 | getFixedHeaderReserved() );

   std::string szBody = SerializeBody();
   SerializeLength( szRetval, szBody.size() );
   szRetval.append( szBody );
   return szRetval;
}

void
ControlPacket::setType( unsigned char aiType )
{
   if( aiType == 0 || aiType > 14 )
   {
      throw MalformedPacket();
   }
   m_iPacketType = (PacketTypes)aiType;
}

unsigned char
ControlPacket::getFixedHeaderReserved() const
{
   return m_iReserved;
}

void
ControlPacket::SerializeLength( std::string& aszBuf, size_t aiLen ) const
{
   size_t iSize = aiLen;
   do
   {
      unsigned char byte = iSize % 0x80;
      iSize = iSize / 0x80;

      if( iSize > 0 )
      {
         byte = byte | 0x80;
      }
      aszBuf.append( 1, byte );
   } while( iSize > 0 );
}

}