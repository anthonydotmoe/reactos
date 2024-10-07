#pragma once

#include "precomp.h"

#include <string>
#include <vector>
#include "mmc.h"

struct SnapinInfo
{
    std::wstring name;
    std::wstring clsid;
    
    // ISnapinAbout members
    BOOL hasAbout;
    CLSID aboutclsid;
    std::wstring provider;
    std::wstring description;
    
    // HICON Main icon for "About" property page
    //CHandle hiAppIcon;
    
    // Index into m_ilSmNodes for Small icon (16x16 pixels)
    int iSmallImage;
    
    // Index into m_ilSmNodes for Small open-folder icon (16x16 px)
    int iSmallImageOpen;
    
    // Index into m_ilLgNodes for Large Icon (32x32 px)
    int iLargeImage;
};

class NodeManager
{
public:
    NodeManager();
    ~NodeManager();
    
    std::vector<SnapinInfo> m_colStandalone;
    std::vector<SnapinInfo> m_colExtensions;

    // Small Image List for Node icons
    HIMAGELIST m_ilSmNodes;

    // Large Image List for Node icons
    HIMAGELIST m_ilLgNodes;

private:
    void InitializeImageLists();
    void UpdateSnapinInfo(std::vector<SnapinInfo> &snapins);

};