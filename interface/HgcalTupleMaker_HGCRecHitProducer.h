#ifndef HgcalTupleMaker_HGCRecHitProducer_h
#define HgcalTupleMaker_HGCRecHitProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_HGCRecHitProducer : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_HGCRecHitProducer(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag rawInputTag;
  const edm::InputTag hgceeRecHitInputTag;
  const edm::InputTag hgchebRecHitInputTag;
  const edm::InputTag hgchefRecHitInputTag;
};

#endif
