#if !defined(AFX_Volume_H_224B21B8_C13C_2248_BF23_124095086233__INCLUDED_)
#define AFX_Volume_H_224B21B8_C13C_2248_BF23_124095086233__INCLUDED_
//================ version 1.2
/* this class is created in version 1.2 to couple the system to a force
for taraget volume.
*/
#include "SimDef.h"
#include "vertex.h"
#include "triangle.h"
#include "links.h"
class Volume
{
public:
    
    Volume(std::string State,int eqtime, double VolK, double VolumePercent);
    ~Volume();


       inline double GetTotalVolume()                  {return m_TotalVolume;}
       inline double GetKV()                           {return m_KV;}
       inline std::string GetState()                   {return m_State;}
       inline double GetReferenceVolume()                   {return m_ReferenceVoulme;}
    


public:
    
    void UpdateTotalVolume(double);
    void UpdateKV(double);
    void UpdateState(std::string);
    void UpdateReferenceVolume(double);
    
    

    double VolumeofTrianglesAroundVertex(vertex * pVeretx);   ///
    double SingleTriangleVolume(triangle * ptriangle);   ///
    double TotalVolumeFromTriangels(std::vector<triangle *> pTriangle);   ///
    double GetTargetVolume(int step);
    
    //=====
    
    
    
private:
    double m_TotalVolume;
    int m_NoEQStep;
    double m_KV;
    std::string m_State;
    double m_ReferenceVoulme;
    double m_VolumePercent;



    





};


#endif
