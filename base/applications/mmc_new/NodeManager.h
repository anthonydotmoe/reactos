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
};

class NodeManager
{
public:
    NodeManager();

private:
    std::vector<SnapinInfo> m_colStandalone;
    std::vector<SnapinInfo> m_colExtensions;
};