#ifndef HgcalTupleMaker_HGCClusterProducer_h
#define HgcalTupleMaker_HGCClusterProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_HGCClusterProducer : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_HGCClusterProducer(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag rawInputTag;
};

#endif
