import FWCore.ParameterSet.Config as cms

hgcalTuplePFTaus = cms.EDProducer("HgcalTupleMaker_PFTaus",
    InputTag = cms.InputTag('hpsPFTauProducer'),
    Prefix = cms.string('PFTau'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10),
    LooseCombinedIsolationTag = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits"),
    DecayModeFindingTag = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
    MuonRejectionTag = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection")
)
