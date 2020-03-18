#if !defined(AFX_Topology_H_7F4B21B8_D13C_9321_QF23_124095086234__INCLUDED_)
#define AFX_Topology_H_7F4B21B8_D13C_9321_QF23_124095086234__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"

class Topology
{
public:
    
    Topology(Vec3D *Box ,double * pminangle, double * pLmin , double * pLmax);
    ~Topology();
    
    
    //inline std::vector<vertex> GetVertex()                        {return m_Vertex;}
    //inline std::vector<triangle> GetTriangle()                       {return m_Triangle;}
    inline std::vector<vertex*> GetVertex()                        {return m_pAllV;}
    inline std::vector<triangle*> GetTriangle()                       {return m_pAllT;}
    inline std::vector<links*> GetLinks()                       {return m_pLinks;}
    inline std::vector<links*> GetHalfLinks()                       {return m_pHL;}
    inline std::vector<links*> GetMHalfLinks()                       {return m_pMHL;}
    inline const  bool GetTopologyHealth()                       {return m_TopologyHealth;}
    
public:
    
    void Write(std::vector<vertex* > ver, std::vector<triangle* > triangle,  std::string Filename);
    void FastReadQFile(std::string input );

private:
    
    std::vector<vertex> m_Vertex;
    std::vector<triangle> m_Triangle;
    std::vector<links> m_Links;
private:
    
    std::vector<vertex*>      m_pAllV;
    std::vector<triangle*>    m_pAllT;
    std::vector<links*>       m_pLinks;
    std::vector<links*>       m_pHL;
    std::vector<links*>       m_pMHL;
    
    Vec3D m_Box;
    Vec3D *m_pBox;
    
    
    double *m_pLmin2;
    double *m_pLmax2;
    double *m_pminAngle;
    bool m_TopologyHealth;
private:
    double LengthBetweenTwoVertex(vertex* v1, vertex* v2);
    bool   CheckFaceAngle(links * l);
    Vec3D   CalculateNormal(vertex*,vertex*,vertex*);
    
};


#endif
