// -*- C++ -*-
//
// Package:    UserCode/CTPPSInspect
// Class:      CTPPSInspect
//
/**\class CTPPSInspect CTPPSInspect.cc UserCode/CTPPSInspect/plugins/CTPPSInspect.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Cristovao da Cruz e Silva
//         Created:  Mon, 09 Oct 2017 21:35:19 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class CTPPSInspect : public edm::one::EDAnalyzer<>  {
   public:
      explicit CTPPSInspect(const edm::ParameterSet&);
      ~CTPPSInspect();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::ParameterSet param_;
      int verbosity_;
      edm::InputTag srcCTPPSPixelCluster_;
      edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelCluster>> tokenCTPPSPixelCluster_;
      //RPixClusterToHit cluster2hit_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CTPPSInspect::CTPPSInspect(const edm::ParameterSet& iConfig):
param_(iConfig)

{
   //now do what ever initialization is needed
   //usesResource("TFileService");
  verbosity_ = conf.getUntrackedParameter<int> ("Verbosity");

  srcCTPPSPixelCluster_ = conf.getParameter<edm::InputTag>("RPixClusterTag");

  tokenCTPPSPixelCluster_ = consumes<edm::DetSetVector<CTPPSPixelCluster> >(src_);

}


CTPPSInspect::~CTPPSInspect()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CTPPSInspect::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<edm::DetSetVector<CTPPSPixelCluster> > rpCl;
   iEvent.getByToken(tokenCTPPSPixelCluster_, rpCl);

   std::cout << "There are " << rpCl.size() << " clusters." << std::endl;

   for (const auto &ds_cluster : input)
   {
    std::cout << ds_cluster << std::endl;
    std::cout << ds_cluster.id << std::endl;
   }



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
CTPPSInspect::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
CTPPSInspect::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CTPPSInspect::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("Verbosity",0);
  desc.add<edm::InputTag>("RPixClusterTag",edm::InputTag("ctppsPixelClusters"));
  descriptions.add("ctppsInspector", desc);
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSInspect);
