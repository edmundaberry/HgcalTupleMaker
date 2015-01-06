#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_Tree.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_Event.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_GenParticles.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_GenJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFTaus.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFCands.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFClusters.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_CaloJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HgcalRecHits.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_Vertex.h"

#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFCandProducer.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HGCRecHitProducer.h"
#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HGCClusterProducer.h"

DEFINE_FWK_MODULE(HgcalTupleMaker_Tree);
DEFINE_FWK_MODULE(HgcalTupleMaker_Event);
DEFINE_FWK_MODULE(HgcalTupleMaker_GenParticles);
DEFINE_FWK_MODULE(HgcalTupleMaker_GenJets);
DEFINE_FWK_MODULE(HgcalTupleMaker_PFTaus);
DEFINE_FWK_MODULE(HgcalTupleMaker_PFJets);
DEFINE_FWK_MODULE(HgcalTupleMaker_PFCands);
DEFINE_FWK_MODULE(HgcalTupleMaker_PFClusters);
DEFINE_FWK_MODULE(HgcalTupleMaker_CaloJets);
DEFINE_FWK_MODULE(HgcalTupleMaker_HgcalRecHits);
DEFINE_FWK_MODULE(HgcalTupleMaker_Vertex);

DEFINE_FWK_MODULE(HgcalTupleMaker_PFCandProducer);
DEFINE_FWK_MODULE(HgcalTupleMaker_HGCRecHitProducer);
DEFINE_FWK_MODULE(HgcalTupleMaker_HGCClusterProducer);



