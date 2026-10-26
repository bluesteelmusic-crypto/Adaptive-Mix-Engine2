#pragma once
#include <algorithm>
#include <cmath>
#include <array>

namespace ame {
class StereoDSP {
public:
 void prepare(double sr) { sampleRate=std::max(8000.0,sr); reset(); }
 void reset() { for(auto& c:low) c=0; envelope=0; currentGain=1; }
 void process(const float* inL,const float* inR,float* outL,float* outR,int n,float inputDb,float outputDb,float lowControlDb,float strength,bool bypass) {
  const float inGain=std::pow(10.f,inputDb/20.f),outGain=std::pow(10.f,outputDb/20.f);
  const float alpha=std::exp(-2.f*3.14159265358979323846f*110.f/(float)sampleRate);
  const float attack=std::exp(-1.f/(0.012f*(float)sampleRate));
  const float release=std::exp(-1.f/(0.16f*(float)sampleRate));
  const float smooth=std::exp(-1.f/(0.008f*(float)sampleRate));
  for(int i=0;i<n;++i) {
   float l=(inL?inL[i]:0.f)*inGain,r=(inR?inR[i]:0.f)*inGain;
   if(bypass) {outL[i]=(inL?inL[i]:0.f);outR[i]=(inR?inR[i]:0.f);continue;}
   low[0]=alpha*low[0]+(1-alpha)*l;
   low[1]=alpha*low[1]+(1-alpha)*r;
   float detector=std::max(std::abs(low[0]),std::abs(low[1]));
   float coeff=detector>envelope?attack:release;
   envelope=coeff*envelope+(1-coeff)*detector;
   // Only reduce low-band energy above a conservative -18 dBFS detector threshold.
   float activity=std::clamp((envelope-0.126f)/0.15f,0.f,1.f);
   float target=std::pow(10.f,-std::clamp(lowControlDb,0.f,3.f)*std::clamp(strength,0.f,1.f)*activity/20.f);
   currentGain=smooth*currentGain+(1-smooth)*target;
   outL[i]=(l+(currentGain-1)*low[0])*outGain;
   outR[i]=(r+(currentGain-1)*low[1])*outGain;
  }
 }
private:
 double sampleRate=48000;
 std::array<float,2> low{0,0};
 float envelope=0,currentGain=1;
};
}
