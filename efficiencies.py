import FWCore.ParameterSet.Config as cms
#from Configuration.StandardSequences.Eras import eras

# If runnint on MC os data, set the flag below accordingly
isMC = False

#process = cms.Process("Testing", eras.ctpps_2016)
process = cms.Process("Efficiency")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
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

# Local track filter
process.load("UserCode.CTPPSHLT.hltCTPPSLocalTrackFilter_cfi")

# xi Producer
process.load("UserCode.CTPPSHLT.CTPPSXiAnalyzer_cfi")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( "out.root" )
                                   )

if isMC:
  process.p = cms.Path(process.recoCTPPS * process.hltCTPPSLocalTrackFilter)
else:
  process.p = cms.Path(process.ctppsRawToDigi * process.recoCTPPS * process.ctppsXiAnalyzer * process.hltCTPPSLocalTrackFilter)
