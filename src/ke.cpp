#include "../include/ke.h"
#include <fstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <sstream>
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
  std::string line;
  std::string tempWord;
  getline(cxFile,line);
  getline(cxFile,line);
  getline(cxFile,line);
  while (getline(cxFile,line))
  {
    std::stringstream strSec(line);
    getline(strSec,tempWord,';');
    En=stod(tempWord);
    getline(strSec,tempWord,';');
    Cx=stod(tempWord);
    m_crossX[a_matName][a_CXType].push_back({En,Cx});
  }
}
void materialManager::addMaterial(std::string a_matName,
                    std::vector<std::pair<std::string,std::string>> a_files)
{

  for(auto fi:a_files)
  {
    addCX(a_matName,fi.first,fi.second);
    if(fi.first!="tot")
    {
      m_matTypes[a_matName].push_back(fi.first);
    }
    m_mats.push_back(a_matName);
  }
}
void materialManager::addNu(std::string a_matName,double a_nu)
{
  m_nuBar[a_matName]=a_nu;
}
void materialManager::addDensity(std::string a_matName,double a_rho, double a_A)
{
  if (a_A<0)
  {
    m_matDens[a_matName]=a_rho;
  }
  else
  {
    m_matDens[a_matName]=a_rho/a_A;
    m_As[a_matName]=a_A;
  }
}
std::string materialManager::matFinder(double a_x, double a_y)
{
  for(std::string reg : m_regions)
  {
    double xLow=m_geo[reg].second.first.first;
    double xHigh=m_geo[reg].second.first.second;
    double yLow=m_geo[reg].second.second.first;
    double yHigh=m_geo[reg].second.second.second;
    if(a_x>xLow && a_x<xHigh && a_y>yLow && a_y<yHigh)
    {
      return m_geo[reg].first;
    }
  }
  return "void";
}
double materialManager::getCX(std::string a_matName,std::string a_type
                        ,double a_En)
{
  bool foundH=false;
  double eLow=0;
  double cxLow=0;
  double eHigh=0;
  double cxHigh=0;
  double En=a_En*1000; //covert to eV

  for(std::pair enX: m_crossX[a_matName][a_type])
  {
    if(En==enX.first)
    {
      return enX.second;
    }
    if(En<enX.first)
    {
      if(!foundH)
      {
        eHigh=enX.first;
        cxHigh=enX.second;
        foundH=true;
      }
      continue;
    }
    else if(En>enX.first)
    {
      eLow=enX.first;
      cxLow=enX.second;
    }
  }
  double efrac=(En-eLow)/(eHigh-eLow);
  return cxLow+efrac*(cxHigh-cxLow);
}
double materialManager::getCXTot(std::string a_matName,double a_En)
{
  // double totalCX=0;
  // for (std::string ty:m_matTypes[a_matName])
  // {
  //   totalCX=totalCX+getCX(a_matName,ty,a_En);
  // }
  // return totalCX;
  return getCX(a_matName,"tot",a_En);
}
double materialManager::getDist(std::string a_matName,double a_En,double a_eta)
{
  double totCX=getCXTot(a_matName,a_En)*6.022E9*m_matDens[a_matName];
  return -1*std::log(1-a_eta)/totCX;
}
std::string materialManager::getReactionType(double a_eta,std::string a_matName,
                                double a_En)
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
  // std::cout<<"something went wrong in reaction sample"<<std::endl;
  return "scat";
}
double materialManager::getScatEn(double a_En, double a_eta,std::string a_matName)
{
  double alpha=pow(((m_As[a_matName]-1)/(m_As[a_matName])),2);
  return alpha*a_En+a_eta*(a_En-alpha*a_En);
}
 std::pair<int,std::vector<double>> materialManager::getFisInfo(std::string a_matName,double a_eta,
                            randomGen* a_rand)
{
  double nu=m_nuBar[a_matName];
  int nuInt =int(nu);
  int nF=0;
  std::vector<double> es;
  if(a_eta<=nu-nuInt)
  {
    nF=nuInt+1;
  }
  else
  {
    nF=nuInt;
  }
  for(int i=0 ; i<nF ; i++)
  {
    double minE=1e-8;
    double maxE=15.0;
    double maxP=0.358206;
    bool reject=true;
    while(reject)
    {
      double eta1=a_rand->getNormRand();
      double eta2=a_rand->getNormRand();
      double xx=minE+eta1*(maxE-minE);
      double p=0.453*std::exp(-1.036*xx)*std::sinh(std::pow(2.29*xx,1/2));
      if(eta2*maxP<=p)
      {
        reject=false;
        es.push_back(xx/1000); //convert to kev
      }
    }
  }
  pair<int,std::vector<double>> outp={nF,es};
  return outp;
}
void materialManager::addShape(std::string a_reg, std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh)
{
  m_regions.push_back(a_reg);
  std::pair<std::pair<double,double>,std::pair<double,double>> posPair=
                                    {{a_xLow,a_xHigh},{a_yLow,a_yHigh}};
  std::pair<std::string,std::pair<std::pair<double,double>,std::pair<double,double>>> outPair=
    {a_reg,posPair};
  m_geo[a_reg]=outPair;
}
void materialManager::addShapeFromFile(std::string a_fileName)
{
  std::ifstream shapeFile(a_fileName);
  std::string reg;
  std::string mat;
  double xlow;
  double ylow;
  double xhigh;
  double yhigh;
  std::string line;
  std::string tempWord;
  while (getline(shapeFile,line))
  {
    std::stringstream linStream(line);
    getline(linStream,tempWord,',');
    reg=tempWord;
    getline(linStream,tempWord,',');
    mat=tempWord;
    getline(linStream,tempWord,',');
    xlow=std::stod(tempWord);
    getline(linStream,tempWord,',');
    ylow=std::stod(tempWord);
    getline(linStream,tempWord,',');
    xhigh=std::stod(tempWord);
    getline(linStream,tempWord,',');
    yhigh=std::stod(tempWord);
    std::cout<<"adding:"<<mat<<std::endl;
    addShape(reg,mat,xlow,ylow,xhigh,yhigh);
  }
}
std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> materialManager::getCXS()
{
  return m_crossX;
}
std::map<std::string,double> materialManager::getDens()
{
  return m_matDens;
}
std::map<std::string,double> materialManager::getNus()
{
  return m_nuBar;
}
tallies::tallies()
{
  // nothing here
}
tallies::tallies(state a_state,materialManager* a_mats)
{
  m_colEst=0;
  m_absEst=0;
  m_pathEst=0;
  m_num=a_state.getNumParticles();
  std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> crossX=a_mats->getCXS();
  std::map<std::string,double> matDens=a_mats->getDens();
  std::map<std::string,double> nuBar = a_mats->getNus();
  int escp=0;
  int fis=0;
  int cap=0;
  double en=0;
  int alive=0;
  for(neutron nu : a_state.getParticles())
  {
    int death=nu.getDeath();
    if(death>-1)
    {
      if(death==0)
      {
        escp+=1;
      }
      if(death==1)
      {
        cap+=1;
      }
      if(death==2)
      {
        fis+=1;
      }
    }
    else
    {
      en+=nu.getE();
      alive+=1;
    }
    std::string mat=nu.getMat();
    if(mat!="void")
    {
      if (crossX[mat].find("fis") == crossX[mat].end())
      {}
      else
      {
        double dis=nu.getStep();
        double cxt=a_mats->getCXTot(mat,nu.getE());
        double cxf=a_mats->getCX(mat,"fis",nu.getE());
        double cxa=a_mats->getCX(mat,"abs",nu.getE());
        double cxTot=cxt*6.022E9*matDens[mat];
        double cxFis=cxf*6.022E9*matDens[mat];
        double cxAbs=cxa*6.022E9*matDens[mat];
        if(nu.getCol())
        {
          m_colEst+=cxFis/cxTot*nuBar[mat]; //*nu.getWeight();
        }
        m_pathEst+=dis*cxFis*nuBar[mat];//*nu.getWeight();
        if(death==1||death==2)
        {
          m_absEst+=nuBar[mat]*cxFis/(cxFis+cxAbs);//*nu.getWeight();
        }
      }
    }
  }
  m_avEnergy=en/alive;
  m_avEscape=escp/m_num;
  m_avFis=fis/m_num;
  m_avCap=cap/m_num;
  m_avSurvive=alive/m_num;
}
double tallies::getAvEn()
{
  return m_avEnergy;
}
double tallies::getEscape()
{
  return m_avEscape;
}
double tallies::getFis()
{
  return m_avFis;
}
double tallies::getCap()
{
  return m_avCap;
}
double tallies::getAlive()
{
  return m_avSurvive;
}
double tallies::getColEst()
{
  return m_colEst;
}
double tallies::getAbsEst()
{
  return m_absEst;
}
double tallies::getPathEst()
{
  return m_pathEst;
}
int tallies::getNum()
{
  return m_num;
}
reducedState::reducedState()
{
  // nothing to do here
}
void reducedState::addReduced(tallies a_tally)
{
  m_reducedStates.push_back(a_tally);
}
std::vector<tallies> reducedState::getTallies()
{
  return m_reducedStates;
}
randomGen::randomGen(int a_x)
{
  m_a=104729;
  m_b=8675309;
  m_m=int(std::pow(2,32));
  m_xi=a_x;
}
randomGen::randomGen()
{
  m_a=104729;
  m_b=8675309;
  m_m=int(std::pow(2,64));
  m_xi=8675;
}
void randomGen::setSeed(int a_x)
{
  m_xi=a_x;
}
void randomGen::setPars(int a_a,int a_b,int a_m)
{
  m_a=a_a;
  m_b=a_b;
  m_m=a_m;
}
int randomGen::getRand()
{
  m_xi=(m_a*m_xi+m_b)%m_m;
  return m_xi;
}
double randomGen::getNormRand()
{
  m_xi=(m_a*m_xi+m_b)%m_m;
  double nor=double(m_xi)/double(m_m);
  return nor;
}

