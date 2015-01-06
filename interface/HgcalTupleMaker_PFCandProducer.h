#ifndef HgcalTupleMaker_PFCandProducer_h
#define HgcalTupleMaker_PFCandProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_PFCandProducer : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_PFCandProducer(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag rawInputTag;
  const unsigned int  maxSize;
};

#endif
