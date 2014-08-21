#ifndef HgcalTupleMaker_JetTools_h
#define HgcalTupleMaker_JetTools_h

#include "DataFormats/JetReco/interface/PFJet.h"
#include "HGCALANA/HgcalTupleMaker/interface/Njettiness.hh"

namespace JetTools { 

  double getTau(int num, const reco::PFJet & object);

};

#endif 
