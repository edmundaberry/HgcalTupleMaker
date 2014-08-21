#ifndef HgcalTupleMaker_CaloJets_h
#define HgcalTupleMaker_CaloJets_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

class HgcalTupleMaker_CaloJets : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_CaloJets(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
  const unsigned int    maxSize;
};

#endif
