typedef char string[50];

#define REQ 1
#define ACK 2
#define DATA 3

typedef struct RARP_PACKET
{
	int mode;
	string src_ip;
	string dest_ip;
	string src_mac;
	string dest_mac;
	string data;
}rarp;

rarp genpacket(int mode)
{
	rarp packet;
	packet.mode = mode;
	printf("IP Address of Source: ");
	scanf("%s", packet.src_ip);
	printf("MAC Address of Source: ");
	scanf("%s", packet.src_mac);
	printf("MAC Address of Destination: ");
	scanf("%s", packet.dest_mac);
	printf("16 bit data: ");
	scanf("%s", packet.data);
	return packet;
}

void print(rarp packet)
{
	if (packet.mode == REQ)
	{
		printf("|%d|%s|%s|%s|%s|\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_mac, "0.0.0.0");
	}
	if (packet.mode == ACK)
	{
		printf("|%d|%s|%s|%s|%s|\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_mac, packet.dest_ip);
	}
	if (packet.mode == DATA)
	{
		printf("|%d|%s|%s|%s|%s|%s|\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_mac, packet.dest_ip, packet.data);
	}
}