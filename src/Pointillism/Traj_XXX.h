#if !defined(AFX_Traj_XXX_H_7F4B21B8_D13C_9321_QF23_885095086234__INCLUDED_)
#define AFX_Traj_XXX_H_7F4B21B8_D13C_9321_QF23_885095086234__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"
#include "inclusion.h"
#include "Nfunction.h"

class Traj_XXX
{
public:
    
    Traj_XXX(Vec3D *pBox);
     ~Traj_XXX();



        inline std::vector<vertex*> GetVertex()                        {return m_pAllV;}
        inline std::vector<triangle*> GetTriangle()                       {return m_pAllT;}
        inline std::vector<links*> GetLinks()                       {return m_pLinks;}
        inline std::vector<links*> GetHalfLinks()                       {return m_pHL;}
        inline std::vector<links*> GetMHalfLinks()                       {return m_pMHL;}
        inline std::vector<inclusion*> GetInclusion()                       {return m_pInclusion;}
        inline int GetStep()                     			  {return m_Step;}
        inline bool GetCondition()                     			  {return m_Condition;}

public:

void WriteTSI(int step ,  std::string filename, std::vector< vertex* > pver, std::vector< triangle* > ptriangle,   std::vector< inclusion* > pinc);
void ReadTSI(std::string filename);


private:

    std::vector<vertex> m_Vertex;
    std::vector<triangle> m_Triangle;
    std::vector<links> m_Links;
    std::vector<inclusion>  m_Inclusion;
    std::vector<vertex*>      m_pAllV;
    std::vector<triangle*>    m_pAllT;
    std::vector<links*>       m_pLinks;
    std::vector<links*>       m_pHL;
    std::vector<links*>       m_pMHL;
    std::vector<inclusion*>   m_pInclusion;
    bool m_Condition;
    Vec3D *m_pBox;
    std::string m_filename;
    int m_Step;
    std::string m_tsiFolder_name;
    std::string m_tsiPrecision;



};


#endif
