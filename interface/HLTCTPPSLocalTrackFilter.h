// <author>Cristovao Beirao da Cruz e Silva</author>
// <email>cbeiraod@cern.ch</email>
// <created>2017-10-26</created>
// <description>
// </description>

#ifndef HLTCTPPSLocalTrackFilter_h
#define HLTCTPPSLocalTrackFilter_h

#include "HLTrigger/HLTcore/interface/HLTFilter.h"

#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

class HLTCTPPSLocalTrackFilter : public HLTFilter
{
public:
  explicit HLTCTPPSLocalTrackFilter(const edm::ParameterSet&);
  ~HLTCTPPSLocalTrackFilter() override;

  static void fillDescriptions(edm::ConfigurationDescriptions&);
  bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs&) const override;

private:
  edm::ParameterSet param_;

  edm::InputTag pixelLocalTrackInputTag_; // Input tag identifying the pixel detector
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelLocalTrackToken_;

  edm::InputTag stripLocalTrackInputTag_; // Input tag identifying the strip detector
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelCluster>> stripLocalTrackToken_;

  edm::InputTag diamondLocalTrackInputTag_; // Input tag identifying the diamond detector
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelCluster>> diamondLocalTrackToken_;

  unsigned int detectorBitset_;

  int minTracks_;
  int minTracksPerArm_;

  int triggerType_;

  bool usePixel_;
  bool useStrip_;
  bool useDiamond_;

protected:
}

#endif
