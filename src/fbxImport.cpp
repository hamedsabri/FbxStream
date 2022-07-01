#include "fbxImport.h"
#include "fbxReadWriteStream.h"

#include "fmt/ostream.h"
#include <iostream>

namespace fbxNS 
{

FbxImport::FbxImport( FbxManager* manager )
    : m_manager( manager ) 
{
    m_fbxImporter = FbxImporter::Create( m_manager, "" );

    auto ios = FbxIOSettings::Create( m_manager, "IOSRoot" );
    m_manager->SetIOSettings( ios );

    // import settings
    if ( m_fbxImporter->IsFBX() ) {
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_MATERIAL, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_TEXTURE, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_LINK, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_SHAPE, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_GOBO, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_ANIMATION, true );
        m_manager->GetIOSettings()->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS, true );
    }
}

bool FbxImport::readFromFile( const std::string &filepath, FbxScene* scene ) 
{
    bool status { true };

    // initialize the the importer from input file path
    status = m_fbxImporter->Initialize( filepath.c_str(), -1, m_manager->GetIOSettings() );
    if ( !status ) {
        fmt::print(std::cerr, "Error returned: %s\n", m_fbxImporter->GetStatus().GetErrorString());
        m_fbxImporter->Destroy();
        return status;
    }

    // import the scene
    status = m_fbxImporter->Import( scene );
    if ( !status ) {
        fmt::print(std::cerr, "Error loading FBX file: %s\n", m_fbxImporter->GetStatus().GetErrorString());
        m_fbxImporter->Destroy();
        return status;
    }

    m_fbxImporter->Destroy();

    return status;
}

bool FbxImport::readFromString( const std::string &rawData, FbxScene* scene ) 
{
    bool status{ true };

    // initialize the the importer from stream
    FbxReadWriteStream fbxReadWriteStream( rawData, m_manager );

    status = m_fbxImporter->Initialize( &fbxReadWriteStream, nullptr, -1, m_manager->GetIOSettings() );
    if ( !status ) {
        fmt::print(std::cerr, "Error returned: %s\n", m_fbxImporter->GetStatus().GetErrorString());        
        m_fbxImporter->Destroy();
        return status;
    }

    // import the scene
    status = m_fbxImporter->Import( scene );
    if ( !status ) {
        fmt::print(std::cerr, "Error loading FBX file: %s\n", m_fbxImporter->GetStatus().GetErrorString());        
        m_fbxImporter->Destroy();
        return status;
    }

    m_fbxImporter->Destroy();

    return status;
}

const FileVersions FbxImport::getFileVersions() const {
    int fbxMajor;
    int fbxMinor;
    int fbxRevision;

    m_fbxImporter->GetFileVersion( fbxMajor, fbxMinor, fbxRevision );

    return {
               fbxMajor,
               fbxMinor,
               fbxRevision
    };
}

} // namespace fbxNS
