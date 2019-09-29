#include "protocol.hpp"

int proto_check_msgdef(ProtoMsg *msg)
{
	int ret = -1;

	switch (msg->payload.msg) {
	case MSG_HELLO:
	case MSG_UPDATE:
		ret = 0;
		break;
	}

	return ret;
}
