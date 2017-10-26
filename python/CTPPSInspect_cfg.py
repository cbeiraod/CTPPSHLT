import FWCore.ParameterSet.Config as cms

process = cms.Process("CTPPSInspector")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/data/Run2017C/JetHT/RAW/v1/000/299/368/00000/00C1FD8A-ED6B-E711-AFC4-02163E01A51A.root'
    )
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

process.ctppsInspect = cms.EDAnalyzer('CTPPSInspect'
)


process.p = cms.Path(process.ctppsRawToDigi * process.recoCTPPS * process.ctppsInspect)
