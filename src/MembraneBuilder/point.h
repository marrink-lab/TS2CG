#if !defined(AFX_point_H_999B21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_point_H_999B21B8_C13C_5648_BF23_124095086234__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "UnitCell.h"
#include "inclusion.h"

class point
{
public:
    
	point(int id, double area, Vec3D x,Vec3D n, Vec3D p1, Vec3D p2, std::vector <double> c );
    ~point();


        inline const int GetID()                const  {return m_ID;}
        inline Vec3D  GetPos()                        {return m_Pos;}
        inline double  GetArea()                        {return m_Area;}
        inline Vec3D  GetNormal()                        {return m_Normal;}
        inline Vec3D  GetP1()                        {return m_P1;}
        inline Vec3D  GetP2()                        {return m_P2;}
        inline std::vector <double>  GetCurvature()                        {return m_C;}
        inline UnitCell *GetpointUnitCell()        const        {return m_PointUnitCell;}
        inline inclusion *GetInclusion()        const        {return m_pInc;}
        inline bool Hasinc()        const        {return m_HasInc;}



public:
    

  void UpdateBox(Vec3D* z);
  void UpdatePos( Vec3D X);
  void UpdateInclusion(inclusion *pInc);
  void UpdatepointUnitCell(UnitCell * z);
  void UpdateArea(double);

public:


private:

    double m_Area;
    Vec3D m_Pos;
    Vec3D m_Normal;
    Vec3D m_P1;
    Vec3D m_P2;
    std::vector <double> m_C;
    UnitCell *m_PointUnitCell;
    Vec3D *m_pBox;
    int m_ID;
    inclusion *m_pInc;
    bool m_HasInc;
};


#endif
