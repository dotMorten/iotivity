//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <OCPlatform.h>
#include <OCApi.h>
#include <oic_malloc.h>
#include <gtest/gtest.h>

namespace OCPlatformTest
{
    using namespace OC;

    static const char* SVR_DB_FILE_NAME = "./oic_svr_db_server.dat";
    const OCResourceHandle HANDLE_ZERO = 0;
    const std::string gResourceTypeName = "core.res";
    const std::string gResourceInterface = DEFAULT_INTERFACE;
    const uint8_t gResourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
    OCResourceHandle resourceHandle;

    //OCPersistent Storage Handlers
    static FILE* client_open(const char * /*path*/, const char *mode)
    {
        std::cout << "<===Opening SVR DB file = './oic_svr_db_client.dat' with mode = '" << mode
                << "' " << std::endl;
        return fopen(SVR_DB_FILE_NAME, mode);
    }
    OCPersistentStorage gps {client_open, fread, fwrite, fclose, unlink };

    // Callbacks
    OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> /*request*/)
    {
        return OC_EH_OK;
    }

    void foundResource(std::shared_ptr<OCResource> /*resource*/)
    {
    }

    void receivedDeviceInfo(const OCRepresentation& /*rep*/)
    {
    }

    void presenceHandler(OCStackResult /*result*/,
            const unsigned int /*nonce*/, const std::string& /*hostAddress*/)
    {
    }

#ifdef WITH_CLOUD
    void onObserve(const HeaderOptions, const OCRepresentation&, const int&, const int&)
    {
    }
