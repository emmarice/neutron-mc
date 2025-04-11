#include ke.h
#include <fstream>
// puts the class in its basic state
materialManager::materialManager()
{

}
void materialManager::addCX(std::string a_matName,std::string a_CXType,
                     std::string a_fileName)
{
  std::ifstream cxFile(a_fileName);
  double En;
  double Cx;
  getline(cxFile,line)
  getline(cxFile,line)
  getline(cxFile,line)
  while (getline(cxFile,line))
  {
    stringstream strSec(line, ';');
    strSec>>En>>Cx;
    m_crossX[a_matName][a_CXType].push_back({En,Cx});
  }
}
void materialManager::addMaterial(std::string a_matName,
                    std::vector<std::pair<std::string,std::string>> a_files)
{

  for(std::string fi:a_files)
  {
    addCX(a_matName,fi.first,fi.second);
  }
}
double materialManager::getCX(std::string a_matName,std::string a_type,double a_En)
{

}
double materialManager::getDist(std::string a_matName,double a_eta)
{

}
std::string materialManager::getReactionType(double a_eta,std::string a_matName,
                                double a_energy);
{

}