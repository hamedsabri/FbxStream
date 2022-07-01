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

#include "fbxImport.h"
#include "fbxExport.h"
#include "common.h"

#include <fmt/core.h>

#include <assert.h>

namespace 
{
    void usage(const std::string& program) 
    {
        fmt::print("Usage: {} <input.fbx>\n", program);
    }

    bool hasFbxExtension(const std::string& inputFilePath)
    {
        auto len = inputFilePath.length();
        auto pos = inputFilePath.rfind('.', len);

        if (pos != std::string::npos) {
            auto ext = inputFilePath.substr(pos + 1 , len - 1);
            if (ext == "fbx" || ext == "FBX") {
                return true;
            } else {
                return false;
            }
        }

        return false;
    }

} // namespace

int main(int argc, char** argv)
{
    const std::string& program = argv[0];
    if (argv[1] == nullptr) {
        usage(program);
        fmt::print("Error: no input file provided!\n");
        return 1;
    }

    const std::string& inputFilePath = argv[1];
    if (!hasFbxExtension(inputFilePath)) {
        fmt::print("Error: no valid fbx file format provided!\n");
        return 1;
    }

    FbxManager* fbxManager {nullptr};
    FbxScene* fbxScene {nullptr};
    bool status {false};

    // initialize the FBX SDK.
    status = fbxNS::initialize(fbxManager, fbxScene);
    if (!status) {
        fmt::print("An error occurred initializing the fbx sdk...\n");
        fbxNS::destroy(fbxManager);
        return 1;
    }

    // fbx data in string format
    std::string fbxDataOld;

    // read fbx from file and write it to a string
    {
        fbxNS::FbxImport importer(fbxManager);
        importer.readFromFile(inputFilePath, fbxScene);
        assert(fbxScene->GetRootNode());

        fbxNS::FbxExport exporter(fbxManager);
        exporter.writeToString(fbxDataOld, fbxScene);
        assert(fbxDataOld.length() > 0);
    }

    // fbx data in string format
    std::string fbxDataNew;

    // read back from memory and write it again to a new string
    {   
        fbxNS::FbxImport importer(fbxManager);
        importer.readFromString(fbxDataOld, fbxScene);
        assert(fbxScene->GetRootNode());

        fbxNS::FbxExport exporter(fbxManager);
        exporter.writeToString(fbxDataNew, fbxScene);
        assert(fbxDataNew.length() > 0);
    }

    if (fbxDataNew.length() == fbxDataOld.length()) {
        fmt::print("FbxStream read/write passed successfully!\n");
    } else {
        fmt::print("FbxStream read/write failed!\n");        
    }

    fbxNS::destroy(fbxManager);

    return 0;
}
