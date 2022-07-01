// Copyright (C) 2022 Hamed Sabri
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <fbxsdk.h>

#include <sstream>
#include <string>

namespace fbxNS 
{

// FbxReadWriteStream class to access FBX data directly from stream.
class FbxReadWriteStream final : public FbxStream 
{

public:
    FbxReadWriteStream( const std::string &fbxReadData, FbxManager* manager );

    virtual ~FbxReadWriteStream() = default;

    /// Return the written data from ostringstream
    const std::string getData() const;

    /// Query the current state of the stream.
    FbxStream::EState GetState() override;

    /// Open the stream.
    /// return True if successful.
    /// remark Each time the stream is open or closed, the stream position must be reset to zero.
    bool Open( void* pStreamData ) override;

    /// Close the stream.
    /// return True if successful.
    /// remark Each time the stream is open or closed, the stream position must be reset to zero.
    bool Close() override;

    /// Empties the internal data of the stream.
    /// return True if successful.
    bool Flush() override;

    /// Writes a memory block.
    /// param pData Pointer to the memory block to write.
    /// param pSize Size (in bytes) of the memory block to write.
    /// return The number of bytes written in the stream.
    int Write( const void* pData, int pSize ) override;

    /// Read bytes from the stream and store them in the memory block.
    /// param pData Pointer to the memory block where the read bytes are stored.
    /// param pSize Number of bytes read from the stream.
    /// return The actual number of bytes successfully read from the stream.
    int Read( void* pData, int pSize ) const override;

    /// If not specified by KFbxImporter::Initialize(), the importer will ask
    /// the stream to select an appropriate reader ID to associate with the stream.
    /// FbxIOPluginRegistry can be used to locate id by extension or description.
    /// Return -1 to allow FBX to select an appropriate default.
    int GetReaderID() const override;

    /// If not specified by KFbxExporter::Initialize(), the exporter will ask
    /// the stream to select an appropriate writer ID to associate with the stream.
    /// KFbxIOPluginRegistry can be used to locate id by extension or description.
    /// Return -1 to allow FBX to select an appropriate default.
    int GetWriterID() const override;

    /// Adjust the current stream position.
    /// param pSeekPos Pre-defined position where offset is added (FbxFile::eBegin, FbxFile::eCurrent:, FbxFile::eEnd)
    /// param pOffset Number of bytes to offset from pSeekPos.
    void Seek( const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos ) override;

    /// Get the current stream position.
    /// return Current number of bytes from the beginning of the stream.
    long GetPosition() const override;

    /// Set the current stream position.
    /// param pPosition Number of bytes from the beginning of the stream to seek to.
    void SetPosition( long pPosition ) override;

    /// Return 0 if no errors occurred. Otherwise, return 1 to indicate
    /// an error. This method will be invoked whenever FBX needs to verify
    /// that the last operation succeeded.
    int GetError() const override;

    /// Clear current error condition by setting the current error value to 0.
    void ClearError()  override;

private:

    FbxManager* m_fbxManager;

    // fbx read data
    std::string m_fbxReadData;

    // current stream position
    mutable long m_currentPos;

    // current state of the stream
    FbxStream::EState m_state;

    // fbx written data
    std::ostringstream m_oss;
};

} // namespace fbxNS
