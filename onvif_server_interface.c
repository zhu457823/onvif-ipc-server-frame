/**@file    onvif_server_interface.c
 * @note    Hangzhou Hikvision Automotive Technology Co., Ltd. All Right Reserved.
 * @brief   onvif profile S Specification v1.0版本,Date Dec.2011
 *
 * @author  zhujinlin
 * @date    2020-2-11
 * @version V1.0
 *
 * @note History:
 * @note 2020-2-11 zjl 实现onvif server定义的函数接口
 */


#include "wsaapi.h"
#include "soapH.h"
#include "soapStub.h"
#include "common.h"
//#include "wsdd.nsmap"


/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, 
						char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, 
						struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role,
						struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye)
{	
	return SOAP_OK;
}

static char g_uuid[64] = { 0 };
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe)
{
    printf("---------------------------------------------\n");
    char                            ip_addr[32] = { 0 };
    char                            mac_addr[13] = { 0 };
    struct wsdd__ScopesType* pScopes = NULL;
    char                            str_tmp[256] = { 0 };

    char scopes_message[] =
        "onvif://www.onvif.org/type/NetworkVideoTransmitter\r\n"
        "onvif://www.onvif.org/Profile/Streaming\r\n"
        "onvif://www.onvif.org/Profile/Q/Operational\r\n"
        "onvif://www.onvif.org/hardware/HD1080P\r\n"
        "onvif://www.onvif.org/name/discover_test\r\n"
        "onvif://www.onvif.org/location/city/HangZhou\r\n"
        "onvif://www.onvif.org/location/country/China\r\n";

    sprintf(ip_addr, "%u.%u.%u.%u", ((soap->ip) >> 24) & 0xFF, ((soap->ip) >> 16) & 0xFF, ((soap->ip) >> 8) & 0xFF, (soap->ip) & 0xFF);
    sprintf(mac_addr, "000c29c9338f");

    // verify scropes
    if (wsdd__Probe->Scopes && wsdd__Probe->Scopes->__item)
    {
        if (wsdd__Probe->Scopes->MatchBy)
        {
        }
        else
        {
        }
    }

    // response ProbeMatches
    struct wsdd__ProbeMatchesType   wsdd__ProbeMatches = { 0 };
    struct wsdd__ProbeMatchType* pProbeMatchType = NULL;
    struct wsa__Relationship* pWsa__RelatesTo = NULL;
    char* pMessageID = NULL;

    pProbeMatchType = (struct wsdd__ProbeMatchType*)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));
    soap_default_wsdd__ProbeMatchType(soap, pProbeMatchType);

    //sprintf(str_tmp, "http://%s/onvif/device_service", ip_addr);
    sprintf(str_tmp, "http://%s:%d/onvif/device_service", ONVIF_TCP_IP, ONVIF_TCP_PORT);
    pProbeMatchType->XAddrs = soap_strdup(soap, str_tmp);
    if (wsdd__Probe->Types && strlen(wsdd__Probe->Types))
        pProbeMatchType->Types = soap_strdup(soap, wsdd__Probe->Types);
    else
        pProbeMatchType->Types = soap_strdup(soap, "dn:NetworkVideoTransmitter tds:Device");

    pProbeMatchType->MetadataVersion = 1;

    // Build Scopes Message
    pScopes = (struct wsdd__ScopesType*)soap_malloc(soap, sizeof(struct wsdd__ScopesType));
    soap_default_wsdd__ScopesType(soap, pScopes);
    //pScopes->MatchBy = soap_strdup(soap, "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/rfc3986");
    pScopes->MatchBy = NULL;
    pScopes->__item = soap_strdup(soap, scopes_message);
    pProbeMatchType->Scopes = pScopes;

    if (!strlen(g_uuid))
        snprintf(g_uuid, 64, "%s", soap_wsa_rand_uuid(soap));
    pMessageID = g_uuid;
    // snprintf(str_tmp, 256, "%s-%s", pMessageID, mac_addr);
    sprintf(str_tmp, "%s", pMessageID);
    printf("g_uuid: %s\n", pMessageID);

    pProbeMatchType->wsa__EndpointReference.Address = soap_strdup(soap, pMessageID);

    wsdd__ProbeMatches.__sizeProbeMatch = 1;
    wsdd__ProbeMatches.ProbeMatch = pProbeMatchType;

    // Build SOAP Header
    pWsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));
    soap_default__wsa__RelatesTo(soap, pWsa__RelatesTo);
    pWsa__RelatesTo->__item = soap->header->wsa__MessageID;
    soap->header->wsa__RelatesTo = pWsa__RelatesTo;
    soap->header->wsa__Action = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches");
    soap->header->wsa__To = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous");

    soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &wsdd__ProbeMatches);

    return SOAP_OK;	
}


SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType* wsdd__ResolveMatches)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetServiceCapabilities(struct soap* soap, struct _ns1__GetServiceCapabilities *ns1__GetServiceCapabilities, 
						struct _ns1__GetServiceCapabilitiesResponse *ns1__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateProfile(struct soap* soap, struct _ns1__CreateProfile *ns1__CreateProfile, 
						struct _ns1__CreateProfileResponse *ns1__CreateProfileResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetProfiles(struct soap* soap, struct _ns1__GetProfiles *ns1__GetProfiles, 
						struct _ns1__GetProfilesResponse *ns1__GetProfilesResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__AddConfiguration(struct soap* soap, struct _ns1__AddConfiguration *ns1__AddConfiguration, 
						struct _ns1__AddConfigurationResponse *ns1__AddConfigurationResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__RemoveConfiguration(struct soap* soap, struct _ns1__RemoveConfiguration *ns1__RemoveConfiguration, 
						struct _ns1__RemoveConfigurationResponse *ns1__RemoveConfigurationResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteProfile(struct soap* soap, struct _ns1__DeleteProfile *ns1__DeleteProfile, 
						struct _ns1__DeleteProfileResponse *ns1__DeleteProfileResponse)
{	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurations, 
						struct _ns1__GetVideoSourceConfigurationsResponse *ns1__GetVideoSourceConfigurationsResponse)
{
	return SOAP_OK;	
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurations, 
						struct _ns1__GetVideoEncoderConfigurationsResponse *ns1__GetVideoEncoderConfigurationsResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioSourceConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurations, 
						struct _ns1__GetAudioSourceConfigurationsResponse *ns1__GetAudioSourceConfigurationsResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioEncoderConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurations, 
						struct _ns1__GetAudioEncoderConfigurationsResponse *ns1__GetAudioEncoderConfigurationsResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__GetAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAnalyticsConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAnalyticsConfigurations, 
						struct _ns1__GetAnalyticsConfigurationsResponse *ns1__GetAnalyticsConfigurationsResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMetadataConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetMetadataConfigurations, 
						struct _ns1__GetMetadataConfigurationsResponse *ns1__GetMetadataConfigurationsResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioOutputConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurations,
						struct _ns1__GetAudioOutputConfigurationsResponse *ns1__GetAudioOutputConfigurationsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioDecoderConfigurations(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurations, 
						struct _ns1__GetAudioDecoderConfigurationsResponse *ns1__GetAudioDecoderConfigurationsResponse)
{
	return SOAP_OK;
}
					
/** Web service operation '__ns1__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoSourceConfiguration(struct soap* soap,
						struct _ns1__SetVideoSourceConfiguration *ns1__SetVideoSourceConfiguration, 
						struct ns1__SetConfigurationResponse *ns1__SetVideoSourceConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoEncoderConfiguration(struct soap* soap, 
						struct _ns1__SetVideoEncoderConfiguration *ns1__SetVideoEncoderConfiguration, 
						struct ns1__SetConfigurationResponse *ns1__SetVideoEncoderConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioSourceConfiguration(struct soap* soap, 
						struct _ns1__SetAudioSourceConfiguration *ns1__SetAudioSourceConfiguration,
						struct ns1__SetConfigurationResponse *ns1__SetAudioSourceConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioEncoderConfiguration(struct soap* soap, 
						struct _ns1__SetAudioEncoderConfiguration *ns1__SetAudioEncoderConfiguration, 
						struct ns1__SetConfigurationResponse *ns1__SetAudioEncoderConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetMetadataConfiguration(struct soap* soap, 
						struct _ns1__SetMetadataConfiguration *ns1__SetMetadataConfiguration, 
						struct ns1__SetConfigurationResponse *ns1__SetMetadataConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioOutputConfiguration(struct soap* soap, 
						struct _ns1__SetAudioOutputConfiguration *ns1__SetAudioOutputConfiguration, 
						struct ns1__SetConfigurationResponse *ns1__SetAudioOutputConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioDecoderConfiguration(struct soap* soap, 
						struct _ns1__SetAudioDecoderConfiguration *ns1__SetAudioDecoderConfiguration,
						struct ns1__SetConfigurationResponse *ns1__SetAudioDecoderConfigurationResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurationOptions, 
						struct _ns1__GetVideoSourceConfigurationOptionsResponse *ns1__GetVideoSourceConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurationOptions, 
						struct _ns1__GetVideoEncoderConfigurationOptionsResponse *ns1__GetVideoEncoderConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioSourceConfigurationOptions(struct soap* soap,
						struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurationOptions, 
						struct _ns1__GetAudioSourceConfigurationOptionsResponse *ns1__GetAudioSourceConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioEncoderConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurationOptions, 
						struct _ns1__GetAudioEncoderConfigurationOptionsResponse *ns1__GetAudioEncoderConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMetadataConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetMetadataConfigurationOptions, 
						struct _ns1__GetMetadataConfigurationOptionsResponse *ns1__GetMetadataConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioOutputConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurationOptions, 
						struct _ns1__GetAudioOutputConfigurationOptionsResponse *ns1__GetAudioOutputConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioDecoderConfigurationOptions(struct soap* soap, 
						struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurationOptions, 
						struct _ns1__GetAudioDecoderConfigurationOptionsResponse *ns1__GetAudioDecoderConfigurationOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetVideoEncoderInstances' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderInstances(struct soap* soap, 
						struct _ns1__GetVideoEncoderInstances *ns1__GetVideoEncoderInstances, 
						struct _ns1__GetVideoEncoderInstancesResponse *ns1__GetVideoEncoderInstancesResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetStreamUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetStreamUri(struct soap* soap, 
						struct _ns1__GetStreamUri *ns1__GetStreamUri, struct _ns1__GetStreamUriResponse *ns1__GetStreamUriResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__StartMulticastStreaming(struct soap* soap, 
						struct ns1__StartStopMulticastStreaming *ns1__StartMulticastStreaming, 
						struct ns1__SetConfigurationResponse *ns1__StartMulticastStreamingResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__StopMulticastStreaming(struct soap* soap, 
						struct ns1__StartStopMulticastStreaming *ns1__StopMulticastStreaming,
						struct ns1__SetConfigurationResponse *ns1__StopMulticastStreamingResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetSynchronizationPoint(struct soap* soap, 
						struct _ns1__SetSynchronizationPoint *ns1__SetSynchronizationPoint, 
						struct _ns1__SetSynchronizationPointResponse *ns1__SetSynchronizationPointResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetSnapshotUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetSnapshotUri(struct soap* soap, 
						struct _ns1__GetSnapshotUri *ns1__GetSnapshotUri, struct _ns1__GetSnapshotUriResponse *ns1__GetSnapshotUriResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceModes(struct soap* soap, 
						struct _ns1__GetVideoSourceModes *ns1__GetVideoSourceModes, 
						struct _ns1__GetVideoSourceModesResponse *ns1__GetVideoSourceModesResponse)
{
	return SOAP_OK;
}

/** Web service operation '__ns1__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoSourceMode(struct soap* soap,
						struct _ns1__SetVideoSourceMode *ns1__SetVideoSourceMode,
						struct _ns1__SetVideoSourceModeResponse *ns1__SetVideoSourceModeResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetOSDs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetOSDs(struct soap* soap, 
						struct _ns1__GetOSDs *ns1__GetOSDs, struct _ns1__GetOSDsResponse *ns1__GetOSDsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetOSDOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetOSDOptions(struct soap* soap,
						struct _ns1__GetOSDOptions *ns1__GetOSDOptions,
						struct _ns1__GetOSDOptionsResponse *ns1__GetOSDOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetOSD(struct soap* soap, struct _ns1__SetOSD *ns1__SetOSD, 
						struct ns1__SetConfigurationResponse *ns1__SetOSDResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__CreateOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateOSD(struct soap* soap, struct _ns1__CreateOSD *ns1__CreateOSD, 
						struct _ns1__CreateOSDResponse *ns1__CreateOSDResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__DeleteOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteOSD(struct soap* soap, struct _ns1__DeleteOSD *ns1__DeleteOSD, 
						struct ns1__SetConfigurationResponse *ns1__DeleteOSDResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetMasks' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMasks(struct soap* soap, struct _ns1__GetMasks *ns1__GetMasks,
						struct _ns1__GetMasksResponse *ns1__GetMasksResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__GetMaskOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMaskOptions(struct soap* soap, struct _ns1__GetMaskOptions *ns1__GetMaskOptions,
						struct _ns1__GetMaskOptionsResponse *ns1__GetMaskOptionsResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__SetMask' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetMask(struct soap* soap, struct _ns1__SetMask *ns1__SetMask, 
						struct ns1__SetConfigurationResponse *ns1__SetMaskResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__CreateMask' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateMask(struct soap* soap, struct _ns1__CreateMask *ns1__CreateMask, 
						struct _ns1__CreateMaskResponse *ns1__CreateMaskResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__ns1__DeleteMask' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteMask(struct soap* soap, struct _ns1__DeleteMask *ns1__DeleteMask,
						struct ns1__SetConfigurationResponse *ns1__DeleteMaskResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tdn__Hello' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse)
{
	return SOAP_OK;
}

/** Web service operation '__tdn__Bye' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse)
{
	return SOAP_OK;
}

/** Web service operation '__tdn__Probe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse)
{
	return SOAP_OK;
}

/** Web service operation '__tds__GetServices' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, 
						struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* soap, 
						struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities,
						struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDeviceInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* soap, 
						struct _tds__GetDeviceInformation *tds__GetDeviceInformation, 
						struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
	printf("-------------------------__tds__GetDeviceInformation------------------------------\n");
	tds__GetDeviceInformationResponse->Manufacturer = (char*)soap_malloc(soap, sizeof(char) * 32);
	tds__GetDeviceInformationResponse->Model = (char*)soap_malloc(soap, sizeof(char) * 32);
	tds__GetDeviceInformationResponse->FirmwareVersion = (char*)soap_malloc(soap, sizeof(char) * 32);
	tds__GetDeviceInformationResponse->SerialNumber = (char*)soap_malloc(soap, sizeof(char) * 32);
	tds__GetDeviceInformationResponse->HardwareId = (char*)soap_malloc(soap, sizeof(char) * 32);

	strcpy(tds__GetDeviceInformationResponse->Manufacturer, "HIKAUTO");
	strcpy(tds__GetDeviceInformationResponse->Model, "HD1080P");
	strcpy(tds__GetDeviceInformationResponse->FirmwareVersion, "v1.0.0");
	strcpy(tds__GetDeviceInformationResponse->SerialNumber, "IPCTEST12345");
	strcpy(tds__GetDeviceInformationResponse->HardwareId, "1.0");

	return SOAP_OK;
	
}
						
/** Web service operation '__tds__SetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* soap, 
						struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, 
						struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* soap, 
						struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, 
						struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetSystemFactoryDefault' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* soap, 
						struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, 
						struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__UpgradeSystemFirmware' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, 
						struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, 
						struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
	return SOAP_OK;
}
						
/** Web service operation '__tds__SystemReboot' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, 
						struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
	
	
}
						
/** Web service operation '__tds__RestoreSystem' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, 
						struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
	
	
}
						
/** Web service operation '__tds__GetSystemBackup' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, 
						struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
	
	
}
						
/** Web service operation '__tds__GetSystemLog' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog,
						struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
	
	
}
						
/** Web service operation '__tds__GetSystemSupportInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, 
						struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, 
						struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
	
	
}
						
/** Web service operation '__tds__GetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, 
						struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
	
	
}
						
/** Web service operation '__tds__SetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, 
						struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
	
	
}						
/** Web service operation '__tds__AddScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, 
						struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
	
	
}
						
/** Web service operation '__tds__RemoveScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, 
						struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
	
	
}
						
/** Web service operation '__tds__GetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, 
						struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
	
	
}
						
/** Web service operation '__tds__SetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode,
						struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
	
	
}
						
/** Web service operation '__tds__GetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, 
						struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
	
	
}
						
/** Web service operation '__tds__SetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, 
						struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
	
	
}
						
/** Web service operation '__tds__GetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, 
						struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
	
	
}
						
/** Web service operation '__tds__GetEndpointReference' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, 
						struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
	
	
}
						
/** Web service operation '__tds__GetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, 
						struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{
	
	
}
						
/** Web service operation '__tds__SetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser,
						struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers,
						struct _tds__GetUsersResponse *tds__GetUsersResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__CreateUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers,
						struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__DeleteUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, 
						struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, 
						struct _tds__SetUserResponse *tds__SetUserResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetWsdlUrl' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, 
						struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities,
						struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, 
						struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap, struct _tds__GetHostname *tds__GetHostname, 
						struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname,
						struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetHostnameFromDHCP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP,
						struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS,
						struct _tds__GetDNSResponse *tds__GetDNSResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, 
						struct _tds__SetDNSResponse *tds__SetDNSResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, 
						struct _tds__GetNTPResponse *tds__GetNTPResponse)
{	
	return SOAP_OK;
}						
/** Web service operation '__tds__SetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{	
	return SOAP_OK;
}

/** Web service operation '__tds__GetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, 
						struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, 
						struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, 
						struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, 
						struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, 
						struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, 
						struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, 
						struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols,
						struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, 
						struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, 
						struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, 
						struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway,
						struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, 
						struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, 
						struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, 
						struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration,
						struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, 
						struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, 
						struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, 
						struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter,
						struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, 
						struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, 
						struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__AddIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap,
						struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter,
						struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__RemoveIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, 
						struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, 
						struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, 
						struct _tds__GetAccessPolicy *tds__GetAccessPolicy, 
						struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, 
						struct _tds__SetAccessPolicy *tds__SetAccessPolicy, 
						struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__CreateCertificate' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap, 
						struct _tds__CreateCertificate *tds__CreateCertificate, 
						struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap, 
						struct _tds__GetCertificates *tds__GetCertificates, 
						struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* soap, 
						struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus,
						struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* soap, 
						struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, 
						struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__DeleteCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* soap, 
						struct _tds__DeleteCertificates *tds__DeleteCertificates, 
						struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetPkcs10Request' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap, 
						struct _tds__GetPkcs10Request *tds__GetPkcs10Request, 
						struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__LoadCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap, 
						struct _tds__LoadCertificates *tds__LoadCertificates, 
						struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* soap, 
						struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode,
						struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* soap, 
						struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode,
						struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetRelayOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap, 
						struct _tds__GetRelayOutputs *tds__GetRelayOutputs, 
						struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetRelayOutputSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, 
						struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, 
						struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetRelayOutputState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, 
						struct _tds__SetRelayOutputState *tds__SetRelayOutputState, 
						struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, 
						struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, 
						struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap, 
						struct _tds__GetCACertificates *tds__GetCACertificates, 
						struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{	
	return SOAP_OK;
}						
/** Web service operation '__tds__LoadCertificateWithPrivateKey' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* soap, 
						struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, 
						struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetCertificateInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* soap, 
						struct _tds__GetCertificateInformation *tds__GetCertificateInformation, 
						struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__LoadCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* soap, 
						struct _tds__LoadCACertificates *tds__LoadCACertificates, 
						struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__CreateDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* soap, 
						struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration,
						struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* soap, 
						struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, 
						struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* soap, 
						struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, 
						struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDot1XConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* soap, 
						struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, 
						struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__DeleteDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* soap, 
						struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, 
						struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDot11Capabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* soap, 
						struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, 
						struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetDot11Status' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, 
						struct _tds__GetDot11Status *tds__GetDot11Status, 
						struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__ScanAvailableDot11Networks' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap,
						struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks,
						struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetSystemUris' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, 
						struct _tds__GetSystemUris *tds__GetSystemUris, 
						struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__StartFirmwareUpgrade' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, 
						struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, 
						struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__StartSystemRestore' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, 
						struct _tds__StartSystemRestore *tds__StartSystemRestore, 
						struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetStorageConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, 
						struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, 
						struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__CreateStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, 
						struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, 
						struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfiguration(struct soap* soap, 
						struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration,
						struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetStorageConfiguration(struct soap* soap, 
						struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, 
						struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__DeleteStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap,
						struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, 
						struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__GetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetGeoLocation(struct soap* soap, 
						struct _tds__GetGeoLocation *tds__GetGeoLocation, 
						struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__SetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetGeoLocation(struct soap* soap, 
						struct _tds__SetGeoLocation *tds__SetGeoLocation,
						struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tds__DeleteGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteGeoLocation(struct soap* soap, 
						struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, 
						struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__PullMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PullMessages(struct soap* soap, 
						struct _tev__PullMessages *tev__PullMessages, 
						struct _tev__PullMessagesResponse *tev__PullMessagesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Seek' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Seek(struct soap* soap, struct _tev__Seek *tev__Seek, struct _tev__SeekResponse *tev__SeekResponse)
{	
	return SOAP_OK;
}

/** Web service operation '__tev__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__SetSynchronizationPoint(struct soap* soap, 
						struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, 
						struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Unsubscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe(struct soap* soap, 
						struct _wsnt__Unsubscribe *wsnt__Unsubscribe, 
						struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetServiceCapabilities(struct soap* soap, 
						struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, 
						struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__CreatePullPointSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPointSubscription(struct soap* soap, 
						struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription,
						struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__GetEventProperties' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventProperties(struct soap* soap, 
						struct _tev__GetEventProperties *tev__GetEventProperties, 
						struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Renew' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, 
						struct _wsnt__RenewResponse *wsnt__RenewResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Unsubscribe_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe_(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, 
						struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Subscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Subscribe(struct soap* soap, struct _wsnt__Subscribe *wsnt__Subscribe, 
						struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__GetCurrentMessage' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetCurrentMessage(struct soap* soap, 
						struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, 
						struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse)
{	
	return SOAP_OK;
}
						
/** Web service one-way operation '__tev__Notify' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify(struct soap* soap, struct _wsnt__Notify *wsnt__Notify)
{	
	return SOAP_OK;
}

/** Web service operation '__tev__GetMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetMessages(struct soap* soap, struct _wsnt__GetMessages *wsnt__GetMessages, 
						struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__DestroyPullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__DestroyPullPoint(struct soap* soap, 
						struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, 
						struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse)
{	
	return SOAP_OK;
}
						
/** Web service one-way operation '__tev__Notify_' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify_(struct soap* soap, struct _wsnt__Notify *wsnt__Notify)
{	
	return SOAP_OK;
}

/** Web service operation '__tev__CreatePullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPoint(struct soap* soap, struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, 
						struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Renew_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew_(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, 
						struct _wsnt__RenewResponse *wsnt__RenewResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__Unsubscribe__' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe__(struct soap* soap, 
						struct _wsnt__Unsubscribe *wsnt__Unsubscribe, 
						struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__PauseSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PauseSubscription(struct soap* soap, 
						struct _wsnt__PauseSubscription *wsnt__PauseSubscription, 
						struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tev__ResumeSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__ResumeSubscription(struct soap* soap,
						struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, 
						struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetServiceCapabilities(struct soap* soap, 
						struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, 
						struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetImagingSettings(struct soap* soap, 
						struct _timg__GetImagingSettings *timg__GetImagingSettings,
						struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__SetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetImagingSettings(struct soap* soap, 
						struct _timg__SetImagingSettings *timg__SetImagingSettings, 
						struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetOptions(struct soap* soap, 
						struct _timg__GetOptions *timg__GetOptions, 
						struct _timg__GetOptionsResponse *timg__GetOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__Move' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Move(struct soap* soap, 
						struct _timg__Move *timg__Move, 
						struct _timg__MoveResponse *timg__MoveResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Stop(struct soap* soap, struct _timg__Stop *timg__Stop, 
						struct _timg__StopResponse *timg__StopResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetStatus(struct soap* soap, 
						struct _timg__GetStatus *timg__GetStatus, 
						struct _timg__GetStatusResponse *timg__GetStatusResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetMoveOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetMoveOptions(struct soap* soap, 
						struct _timg__GetMoveOptions *timg__GetMoveOptions, 
						struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetPresets(struct soap* soap, 
						struct _timg__GetPresets *timg__GetPresets, 
						struct _timg__GetPresetsResponse *timg__GetPresetsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__GetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetCurrentPreset(struct soap* soap, 
						struct _timg__GetCurrentPreset *timg__GetCurrentPreset, 
						struct _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__timg__SetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetCurrentPreset(struct soap* soap, 
						struct _timg__SetCurrentPreset *timg__SetCurrentPreset, 
						struct _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetServiceCapabilities(struct soap* soap, 
						struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, 
						struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetRelayOutputOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputOptions(struct soap* soap, 
						struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, 
						struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSources(struct soap* soap, 
						struct tmd__Get *tmd__GetAudioSources, struct tmd__GetResponse *tmd__GetAudioSourcesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputs(struct soap* soap, 
						struct tmd__Get *tmd__GetAudioOutputs, struct tmd__GetResponse *tmd__GetAudioOutputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSources(struct soap* soap, 
						struct tmd__Get *tmd__GetVideoSources, struct tmd__GetResponse *tmd__GetVideoSourcesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetVideoOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputs(struct soap* soap, 
						struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs, 
						struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfiguration(struct soap* soap, 
						struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, 
						struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetVideoOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfiguration(struct soap* soap, 
						struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, 
						struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfiguration(struct soap* soap, 
						struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, 
						struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfiguration(struct soap* soap, 
						struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, 
						struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoSourceConfiguration(struct soap* soap,
						struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, 
						struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetVideoOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoOutputConfiguration(struct soap* soap, 
						struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, 
						struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioSourceConfiguration(struct soap* soap, 
						struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration, 
						struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioOutputConfiguration(struct soap* soap,
						struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration, 
						struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfigurationOptions(struct soap* soap, 
						struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions,
						struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse)
{	
	return SOAP_OK;
}						
						
/** Web service operation '__tmd__GetVideoOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfigurationOptions(struct soap* soap,
						struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions,
						struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfigurationOptions(struct soap* soap, 
						struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions, 
						struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfigurationOptions(struct soap* soap, 
						struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, 
						struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetRelayOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputs(struct soap* soap, 
						struct _tds__GetRelayOutputs *tds__GetRelayOutputs, 
						struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetRelayOutputSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputSettings(struct soap* soap, 
						struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, 
						struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetRelayOutputState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputState(struct soap* soap,
						struct _tds__SetRelayOutputState *tds__SetRelayOutputState, 
						struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetDigitalInputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetDigitalInputs(struct soap* soap, 
						struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs, 
						struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetDigitalInputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetDigitalInputConfigurationOptions(struct soap* soap, 
						struct _tmd__GetDigitalInputConfigurationOptions *tmd__GetDigitalInputConfigurationOptions,
						struct _tmd__GetDigitalInputConfigurationOptionsResponse *tmd__GetDigitalInputConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetDigitalInputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetDigitalInputConfigurations(struct soap* soap, 
						struct _tmd__SetDigitalInputConfigurations *tmd__SetDigitalInputConfigurations, 
						struct _tmd__SetDigitalInputConfigurationsResponse *tmd__SetDigitalInputConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetSerialPorts' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPorts(struct soap* soap, 
						struct _tmd__GetSerialPorts *tmd__GetSerialPorts, 
						struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetSerialPortConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfiguration(struct soap* soap, 
						struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration, 
						struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SetSerialPortConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetSerialPortConfiguration(struct soap* soap, 
						struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration, 
						struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__GetSerialPortConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfigurationOptions(struct soap* soap,
						struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, 
						struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tmd__SendReceiveSerialCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tmd__SendReceiveSerialCommand(struct soap* soap, 
						struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, 
						struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(struct soap* soap, 
						struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, 
						struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(struct soap* soap,
						struct _tptz__GetConfigurations *tptz__GetConfigurations, 
						struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap, 
						struct _tptz__GetPresets *tptz__GetPresets, 
						struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap, 
						struct _tptz__SetPreset *tptz__SetPreset, 
						struct _tptz__SetPresetResponse *tptz__SetPresetResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap, 
						struct _tptz__RemovePreset *tptz__RemovePreset, 
						struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap, 
						struct _tptz__GotoPreset *tptz__GotoPreset, 
						struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap, 
						struct _tptz__GetStatus *tptz__GetStatus, 
						struct _tptz__GetStatusResponse *tptz__GetStatusResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap, 
						struct _tptz__GetConfiguration *tptz__GetConfiguration, 
						struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap, 
						struct _tptz__GetNodes *tptz__GetNodes, 
						struct _tptz__GetNodesResponse *tptz__GetNodesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetNode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap, 
						struct _tptz__GetNode *tptz__GetNode, 
						struct _tptz__GetNodeResponse *tptz__GetNodeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__SetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap, 
						struct _tptz__SetConfiguration *tptz__SetConfiguration, 
						struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(struct soap* soap, 
						struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, 
						struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap, 
						struct _tptz__GotoHomePosition *tptz__GotoHomePosition, 
						struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap, 
						struct _tptz__SetHomePosition *tptz__SetHomePosition, 
						struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap,
						struct _tptz__ContinuousMove *tptz__ContinuousMove, 
						struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap, 
						struct _tptz__RelativeMove *tptz__RelativeMove, 
						struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(struct soap* soap, 
						struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, 
						struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__AbsoluteMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap,
						struct _tptz__AbsoluteMove *tptz__AbsoluteMove, 
						struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GeoMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GeoMove(struct soap* soap, struct _tptz__GeoMove *tptz__GeoMove, 
						struct _tptz__GeoMoveResponse *tptz__GeoMoveResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap, 
						struct _tptz__Stop *tptz__Stop, struct _tptz__StopResponse *tptz__StopResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetPresetTours' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap, 
						struct _tptz__GetPresetTours *tptz__GetPresetTours, 
						struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap, 
						struct _tptz__GetPresetTour *tptz__GetPresetTour, 
						struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetPresetTourOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(struct soap* soap, 
						struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, 
						struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__CreatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap, 
						struct _tptz__CreatePresetTour *tptz__CreatePresetTour, 
						struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__ModifyPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap, 
						struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, 
						struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__OperatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(struct soap* soap, 
						struct _tptz__OperatePresetTour *tptz__OperatePresetTour, 
						struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__RemovePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap, 
						struct _tptz__RemovePresetTour *tptz__RemovePresetTour, 
						struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__tptz__GetCompatibleConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetCompatibleConfigurations(struct soap* soap, 
						struct _tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, 
						struct _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, 
						struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities,
						struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, 
						struct _trt__GetVideoSources *trt__GetVideoSources, 
						struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, 
						struct _trt__GetAudioSources *trt__GetAudioSources, 
						struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap, 
						struct _trt__GetAudioOutputs *trt__GetAudioOutputs, 
						struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__CreateProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap, 
						struct _trt__CreateProfile *trt__CreateProfile, 
						struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap, 
						struct _trt__GetProfile *trt__GetProfile, 
						struct _trt__GetProfileResponse *trt__GetProfileResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetProfiles' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, 
						struct _trt__GetProfiles *trt__GetProfiles,
						struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* soap, 
						struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, 
						struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* soap, 
						struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, 
						struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* soap, 
						struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, 
						struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* soap, 
						struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration,
						struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddPTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* soap, 
						struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, 
						struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* soap, 
						struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, 
						struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* soap, 
						struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, 
						struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* soap, 
						struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, 
						struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__AddAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* soap, 
						struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, 
						struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* soap, 
						struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, 
						struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* soap, 
						struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, 
						struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* soap, 
						struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration,
						struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* soap,
						struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, 
						struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemovePTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* soap, 
						struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, 
						struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, 
						struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, 
						struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* soap, 
						struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, 
						struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* soap, 
						struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, 
						struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__RemoveAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* soap, 
						struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, 
						struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__DeleteProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap, 
						struct _trt__DeleteProfile *trt__DeleteProfile, 
						struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(struct soap* soap, 
						struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations,
						struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* soap, 
						struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations,
						struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* soap, 
						struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations,
						struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(struct soap* soap, 
						struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, 
						struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(struct soap* soap, 
						struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, 
						struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(struct soap* soap, 
						struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, 
						struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(struct soap* soap, 
						struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, 
						struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(struct soap* soap, 
						struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, 
						struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(struct soap* soap, 
						struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, 
						struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* soap, 
						struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration,
						struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* soap, 
						struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, 
						struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* soap,
						struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, 
						struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* soap, 
						struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, 
						struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* soap,
						struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, 
						struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* soap, 
						struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, 
						struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* soap, 
						struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, 
						struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, 
						struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, 
						struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, 
						struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap,
						struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations,
						struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, 
						struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, 
						struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, 
						struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetCompatibleAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, 
						struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations,
						struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* soap, 
						struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration,
						struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* soap, 
						struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, 
						struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* soap,
						struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, 
						struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* soap, 
						struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, 
						struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* soap, 
						struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration,
						struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* soap,
						struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, 
						struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* soap, 
						struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration,
						struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* soap,
						struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, 
						struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* soap, 
						struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, 
						struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, 
						struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions,
						struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* soap, 
						struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions,
						struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, 
						struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, 
						struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* soap, 
						struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, 
						struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{	
	return SOAP_OK;
}						
						
/** Web service operation '__trt__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* soap, 
						struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions,
						struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, 
						struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, 
						struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetGuaranteedNumberOfVideoEncoderInstances' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, 
						struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, 
						struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetStreamUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap, 
						struct _trt__GetStreamUri *trt__GetStreamUri, 
						struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* soap, 
						struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, 
						struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* soap, 
						struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, 
						struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* soap, 
						struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, 
						struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetSnapshotUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap, 
						struct _trt__GetSnapshotUri *trt__GetSnapshotUri,
						struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceModes(struct soap* soap, 
						struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, 
						struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceMode(struct soap* soap, 
						struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode,
						struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse)
{	
	return SOAP_OK;
}
						
/** Web service operation '__trt__GetOSDs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDs(struct soap* soap, 
						struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse)
{	
	return SOAP_OK;
}
					
/** Web service operation '__trt__GetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSD(struct soap* soap, struct _trt__GetOSD *trt__GetOSD, 
						struct _trt__GetOSDResponse *trt__GetOSDResponse)
{
	return SOAP_OK;
}						
/** Web service operation '__trt__GetOSDOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDOptions(struct soap* soap, 
					struct _trt__GetOSDOptions *trt__GetOSDOptions,
					struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse)
{
	return SOAP_OK;
}	
					
/** Web service operation '__trt__SetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetOSD(struct soap* soap, 
					struct _trt__SetOSD *trt__SetOSD, 
					struct _trt__SetOSDResponse *trt__SetOSDResponse)
{
	return SOAP_OK;
}	
					
/** Web service operation '__trt__CreateOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateOSD(struct soap* soap, 
					struct _trt__CreateOSD *trt__CreateOSD, 
					struct _trt__CreateOSDResponse *trt__CreateOSDResponse)
{
	return SOAP_OK;
}						
/** Web service operation '__trt__DeleteOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteOSD(struct soap* soap, 
					struct _trt__DeleteOSD *trt__DeleteOSD, 
					struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse)
{
	return SOAP_OK;
}						

