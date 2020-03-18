#if !defined(AFX_ReadLipidLibrary_H_9D4B21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_ReadLipidLibrary_H_9D4B21B8_C13C_5648_BF23_124095086234__INCLUDED_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <list>
#include <map>
#include <iomanip>
#include <valarray>
#include "LMatrix.h"
#include "Nfunction.h"
#include "Argument.h"
#include "Vec3D.h"
#include "bead.h"
#include "molecules.h"

struct MolType {
    std::vector<bead> Beads;
    std::string MolName;
    int beadnumber;
    double molarea;
} ;
class ReadLipidLibrary
{
public:
    
	ReadLipidLibrary(Argument *pArgu);
	virtual ~ReadLipidLibrary();
    
    std::map<std::string , MolType>  GetMolType()                const  {return m_MoleculesType;}
    bool  GetHealth()                  {return m_Health;}
    std::string  GetLiBVersion()                  {return m_LiBVersion;}
    std::string  GetLiBTitle()                  {return m_LiBTitle;}


public:

private:
    std::vector<bead*> m_pAllBeads;
    std::map<std::string , MolType>  m_MoleculesType;
    std::vector<molecules> m_AllMolecules;
    std::vector<molecules> m_pAllMolecules;
    bool m_Health;
    double m_BondLenght;
    std::string m_LiBTitle;
    std::string m_LiBVersion;

private:

private:
};


#endif
