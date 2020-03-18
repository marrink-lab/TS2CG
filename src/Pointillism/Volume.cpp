


#include "Volume.h"

Volume::Volume(std::string State,int eqtime, double VolK, double VolumePercent)
{
    
    m_KV = VolK;
    m_TotalVolume = 0.0;
    m_NoEQStep = eqtime;
    m_State = State;
    m_VolumePercent = VolumePercent;
    m_ReferenceVoulme = 0;
}

Volume::~Volume()
{
    
}


double Volume::VolumeofTrianglesAroundVertex(vertex *pv)
{
    double vol=0;
    
    
    std::vector <triangle *> pvT=pv->GetVTraingleList();

    
    for (std::vector<triangle *>::iterator it = pvT.begin() ; it != pvT.end(); ++it)
    {
        
        vol+=SingleTriangleVolume(*it);
        
    }
    
    return vol;
}
//==========================================================
double Volume::TotalVolumeFromTriangels(std::vector<triangle *> pTriangle)
{
    double V=0.0;
    
    for (std::vector<triangle *>::iterator it = pTriangle.begin() ; it != pTriangle.end(); ++it)
    {
        
        V+=SingleTriangleVolume(*it);
        
    }
    
    return V;
    
}
double Volume::SingleTriangleVolume(triangle *pt)
{
    double vol=0;
    
    vertex* pv= pt->GetV1();
    double area= pt->GetArea();
    Vec3D Norm=pt->GetNormalVector();
    
    Vec3D R (pv->GetVXPos(),pv->GetVYPos(),pv->GetVZPos());
    
    
    
    vol=area*(R.dot(Norm,R))/3.0;
    
    
    return vol;
}

void Volume::UpdateTotalVolume(double x)
{
    
    m_TotalVolume = x;
}
void Volume::UpdateKV(double x)
{
    m_KV = x;
}
void Volume::UpdateState(std::string x)
{
    m_State = x;
}
double Volume::GetTargetVolume(int step)
{
    double tvol=0;
    
    double alpha=(double(step)/double(m_NoEQStep));
    
   if(step<m_NoEQStep)
   tvol= alpha*m_VolumePercent*m_ReferenceVoulme+(1-alpha)*m_ReferenceVoulme;
   else
    tvol= m_VolumePercent*m_ReferenceVoulme;
    
    return tvol;
}
void Volume::UpdateReferenceVolume(double x)
{
    m_ReferenceVoulme = x;
}
