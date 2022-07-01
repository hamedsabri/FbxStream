#include "common.h"

#include <fmt/core.h>

namespace fbxNS {

bool initialize(FbxManager*& pManager, FbxScene*& pScene)
{
    pManager = FbxManager::Create();
    if( !pManager ) {
        fmt::print("Error: Unable to create FBX Manager!\n");
        return false;
    }
    else {
        fmt::print("Autodesk FBX SDK version {}\n", pManager->GetVersion());
    }

    FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(ios);

    // create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "Empty Scene");
    if( !pScene ) {
        fmt::print("Error: Unable to create FBX scene!\n");
        return false;
    }

    return true;
}

void destroy(FbxManager* pManager)
{
    if( pManager ) {
        pManager->Destroy();
        fmt::print("FBX Sdk destroyed successfully!\n");
    }
}

bool loadScene(FbxManager* pManager, FbxDocument* pScene, const std::string& pFilename)
{
    auto fileMajor{-1}, fileMinor{-1}, fileRevision{-1};
    bool status = false;

    // create an importer
    FbxImporter* importer = FbxImporter::Create(pManager, "");

    // initialize the importer
    status = importer->Initialize(pFilename.data(), -1, pManager->GetIOSettings());
    if (!status) {
        return false;
    }

    if (importer->IsFBX()) {
        importer->GetFileVersion(fileMajor, fileMinor, fileRevision);
        fmt::print("FBX file format version for file {} is {}.{}.{}\n", pFilename.data(), fileMajor, fileMinor, fileRevision);
    }

    // import the contents of the file into the scene.
    status = importer->Import(pScene);
    
    if (status) {
        // check the scene integrity
        FbxStatus stat;
        FbxArray< FbxString*> details;
        FbxSceneCheckUtility sceneCheck(FbxCast<FbxScene>(pScene), &stat, &details);
        status = sceneCheck.Validate(FbxSceneCheckUtility::eCkeckData);
    }

    // destroy the importer
    importer->Destroy();

    return status;
}

} // namespace fbxNS
