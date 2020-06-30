

#include <stdio.h>
#include "point.h"
//#include "links.h"
point::point(int id,double area, Vec3D x,Vec3D n, Vec3D p1, Vec3D p2, std::vector <double> c)
{

    m_Area = area;
    m_Pos = x;
    m_Normal = n;
    m_P1 = p1;
    m_P2 = p2;
    m_C = c;
    m_ID = id;
    m_UpperLayer = true;
}


point::~point()
{
    
}


void point::UpdateBox(Vec3D *x)
{
m_pBox=x;
}
void point::UpdatePos(Vec3D x)
{
    m_Pos = x;
    
}
void point::UpdateArea(double x)
{
    m_Area = x;
    
}
void point::UpdatepointUnitCell(UnitCell * z)
{
    m_PointUnitCell = z;
}

void point::UpdateInclusion(inclusion *pInc)
{
    m_HasInc = true;
    m_pInc = pInc;
}
void point::UpdateUpperLayer(bool x)
{
    m_UpperLayer=x;
}












