#ifndef HgcalTupleMaker_PFJets_h
#define HgcalTupleMaker_PFJets_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/PFJet.h"

class HgcalTupleMaker_PFJets : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_PFJets(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  double getTau(int num, const reco::PFJet & object);
  const edm::InputTag   inputTag;
  const edm::InputTag   subjetInputTag;
  const edm::InputTag   candInputTag;
  const std::string     prefix,suffix;
  const unsigned int    maxSize;
  bool                  hasSubjets;
  bool                  isSubjets;

};

#endif
