#include "fbxExport.h"
#include "fbxReadWriteStream.h"

#include "fmt/ostream.h"
#include <iostream>

namespace fbxNS 
{

FbxExport::FbxExport( FbxManager* manager )
    : m_manager( manager ) 
{
    m_fbxExporter = FbxExporter::Create( m_manager, "" );

    // export settings
    auto ios = FbxIOSettings::Create( m_manager, "IOSRoot" );
    m_manager->SetIOSettings( ios );

    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_MATERIAL, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_TEXTURE, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_EMBEDDED, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_SHAPE, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_GOBO, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_ANIMATION, true );
    m_manager->GetIOSettings()->SetBoolProp( EXP_FBX_GLOBAL_SETTINGS, true );
}

bool FbxExport::writeToFile( const std::string &filepath, FbxScene* scene )
{
    bool status { true };

    // set data format to ascii
    const int format = m_manager->GetIOPluginRegistry()->FindWriterIDByDescription( "FBX ascii (*.fbx)" );

    // initialize the exporter from file path
    status = m_fbxExporter->Initialize( filepath.c_str(), format, m_manager->GetIOSettings() );
    if ( !status ) {
        fmt::print(std::cerr, "%s\n", "Error: FbxExporter::Initialize() failed.");
        fmt::print(std::cerr, "%s\n", m_fbxExporter->GetStatus().GetErrorString());
        m_fbxExporter->Destroy();
        return status;
    }

    m_fbxExporter->Export( scene );
    m_fbxExporter->Destroy();

    return status;
}

bool FbxExport::writeToString( std::string &outString, FbxScene* scene ) 
{
    bool status { true };

    // set data format to ascii
    const int format = m_manager->GetIOPluginRegistry()->FindWriterIDByDescription( "FBX ascii (*.fbx)" );

    // initialize the exporter from stream
    FbxReadWriteStream fbxReadWriteStream( "", m_manager );

    status = m_fbxExporter->Initialize( &fbxReadWriteStream, nullptr, format, m_manager->GetIOSettings() );
    if ( !status ) {
        fmt::print(std::cerr, "%s\n", "Error: FbxExporter::Initialize() failed.");
        fmt::print(std::cerr, "%s\n", m_fbxExporter->GetStatus().GetErrorString());
        m_fbxExporter->Destroy();
        return status;
    }

    m_fbxExporter->Export( scene );
    m_fbxExporter->Destroy();

    // store the data from stream
    outString = fbxReadWriteStream.getData();

    return status;
}

} // namespace fbxNS
