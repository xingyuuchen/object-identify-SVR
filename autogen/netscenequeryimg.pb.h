// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: netscenequeryimg.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_netscenequeryimg_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_netscenequeryimg_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3013000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3013000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_netscenequeryimg_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_netscenequeryimg_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_netscenequeryimg_2eproto;
namespace netscenequeryimg {
class NetSceneQueryImgReq;
class NetSceneQueryImgReqDefaultTypeInternal;
extern NetSceneQueryImgReqDefaultTypeInternal _NetSceneQueryImgReq_default_instance_;
class NetSceneQueryImgResp;
class NetSceneQueryImgRespDefaultTypeInternal;
extern NetSceneQueryImgRespDefaultTypeInternal _NetSceneQueryImgResp_default_instance_;
}  // namespace netscenequeryimg
PROTOBUF_NAMESPACE_OPEN
template<> ::netscenequeryimg::NetSceneQueryImgReq* Arena::CreateMaybeMessage<::netscenequeryimg::NetSceneQueryImgReq>(Arena*);
template<> ::netscenequeryimg::NetSceneQueryImgResp* Arena::CreateMaybeMessage<::netscenequeryimg::NetSceneQueryImgResp>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace netscenequeryimg {

enum NetSceneQueryImgResp_ItemType : int {
  NetSceneQueryImgResp_ItemType_PLANT = 0,
  NetSceneQueryImgResp_ItemType_ANIMAL = 1,
  NetSceneQueryImgResp_ItemType_LANDMARK = 2
};
bool NetSceneQueryImgResp_ItemType_IsValid(int value);
constexpr NetSceneQueryImgResp_ItemType NetSceneQueryImgResp_ItemType_ItemType_MIN = NetSceneQueryImgResp_ItemType_PLANT;
constexpr NetSceneQueryImgResp_ItemType NetSceneQueryImgResp_ItemType_ItemType_MAX = NetSceneQueryImgResp_ItemType_LANDMARK;
constexpr int NetSceneQueryImgResp_ItemType_ItemType_ARRAYSIZE = NetSceneQueryImgResp_ItemType_ItemType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* NetSceneQueryImgResp_ItemType_descriptor();
template<typename T>
inline const std::string& NetSceneQueryImgResp_ItemType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, NetSceneQueryImgResp_ItemType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function NetSceneQueryImgResp_ItemType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    NetSceneQueryImgResp_ItemType_descriptor(), enum_t_value);
}
inline bool NetSceneQueryImgResp_ItemType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, NetSceneQueryImgResp_ItemType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<NetSceneQueryImgResp_ItemType>(
    NetSceneQueryImgResp_ItemType_descriptor(), name, value);
}
// ===================================================================

class NetSceneQueryImgReq PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:netscenequeryimg.NetSceneQueryImgReq) */ {
 public:
  inline NetSceneQueryImgReq() : NetSceneQueryImgReq(nullptr) {}
  virtual ~NetSceneQueryImgReq();

  NetSceneQueryImgReq(const NetSceneQueryImgReq& from);
  NetSceneQueryImgReq(NetSceneQueryImgReq&& from) noexcept
    : NetSceneQueryImgReq() {
    *this = ::std::move(from);
  }

  inline NetSceneQueryImgReq& operator=(const NetSceneQueryImgReq& from) {
    CopyFrom(from);
    return *this;
  }
  inline NetSceneQueryImgReq& operator=(NetSceneQueryImgReq&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const NetSceneQueryImgReq& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NetSceneQueryImgReq* internal_default_instance() {
    return reinterpret_cast<const NetSceneQueryImgReq*>(
               &_NetSceneQueryImgReq_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(NetSceneQueryImgReq& a, NetSceneQueryImgReq& b) {
    a.Swap(&b);
  }
  inline void Swap(NetSceneQueryImgReq* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NetSceneQueryImgReq* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NetSceneQueryImgReq* New() const final {
    return CreateMaybeMessage<NetSceneQueryImgReq>(nullptr);
  }

  NetSceneQueryImgReq* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NetSceneQueryImgReq>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NetSceneQueryImgReq& from);
  void MergeFrom(const NetSceneQueryImgReq& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NetSceneQueryImgReq* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "netscenequeryimg.NetSceneQueryImgReq";
  }
  protected:
  explicit NetSceneQueryImgReq(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_netscenequeryimg_2eproto);
    return ::descriptor_table_netscenequeryimg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kImgBytesFieldNumber = 1,
  };
  // optional bytes img_bytes = 1;
  bool has_img_bytes() const;
  private:
  bool _internal_has_img_bytes() const;
  public:
  void clear_img_bytes();
  const std::string& img_bytes() const;
  void set_img_bytes(const std::string& value);
  void set_img_bytes(std::string&& value);
  void set_img_bytes(const char* value);
  void set_img_bytes(const void* value, size_t size);
  std::string* mutable_img_bytes();
  std::string* release_img_bytes();
  void set_allocated_img_bytes(std::string* img_bytes);
  private:
  const std::string& _internal_img_bytes() const;
  void _internal_set_img_bytes(const std::string& value);
  std::string* _internal_mutable_img_bytes();
  public:

  // @@protoc_insertion_point(class_scope:netscenequeryimg.NetSceneQueryImgReq)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr img_bytes_;
  friend struct ::TableStruct_netscenequeryimg_2eproto;
};
// -------------------------------------------------------------------

class NetSceneQueryImgResp PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:netscenequeryimg.NetSceneQueryImgResp) */ {
 public:
  inline NetSceneQueryImgResp() : NetSceneQueryImgResp(nullptr) {}
  virtual ~NetSceneQueryImgResp();

