//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/mru_replacer.h"

namespace bustub
{

	MRUReplacer::MRUReplacer(size_t num_pages) {}

	MRUReplacer::~MRUReplacer() = default;

	auto MRUReplacer::Victim(frame_id_t *frame_id) -> bool { return false; }

	void MRUReplacer::Pin(frame_id_t frame_id) {}

	void MRUReplacer::Unpin(frame_id_t frame_id) {}

	auto MRUReplacer::Size() -> size_t { return 0; }

} // namespace bustub
