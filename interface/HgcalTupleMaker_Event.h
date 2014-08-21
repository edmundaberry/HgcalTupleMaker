#ifndef HgcalTupleMaker_Event_h
#define HgcalTupleMaker_Event_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HgcalTupleMaker_Event : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_Event(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
