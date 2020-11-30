#include <stdlib.h>
#include <iostream>
#include <fstream>
#ifndef _MSC_VER
#include "unistd.h"
#endif
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "IcmpLayer.h"
#include "Packet.h"
#include "PcapLiveDeviceList.h"
#include "NetworkUtils.h"
#include "Common.h"
#include "SystemUtils.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


bool icmppacket(char* strip){
	/*----------------------------*/
	// get target ip address 
	struct sockaddr_in temp;
	inet_pton(AF_INET, strip, &(temp.sin_addr));	
	// got ip 
	IPv4Address targetip = temp.sin_addr;


	/*----------------------------*/
	// get my ip address 
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;
	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	IPv4Address myip = (struct sockaddr_in*)(&ifr.ifr_addr)->sin_addr;

	/*----------------------------*/
	// get my mac address  
	dev = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(pitcherIP);	

	uint8_t* data = 0;  
	size_t dataLen=0;	
	size_t icmpMsgId =8;
	uint64_t msgType = 13;

	// Eth first
	// then IPv4 (IPv6 is not supported)
	IPv4Layer ipLayer(srcIPAddr, dstIPAddr);
	ipLayer.getIPv4Header()->timeToLive = 128;
	// set and increment the IP ID
	ipLayer.getIPv4Header()->ipId = pcpp::hostToNet16(ipID++);

	// then ICMP
	IcmpLayer icmpLayer;
	if (sendRequest && icmpLayer.setEchoRequestData(icmpMsgId, 0, msgType, data, dataLen) == NULL)
		EXIT_WITH_ERROR("Cannot set ICMP echo request data");
	else if (!sendRequest && icmpLayer.setEchoReplyData(icmpMsgId, 0, msgType, data, dataLen) == NULL)
		EXIT_WITH_ERROR("Cannot set ICMP echo response data");

	// create an new packet and add all layers to it
	Packet packet;
	packet.addLayer(&ipLayer);
	packet.addLayer(&icmpLayer);
	packet.computeCalculateFields();

	// send the packet through the device
	return dev->sendPacket(&packet);		

}
