#ifndef HgcalTupleMakerVertex
#define HgcalTupleMakerVertex

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HgcalTupleMaker_Vertex : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_Vertex(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
