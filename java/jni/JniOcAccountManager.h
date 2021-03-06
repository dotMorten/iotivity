/*
 *******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef _Included_org_iotivity_base_OcAccountManager
#define _Included_org_iotivity_base_OcAccountManager

#include "JniOcStack.h"
#include "OCAccountManager.h"
#include "JniListenerManager.h"
#include "JniOnGetListener.h"
#include "JniOnPostListener.h"
#include "JniOnDeleteListener.h"
#include "JniOnObserveListener.h"

using namespace OC;

class JniOcAccountManager
{
public:
    JniOcAccountManager(std::shared_ptr<OCAccountManager> accountManager);
    ~JniOcAccountManager();

    std::string host();
    OCConnectivityType connectivityType() const;

    static JniOcAccountManager* getJniOcAccountManagerPtr(JNIEnv *env, jobject thiz);

    OCStackResult signUp(JNIEnv* env, const std::string& authProvider, const std::string& authCode,
                         jobject jListener);
    OCStackResult signUp(JNIEnv* env, const std::string& authProvider, const std::string& authCode,
                         const QueryParamsMap& options, jobject jListener);
    OCStackResult signIn(JNIEnv* env, const std::string& userUuid, const std::string& accessToken,
                         jobject jListener);
    OCStackResult signOut(JNIEnv* env, const std::string& accessToken, jobject jListener);
    OCStackResult refreshAccessToken(JNIEnv* env, const std::string& userUuid,
                                     const std::string& refreshToken, jobject jListener);
    OCStackResult searchUser(JNIEnv* env, const QueryParamsMap& queryMap, jobject jListener);
    OCStackResult deleteDevice(JNIEnv* env, const std::string& accessToken,
                               const std::string& deviceId, jobject jListener);
    OCStackResult createGroup(JNIEnv* env, jobject jListener);
    OCStackResult createGroup(JNIEnv* env, const QueryParamsMap& queryMap, jobject jListener);
    OCStackResult deleteGroup(JNIEnv* env, const std::string& groupId, jobject jListener);
    OCStackResult getGroupInfoAll(JNIEnv* env, jobject jListener);
    OCStackResult getGroupInfo(JNIEnv* env, const std::string& groupId, jobject jListener);
    OCStackResult addPropertyValueToGroup(JNIEnv* env, const std::string& groupId,
                                          const OCRepresentation& propertyValue, jobject jListener);
    OCStackResult deletePropertyValueFromGroup(JNIEnv* env, const std::string& groupId,
                                               const OCRepresentation& propertyValue, jobject jListener);
    OCStackResult updatePropertyValueOnGroup(JNIEnv* env, const std::string& groupId,
                                             const OCRepresentation& propertyValue, jobject jListener);
    OCStackResult observeGroup(JNIEnv* env, jobject jListener);
    OCStackResult cancelObserveGroup();
    OCStackResult observeInvitation(JNIEnv* env, jobject jListener);
    OCStackResult cancelObserveInvitation();
    OCStackResult sendInvitation(JNIEnv* env, const std::string& groupId,
                                 const std::string& userUuid, jobject jListener);
    OCStackResult cancelInvitation(JNIEnv* env, const std::string& groupId,
                                   const std::string& userUuid, jobject jListener);
    OCStackResult replyToInvitation(JNIEnv* env, const std::string& groupId, const bool accept,
                                    jobject jListener);

    JniOnGetListener* addOnGetListener(JNIEnv* env, jobject jListener);
    JniOnPostListener* addOnPostListener(JNIEnv* env, jobject jListener);
    JniOnDeleteListener* addOnDeleteListener(JNIEnv* env, jobject jListener);
    JniOnObserveListener* addOnObserveListener(JNIEnv* env, jobject jListener);

    void removeOnGetListener(JNIEnv* env, jobject jListener);
    void removeOnPostListener(JNIEnv* env, jobject jListener);
    void removeOnDeleteListener(JNIEnv* env, jobject jListener);
    void removeOnObserveListener(JNIEnv* env, jobject jListener);

private:
    JniListenerManager<JniOnGetListener> m_onGetManager;
    JniListenerManager<JniOnPostListener> m_onPostManager;
    JniListenerManager<JniOnDeleteListener> m_onDeleteManager;
    JniListenerManager<JniOnObserveListener> m_onObserveManager;

    std::shared_ptr<OCAccountManager> m_sharedAccountManager;
};

/* DO NOT EDIT THIS FILE BEYOND THIS LINE - it is machine generated */

#ifdef __cplusplus
extern "C" {
#endif

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    getHost
    * Signature: ()Ljava/lang/String;
    */
    JNIEXPORT jstring JNICALL Java_org_iotivity_base_OcAccountManager_getHost
        (JNIEnv *, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    getConnectivityTypeN
    * Signature: ()I
    */
    JNIEXPORT jint JNICALL Java_org_iotivity_base_OcAccountManager_getConnectivityTypeN
        (JNIEnv *, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    signUp0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signUp0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    signUp1
    * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signUp1
        (JNIEnv *, jobject, jstring, jstring, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    signIn0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signIn0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    signOut0
    * Signature: (Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signOut0
        (JNIEnv *, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    refreshAccessToken0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_refreshAccessToken0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    searchUser0
    * Signature: (Ljava/util/Map;Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_searchUser0
        (JNIEnv *, jobject, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    deleteDevice0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deleteDevice0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    createGroup0
    * Signature: (Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_createGroup0
        (JNIEnv *, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    createGroup1
    * Signature: (Ljava/util/Map;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_createGroup1
        (JNIEnv *, jobject, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    deleteGroup0
    * Signature: (Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deleteGroup0
        (JNIEnv *, jobject, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    getGroupInfoAll0
    * Signature: (Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_getGroupInfoAll0
        (JNIEnv *, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    getGroupInfo0
    * Signature: (Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_getGroupInfo0
        (JNIEnv *, jobject, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    addPropertyValueToGroup0
    * Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
    *             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_addPropertyValueToGroup0
        (JNIEnv *, jobject, jstring, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    deletePropertyValueFromGroup0
    * Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
    *             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deletePropertyValueFromGroup0
        (JNIEnv *, jobject, jstring, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    updatePropertyValueOnGroup0
    * Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
    *             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_updatePropertyValueOnGroup0
        (JNIEnv *, jobject, jstring, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    observeGroup0
    * Signature: (Lorg/iotivity/base/OcResource/OnObserveListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_observeGroup0
        (JNIEnv *, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    cancelObserveGroup0
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelObserveGroup0
        (JNIEnv *, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    observeInvitation0
    * Signature: (Lorg/iotivity/base/OcResource/OnObserveListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_observeInvitation0
        (JNIEnv *, jobject, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    cancelObserveInvitation0
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelObserveInvitation0
        (JNIEnv *, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    sendInvitation0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_sendInvitation0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    cancelInvitation0
    * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelInvitation0
        (JNIEnv *, jobject, jstring, jstring, jobject);

    /*
    * Class:     org_iotivity_base_OcAccountManager
    * Method:    replyToInvitation0
    * Signature: (Ljava/lang/String;ZLorg/iotivity/base/OcAccountManager/onDeleteListener;)V
    */
    JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_replyToInvitation0
        (JNIEnv *, jobject, jstring, jboolean, jobject);

#ifdef __cplusplus
}
#endif
#endif
