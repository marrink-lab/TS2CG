#if !defined(AFX_IndexGroup_H_334B21B8_C13C_5648_BF23_124095086235__INCLUDED_)
#define AFX_IndexGroup_H_334B21B8_C13C_5648_BF23_124095086235__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "UnitCell.h"


class IndexGroup
{
public:
    
    IndexGroup(std::string groupname);
    ~IndexGroup();
    
    
    inline std::vector<int> GetBeadsIndex()                const  {return m_pAllIndex;}
    inline std::string GetGroupName()                const  {return m_GroupName;}
    
    
    
    
public:
    
    void AddBeadToGroup(int );
    
    
    
public:
    
    
private:
    std::vector<int> m_pAllIndex;
    std::string m_GroupName;
    
    
};

#endif

