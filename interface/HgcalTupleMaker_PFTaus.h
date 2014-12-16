#ifndef HgcalTupleMaker_PFTaus_h
#define HgcalTupleMaker_PFTaus_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_PFTaus : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_PFTaus(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const edm::InputTag   decayModeFindingTag;
  const edm::InputTag   looseCombinedIsolationTag;
  const edm::InputTag   muonRejectionTag;
  const std::string     prefix,suffix;
  const unsigned int    maxSize;
};

#endif
