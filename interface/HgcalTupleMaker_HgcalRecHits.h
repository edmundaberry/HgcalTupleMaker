#ifndef HgcalTupleMaker_HgcalRecHits_h
#define HgcalTupleMaker_HgcalRecHits_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_HgcalRecHits : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_HgcalRecHits(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     geoTag;
  const std::string     prefix,suffix;
  const bool            isHGEE;
};

#endif
