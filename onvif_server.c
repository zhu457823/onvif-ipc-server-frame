/**@file    onvif_server.c
 * @note    Hangzhou Hikvision Automotive Technology Co., Ltd. All Right Reserved.
 * @brief   onvif profile S Specification v1.0版本,Date Dec.2011
 *
 * @author  zhujinlin
 * @date    2020-2-11
 * @version V1.0
 *
 * @note History:
 * @note 2020-2-11 zjl 实现两个接口，一个接口实现加入组播组，实现发现设备的功能，
 			  	    一个接口监听soap报文，实现相应的操作。
 */

#include "common.h"
#include "soapH.h"

/*
* @brief 加入组播组，监听组播报文，实现设备发现功能
*/
static void * OnvifDiscovered(void *arg)
{
	struct soap UDPserverSoap = {0x0};
	struct ip_mreq mcast;	//组播结构体
	int m_fd = -1;
	int u_fd = -1;

	soap_init1(&UDPserverSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
	soap_set_namespaces(&UDPserverSoap, namespaces);

	m_fd = soap_bind(&UDPserverSoap, NULL, ONVIF_UDP_PORT, 10);
	if(!soap_valid_socket(m_fd))
	{
		soap_print_fault(&UDPserverSoap, stderr);
		exit(1);
	}
	printf("mcast socket bind success, m_fd is %d\n", m_fd);

	mcast.imr_multiaddr.s_addr = inet_addr(ONVIF_UDP_IP);
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);
	//IP_ADD_MEMBERSHIP用于加入某个多播组，之后就可以向这个多播组发送数据或者从多播组接收数据
    if(setsockopt(UDPserverSoap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)
    {
		printf("setsockopt error! error code = %d,err string = %s\n",errno,strerror(errno));
        return 0;
    }

	while(1)
	{
		u_fd = soap_accept(&UDPserverSoap);
		if(!soap_valid_socket(u_fd))
		{
			soap_print_fault(&UDPserverSoap, stderr);
			exit(1);
		}
		
		if(SOAP_OK != soap_serve(&UDPserverSoap))
		{
			soap_print_fault(&UDPserverSoap, stderr);
			printf("soap_print_fault\n");
		}

		printf("IP = %u.%u.%u.%u\n", ((UDPserverSoap.ip)>>24)&0xFF, ((UDPserverSoap.ip)>>16)&0xFF, ((UDPserverSoap.ip)>>8)&0xFF,(UDPserverSoap.ip)&0xFF);
        soap_destroy(&UDPserverSoap);
        soap_end(&UDPserverSoap);
		
	}
		
	//分离运行时的环境
    soap_done(&UDPserverSoap);
    pthread_exit(0);
		
}

/*
* @brief 测试设备发现和soap报文监听接口
*/
int main(int argc, char *argv[])
{	
	pthread_t discovery = 0;
	pthread_create(&discovery, NULL, OnvifDiscovered, NULL);
	pthread_join(discovery, 0);

	return 0;
}

