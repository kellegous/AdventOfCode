#include "status.h"

#include <stdlib.h>
#include <stdio.h>

#include <memory>
#include <mutex>

#include "util.h"

namespace {
std::unique_ptr<Status> noerr;

std::once_flag noerr_once;

void Init() {
  noerr.reset(new Status());
}

}

Status::Status(const char* msg, const char* file) {
  util::StringFormat(&data_, "[%s] %s", file, msg);
}

const char* Status::what() const {
  return data_.c_str();
}

const Status NoErr() {
  std::call_once(noerr_once, Init);
  return *noerr;
}