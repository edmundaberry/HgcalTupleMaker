#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_Event.h"
#include "FWCore/Framework/interface/Event.h"

HgcalTupleMaker_Event::HgcalTupleMaker_Event(const edm::ParameterSet& iConfig):
  rhoInputTag (iConfig.getParameter<edm::InputTag>("RhoInputTag"))
{
  produces <unsigned int> ( "run"    );
  produces <unsigned int> ( "event"  );
  produces <unsigned int> ( "ls"     );
  produces <double>       ( "rho"    );
}

void HgcalTupleMaker_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rhoInputTag, rhoHandle);
  
  std::auto_ptr<double       >  rho   ( new double      (*rhoHandle)                );
  std::auto_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::auto_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::auto_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );
  
  iEvent.put( run,   "run"   );
  iEvent.put( event, "event" );
  iEvent.put( ls   , "ls"    );
  iEvent.put( rho  , "rho"   );

}
