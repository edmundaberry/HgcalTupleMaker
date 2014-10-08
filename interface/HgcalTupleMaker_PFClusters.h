#ifndef HgcalTupleMaker_PFClusters_h
#define HgcalTupleMaker_PFClusters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_PFClusters : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_PFClusters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const edm::InputTag   recHitInputTag;
  const std::string     prefix,suffix;
};

#endif
