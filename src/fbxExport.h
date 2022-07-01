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

#include <string>

namespace fbxNS 
{

// FbxExport class to export the Autodesk FBX file format.
class FbxExport final 
{
public:
    /// constructor
    /// @param manager: Fbx manager to use.
    FbxExport( FbxManager* manager );

    /// destructor
    ~FbxExport() = default;

    /// delete the copy/move constructors assignment operators.
    FbxExport( const FbxExport & ) = delete;
    FbxExport &operator=( const FbxExport & ) = delete;
    FbxExport( FbxExport && ) = delete;
    FbxExport &operator=( FbxExport && ) = delete;

    /// write a fbx scene to a file path ( ascii format )
    /// @param filepath: The file path to write to.
    /// @param scene: Fbx scene to export.
    /// @returns: true if write was successful.
    bool writeToFile( const std::string &filepath, FbxScene* scene );

    /// Write a fbx scene to a string ( ascii format )
    /// @param[out] outString: The string to write to.
    /// @param scene: Fbx scene to export to.
    /// @returns: true if write was successful.
    bool writeToString( std::string &outString, FbxScene* scene );

private:
    FbxManager* m_manager;

    FbxExporter* m_fbxExporter;
};

} // namespace fbxNS
