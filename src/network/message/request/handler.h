#pragma once

#include "request.h"
#include "lexer.h"
#include "../../../database/db.h"

#include <vector.h>

bool req_handle(msg_req_t* req, i32 sockfd, dbdata_t* db);
