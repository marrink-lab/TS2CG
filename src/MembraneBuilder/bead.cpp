

#include <stdio.h>
#include "bead.h"
//#include "links.h"
bead::bead(int id, std::string name, std::string type, std::string resname, int resid, double x, double y, double z)
{

m_X = x;
m_Y = y;
m_Z = z;
m_ID = id;
    m_BeadName = name;
    m_BeadType = type;
    m_ResName = resname;
    m_Resid = resid;

    m_hasMol =false;
}
bead::bead(int id, std::string name, std::string type, std::string resname, int resid)
{

m_ID=id;
m_BeadName = name;
m_BeadType = type;
m_X=0;
m_Y=0;
m_Z=0;
    m_ResName = resname;
    m_Resid = resid;
    m_hasMol =false;

}

bead::~bead()
{
    
}


void bead::UpdateBox(Vec3D *x)
{
m_pBox=x;
}
void bead::UpdateXPos(double x)
{

		m_X=x;

    
}
void bead::UpdateYPos(double x)
{

		m_Y=x;

}
void bead::UpdateZPos(double x)
{
    
    m_Z=x;

}
void bead::UpdatePos(Vec3D* B, double x, double y, double z)
{
    m_pBox=B;
    this->UpdateXPos(x);
    this->UpdateYPos(y);
    this->UpdateZPos(z);


}
void bead::UpdatePos(double x, double y, double z)
{
    this->UpdateXPos(x);
    this->UpdateYPos(y);
    this->UpdateZPos(z);
    
    
}
void bead::UpdateBeadUnitCell(UnitCell * z)
{
    m_BeadUnitCell = z;
}
void bead::UpdateBeadMol(molecules * z)
{
    m_pBeadMol = z;
}
void bead::UpdateHasMol(bool z)
{
    m_hasMol = z;
}












