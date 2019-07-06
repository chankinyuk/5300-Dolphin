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
typedef uint16_t u16;

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
  // TODO ensure _get_header() is implemented properly
  u16 size = this->num_records;
  RecordIDs* idRecordList = new RecordIDs(size);
  for (unsigned i = 0; i < size + 1; i++) {
    if (this->get_header(i)[1] != 0) {
      idRecordList->push_back(this->get_header(i)[1]);
    }
  }
  return idRecordList;
}

void SlottedPage::get_header(u16 &size, u16 &loc, RecordID id) {
  //Get the size and offset for given id. For id of zero, it is the block header.
  //        return self._get_n(4 * id), self._get_n(4 * id + 2)
  // TODO implement

}

// Store the size and offset for given id. For id of zero, store the block header.
void SlottedPage::put_header(RecordID id, u16 size, u16 loc) {
  if (id == 0) { // called the put_header() version and using the default params
    size = this->num_records;
    loc = this->end_free;
  }
  put_n(4*id, size);
  put_n(4*id + 2, loc);
}

bool SlottedPage::has_room(u16 size) {
  // TODO implement
}

void SlottedPage::slide(u16 start, u16 end) {
  // TODO implement
}

// Get 2-byte integer at given offset in block
u16 SlottedPage::get_n(u16 offset) {
  return *(u16*)this->address(offset);
}

// Put a 2-byte integer at given offset in block.
void SlottedPage::put_n(u16 offset, u16 n) {
  *(u16*)this->address(offset) = n;
}

// Make a void* pointer for a given offset into the data block.
void* SlottedPage::address(u16 offset) {
  return (void*)((char*)this->block.get_data() + offset);
}