#endif

    void directPairHandler(std::shared_ptr<OCDirectPairing> /*dev*/, OCStackResult /*res*/)
    {
    }

    void pairedHandler(const PairedDevices& /*list*/)
    {
    }

    //Helper methods
    void DeleteStringLL(OCStringLL* ll)
    {
        if (!ll)
        {
            return;
        }

        DeleteStringLL(ll->next);
        delete[] ll->value;
        OICFree(ll);
    }

    void DeleteDeviceInfo(OCDeviceInfo deviceInfo)
    {
        delete[] deviceInfo.deviceName;
        DeleteStringLL(deviceInfo.types);
        delete[] deviceInfo.specVersion;
        DeleteStringLL(deviceInfo.dataModelVersions);
    }

    void DuplicateString(char ** targetString, std::string sourceString)
    {
        *targetString = new char[sourceString.length() + 1];
        strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
    }

    bool OCResourcePayloadAddStringLL(OCStringLL **stringLL, std::string value)
    {
        if (!stringLL)
        {
            return false;
        }

        char *dup = NULL;
        DuplicateString(&dup, value);
        if (!*stringLL)
        {
            *stringLL = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
            (*stringLL)->value = dup;
            return true;
        }
        else
        {
            OCStringLL *temp = *stringLL;
            while(temp->next)
            {
                temp = temp->next;
            }
            temp->next = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
            temp->next->value = dup;
            return true;
        }
    }

    OCResourceHandle RegisterResource(std::string uri, std::string type, std::string iface)
    {
        PlatformConfig cfg
        { OC::ServiceType::InProc, OC::ModeType::Server, "0.0.0.0", 0,
                OC::QualityOfService::LowQos, &gps };
        OCPlatform::Configure(cfg);
        EXPECT_EQ(OC_STACK_OK,OCPlatform::registerResource(
                                        resourceHandle, uri, type,
                                        iface, entityHandler, gResourceProperty));
        return resourceHandle;
    }

    OCResourceHandle RegisterResource(std::string uri, std::string type)
    {
        PlatformConfig cfg
        { OC::ServiceType::InProc, OC::ModeType::Server, "0.0.0.0", 0,
                OC::QualityOfService::LowQos, &gps };
        OCPlatform::Configure(cfg);
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        resourceHandle, uri, type,
                                        gResourceInterface, entityHandler, gResourceProperty));
        return resourceHandle;
    }

    OCResourceHandle RegisterResource(std::string uri)
    {
        PlatformConfig cfg
        { OC::ServiceType::InProc, OC::ModeType::Server, "0.0.0.0", 0,
                OC::QualityOfService::LowQos, &gps };
        OCPlatform::Configure(cfg);
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        resourceHandle, uri, gResourceTypeName,
                                        gResourceInterface, entityHandler, gResourceProperty));
        return resourceHandle;
    }

    //Configure
    // Enable it when the stack throw an exception
    // https://jira.iotivity.org/browse/IOT-428
    TEST(ConfigureTest, DISABLED_ConfigureInvalidModeType)
    {
        PlatformConfig cfg {
             OC::ServiceType::InProc,
            (OC::ModeType)99,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos
         };
         OCPlatform::Configure(cfg);
         EXPECT_ANY_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }

    // Enable it when the stack throw an exception
    // https://jira.iotivity.org/browse/IOT-428
    TEST(ConfigureTest, DISABLED_ConfigureInvalidServiceType)
    {
        PlatformConfig cfg {
             (OC::ServiceType)99,
            OC::ModeType::Client,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos
         };
         OCPlatform::Configure(cfg);
         EXPECT_ANY_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }

    // Enable it when the stack throw an exception
    // https://jira.iotivity.org/browse/IOT-428
    TEST(ConfigureTest, DISABLED_ConfigureClientOutProc)
    {
        PlatformConfig cfg {
            OC::ServiceType::OutOfProc,
            OC::ModeType::Client,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos
        };
        std::string uri = "/a/light66";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        OCPlatform::Configure(cfg);
        EXPECT_ANY_THROW(OCPlatform::registerResource(
             resourceHandle, uri, type,
             gResourceInterface, entityHandler, gResourceProperty));
    }

    TEST(ConfigureTest, ConfigureServerOutProc)
    {
        PlatformConfig cfg
        {
            OC::ServiceType::OutOfProc,
            OC::ModeType::Server,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos, &gps
        };
        std::string uri = "/a/light67";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        OCPlatform::Configure(cfg);

        EXPECT_ANY_THROW(OCPlatform::registerResource(
             resourceHandle, uri, type,
             gResourceInterface, entityHandler, gResourceProperty));
    }

    TEST(ConfigureTest, ConfigureDefault)
    {
        std::string uri = "/a/light68";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        PlatformConfig cfg = {};
        OCPlatform::Configure(cfg);

        EXPECT_NO_THROW(OCPlatform::registerResource(
                 resourceHandle, uri, type,
                 gResourceInterface, entityHandler, gResourceProperty));
    }

    TEST(ConfigureTest, ConfigureServer)
    {
        std::string uri = "/a/light69";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        PlatformConfig cfg {
            OC::ServiceType::InProc,
            OC::ModeType::Server,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos, &gps
        };
        OCPlatform::Configure(cfg);

        EXPECT_NO_THROW(OCPlatform::registerResource(
                 resourceHandle, uri, type,
                 gResourceInterface, entityHandler, gResourceProperty));
    }

    TEST(ConfigureTest, ConfigureClient)
    {
        std::string uri = "/a/light70";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        PlatformConfig cfg
        {
            OC::ServiceType::InProc,
            OC::ModeType::Client,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos,
            &gps
        };

        EXPECT_NO_THROW(OCPlatform::registerResource(
                 resourceHandle, uri, type,
                 gResourceInterface, entityHandler, gResourceProperty));
    }
    //PersistentStorageTest
    TEST(ConfigureTest, ConfigureDefaultNULLPersistentStorage)
    {
        PlatformConfig cfg {
             OC::ServiceType::InProc,
             OC::ModeType::Both,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos
         };
         OCPlatform::Configure(cfg);
         EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }

    //PersistentStorageTest
    TEST(ConfigureTest, ConfigureNULLPersistentStorage)
    {
        PlatformConfig cfg {
             OC::ServiceType::InProc,
             OC::ModeType::Both,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos,
             nullptr
         };
         OCPlatform::Configure(cfg);
         EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }

    //PersistentStorageTest
    TEST(ConfigureTest, ConfigurePersistentStorage)
    {
         PlatformConfig cfg {
             OC::ServiceType::InProc,
             OC::ModeType::Both,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos,
             &gps
         };
         OCPlatform::Configure(cfg);
         EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }

    //PersistentStorageTest
    TEST(ConfigureTest, ConfigureNULLHandlersPersistentStorage)
    {
        PlatformConfig cfg {
             OC::ServiceType::InProc,
             OC::ModeType::Both,
             "0.0.0.0",
             0,
             OC::QualityOfService::LowQos,
             &gps
         };
         OCPlatform::Configure(cfg);
         EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
     }


    //RegisterResourceTest
    TEST(RegisterResourceTest, RegisterSingleResource)
    {
        std::string uri = "/a/res2";
        EXPECT_NE(HANDLE_ZERO, RegisterResource(uri));
    }

    TEST(RegisterResourceTest, RegisterMultipleResources)
    {
        std::string uri = "/a/multi";
        //Good enough for 5 resources.
        for(int i=0; i< 5; i++)
        {
            uri +=std::to_string(i);
            EXPECT_NE(HANDLE_ZERO, RegisterResource(uri));
        }
    }

    TEST(RegisterResourceTest, ReregisterResource)
    {
        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light5"),
            std::string("core.light"));
        EXPECT_EQ(OC_STACK_OK, OC::OCPlatform::unregisterResource(resourceHandle));

        EXPECT_NE(HANDLE_ZERO, RegisterResource(std::string("/a/light5"),
            std::string("core.light")));

    }

    TEST(RegisterResourceTest, RegisterEmptyResource)
    {
        // We should not allow empty URI.
        std::string emptyStr = "";
        EXPECT_ANY_THROW(OCPlatform::registerResource(resourceHandle, emptyStr, emptyStr,
                                        emptyStr, entityHandler, gResourceProperty));
    }

    TEST(RegisterResourceTest, RegisterZeroResourceProperty)
    {
        std::string uri = "/a/light6";
        std::string type = "core.light";
        uint8_t gResourceProperty = 0;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                resourceHandle, uri, type,
                gResourceInterface, entityHandler, gResourceProperty));
    }

    //UnregisterTest
    TEST(UnregisterTest, UnregisterZeroHandleValue)
    {
        EXPECT_ANY_THROW(OC::OCPlatform::unregisterResource(HANDLE_ZERO));
    }

    //UnbindResourcesTest
    TEST(UnbindResourcesTest, UnbindResources)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("a/home"),
            std::string("core.home"));
        OCResourceHandle resourceKitchen = RegisterResource(std::string("a/kitchen"),
            std::string("core.kitchen"), LINK_INTERFACE);
        OCResourceHandle resourceRoom = RegisterResource(std::string("a/office"),
            std::string("core.office"), LINK_INTERFACE);

        std::vector<OCResourceHandle> rList;
        rList.push_back(resourceKitchen);
        rList.push_back(resourceRoom);
        EXPECT_EQ(OC_STACK_OK, OCPlatform::bindResources(resourceHome, rList));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unbindResources(resourceHome, rList));
    }

    TEST(UnbindResourcesTest, UnbindResourcesWithZero)
    {
        OCResourceHandle resourceHandle1 = 0;
        OCResourceHandle resourceHandle2 = 0;
        OCResourceHandle resourceHandle3 = 0;

        std::vector<OCResourceHandle> rList;

        rList.push_back(resourceHandle2);
        rList.push_back(resourceHandle3);

        EXPECT_ANY_THROW(OCPlatform::unbindResources(resourceHandle1, rList));
    }

    //BindInterfaceToResourceTest
    TEST(BindInterfaceToResourceTest, BindResourceInterface)
    {
        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light"),
            std::string("core.light"));
        OCStackResult result = OC::OCPlatform::bindInterfaceToResource(resourceHandle,
            BATCH_INTERFACE);
        EXPECT_EQ(OC_STACK_OK, result);
    }
    
