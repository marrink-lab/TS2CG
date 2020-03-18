

#include <stdio.h>
#include "IndexGroup.h"
IndexGroup::IndexGroup(std::string groupname)
{
    m_GroupName = groupname;
}
IndexGroup::~IndexGroup()
{
    
}
void IndexGroup::AddBeadToGroup(int i)
{
    
    
    m_pAllIndex.push_back(i);
}















