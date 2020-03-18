#if !defined(AFX_NFUNCTION_H_7F4A21B8_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_NFUNCTION_H_7F4A21B8_B13C_11D3_BF19_004095086186__INCLUDED_
 
class Nfunction
{
public:
Nfunction();
 ~Nfunction();
public:
    int AFactor(int R);
    std::string trim(const std::string& str );

//Nfunction(int a);
std::string Int_to_String(double ); //   A function for making a string from a intger
int String_to_Int(std::string);
double String_to_Double(std::string);
double Word_Searcher(std::string, std::string); // A function get a word and a filename and looking for word into the file
    bool FileExist (const std::string& name);
    std::vector<std::string> split(std::string str);

std::string Word_Searcher_String(std::string, std::string); // A function get a word and a filename and looking for word into the file
void Change_Line();
    void Write_One_LogMessage(std::string);
    void Write_One_ErrorMessage(std::string);
    void CleanFiles();

bool isEven(int x);
private :
int L;
double NUMBER;
std::string WORD;


};





#endif
