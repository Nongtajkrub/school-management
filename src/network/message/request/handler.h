#pragma once

#include "../../../database/db.h"
#include "../message.h"

#include <vector.h>

typedef msg_t msg_req_t;

bool req_handle(msg_req_t* req, i32 sockfd, database_t* db);
