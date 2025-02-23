#pragma once

#include <vector.h>
#include "request.h"
#include "lexer.h"
#include "../../database/db.h"

bool req_handle(req_t* req, i32 sockfd, dbdata_t* db);
