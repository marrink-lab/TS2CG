
#include "Argument.h"
#include "help.h"
#include "Nfunction.h"

Argument::Argument(std::vector <std::string> argument)
{
    m_Argument=argument;

    Nfunction f;
    
    

    
    m_BondL              = 0.1;
    m_Health             = true;
    m_DTSFolder            = "point";
    m_LipidLibrary          = "no";
    m_GeneralOutputFilename	 = "output";
    m_SoftWareVersion = SoftWareVersion;
    m_InclusionDirectionType = "Global";
    m_ArgCon=1;
    std::string Arg1;
    m_Seed = 9474;
    m_Renorm = false;
    m_Iter = 4;
    
    if (m_Argument.size()>300)
    {
        std::cout << "Error: to many argument ";
        std::cout<<"\n"<<"For more information and tips execute MCL -h"<<"\n";
       m_ArgCon=0;
        m_Health = false;
        
    }
    else if (m_Argument.size() == 2)
    {
        
        Arg1 = m_Argument.at(1);
        if (Arg1 == "-h" )
        {
        // help message should be made
            help helpmessage(m_Argument.at(0));
            m_ArgCon=0;
            m_Health = false;
        }

    }
    else
    {

        for (long i=1;i<m_Argument.size();i=i+2)
        {
            Arg1 = m_Argument.at(i);
            if(Arg1=="-dts")
            {
                m_DTSFolder = m_Argument.at(i+1);
            }
            else if(Arg1=="-defout")
            {
                m_GeneralOutputFilename = m_Argument.at(i+1);
            }
            else if(Arg1=="-renorm")
            {
                i=i-1;
                m_Renorm = true;

            }
            else if(Arg1=="-incdirtype")
            {
                m_InclusionDirectionType = m_Argument.at(i+1);
                if(m_InclusionDirectionType!="Local" || m_InclusionDirectionType!="Global")
                {
                    std::cout<<"Error: The inclusion direction type is unknown \n";
                }
                
            }
            else if(Arg1=="-iter")
            {
               m_Iter = f.String_to_Double(m_Argument.at(i+1));
                
            }
            else if(Arg1=="-LLIB")
            {
                m_LipidLibrary = m_Argument.at(i+1);
                
                if(m_LipidLibrary.substr(m_LipidLibrary.find_last_of(".") + 1) != LIBExt)
                {

                    m_LipidLibrary = m_LipidLibrary + "." +LIBExt;
                }
                if (f.FileExist (m_LipidLibrary)!=true)
                {
                    
                    std::cout<<" Error: lipid library file, with name "<<m_LipidLibrary<<" does not exist \n";
                    m_Health = false;
                }
            }
            else if(Arg1=="-str")
            {
                m_StrFileName = m_Argument.at(i+1);
                if(m_StrFileName.substr(m_StrFileName.find_last_of(".") + 1) != STRExt)
                {
                    m_StrFileName = m_StrFileName + "." + STRExt;
                }
                if (f.FileExist (m_StrFileName)!=true)
                {

                    std::cout<<" Error: str file, with name "<<m_StrFileName<<" does not exist \n";
                    m_Health = false;
                }
            }
            else if(Arg1=="-seed")
            {
                m_Seed = f.String_to_Double(m_Argument.at(i+1));
            }
            else if(Arg1=="-Bondlength")
            {
                m_BondL = f.String_to_Double(m_Argument.at(i+1));
            }
            else
            {
                std::cout << "Error: wrong command :"<<Arg1;
                std::cout<<"\n"<<"For more information and tips execute ./DMC -h"<<"\n";
                m_ArgCon=0;
                m_Health = false;
            }
        }
        
    }


    
}






Argument::~Argument()
{
   
}
