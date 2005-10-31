#include <netinet/ether.h>
#include <netinet/in.h>
#include <stdio.h>
#include <inttypes.h>
#include <dlfcn.h>
#include <map>
#include "libpacketdump.h"

struct ieee_802_11_header {
        uint8_t      protocol:2;
        uint8_t      type:2;
        uint8_t      subtype:4;
        uint8_t      to_ds:1;
        uint8_t      from_ds:1;
        uint8_t      more_frag:1;
        uint8_t      retry:1;
        uint8_t      power:1;
        uint8_t      more_data:1;
        uint8_t      wep:1;
        uint8_t      order:1;
        uint16_t     duration;
        uint8_t      mac1[6];
        uint8_t      mac2[6];
        uint8_t      mac3[6];
        uint16_t     SeqCtl;
        uint8_t      mac4[6];
        uint8_t      data[1];
};

struct ieee_802_11_payload {
        uint16_t     type;
        uint8_t      data[1];
};

extern "C"
void decode(int link_type,char *packet,int len)
{
	printf(" 802.11:");
	struct ieee_802_11_header *hdr = (struct ieee_802_11_header *)packet;

	printf(" %s",ether_ntoa((struct ether_addr*)(hdr->mac1)));
	printf(" %s",ether_ntoa((struct ether_addr*)(hdr->mac2)));
	printf(" %s",ether_ntoa((struct ether_addr*)(hdr->mac3)));

	struct ieee_802_11_payload *pld = (struct ieee_802_11_payload *) ((int)packet + sizeof(struct ieee_802_11_header) - 2);
	uint16_t type = htons(pld->type);
	printf(" %04x\n",type);
	decode_next((char *)pld->data,len-(sizeof(struct ieee_802_11_header)),"eth",type);
	return;
}
