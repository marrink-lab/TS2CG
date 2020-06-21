#if !defined(AFX_MakePBCTS_H_4F4B21B8_D13C_9321_SF23_124095086234__INCLUDED_)
#define AFX_MakePBCTS_H_4F4B21B8_D13C_9321_SF23_124095086234__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"

class MakePBCTS
{
public:
    
    MakePBCTS(std::string file);
    ~MakePBCTS();
    
    

//    inline const  bool GetMakePBCTSHealth()                       {return m_MakePBCTSHealth;}
    
public:
    
    void Write(std::vector<vertex* > ver, std::vector<triangle* > triangle,  std::string Filename);
  //  void FastReadQFile(std::string input );

private:
    bool m_Health;
    std::vector<vertex> m_Vertex;
    std::vector<triangle> m_Triangle;
    std::vector<links> m_Links;
private:
    
    std::vector<vertex*>      m_pAllV;
    std::vector<triangle*>    m_pAllT;
    std::vector<links*>       m_pLinks;
    std::vector<links*>       m_pEdgeLink;
    std::vector<links*>       m_pOrderEdgeLink;

    std::vector<links*>       m_pHL;
    std::vector<links*>       m_pMHL;
    
    Vec3D m_Box;
    Vec3D *m_pBox;

    

private:
    double LengthBetweenTwoVertex(vertex* v1, vertex* v2);
    Vec3D   LinkVec(links* l);
    bool    CheckFaceAngle(links * l);
    Vec3D   CalculateNormal(vertex*,vertex*,vertex*);
    
};


#endif
