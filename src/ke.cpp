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
    m_matTypes[a_matName].pushback(fi.first);
  }
}
std::string materialManager::matFinder(double x, double y)
{
  return"idkPuOrSomething";
}
double materialManager::getCX(std::string a_matName,std::string a_type
                        ,double a_En)
{
  bool foundLow=false;
  double eLow=0;
  double cxLow=0;
  double eHigh=0;
  double cxHigh=0;

  for(std::pair enX: m_crossX[a_matName][a_type])
  {
    if(foundLow)
    {
      eHigh=enX.first;
      cxHigh=enX.second;
      continue;
    }
    else if(a_En>enX.first)
    {
      eLow=enX.first;
      cxLow=enX.second;
      foundLow=true
    }
  }
  double efrac=(a_En-eLow)/(eHigh-Elow);
  return std::lerp(cxLow,cxHigh,efrac);
}
double materialManager::getCXTot(std::string a_matName,double a_En)
{
  totalCX=0;
  for (std::string ty:m_matTypes[a_matName])
  {
    totalCX=totalCX+getCX(a_matName,ty.a_En);
  }
  return totalCX;
}
double materialManager::getDist(std::string a_matName,double a_En,double a_eta)
{
  double totCX=getCXTot(a_matName,a_En)*6.022E9*m_matDens[a_matName];
  return -1*std::log(1-a_eta)/totCX;
}
std::string materialManager::getReactionType(double a_eta,std::string a_matName,
                                double a_En);
{
  double totCX=getCXTot(a_matName,a_En);
  double num=0;
  for (std::string ty:m_matTypes[a_matName])
  {
    num=num+getCX(a_matName,ty,a_En);
    if(a_eta<=num/totCX)
    {
      return ty;
    }
  }
}