 #if !defined(AFX_GenerateMolType_CPP_9T4A21B7_C13C_1223_BFSS_124095086234__INCLUDED_)
#define AFX_GenerateMolType_CPP_9T4A21B7_C13C_1223_BFSS_124095086234__INCLUDED_

#include <stdio.h>
#include <math.h>
#include "GenerateMolType.h"
#include "GroFile.h"



GenerateMolType::GenerateMolType(Argument *pArgu)
{

    m_Health =true;
    Nfunction f;
    std::string strfilename = pArgu->GetStructureFileName();
    m_BondLenght = pArgu->GetBond_length();
    
    

            std::cout<<"--> Generating molecule types from  "<<strfilename<<"  file and our library   -------------"<<"\n";


//=======================================================
//==== File has been checked
//=======================================================

    std::ifstream strfile;
    strfile.open(strfilename.c_str());
    std::string str;
    std::vector<std::string> gfilenames;
//=======================================================
//==== reading all the gro files
//=======================================================
    while (true)
    {
        strfile>>str;
        
        if(strfile.eof())
            break;
        if(str=="include")
        {
            strfile>>str;
            if (f.FileExist(str)==true)
            gfilenames.push_back(str);
            else
            {
                std::cout<<"--> Error: File name "<<str<<" included in the "<<strfilename<<" does not exist \n";
                std::cout<<"--> all other steps are terminated \n";
                m_Health = false;
                break;
            }
        }
        std::getline (strfile,str);

    }
    strfile.close();
    
    //====== Check if a Lib is defined, otherwise read what we have here
    if(pArgu->GetLipidLibrary()=="no")
    {
        std::cout<<"-> Note the lipids will be generated from the enternal memory (Martini 2) of "<<SoftWareName<<".  \n";
        LiBMol();
    }
    else
    {
        ReadLipidLibrary ExternalLIB(pArgu);
        m_MoleculesType = ExternalLIB.GetMolType();
        if(ExternalLIB.GetHealth()==true)
        {
        std::cout<<"--> Note: the lipids will be generated from "<<ExternalLIB.GetLiBTitle()<<"Lipid Library, of version: "<<ExternalLIB.GetLiBVersion()<<"\n";
        std::cout<<"--> This library contains "<<m_MoleculesType.size()<<" lipid types \n";
        }
        else
        {
            std::cout<<"--> Faild in reading the library "<<ExternalLIB.GetLiBTitle()<<"\n";
            m_Health=false;
        }
    }
if(m_Health==true)
{
    for (std::vector<std::string>::iterator it = gfilenames.begin() ; it != gfilenames.end(); ++it)
    {
        GroFile GF(*it);        
        std::vector<bead> vbeads = GF.GetAllBeads();
        std::string molname =GF.GetTitle();
        
        MolType temMOL;
        temMOL.Beads = vbeads;
        temMOL.MolName = molname;
        temMOL.beadnumber = vbeads.size();
        double cmolarea = MolAreaCal(temMOL);
        temMOL.molarea = cmolarea;
        m_MoleculesType.insert(std::pair<std::string, MolType>(molname, temMOL));

    }
    int nolib = gfilenames.size();

    

    
    
    
    std::cout<<" ************************************************************** \n";
    std::cout<<" --> We have generated "<<m_MoleculesType.size()<<" mol type. "<<nolib<<" are from the included *.gro files "<<"\n";
    std::cout<<" ************************************************************** \n";
    std::cout<<" **** Molecule list and number of their particles are listed bellow  **** "<<"\n";

   for ( std::map<std::string, MolType>::iterator it = m_MoleculesType.begin(); it != m_MoleculesType.end(); it++ )
    {
        std::cout <<"*    "<< it->first  <<" ---> "<< (it->second).beadnumber<<"  area of "<<(it->second).molarea<<"  nm^2"<< std::endl ;
    }
    
    std::cout<<" **** Do not panic, the APL might not match the value  given in the "<<strfilename<<" file. This will be corrected in next steps **** "<<"\n";

    

}
    
    
    
    //================ Some checks on the other line of the file =======
    int l=0;
    bool flagLsection=false;
    bool flagPsection=false;
    bool flag = false;

    if(m_Health==true)
    {
        std::ifstream strfile;
        strfile.open(strfilename.c_str());
        while (true)
        {
            l++;
            std::getline (strfile,str);
            if(strfile.eof())
                break;
            
            std::vector<std::string> Line = f.split(str);
            
            if(Line.size()!=0 && (Line.at(0)).at(0)!=';')
            {

                    if((Line.at(0)).at(0)=='[' && flag==false)
                    {
                        flag=true;

                    }
                    else if((Line.at(0))=="End" && flag==true)
                    {
                        flag=false;
                    }
                    else if((Line.at(0))=="End" && flag==false)
                    {
                        std::cout<<" Error GMT: line "<<l<<" Mol Type <"<<Line.at(0)<<"> has not been defined  \n";
                        m_Health = false;
                        break;
                        
                    }
                    else if(flag==true)
                    {
                        
                        std::map<std::string, MolType>::iterator iter = m_MoleculesType.find(Line.at(0));
                        if (iter == m_MoleculesType.end() )
                        {
                            std::cout<<" Error GMT: line "<<l<<" Mol Type <"<<Line.at(0)<<"> has not been defined  \n";
                            m_Health = false;
                            break;
                        }
                    }
                
            }
        }
        
    }
    //=======================
    
}
GenerateMolType::~GenerateMolType()
{
    
}


