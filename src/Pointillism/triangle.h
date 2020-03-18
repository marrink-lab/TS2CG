#if !defined(AFX_triangle_H_6Q4B21B8_C13C_5648_BF23_124095086233__INCLUDED_)
#define AFX_triangle_H_6Q4B21B8_C13C_5648_BF23_124095086233__INCLUDED_

#include "SimDef.h"
#include "vertex.h"
#include "Vec3D.h"


class triangle
{
public:
    
	triangle(int id, vertex *v1, vertex *v2, vertex *v3);
	triangle(int id);
	 ~triangle();




        inline const int GetTriID()                const  {return m_ID;}


        inline vertex *GetV1()                  	{return m_V1;}
        inline vertex *GetV2()                  	{return m_V2;}
        inline vertex *GetV3()                  	{return m_V3;}
        inline bool GetRepresentation()             {return m_Representation;}
        inline double GetArea()                  	{return m_Area;}
    	inline Vec3D GetAreaVector()                {return m_AreaVector;}
        inline Vec3D GetNormalVector()              {return m_Normal;}
#if BACKMAP == Enabled
        inline bool GetGotMashed()              {return m_GotMashed;}
#endif


    


public:
    
void UpdateRepresentation(bool); 	/// this is for visulaization output and does not effect the simulation
void UpdateNormal_Area(Vec3D *Box);
void UpdateVertex(vertex *v1,vertex *v2,vertex *v3);
void ReadTriangleFromFile(std::ifstream *inputfile,std::vector <vertex *> pv);
    void WriteTriangleToFile(std::ofstream *inputfile);
#if BACKMAP == Enabled
    void UpdateGotMashed(bool mash);
#endif
private:



private:

vertex *m_V1;
vertex *m_V2;
vertex *m_V3;
  int m_ID;
  bool m_Representation;
  Vec3D m_Normal;
  Vec3D m_AreaVector;
  double m_Area;
    
#if BACKMAP == Enabled
    bool m_GotMashed;
#endif

    





};


#endif