#if defined (_MSC_VER)
    TEST(BindInterfaceToResourceTest, DISABLED_BindZeroResourceInterface)
#else
    TEST(BindInterfaceToResourceTest, BindZeroResourceInterface)
#endif
    {
        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light1"),
            std::string("core.light"));
        EXPECT_ANY_THROW(OC::OCPlatform::bindInterfaceToResource(resourceHandle, 0));
    }

    //BindTypeToResourceTest
    TEST(BindTypeToResourceTest, BindResourceType)
    {
        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light3"),
            std::string("core.light"));
        OCStackResult result = OC::OCPlatform::bindTypeToResource(resourceHandle,
            "core.brightlight");
        EXPECT_EQ(OC_STACK_OK, result);
    }
    
#if defined (_MSC_VER)
    TEST(BindTypeToResourceTest, DISABLED_BindZeroResourceType)
#else
    TEST(BindTypeToResourceTest, BindZeroResourceType)
#endif
    {
        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light4"),
            std::string("core.light"));
        EXPECT_ANY_THROW(OC::OCPlatform::bindTypeToResource(resourceHandle, 0));
    }

    //UnbindResourceTest
    TEST(UnbindResourceTest, BindAndUnbindResource)
    {
        OCResourceHandle resourceHandle1 = RegisterResource(std::string("a/unres"),
            std::string("core.unres"));
        OCResourceHandle resourceHandle2 = RegisterResource(std::string("a/unres2"),
            std::string("core.unres"), LINK_INTERFACE);

        EXPECT_EQ(OC_STACK_OK, OCPlatform::bindResource(resourceHandle1, resourceHandle2));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unbindResource(resourceHandle1, resourceHandle2));
    }

    //PresenceTest
    TEST(PresenceTest, DISABLED_StartAndStopPresence)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::startPresence(30));
        EXPECT_NE(HANDLE_ZERO, RegisterResource( std::string("/a/Presence"),
            std::string("core.Presence")));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::stopPresence());
    }

    TEST(OCPlatformTest, UnbindZeroRsourceHandleValue)
    {
        EXPECT_ANY_THROW(OCPlatform::unbindResource(HANDLE_ZERO, HANDLE_ZERO));
    }

    //NotifyAllObserverTest
    TEST(NotifyAllObserverTest, NotifyAllObservers)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs1"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithLowQos)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs2"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::LowQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithMidQos)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs3"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::MidQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithNaQos)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs4"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::NaQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithHighQos)
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs5"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::HighQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObservers)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObservers)
#endif
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs6"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithLowQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithLowQos)
#endif
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs7"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::LowQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithMidQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithMidQos)
#endif
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs8"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::MidQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithNaQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithNaQos)
#endif
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs9"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::NaQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithHighQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithHighQos)
#endif
    {
        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs10"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::HighQos));
    }

    //DeviceEntityHandlerTest
    TEST(DeviceEntityHandlerTest, SetDefaultDeviceEntityHandler)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setDefaultDeviceEntityHandler(entityHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setDefaultDeviceEntityHandler(nullptr));
    }


    //FindResource test
    TEST(FindResourceTest, DISABLED_FindResourceValid)
    {
      std::ostringstream requestURI;
      requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
      EXPECT_EQ(OC_STACK_OK, OCPlatform::findResource("", requestURI.str(),
              CT_DEFAULT, &foundResource));
    }

