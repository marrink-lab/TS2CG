 #if !defined(AFX_ReadLipidLibrary_CPP_8S4A21B7_C13C_1223_BFSS_124095086234__INCLUDED_)
#define AFX_ReadLipidLibrary_CPP_8S4A21B7_C13C_1223_BFSS_124095086234__INCLUDED_

#include <stdio.h>
#include <math.h>
#include "ReadLipidLibrary.h"



ReadLipidLibrary::ReadLipidLibrary(Argument *pArgu)
{

    m_Health =true;
    Nfunction f;
    std::string LiBfilename = pArgu->GetLipidLibrary();
    m_BondLenght = pArgu->GetBond_length();
    
    



//=======================================================
//==== File has been checked
//=======================================================

    std::ifstream LiBFile;
    LiBFile.open(LiBfilename.c_str());
    std::string str;
//=======================================================
//==== reading Lib File
//=======================================================
    //std::string m_LiBTitle;
    //std::string m_LiBVersion;
    LiBFile>>str;
    if(str=="Description")
    {
    std::getline (LiBFile,m_LiBTitle);
    }
    else
    {
        std::cout<<"Error: broken Lib File "<<LiBfilename<<"\n";
        m_Health = false;
    }
    LiBFile>>str;
    if(str=="Version")
    {
        std::getline (LiBFile,m_LiBVersion);
    }
    else if(m_Health==true)
    {
        std::cout<<"Error: broken Lib File "<<LiBfilename<<"\n";
        m_Health = false;
    }
    
    std::vector<bead> molbead;
    bool start = false;

    if(m_Health==true)
    {
        std::string lipidname;
        while (true)
        {
            std::getline (LiBFile,str);
            
            if(LiBFile.eof())
            {
                MolType Mol;
                Mol.Beads = molbead;
                Mol.beadnumber=molbead.size();
                Mol.molarea = 0.68;
                Mol.MolName = lipidname;
                m_MoleculesType.insert(std::pair<std::string, MolType>(lipidname, Mol));
                molbead.clear();
                break;
            }
        
            str = f.trim(str);
            if(str.size()!=0 && str.at(0)!=';')
            {
                if(str.at(0)=='[')
                {
                    if (start==true )
                    {
                    MolType Mol;
                    Mol.Beads = molbead;
                    Mol.beadnumber=molbead.size();
                    Mol.molarea = 0.68;
                    Mol.MolName = lipidname;
                    m_MoleculesType.insert(std::pair<std::string, MolType>(lipidname, Mol));
                    molbead.clear();
                    }
                    start =true;
                    str.erase(std::remove(str.begin(), str.end(), '['), str.end());
                    str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
                    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                    lipidname = str;
                    
                }
                else if (start==true )
                {
                    // read the MOL
                    std::vector<std::string> Line = f.split(str);
                    if(Line.size()>=5)
                    {
                    int id = f.String_to_Int(Line.at(0));
                    std::string beadname =Line.at(1);
                    double X=f.String_to_Double(Line.at(2));
                    double Y=f.String_to_Double(Line.at(3));
                    double Z=f.String_to_Double(Line.at(4));
                    bead b1(id, beadname, "C", lipidname, 1, X*m_BondLenght,Y*m_BondLenght,Z*m_BondLenght);
                    molbead.push_back(b1);
                        if(Line.size()>5 && (Line.at(5)).at(0)!=';')
                        {
                            std::cout<<"error: line contains too many elements "<<str<<" \n";
                            m_Health = false;
                            break;
                        }
                    }
                    else
                    {
                        std::cout<<"error: line contains too little elements "<<str<<" \n";
                        m_Health = false;
                        break;
                    }
                }
                else
                {
                    std::cout<<"error3: invalid line "<<str<<" \n";
                    m_Health = false;
                    break;
                }
                    
            }
            
            
        
        
        }
    }

    



    
}
ReadLipidLibrary::~ReadLipidLibrary()
{
    
}



#endif



