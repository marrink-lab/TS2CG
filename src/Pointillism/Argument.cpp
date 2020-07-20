
#include "Argument.h"
#include "Nfunction.h"
#include "help.h"

Argument::Argument(std::vector <std::string> argument)
{
    m_Argument=argument;
    Nfunction f;
    m_Ly     			 = "Error";
    m_Lx     			 = "Error";
    m_Boundary       		 = "Error";
    m_Seed                       = "Error";
    m_TopologyFile               = "Error";
    m_MinFaceAngle		 = "Error";
    m_GeneralOutputFilename	 = "Error";
    m_SoftWareVersion = SoftWareVersion;
    m_IndexFileName = "inx.inx";



    m_ArgCon=1;
    std::string Arg1;
    
    m_excutablename = m_Argument.at(0);
    
    if (m_Argument.size() == 2)
    {
        
        Arg1 = m_Argument.at(1);
        if (Arg1 == "-h" )
        {
        // help message should be made
            help helpmessage(m_SoftWareVersion,m_excutablename);
            m_ArgCon=0;
        }
        else
        {
            std::cout << "Error:  argument "<<Arg1<<" is not recognized \n";
            help helpmessage(m_SoftWareVersion,m_excutablename);
            m_ArgCon=0;
        }
    }
    else
    {

        for (long i=1;i<m_Argument.size();i=i+2)
        {
            Arg1 = m_Argument.at(i);
           if(Arg1=="-top")
            {
                m_TopologyFile = m_Argument.at(i+1);
            }
            else if(Arg1=="-lx")
            {
                m_Lx = m_Argument.at(i+1);
            }
            else if(Arg1=="-ly")
            {
                m_Ly = m_Argument.at(i+1);
            }
            else if(Arg1=="-lz")
            {
                m_Lz = m_Argument.at(i+1);
            }
            else if(Arg1=="-bound")
            {
                m_Boundary = m_Argument.at(i+1);
            }
            else if(Arg1=="-seed")
            {
                m_Seed = m_Argument.at(i+1);
            }
            else if(Arg1=="-angle")
            {
                m_MinFaceAngle = m_Argument.at(i+1);
            }
            else if(Arg1=="-defout")
            {
                m_GeneralOutputFilename = m_Argument.at(i+1);
            }
            else if(Arg1=="-ndx")
            {
                m_IndexFileName = m_Argument.at(i+1);
            }

            else if(Arg1=="-degree" )
            {
                
                // this data will be read directly from the class to generate morphology
                
                //  i++;
                
            }
            else if(Arg1=="-smooth"  )
            {
                
                  i=i-1;
                
            }
            else if(Arg1=="-ap" || Arg1=="-zoom" || Arg1=="-apwall" || Arg1=="-TSfile" || Arg1=="-bilayerThickness" || Arg1=="-rescalefactor" || Arg1=="-layer"|| Arg1=="-o" || Arg1=="-shape" || Arg1=="-monolayer" || Arg1=="-resizebox")
            {
                
                // this data will be read directly from the class
                
              //  i++;
                
            }
            else if(Arg1=="-ap" || Arg1=="-gmxCfile"  || Arg1=="-Mashno" || Arg1=="-AlgType" || Arg1=="-r")
            {
                
                // this data will be read directly from MD dynamic wall class
                
                //  i++;
                
            }
            else if(Arg1=="-n"  )
            {
                
                // this is for gen morphology class
                
            }
            else
            {
                std::cout << "Error:  argument "<<Arg1<<" is not recognized \n";
                help helpmessage(m_SoftWareVersion,m_excutablename);
                m_ArgCon=0;
                break;


            }
        }
        
    }


    
}






Argument::~Argument()
{
   
}