double GenerateMolType::MolAreaCal(MolType mol)
{
    
    
    
    
    double area = 0;
    std::vector<bead> vbeads = mol.Beads;
    double rmx2=0;
    double xcm=0;
    double ycm=0;
    for (std::vector<bead>::iterator it = vbeads.begin() ; it != vbeads.end(); ++it)
    {
        xcm=xcm+(*it).GetXPos()/double(vbeads.size());
        ycm=ycm+(*it).GetYPos()/double(vbeads.size());
        
    }
    for (std::vector<bead>::iterator it = vbeads.begin() ; it != vbeads.end(); ++it)
    {
        double x=(*it).GetXPos()-xcm;
        double y=(*it).GetYPos()-ycm;
        
        if(rmx2<x*x+y*y)
            rmx2=(x*x+y*y);
    }
    
    
    area=acos(-1)*rmx2;
    return area;
}
//=================================================================================================================
//=========================== Mol Lib 
//============================================================================================================
void GenerateMolType::LiBMol()
{
  

    Mol_Def_DPhospholipid("DLPC", "- - - NC3 - PO4 GL1 GL2 C1A C2A C3A - - - C1B C2B C3B - - -", 0.65);
    
    double l = m_BondLenght;
    
    //====== DOPC ===========//
    {
    std::vector<bead> molbead;
        {Vec3D X(0,0,1);
        X=X*l;
        bead b1(1, "NC3", "Q0", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,0);
        X=X*l;
        bead b1(2, "PO4", "Qa", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-1);
        X=X*l;
        bead b1(3, "GL1", "Na", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-2);
        X=X*l;
        bead b1(4, "GL2", "Na", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-3);
        X=X*l;
        bead b1(5, "G1A", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-4);
        X=X*l;
        bead b1(6, "G2A", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-5);
        X=X*l;
        bead b1(7, "D3A", "C3", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,0,-6);
        X=X*l;
        bead b1(8, "D4A", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,1,-2);
        X=X*l;
        bead b1(9, "C1B", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,1,-3);
        X=X*l;
        bead b1(10, "C2B", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,1,-4);
        X=X*l;
        bead b1(11, "C3B", "C3", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
        {Vec3D X(0,1,-5);
        X=X*l;
        bead b1(12, "C4B", "C1", "DOPC", 1, X(0),X(1),X(2));
        molbead.push_back(b1);}
   
    MolType Mol;
    Mol.Beads = molbead;
    Mol.beadnumber=12;
        Mol.molarea = 0.7;
    Mol.MolName = "DOPC";
    m_MoleculesType.insert(std::pair<std::string, MolType>("DOPC", Mol));
    }
    

    
    //====== POPC ===========//
    {
        std::vector<bead> molbead;
        {Vec3D X(0,0,1);
            X=X*l;
            bead b1(1, "NC3", "Q0", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,0);
            X=X*l;
            bead b1(2, "PO4", "Qa", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-1);
            X=X*l;
            bead b1(3, "GL1", "Na", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-2);
            X=X*l;
            bead b1(4, "GL2", "Na", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-3);
            X=X*l;
            bead b1(5, "G1A", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-4);
            X=X*l;
            bead b1(6, "G2A", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-5);
            X=X*l;
            bead b1(7, "D3A", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-6);
            X=X*l;
            bead b1(8, "D4A", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-2);
            X=X*l;
            bead b1(9, "C1B", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-3);
            X=X*l;
            bead b1(10, "C2B", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-4);
            X=X*l;
            bead b1(11, "C3B", "C3", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-5);
            X=X*l;
            bead b1(12, "C4B", "C1", "POPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        
        MolType Mol;
        Mol.Beads = molbead;
        Mol.beadnumber=12;
        Mol.molarea = 0.69;
        Mol.MolName = "POPC";
        m_MoleculesType.insert(std::pair<std::string, MolType>("POPC", Mol));
    }
    //===
    //====== DPPC ===========//
    {
        std::vector<bead> molbead;
        {Vec3D X(0,0,1);
            X=X*l;
            bead b1(1, "NC3", "Q0", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,0);
            X=X*l;
            bead b1(2, "PO4", "Qa", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-1);
            X=X*l;
            bead b1(3, "GL1", "Na", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-2);
            X=X*l;
            bead b1(4, "GL2", "Na", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-3);
            X=X*l;
            bead b1(5, "G1A", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-4);
            X=X*l;
            bead b1(6, "G2A", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-5);
            X=X*l;
            bead b1(7, "D3A", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-6);
            X=X*l;
            bead b1(8, "D4A", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-2);
            X=X*l;
            bead b1(9, "C1B", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-3);
            X=X*l;
            bead b1(10, "C2B", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-4);
            X=X*l;
            bead b1(11, "C3B", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,1,-5);
            X=X*l;
            bead b1(12, "C4B", "C1", "DPPC", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        
        MolType Mol;
        Mol.Beads = molbead;
        Mol.beadnumber=12;
        Mol.molarea = 0.68;
        Mol.MolName = "DPPC";
        m_MoleculesType.insert(std::pair<std::string, MolType>("DPPC", Mol));
    }
     

    
    //====== CHOL ===========//
    {
        std::vector<bead> molbead;
        {Vec3D X(0,0,0);
            X=X*l;
            bead b1(1, "ROH", "SP1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-1);
            X=X*l;
            bead b1(2, "R1", "SC1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-2);
            X=X*l;
            bead b1(3, "R2", "SC3", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-3);
            X=X*l;
            bead b1(4, "R3", "SC1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-4);
            X=X*l;
            bead b1(5, "R4", "SC1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-5);
            X=X*l;
            bead b1(6, "R5", "SC1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-6);
            X=X*l;
            bead b1(7, "C1", "SC1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        {Vec3D X(0,0,-7);
            X=X*l;
            bead b1(8, "C2", "C1", "CHOL", 1, X(0),X(1),X(2));
            molbead.push_back(b1);}
        
        MolType Mol;
        Mol.Beads = molbead;
        Mol.beadnumber=8;
        Mol.molarea = 0.4;
        Mol.MolName = "CHOL";
        m_MoleculesType.insert(std::pair<std::string, MolType>("CHOL", Mol));
    }
    

    
}
void  GenerateMolType::Mol_Def_DPhospholipid(std::string resname, std::string BeadDEf, double APL)
{
    MolType newmoltype;
    Nfunction f;
    
    std::vector <std::string> AllBead = f.split(BeadDEf);
    int noBeads=0;
    std::vector<bead> molbead;

    for (int i =0; i<AllBead.size(); i++)
    {
        if(AllBead.at(i)!="-")
            noBeads++;
    }
    
    int beadid=0;
    for (int i =0; i<AllBead.size(); i++)
    {
       
       if(AllBead.at(i)!="-")
        {
            beadid++;
            
            Vec3D X(0,0,0);

            
            
            if(i<14 && i!=7)
            {
               X(2)=5-i;
            }
            else if(i==7)
            {
                X(2)=5-i+1;
                X(1)=1;

            }
            else
            {
                X(2)=13-i;
                X(1)=1;
            }
            X=X*m_BondLenght;
            bead b1(1, AllBead.at(i), "Q", resname, 1, X(0),X(1),X(2));
            molbead.push_back(b1);
        }
        
    }

        MolType Mol;
        Mol.Beads = molbead;
        Mol.beadnumber=noBeads;
        Mol.molarea = APL;
        Mol.MolName = resname;
        m_MoleculesType.insert(std::pair<std::string, MolType>(resname, Mol));

}












#endif



