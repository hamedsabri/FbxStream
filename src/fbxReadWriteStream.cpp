#include "fbxReadWriteStream.h"

#include  <algorithm>

namespace fbxNS 
{

FbxReadWriteStream::FbxReadWriteStream( const std::string &fbxReadData, FbxManager* manager )
    : m_fbxManager( manager )
    , m_fbxReadData( fbxReadData )
    , m_currentPos( 0 )
    , m_state( FbxStream::EState::eEmpty )
{}

const std::string FbxReadWriteStream::getData() const 
{
    return m_oss.str();
}

FbxStream::EState FbxReadWriteStream::GetState() 
{
    return m_state;
}

bool FbxReadWriteStream::Open( void* pStreamData ) 
{
    m_currentPos = 0;
    m_state = FbxStream::eOpen;
    return true;
}

bool FbxReadWriteStream::Close() 
{
    m_state = FbxStream::eClosed;
    return true;
}

bool FbxReadWriteStream::Flush() 
{
    m_currentPos = 0;
    return true;
}

int FbxReadWriteStream::Write( const void* pData, int pSize ) 
{
    auto begin = static_cast< long >( m_oss.tellp() );

    // write the data chunk
    m_oss.write( static_cast< const char* >( pData ), pSize );

    auto end = static_cast< long >( m_oss.tellp() );

    return end - begin;
}

int FbxReadWriteStream::Read( void* pData, int pSize ) const 
{
    const int totalAvailableBytes = m_fbxReadData.size() - m_currentPos;
    const int numBytesRead = std::min( totalAvailableBytes, pSize );

    if ( numBytesRead > 0 ) {
        // copy the data
        memcpy( pData, ( m_fbxReadData.data() + m_currentPos ), numBytesRead );

        // update the current postion
        m_currentPos += numBytesRead;
    }
    return numBytesRead;
}

int FbxReadWriteStream::GetReaderID() const 
{
    // search for the reader id using the file extensione.
    return m_fbxManager->GetIOPluginRegistry()->FindReaderIDByExtension( "fbx" );
}

int FbxReadWriteStream::GetWriterID() const 
{
    // search for the writer id using the file extension.
    return m_fbxManager->GetIOPluginRegistry()->FindWriterIDByExtension( "fbx" );
}

void FbxReadWriteStream::Seek( const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos ) 
{
    switch ( pSeekPos ) {

    case FbxFile::eBegin:
        m_currentPos = pOffset;
        break;

    case FbxFile::eCurrent:
        m_currentPos += pOffset;
        break;

    case FbxFile::eEnd:
        m_currentPos = m_fbxReadData.size() - static_cast< long >( pOffset );
        break;
    }
}

long FbxReadWriteStream::GetPosition() const 
{
    return m_currentPos;
}

void FbxReadWriteStream::SetPosition( long pPosition ) 
{
    m_currentPos = pPosition;
}

int FbxReadWriteStream::GetError() const 
{
    return m_oss.good() ? 0 : 1;
}

void FbxReadWriteStream::ClearError() 
{
    m_oss.clear();
}

} // namespace fbxNS
