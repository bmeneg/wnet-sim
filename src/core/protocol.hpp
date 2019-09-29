#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <iostream>
#include <vector>
#include <climits>

typedef std::pair<unsigned int, std::vector<unsigned int>> proto_route_t;
typedef enum {
	MSG_HELLO,
	MSG_UPDATE,
} proto_msgdef_t;

const unsigned int BROADCAST_ID = UINT_MAX;

struct ProtoMsgPayload {
	proto_msgdef_t msg;
	std::vector<proto_route_t> routing_table;
};

struct ProtoMsg {
	unsigned int src_id;
	unsigned int dst_id;
	struct ProtoMsgPayload payload;
};

int proto_check_msgdef(ProtoMsg *msg);

#endif // PROTOCOL_HPP
