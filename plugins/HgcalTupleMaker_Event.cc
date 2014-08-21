#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_Event.h"
#include "FWCore/Framework/interface/Event.h"

HgcalTupleMaker_Event::HgcalTupleMaker_Event(const edm::ParameterSet& iConfig) {
  produces <unsigned int> ( "run"    );
  produces <unsigned int> ( "event"  );
  produces <unsigned int> ( "ls"     );
}

void HgcalTupleMaker_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::auto_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::auto_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );

  iEvent.put( run,   "run"   );
  iEvent.put( event, "event" );
  iEvent.put( ls   , "ls"    );

}
