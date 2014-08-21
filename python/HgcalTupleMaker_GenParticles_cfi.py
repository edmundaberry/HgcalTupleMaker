import FWCore.ParameterSet.Config as cms

hgcalTupleGenParticles = cms.EDProducer("HgcalTupleMaker_GenParticles",
                                       InputTag = cms.InputTag('genParticles'),
                                       Prefix = cms.string('GenParticle'),
                                       Suffix = cms.string(''),
                                       MaxSize = cms.uint32(25)
                                       )
