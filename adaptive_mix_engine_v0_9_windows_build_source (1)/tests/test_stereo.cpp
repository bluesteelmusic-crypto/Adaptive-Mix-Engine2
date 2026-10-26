#include "../StereoDSP.h"
#include <vector>
#include <cmath>
#include <iostream>
int main(){
 constexpr int n=48000;std::vector<float> a(n),b(n),x(n),y(n),u(n),v(n);
 for(int i=0;i<n;++i){a[i]=0.3f*std::sin(i*0.03f)+0.12f*std::sin(i*0.003f);b[i]=0.2f*std::sin(i*0.04f)+0.14f*std::sin(i*0.003f);}
 ame::StereoDSP full,chunk;full.prepare(48000);chunk.prepare(48000);
 full.process(a.data(),b.data(),x.data(),y.data(),n,0,0,0.75,0.5,false);
 for(int off=0;off<n;){int count=std::min(257,n-off);chunk.process(a.data()+off,b.data()+off,u.data()+off,v.data()+off,count,0,0,0.75,0.5,false);off+=count;}
 float maxerr=0;for(int i=0;i<n;++i){maxerr=std::max(maxerr,std::abs(x[i]-u[i]));maxerr=std::max(maxerr,std::abs(y[i]-v[i]));if(!std::isfinite(x[i])||!std::isfinite(y[i]))return 2;}
 std::cout<<"Max block difference: "<<maxerr<<"\n";if(maxerr>1e-6f)return 1;
 full.reset();full.process(a.data(),b.data(),x.data(),y.data(),n,0,0,0.75,0.5,true);
 for(int i=0;i<n;++i)if(x[i]!=a[i]||y[i]!=b[i])return 3;
 std::cout<<"PASS: block consistency, finite samples, transparent bypass\n";
}