  NetSceneQueryImgResp(const NetSceneQueryImgResp& from);
  NetSceneQueryImgResp(NetSceneQueryImgResp&& from) noexcept
    : NetSceneQueryImgResp() {
    *this = ::std::move(from);
  }

  inline NetSceneQueryImgResp& operator=(const NetSceneQueryImgResp& from) {
    CopyFrom(from);
    return *this;
  }
  inline NetSceneQueryImgResp& operator=(NetSceneQueryImgResp&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const NetSceneQueryImgResp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NetSceneQueryImgResp* internal_default_instance() {
    return reinterpret_cast<const NetSceneQueryImgResp*>(
               &_NetSceneQueryImgResp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(NetSceneQueryImgResp& a, NetSceneQueryImgResp& b) {
    a.Swap(&b);
  }
  inline void Swap(NetSceneQueryImgResp* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NetSceneQueryImgResp* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NetSceneQueryImgResp* New() const final {
    return CreateMaybeMessage<NetSceneQueryImgResp>(nullptr);
  }

  NetSceneQueryImgResp* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NetSceneQueryImgResp>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NetSceneQueryImgResp& from);
  void MergeFrom(const NetSceneQueryImgResp& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NetSceneQueryImgResp* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "netscenequeryimg.NetSceneQueryImgResp";
  }
  protected:
  explicit NetSceneQueryImgResp(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_netscenequeryimg_2eproto);
    return ::descriptor_table_netscenequeryimg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef NetSceneQueryImgResp_ItemType ItemType;
  static constexpr ItemType PLANT =
    NetSceneQueryImgResp_ItemType_PLANT;
  static constexpr ItemType ANIMAL =
    NetSceneQueryImgResp_ItemType_ANIMAL;
  static constexpr ItemType LANDMARK =
    NetSceneQueryImgResp_ItemType_LANDMARK;
  static inline bool ItemType_IsValid(int value) {
    return NetSceneQueryImgResp_ItemType_IsValid(value);
  }
  static constexpr ItemType ItemType_MIN =
    NetSceneQueryImgResp_ItemType_ItemType_MIN;
  static constexpr ItemType ItemType_MAX =
    NetSceneQueryImgResp_ItemType_ItemType_MAX;
  static constexpr int ItemType_ARRAYSIZE =
    NetSceneQueryImgResp_ItemType_ItemType_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  ItemType_descriptor() {
    return NetSceneQueryImgResp_ItemType_descriptor();
  }
  template<typename T>
  static inline const std::string& ItemType_Name(T enum_t_value) {
    static_assert(::std::is_same<T, ItemType>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function ItemType_Name.");
    return NetSceneQueryImgResp_ItemType_Name(enum_t_value);
  }
  static inline bool ItemType_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      ItemType* value) {
    return NetSceneQueryImgResp_ItemType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kItemNameFieldNumber = 2,
    kItemDescFieldNumber = 3,
    kItemTypeFieldNumber = 1,
  };
  // optional string item_name = 2;
  bool has_item_name() const;
  private:
  bool _internal_has_item_name() const;
  public:
  void clear_item_name();
  const std::string& item_name() const;
  void set_item_name(const std::string& value);
  void set_item_name(std::string&& value);
  void set_item_name(const char* value);
  void set_item_name(const char* value, size_t size);
  std::string* mutable_item_name();
  std::string* release_item_name();
  void set_allocated_item_name(std::string* item_name);
  private:
  const std::string& _internal_item_name() const;
  void _internal_set_item_name(const std::string& value);
  std::string* _internal_mutable_item_name();
  public:

  // optional string item_desc = 3;
  bool has_item_desc() const;
  private:
  bool _internal_has_item_desc() const;
  public:
  void clear_item_desc();
  const std::string& item_desc() const;
  void set_item_desc(const std::string& value);
  void set_item_desc(std::string&& value);
  void set_item_desc(const char* value);
  void set_item_desc(const char* value, size_t size);
  std::string* mutable_item_desc();
  std::string* release_item_desc();
  void set_allocated_item_desc(std::string* item_desc);
  private:
  const std::string& _internal_item_desc() const;
  void _internal_set_item_desc(const std::string& value);
  std::string* _internal_mutable_item_desc();
  public:

  // optional .netscenequeryimg.NetSceneQueryImgResp.ItemType item_type = 1 [default = PLANT];
  bool has_item_type() const;
  private:
  bool _internal_has_item_type() const;
  public:
  void clear_item_type();
  ::netscenequeryimg::NetSceneQueryImgResp_ItemType item_type() const;
  void set_item_type(::netscenequeryimg::NetSceneQueryImgResp_ItemType value);
  private:
  ::netscenequeryimg::NetSceneQueryImgResp_ItemType _internal_item_type() const;
  void _internal_set_item_type(::netscenequeryimg::NetSceneQueryImgResp_ItemType value);
  public:

  // @@protoc_insertion_point(class_scope:netscenequeryimg.NetSceneQueryImgResp)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr item_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr item_desc_;
  int item_type_;
  friend struct ::TableStruct_netscenequeryimg_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NetSceneQueryImgReq

// optional bytes img_bytes = 1;
inline bool NetSceneQueryImgReq::_internal_has_img_bytes() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NetSceneQueryImgReq::has_img_bytes() const {
  return _internal_has_img_bytes();
}
inline void NetSceneQueryImgReq::clear_img_bytes() {
  img_bytes_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& NetSceneQueryImgReq::img_bytes() const {
  // @@protoc_insertion_point(field_get:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
  return _internal_img_bytes();
}
inline void NetSceneQueryImgReq::set_img_bytes(const std::string& value) {
  _internal_set_img_bytes(value);
  // @@protoc_insertion_point(field_set:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
}
inline std::string* NetSceneQueryImgReq::mutable_img_bytes() {
  // @@protoc_insertion_point(field_mutable:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
  return _internal_mutable_img_bytes();
}
inline const std::string& NetSceneQueryImgReq::_internal_img_bytes() const {
  return img_bytes_.Get();
}
inline void NetSceneQueryImgReq::_internal_set_img_bytes(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  img_bytes_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void NetSceneQueryImgReq::set_img_bytes(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  img_bytes_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
}
inline void NetSceneQueryImgReq::set_img_bytes(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  img_bytes_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
}
inline void NetSceneQueryImgReq::set_img_bytes(const void* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  img_bytes_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
}
inline std::string* NetSceneQueryImgReq::_internal_mutable_img_bytes() {
  _has_bits_[0] |= 0x00000001u;
  return img_bytes_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* NetSceneQueryImgReq::release_img_bytes() {
  // @@protoc_insertion_point(field_release:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
  if (!_internal_has_img_bytes()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return img_bytes_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void NetSceneQueryImgReq::set_allocated_img_bytes(std::string* img_bytes) {
  if (img_bytes != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  img_bytes_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), img_bytes,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:netscenequeryimg.NetSceneQueryImgReq.img_bytes)
}

// -------------------------------------------------------------------

// NetSceneQueryImgResp

// optional .netscenequeryimg.NetSceneQueryImgResp.ItemType item_type = 1 [default = PLANT];
inline bool NetSceneQueryImgResp::_internal_has_item_type() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool NetSceneQueryImgResp::has_item_type() const {
  return _internal_has_item_type();
}
inline void NetSceneQueryImgResp::clear_item_type() {
  item_type_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::netscenequeryimg::NetSceneQueryImgResp_ItemType NetSceneQueryImgResp::_internal_item_type() const {
  return static_cast< ::netscenequeryimg::NetSceneQueryImgResp_ItemType >(item_type_);
}
inline ::netscenequeryimg::NetSceneQueryImgResp_ItemType NetSceneQueryImgResp::item_type() const {
  // @@protoc_insertion_point(field_get:netscenequeryimg.NetSceneQueryImgResp.item_type)
  return _internal_item_type();
}
inline void NetSceneQueryImgResp::_internal_set_item_type(::netscenequeryimg::NetSceneQueryImgResp_ItemType value) {
  assert(::netscenequeryimg::NetSceneQueryImgResp_ItemType_IsValid(value));
  _has_bits_[0] |= 0x00000004u;
  item_type_ = value;
}
inline void NetSceneQueryImgResp::set_item_type(::netscenequeryimg::NetSceneQueryImgResp_ItemType value) {
  _internal_set_item_type(value);
  // @@protoc_insertion_point(field_set:netscenequeryimg.NetSceneQueryImgResp.item_type)
}

// optional string item_name = 2;
inline bool NetSceneQueryImgResp::_internal_has_item_name() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NetSceneQueryImgResp::has_item_name() const {
  return _internal_has_item_name();
}
inline void NetSceneQueryImgResp::clear_item_name() {
  item_name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& NetSceneQueryImgResp::item_name() const {
  // @@protoc_insertion_point(field_get:netscenequeryimg.NetSceneQueryImgResp.item_name)
  return _internal_item_name();
}
inline void NetSceneQueryImgResp::set_item_name(const std::string& value) {
  _internal_set_item_name(value);
  // @@protoc_insertion_point(field_set:netscenequeryimg.NetSceneQueryImgResp.item_name)
}
inline std::string* NetSceneQueryImgResp::mutable_item_name() {
  // @@protoc_insertion_point(field_mutable:netscenequeryimg.NetSceneQueryImgResp.item_name)
  return _internal_mutable_item_name();
}
inline const std::string& NetSceneQueryImgResp::_internal_item_name() const {
  return item_name_.Get();
}
inline void NetSceneQueryImgResp::_internal_set_item_name(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void NetSceneQueryImgResp::set_item_name(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  item_name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:netscenequeryimg.NetSceneQueryImgResp.item_name)
}
inline void NetSceneQueryImgResp::set_item_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:netscenequeryimg.NetSceneQueryImgResp.item_name)
}
inline void NetSceneQueryImgResp::set_item_name(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:netscenequeryimg.NetSceneQueryImgResp.item_name)
}
inline std::string* NetSceneQueryImgResp::_internal_mutable_item_name() {
  _has_bits_[0] |= 0x00000001u;
  return item_name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* NetSceneQueryImgResp::release_item_name() {
  // @@protoc_insertion_point(field_release:netscenequeryimg.NetSceneQueryImgResp.item_name)
  if (!_internal_has_item_name()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return item_name_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void NetSceneQueryImgResp::set_allocated_item_name(std::string* item_name) {
  if (item_name != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  item_name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), item_name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:netscenequeryimg.NetSceneQueryImgResp.item_name)
}

// optional string item_desc = 3;
inline bool NetSceneQueryImgResp::_internal_has_item_desc() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool NetSceneQueryImgResp::has_item_desc() const {
  return _internal_has_item_desc();
}
inline void NetSceneQueryImgResp::clear_item_desc() {
  item_desc_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& NetSceneQueryImgResp::item_desc() const {
  // @@protoc_insertion_point(field_get:netscenequeryimg.NetSceneQueryImgResp.item_desc)
  return _internal_item_desc();
}
inline void NetSceneQueryImgResp::set_item_desc(const std::string& value) {
  _internal_set_item_desc(value);
  // @@protoc_insertion_point(field_set:netscenequeryimg.NetSceneQueryImgResp.item_desc)
}
inline std::string* NetSceneQueryImgResp::mutable_item_desc() {
  // @@protoc_insertion_point(field_mutable:netscenequeryimg.NetSceneQueryImgResp.item_desc)
  return _internal_mutable_item_desc();
}
inline const std::string& NetSceneQueryImgResp::_internal_item_desc() const {
  return item_desc_.Get();
}
inline void NetSceneQueryImgResp::_internal_set_item_desc(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  item_desc_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void NetSceneQueryImgResp::set_item_desc(std::string&& value) {
  _has_bits_[0] |= 0x00000002u;
  item_desc_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:netscenequeryimg.NetSceneQueryImgResp.item_desc)
}
inline void NetSceneQueryImgResp::set_item_desc(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000002u;
  item_desc_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:netscenequeryimg.NetSceneQueryImgResp.item_desc)
}
inline void NetSceneQueryImgResp::set_item_desc(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000002u;
  item_desc_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:netscenequeryimg.NetSceneQueryImgResp.item_desc)
}
inline std::string* NetSceneQueryImgResp::_internal_mutable_item_desc() {
  _has_bits_[0] |= 0x00000002u;
  return item_desc_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* NetSceneQueryImgResp::release_item_desc() {
  // @@protoc_insertion_point(field_release:netscenequeryimg.NetSceneQueryImgResp.item_desc)
  if (!_internal_has_item_desc()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return item_desc_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void NetSceneQueryImgResp::set_allocated_item_desc(std::string* item_desc) {
  if (item_desc != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  item_desc_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), item_desc,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:netscenequeryimg.NetSceneQueryImgResp.item_desc)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace netscenequeryimg

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::netscenequeryimg::NetSceneQueryImgResp_ItemType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::netscenequeryimg::NetSceneQueryImgResp_ItemType>() {
  return ::netscenequeryimg::NetSceneQueryImgResp_ItemType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_netscenequeryimg_2eproto
