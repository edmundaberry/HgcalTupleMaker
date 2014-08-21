#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

double JetTools::getTau(int num, const reco::PFJet & object) {
    
  unsigned n_particles = object.nConstituents();
  std::vector<fastjet::PseudoJet> FJparticles;
  
  for (unsigned particle = 0; particle < n_particles; particle++){
    const reco::PFCandidate *thisParticle = object.getPFConstituent(particle).get();
    FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
  }
  
  double nsubjettiness_cone = 0.1;
  
  NsubParameters paraNsub = NsubParameters(1.0, nsubjettiness_cone); //assume R=0.7 jet clusering used
  Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
  return double(routine.getTau(num, FJparticles));

}

