#ifndef HgcalTupleMaker_PFCands_h
#define HgcalTupleMaker_PFCands_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_PFCands : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_PFCands(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const edm::InputTag hgceeInputTag ;
  const edm::InputTag hgchefInputTag;
  const edm::InputTag hgchebInputTag;
  const std::string   prefix,suffix;

};

#endif
