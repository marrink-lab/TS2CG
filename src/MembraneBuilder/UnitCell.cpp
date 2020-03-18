

#include <stdio.h>
#include "UnitCell.h"
//#include "links.h"
UnitCell::UnitCell(int id, int i, int j, int k)
{

m_Index.push_back(i);
m_Index.push_back(j);
m_Index.push_back(k);
m_ID=id;


}


UnitCell::~UnitCell()
{
    
}






void UnitCell::AddtoNeighbourUnitCell(UnitCell* z)
{

m_VNeighbourUnitCell.push_back(z);
}


void UnitCell::AddtoVertexList(bead * z)
{

m_BeadList.push_back(z);
}



void UnitCell::RemoveFromVertexList(bead * z)
{




m_BeadList.erase(std::remove(m_BeadList.begin(), m_BeadList.end(), z), m_BeadList.end());




}

UnitCell * UnitCell::ReturnNewCell(int i,int j,int k)
{
    
    int n=0;
    n=(i+1)+3*(j+1)+9*(k+1);
    if(n>13)
    {
        n=n-1;
    }
    UnitCell * P=m_VNeighbourUnitCell.at(n);
    
    return P;
    //m_VertexList.push_back(z);
    
}



