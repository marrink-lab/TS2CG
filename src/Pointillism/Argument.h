#if !defined(AFX_ARGUMENT_H_7F4A21B8_C13C_11D3_BF23_124095086234__INCLUDED_)
#define AFX_ARGUMENT_H_7F4A21B8_C13C_11D3_BF23_124095086234__INCLUDED_

#include "SimDef.h"


class Argument
{
public:
    
	Argument(std::vector <std::string> arg);
	 ~Argument();
    
public:
        inline const std::vector <std::string> GetArgumentString()      const {return m_Argument;}
    
        inline const int         GetArgCon()                            const {return m_ArgCon;}
        inline const std::string GetLx()             		        const {return m_Lx;}
        inline const std::string GetLy()          		        const {return m_Ly;}
        inline const std::string GetLz()                          const {return m_Lz;}
        inline const std::string GetBoundary()		                const {return m_Boundary;}
        inline const std::string GetTopologyFile()		        const {return m_TopologyFile;}
        inline const std::string GetSeed()        	                const {return m_Seed;}
	inline const std::string GetMinFaceAngle()             		const {return m_MinFaceAngle;}
	inline const std::string GetGeneralOutputFilename()             	const {return m_GeneralOutputFilename;}
    inline const std::string GetIndexFileName()                 const {return m_IndexFileName;}
    inline const std::string GetSoftwareVersion()                 const {return m_SoftWareVersion;}

    // // =================

private:
    std::vector <std::string> m_Argument;
    std::string m_excutablename;
    std::string m_TopologyFile;    //
    std::string m_Lx;               // box side in x direction
    std::string m_Ly;               // box side in y direction
    std::string m_Lz;               // box side in y direction
    std::string m_IndexFileName;
    int m_ArgCon;

          
    std::string  m_Boundary;        
    std::string  m_Seed;
    std::string  m_MinFaceAngle;
    std::string  m_GeneralOutputFilename;
    std::string m_SoftWareVersion;


};

#endif
