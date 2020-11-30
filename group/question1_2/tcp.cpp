
#include "stdlib.h"
#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"
#include "VlanLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "UdpLayer.h"
#include "DnsLayer.h"
#include "PcapFileDevice.h"


bool udppacket(str::string stripm, int port){
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

	// create ip layer
	pcpp::IPv4Layer newIPLayer(pcpp::IPv4Address(strip), myip);
	newIPLayer.getIPv4Header()->ipId = pcpp::hostToNet16(2000);
	newIPLayer.getIPv4Header()->timeToLive = 64;
// create a new UDP layer
	pcpp::TCPLayer newTCPLayer(12345, port);	

// create a packet with initial capacity of 100 bytes (will grow automatically if needed)
	pcpp::Packet newPacket(100);
	newPacket.addLayer(&newIPLayer);
	newPacket.addLayer(&newTCPLayer);
	newPacket.computeCalculateFields();

	dev = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(myip);	

	return dev->sendPacket(&packet);		

}
