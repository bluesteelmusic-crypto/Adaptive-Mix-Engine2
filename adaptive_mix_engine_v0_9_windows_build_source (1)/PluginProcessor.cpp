#include <JuceHeader.h>
#include "StereoDSP.h"

class AdaptiveMixStereo final : public juce::AudioProcessor {
public:
 AdaptiveMixStereo():juce::AudioProcessor(BusesProperties().withInput("Stereo In",juce::AudioChannelSet::stereo(),true).withOutput("Stereo Out",juce::AudioChannelSet::stereo(),true)),params(*this,nullptr,"Parameters",layout()){}
 static juce::AudioProcessorValueTreeState::ParameterLayout layout(){
  juce::AudioProcessorValueTreeState::ParameterLayout p;
  auto f=[&](const char* id,const char* name,float min,float max,float def){p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id,1},name,juce::NormalisableRange<float>(min,max,0.01f),def));};
  f("input","Input Gain (dB)",-18,18,0);f("output","Output Gain (dB)",-18,18,0);
  f("low","Low-End Control (dB)",0,3,0.75f);f("strength","Adaptive Strength",0,1,0.5f);
  p.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"bypass",1},"Bypass",false));return p;
 }
 const juce::String getName() const override{return "Adaptive Mix Engine Stereo";}
 bool acceptsMidi() const override{return false;} bool producesMidi() const override{return false;} bool isMidiEffect() const override{return false;}
 double getTailLengthSeconds() const override{return 0;} int getNumPrograms() override{return 1;} int getCurrentProgram() override{return 0;}
 void setCurrentProgram(int) override{} const juce::String getProgramName(int) override{return {};}
 void changeProgramName(int,const juce::String&) override{}
 bool isBusesLayoutSupported(const BusesLayout& l) const override{return l.getMainInputChannelSet()==juce::AudioChannelSet::stereo()&&l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo();}
 void prepareToPlay(double sr,int) override{dsp.prepare(sr);} void releaseResources() override{}
 void processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer&) override{
  juce::ScopedNoDenormals guard;
  if(b.getNumChannels()<2)return;
  // Stereo insert, in-place. DSP reads each sample before writing output.
  dsp.process(b.getReadPointer(0),b.getReadPointer(1),b.getWritePointer(0),b.getWritePointer(1),b.getNumSamples(),
   *params.getRawParameterValue("input"),*params.getRawParameterValue("output"),*params.getRawParameterValue("low"),
   *params.getRawParameterValue("strength"),*params.getRawParameterValue("bypass")>0.5f);
 }
 bool hasEditor() const override{return true;} juce::AudioProcessorEditor* createEditor() override{return new juce::GenericAudioProcessorEditor(*this);}
 void getStateInformation(juce::MemoryBlock& data) override{if(auto xml=params.copyState().createXml())copyXmlToBinary(*xml,data);}
 void setStateInformation(const void* d,int size) override{if(auto xml=getXmlFromBinary(d,size))if(xml->hasTagName(params.state.getType()))params.replaceState(juce::ValueTree::fromXml(*xml));}
private:
 ame::StereoDSP dsp;juce::AudioProcessorValueTreeState params;
};
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new AdaptiveMixStereo();}
