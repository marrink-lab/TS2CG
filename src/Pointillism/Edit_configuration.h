#if !defined(AFX_Edit_configuration_H_8F4B21B8_D13C_9321_TT23_124095086224__INCLUDED_)
#define AFX_Edit_configuration_H_8F4B21B8_D13C_9321_TT23_124095086224__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Argument.h"

class Edit_configuration
{
public:
    
	Edit_configuration( Argument *Argument);
	 ~Edit_configuration();


      //  inline std::vector<vertex> GetVertex()                        {return m_Vertex;}
      //  inline std::vector<triangle> GetTriangle()                       {return m_Triangle;}

public:
int Find_i(int m,int n);

private:

Vec3D *m_pBox;
    std::vector<vertex*>      m_pAllV;
    std::vector<vertex>       m_AllV;
    std::vector<triangle*>    m_pAllT;
    std::vector<links*>       m_pAllLinks;
    std::vector<links*>       m_pHalfLinks1;
    std::vector<links*>       m_pHalfLinks2;
    std::vector<inclusion*>   m_pInc;
    int  m_Iteration;
    double m_Zoom ;
    std::string m_Folder ;
    double m_AP ;
    std::string m_Shape;
    bool m_FindnewBox;
   int m_Nx;
   int m_Ny;
    double m_minRoughness;
    bool m_smooth;
    int m_monolayer;
    double  PPBCM_Cluster(double , std::vector <double>);
    void Rescaling(double zoom );
    std::string m_MosAlType;
    void UpdateGeometry( );
    bool FileExist (const std::string& name);
    void BackMapOneLayer(int layer , std::string file, double);
    void MakeFlatMonolayer(int layer , std::string file, double);
    bool check(std::string file);
    void Minimize(std::string file);

};


#endif