#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullResourceURI)
#else
    TEST(FindResourceTest, FindResourceNullResourceURI)
#endif
    {
      EXPECT_ANY_THROW(OCPlatform::findResource("", nullptr,
              CT_DEFAULT, &foundResource));
    }


#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullResourceURI1)
#else
    TEST(FindResourceTest, FindResourceNullResourceURI1)
#endif
    {
      std::ostringstream requestURI;
      requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
      EXPECT_ANY_THROW(OCPlatform::findResource(nullptr, requestURI.str(),
              CT_DEFAULT, &foundResource));
    }

#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullHost)
#else
    TEST(FindResourceTest, FindResourceNullHost)
#endif
    {
      std::ostringstream requestURI;
      requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
      EXPECT_ANY_THROW(OCPlatform::findResource(nullptr, requestURI.str(),
              CT_DEFAULT, &foundResource));
    }

    TEST(FindResourceTest, FindResourceNullresourceHandler)
    {
      std::ostringstream requestURI;
      requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
      EXPECT_THROW(OCPlatform::findResource("", requestURI.str(),
              CT_DEFAULT, nullptr), OC::OCException);
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithLowQoS)
    {
        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::LowQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithMidQos)
    {
        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::MidQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithHighQos)
    {
        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::HighQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithNaQos)
    {
        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::NaQos));
    }

    //GetDeviceInfo Test
    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithValidParameters)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo));
    }

#if defined (_MSC_VER)
    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoNullDeviceURI)
#else
    TEST(GetDeviceInfoTest, GetDeviceInfoNullDeviceURI)
#endif
    {
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        EXPECT_ANY_THROW(
                OCPlatform::getDeviceInfo("", nullptr, CT_DEFAULT, &receivedDeviceInfo));
    }

    TEST(GetDeviceInfoTest, GetDeviceInfoWithNullDeviceInfoHandler)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_THROW(
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, nullptr),
                OC::OCException);
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithLowQos)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::LowQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithMidQos)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::MidQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithHighQos)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::HighQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithNaQos)
    {
        std::string deviceDiscoveryURI = "/oic/d";
        PlatformConfig cfg;
        OCPlatform::Configure(cfg);
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::NaQos));
    }

    //RegisterDeviceInfo test
    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithValidParameters)
    {
        OCDeviceInfo deviceInfo;
        DuplicateString(&deviceInfo.deviceName, "myDeviceName");
        deviceInfo.types = NULL;
        OCResourcePayloadAddStringLL(&deviceInfo.types, "oic.wk.d");
        OCResourcePayloadAddStringLL(&deviceInfo.types, "oic.d.tv");
        DuplicateString(&deviceInfo.specVersion, "mySpecVersion");
        deviceInfo.dataModelVersions = nullptr;
        OCResourcePayloadAddStringLL(&deviceInfo.dataModelVersions, "myDataModelVersions");
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerDeviceInfo(deviceInfo));
        EXPECT_NO_THROW(DeleteDeviceInfo(deviceInfo));
    }

    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithEmptyObject)
    {
        OCDeviceInfo di = {0, 0, 0, 0};
        EXPECT_ANY_THROW(OCPlatform::registerDeviceInfo(di));
    }

    //SubscribePresence Test
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithValidParameters)
    {
        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle, hostAddress,
                 CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullHost)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullHost)
