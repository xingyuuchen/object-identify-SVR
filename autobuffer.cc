#include "autobuffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include <errno.h>


AutoBuffer::AutoBuffer(size_t _malloc_unit_size)
        : byte_array_(NULL)
        , pos_(0)
        , length_(0)
        , capacity_(0)
        , malloc_unit_size_(_malloc_unit_size) {
}

AutoBuffer::AutoBuffer(const AutoBuffer &_auto_buffer)
        : malloc_unit_size_(128) {
    
}

void AutoBuffer::Write(const unsigned char *_byte_array, size_t _len) {
    if (_len <= 0 || _byte_array == NULL) {
        return;
    }
    if (capacity_ < length_ + _len) {
        size_t enlarge_size;
        if ((length_ + _len - capacity_) % malloc_unit_size_ == 0) {
            enlarge_size = length_ + _len - capacity_;
        } else {
            enlarge_size = ((length_ + _len - capacity_) / malloc_unit_size_ + 1) * malloc_unit_size_;
        }
        AddCapacity(enlarge_size);
    }
    memcpy(Ptr(length_), _byte_array, _len);
    length_ += _len;
}

size_t AutoBuffer::Pos() const {
    return pos_;
}

void AutoBuffer::Seek(const size_t _pos) {
    pos_ = _pos;
}

size_t AutoBuffer::Length() const {
    return length_;
}

unsigned char *AutoBuffer::Ptr(const size_t _offset) const {
    return byte_array_ + _offset;
}

void AutoBuffer::AddCapacity(size_t _size_to_add) {
    if (_size_to_add <= 0) {
        Log("Illegal arg _size:%ld", _size_to_add);
        return;
    }
    Log("[AutoBuffer::AddCapacity] _size_to_add: %ld", _size_to_add);
    void *p = realloc(byte_array_, capacity_ + _size_to_add);
    if (p == NULL) {
        Log("[AutoBuffer::AddCapacity] realloc failed, errno(%d): %s", errno, strerror(errno));
        return;
    }
    capacity_ += _size_to_add;
    byte_array_ = (unsigned char *) p;
}

size_t AutoBuffer::GetCapacity() const {
    return capacity_;
}

AutoBuffer::~AutoBuffer() {
    Reset();
}

void AutoBuffer::Reset() {
    capacity_ = 0;
    length_ = 0;
    pos_ = 0;
    if (byte_array_ != NULL) {
        free(byte_array_);
        byte_array_ = NULL;
    }
}

void AutoBuffer::SetLength(size_t _len) {
    if (_len >= 0) {
        length_ = _len;
    }
}

