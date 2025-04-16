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

  for(auto fi:a_files)
  {
    addCX(a_matName,fi.first,fi.second);
    m_matTypes[a_matName].push_back(fi.first);
    m_mats.push_back(a_matName);
  }
}
std::string materialManager::matFinder(double a_x, double a_y)
{
  for(std::string mat : m_mats)
  {
    xLow=m_geo[mat].first.first;
    xHigh=m_geo[mat].first.second;
    yLow=m_geo[mat].second.first;
    yHigh=m_geo[mat].second.second;
    if(a_x>xLow && a_x<xHigh && a_y>yLow && a_y<yHigh)
    {
      return mat;
    }
  }
  return "void"
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
void materialManager::addShape(std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh)
{
  std::pair<std::pair<double,double>.std::pair<double,double>> posPair=
                                    {{a_xLow,a_xHigh},{a_yLow,a_yHigh}};
  m_geo[a_mat].push_back(posPair);
}