#endif
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle, nullptr,
                 CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullPresenceHandler)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullPresenceHandler)
#endif
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle, nullptr,
                 CT_DEFAULT, nullptr));
    }

    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithResourceType)
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, "core.light", CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullResourceType)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullResourceType)
#endif
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, nullptr, CT_DEFAULT, &presenceHandler));
    }

    TEST(SubscribePresenceTest, DISABLED_UnsubscribePresenceWithValidHandleAndRT)
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, "core.light", CT_DEFAULT, &presenceHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }

    TEST(SubscribePresenceTest, UnsubscribePresenceWithNullHandle)
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        EXPECT_ANY_THROW(OCPlatform::unsubscribePresence(presenceHandle));
    }

    TEST(SubscribePresenceTest, DISABLED_UnsubscribePresenceWithValidHandle)
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, CT_DEFAULT, &presenceHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }

#ifdef WITH_CLOUD
    // SubscribeDevicePresence Test
    TEST(SubscribeDevicePresenceTest, DISABLED_SubscribeDevicePresenceWithValidParameters)
    {
        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribeDevicePresence(presenceHandle,
                hostAddress, di, CT_DEFAULT, &onObserve));
    }

    TEST(SubscribeDevicePresenceTest, SubscribeDevicePresenceWithNullHost)
    {
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_ANY_THROW(OCPlatform::subscribeDevicePresence(presenceHandle,
                        nullptr, di, CT_DEFAULT, &onObserve));
    }

    TEST(SubscribeDevicePresenceTest, SubscribeDevicePresenceWithNullOnObserve)
    {
        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_ANY_THROW(OCPlatform::subscribeDevicePresence(presenceHandle,
                        hostAddress, di, CT_DEFAULT, nullptr));
    }

    TEST(SubscribeDevicePresenceTest, DISABLED_UnsubscribePresenceWithValidHandle)
    {
        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribeDevicePresence(presenceHandle,
                hostAddress, di, CT_DEFAULT, &onObserve));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }
#endif

    TEST(FindDirectPairingTest, FindDirectPairingNullCallback)
    {
        EXPECT_ANY_THROW(OCPlatform::findDirectPairingDevices(1, nullptr));
    }

    TEST(FindDirectPairingTest, FindDirectPairingZeroTimeout)
    {
        EXPECT_ANY_THROW(OCPlatform::findDirectPairingDevices(0, &pairedHandler));
    }

    TEST(GetDirectPairedTest, GetDirectPairedNullCallback)
    {
        EXPECT_ANY_THROW(OCPlatform::getDirectPairedDevices(nullptr));
    }

    TEST(DoDirectPairingTest, DoDirectPairingNullCallback)
    {
        OCDPDev_t peer;
        OCPrm_t pmSel = DP_PRE_CONFIGURED;
        std::string pin("");
        std::shared_ptr<OCDirectPairing> s_dp(new OCDirectPairing(&peer));
        EXPECT_ANY_THROW(OCPlatform::doDirectPairing(s_dp, pmSel, pin, nullptr));
    }

    TEST(DoDirectPairingTest, DoDirectPairingNullPeer)
    {
        OCDPDev_t peer;
        OCPrm_t pmSel = DP_PRE_CONFIGURED;
        std::string pin("");
        std::shared_ptr<OCDirectPairing> s_dp(new OCDirectPairing(&peer));
        EXPECT_ANY_THROW(OCPlatform::doDirectPairing(nullptr, pmSel, pin, &directPairHandler));
    }

    TEST(DoDirectPairingTest, DoDirectPairingNullPeerNullCallback)
    {
        OCDPDev_t peer;
        OCPrm_t pmSel = DP_PRE_CONFIGURED;
        std::string pin("");
        std::shared_ptr<OCDirectPairing> s_dp(new OCDirectPairing(&peer));
        EXPECT_ANY_THROW(OCPlatform::doDirectPairing(nullptr, pmSel, pin, nullptr));
    }
}
