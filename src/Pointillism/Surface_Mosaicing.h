#if !defined(AFX_Surface_Mosaicing_H_7B4B21B8_D13C_9321_BF23_124095086234__INCLUDED_)
#define AFX_Surface_Mosaicing_H_7B4B21B8_D13C_9321_BF23_124095086234__INCLUDED_

#include "SimDef.h"
#include "vertex.h"
#include "links.h"
#include "triangle.h"
#include "Vec3D.h"
#include "inclusion.h"

class Surface_Mosaicing
{
public:
    Surface_Mosaicing(std::string altype, bool smooth);
    Surface_Mosaicing();
	 ~Surface_Mosaicing();




public:

    inline std::vector <links *> GetLinks()             {return m_pFL;}
    inline std::vector <links *> GetHLinks()             {return m_pTMLH1;}
    inline std::vector <triangle *> GetTriangles()         {return m_pFT;}
    inline std::vector <vertex *> GetVertexs()     {return m_pFV;}

    
public:
    void PerformMosaicing(Vec3D * Box, std::vector<vertex* > verm , std::vector<triangle* > ptri, std::vector<links* > plink, std::vector<inclusion* > inc, int iteration);
    void RoughnessOfALink(links *l, double *linklength, double *midpointdistance);
private:

Vec3D m_Box;
Vec3D *m_pBox;
std::vector<vertex* > m_pVers;
std::vector<triangle* > m_pTri;
std::vector<links* >  m_pLinks;
    std::string m_AlgorithmType;
    
    
private:
    void UpdateGeometry();
    void MosaicOneRound();
    void BestEstimateOfMidPointPossition(links *, double *x, double *y,double *z);
    Tensor2 NormalCoord(Vec3D N);

private:
    std::vector<inclusion* > m_Inc;
    std::vector<vertex > m_TMV;
    std::vector<triangle > m_TMT;
    std::vector<links >  m_TML;
    std::vector<links *>  m_pTMLH2;
    std::vector<links *>  m_pTMLH1;

    std::vector<vertex > m_FV;
    std::vector<triangle > m_FT;
    std::vector<links >  m_FL;

    std::vector<vertex* > m_pFV;
    std::vector<triangle* > m_pFT;
    std::vector<links* >  m_pFL;
    bool m_smooth;
};


#endif
