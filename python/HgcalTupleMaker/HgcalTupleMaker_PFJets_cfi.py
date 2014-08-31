import FWCore.ParameterSet.Config as cms


from RecoJets.Configuration.RecoPFJets_cff import ca8PFJetsCHS

ca4PFJetsCHS = ca8PFJetsCHS.clone( rParam = 0.4 )

hgcalTuplePFCA4Jets = cms.EDProducer("HgcalTupleMaker_PFJets",
    InputTag = cms.InputTag('ca4PFJetsCHS'),
    PFCandInputTag = cms.InputTag('particleFlow'),                                  
    Prefix = cms.string('PFCA4Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)

           
myJetProducerSequence   = cms.Sequence ( ca4PFJetsCHS )
myJetTupleMakerSequence = cms.Sequence ( hgcalTuplePFCA4Jets )

