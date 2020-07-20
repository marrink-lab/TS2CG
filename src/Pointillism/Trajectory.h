#if !defined(AFX_Trajectory_H_7F4B21B8_D13C_9321_QF23_445095086234__INCLUDED_)
#define AFX_Trajectory_H_7F4B21B8_D13C_9321_QF23_445095086234__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"
#include "inclusion.h"
#include "Nfunction.h"

class Trajectory
{
public:
    
	Trajectory(Vec3D *  );
     ~Trajectory();


        //inline std::vector<vertex> GetVertex()                        {return m_Vertex;}
        //inline std::vector<triangle> GetTriangle()                       {return m_Triangle;}
        inline std::vector<vertex*> GetVertex()                        {return m_pAllV;}
        inline std::vector<triangle*> GetTriangle()                       {return m_pAllT;}
        inline std::vector<links*> GetLinks()                       {return m_pLinks;}
        inline std::vector<links*> GetHalfLinks()                       {return m_pHL;}
        inline std::vector<links*> GetMHalfLinks()                       {return m_pMHL;}
        inline std::vector<inclusion*> GetInclusion()                       {return m_pInclusion;}
	inline int GetIniStep()                     			  {return m_iniStep;}
	inline bool GetTrajectoryCondition()                     			  {return m_TrajectoryCondition;}
    inline std::vector<int>  GetInclusionTypeID()        {return m_Inclusion_TypeID;}
    inline std::vector<std::string>  GetInclusionType()        {return m_Inclusion_Type;}

public:

void Write(int step ,  std::string filename, std::vector< vertex* > pver, std::vector< triangle* > ptriangle,  std::vector< links* > plinks, std::vector< inclusion* > pinc);
void Read(std::string filename, bool);
void ReadTrajectoryFile(std::string  );
private:

std::vector<vertex> m_Vertex;
std::vector<triangle> m_Triangle;
    std::vector<links> m_Links;
    std::vector<inclusion>  m_Inclusion;
int m_iniStep;
void CopyFile(std::string, std::string);
bool CheckFile(std::string);
void ResizeBox();

private:

    std::vector<vertex*>      m_pAllV;
    std::vector<triangle*>    m_pAllT;
    std::vector<links*>       m_pLinks;
    std::vector<links*>       m_pHL;
    std::vector<links*>       m_pMHL;
        std::vector<inclusion*>       m_pInclusion;
    bool m_TrajectoryCondition;

    Vec3D *m_pBox;
std::string m_filename;
    Nfunction m_f;

    std::vector<int> m_Inclusion_TypeID;
    std::vector<std::string> m_Inclusion_Type;



};


#endif
