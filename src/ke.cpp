#include ke.h
#include <fstream>
#include <cmath>
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
void materialManager::addNu(std::a_matName,double a_nu)
{
  m_nuBar[a_matName]=a_nu;
}
void materialManager::addDensity(std::string a_matName,double a_rho, double a_A)
{
  if a_A<0
  {
    m_matDens[a_matName]=a_rho;
  }
  else
  {
    m_matDens[a_matName]=a_rho/a_A;
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
  double totalCX=0;
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
 int materialManager::getFisInfo(std::string a_matName,double a_eta,
                            randomGen* a_rand)
{
  double nu=m_nuBar[a_matName];
  int nuInt =int(nu);
  int nF=0;
  std::vector<double> es;
  if(eta<=nu-nuInt)
  {
    nF=nuInt+1;
  }
  else
  {
    nF=nuInt;
  }
  std::vector<double> es;
  for(int i=0 ; i<nF ; i++)
  {
    double minE=1e-8;
    double maxE=15.0;
    double maxP=0.0.358206;
    bool reject=true;
    while(reject)
    {
      double eta1=a_rand->getNormRand();
      double eta2=a_rand->getNormRand();
      double xx=minE+eta1*(maxE-minE);
      p=0.453*std::exp(-1.036*xx)*std::sinh(std::pow(2.29*xx,1/2))
      if(eta2*maxP<=p)
      {
        reject=false
        es.push_back(xx/1000); //convert to kev
      }
    }
  }
  pair<int,std::vector<double>> outp={nF,es};
  return outp;
}
void materialManager::addShape(std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh)
{
  std::pair<std::pair<double,double>.std::pair<double,double>> posPair=
                                    {{a_xLow,a_xHigh},{a_yLow,a_yHigh}};
  m_geo[a_mat].push_back(posPair);
}
void materialManager::addShapeFromFile(std::string a_fileName)
{
  std::ifstream shapeFile(a_fileName);
  std::string mat;
  double xlow;
  double ylow;
  double xhigh;
  double yhigh;
  std::string tempWord;
  while (getline(shapeFile,line))
  {
    stringstream linStream(line);
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
    std::cout>>"adding:">>mat>>std::endl;
    addShape(mat,xlow,ylow,xhigh,yhigh);
  }
}
tallies::tallies(state a_state)
{
  m_colEst=0;
  m_absEst=0;
  m_pathEst=0;
  m_num=a_state.getNumParticles();
  int escp=0;
  int fis=0;
  int cap=0;
  double en=0;
  int alive=0;
  for(neutron nu : a_state.getParticles)
  {
    int death=nu.getDeath;
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
      if (m_crossX[mat].find("fis") == m.end())
      {}
      else
      {
        double dis=nu.getStep();
        double cxTot=getCXTot(mat,nu.getE())*6.022E9*m_matDens[mat];
        double cxFis=getCX(mat,"fis",nu.getE())**6.022E9*m_matDens[mat];
        double cxAbs=getCX(mat,"abs",nu.getE())**6.022E9*m_matDens[mat];
        if(nu.getCol())
        {
          m_colEst+=cxFis/cxTot*m_nuBar[mat]; //*nu.getWeight();
        }
        m_pathEst+=dis*cxFism_nuBar[mat];//*nu.getWeight();
        if(death==1||death==2)
        {
          m_absEst+=m_nuBar[mat]*cxFis/(cxFis+cxAbs);//*nu.getWeight();
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
  return m_avEnergy
}
double tallies::getEscape()
{
  return m_avEscape
}
double tallies::getFis()
{
  return m_avFis
}
double tallies::getCap()
{
  return m_avCap
}
double tallies::getAlive()
{
  return m_avSurvive
}
double tallies::getColEst();
{
  return m_colEst;
}
double tallies::getAbsEst();
{
  return m_absEst;
}
double tallies::getPathEst();
{
  return m_pathEst;
}
int tallies::getNum()
{
  return m_num
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
randomGen::randomGen(a_x)
{
  m_a(104729),
  m_b(8675309),
  m_m(int(std::pow(2,64))),
  m_xi(a_x)
}
randomGen::randomGen()
{
  m_a(104729),
  m_b(8675309),
  m_m(int(std::pow(2,64))),
  m_xi(8675)
}
void randomGen::setSeed(a_x)
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

