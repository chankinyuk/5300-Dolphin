/***
 * @file heap_storage.cpp
 * @authors Alicia Mauldin & Kin Chan
 * @see 'Seattle University, CPSC-5300, Summer 2019'
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "heap_storage.h"

using namespace std;
typedef u_int16_t u16;

// Ctor for SlottedPage class
SlottedPage::SlottedPage(Dbt &block, BlockID block_id, bool is_new) : DbBlock(block, block_id, is_new) {
  if (is_new) {
    // Sets initial block header values if new block
    this->num_records = 0;
    this->end_free = DbBlock::BLOCK_SZ - 1;
    put_header();
  } else {
    get_header(this->num_records, this->end_free);
  }
}

// Add a new record to the SlottedPage block and return its id
RecordID SlottedPage::add(const Dbt* data) throw(DbBlockNoRoomError) {
  if (!has_room(data->get_size()))
    throw DbBlockNoRoomError("not enough room for new record");
  u16 id = ++this->num_records;
  u16 size = (u16) data->get_size();
  this->end_free -= size;
  u16 loc = this->end_free + 1;
  put_header();
  put_header(id, size, loc);
  memcpy(this->address(loc), data->get_data(), size);
  return id;
}

// Get record data using its id
Dbt* SlottedPage::get(RecordID record_id) {
  // TODO implement
}

// Update a record with new data
void SlottedPage::put(RecordID record_id, const Dbt &data) throw(DbBlockNoRoomError) {
  // TODO implement
}

// Delete a record
void SlottedPage::del(RecordID record_id) {
  // TODO implement
}

// Returns all record ids in SlottedPage block
RecordIDs* SlottedPage::ids(void) {
  // TODO implement
}
