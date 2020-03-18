#if !defined(AFX_ARGUMENT_H_7F4A21B8_C13C_11D3_BF23_124095086234__INCLUDED_)
#define AFX_ARGUMENT_H_7F4A21B8_C13C_11D3_BF23_124095086234__INCLUDED_

#include "Def.h"


class Argument
{
public:
    
	Argument(std::vector <std::string> arg);
	 ~Argument();
    
public:
        inline const std::vector <std::string> GetArgumentString()      const {return m_Argument;}
    
        inline const int         GetArgCon()                            const {return m_ArgCon;}
        inline const std::string GetDTSFolder()       			const {return m_DTSFolder;}
    inline const std::string GetLipidLibrary()                   const {return m_LipidLibrary;}

    inline const std::string GetStructureFileName()                   const {return m_StrFileName;}

	inline const std::string GetGeneralOutputFilename()             	const {return m_GeneralOutputFilename;}
    inline const bool GetHealth()            const {return m_Health;}
    inline const std::string GetSoftwareVersion()                 const {return m_SoftWareVersion;}
    inline const int GetSeed()                 const {return m_Seed;}
    inline const double GetBond_length()                 const {return m_BondL;}
    inline const bool GetRenorm()                 const {return m_Renorm;}
    inline const double GetIter()                 const {return m_Iter;}

    // // =================

private:
    std::vector <std::string> m_Argument;
    std::string m_DTSFolder;
    std::string m_StrFileName;
    std::string m_LipidLibrary;
    int m_ArgCon;

    bool m_Health;
    std::string  m_GeneralOutputFilename;
    std::string m_SoftWareVersion;
    int m_Seed;
    double m_BondL;
    bool m_Renorm;
    double m_Iter;


};

#endif
