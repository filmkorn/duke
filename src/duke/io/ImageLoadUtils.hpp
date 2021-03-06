#pragma once

#include "duke/io/IO.hpp"
#include "duke/io/IIOOperation.hpp"

#include <functional>

namespace duke {

typedef std::function<ReadOptions(const StreamDescription&)> ReadOptionsFunc;

inline ReadOptionsFunc defaultReadOptions() {
  return [](const StreamDescription&) { return ReadOptions{}; };
}

void loadImage(ReadFrameResult& result, const ReadOptionsFunc& getReadOptions = defaultReadOptions());

ReadFrameResult load(const char* pFilename, const ReadOptionsFunc& getReadOptions = defaultReadOptions());

} /* namespace duke */
