// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: datacode.proto

#include "datacode.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

PROTOBUF_CONSTEXPR proto_package::proto_package(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.name_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.data_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct proto_packageDefaultTypeInternal {
  PROTOBUF_CONSTEXPR proto_packageDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~proto_packageDefaultTypeInternal() {}
  union {
    proto_package _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 proto_packageDefaultTypeInternal _proto_package_default_instance_;
PROTOBUF_CONSTEXPR _file::_file(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.cmd_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.data_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.st_s_)*/0
  , /*decltype(_impl_.st_e_)*/0
  , /*decltype(_impl_.total_)*/int64_t{0}
  , /*decltype(_impl_.id_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct _fileDefaultTypeInternal {
  PROTOBUF_CONSTEXPR _fileDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~_fileDefaultTypeInternal() {}
  union {
    _file _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 _fileDefaultTypeInternal __file_default_instance_;
static ::_pb::Metadata file_level_metadata_datacode_2eproto[2];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_datacode_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_datacode_2eproto = nullptr;

const uint32_t TableStruct_datacode_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto_package, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto_package, _impl_.name_),
  PROTOBUF_FIELD_OFFSET(::proto_package, _impl_.data_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::_file, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.cmd_),
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.st_s_),
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.st_e_),
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.data_),
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.total_),
  PROTOBUF_FIELD_OFFSET(::_file, _impl_.id_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto_package)},
  { 8, -1, -1, sizeof(::_file)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::_proto_package_default_instance_._instance,
  &::__file_default_instance_._instance,
};

const char descriptor_table_protodef_datacode_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016datacode.proto\"+\n\rproto_package\022\014\n\004nam"
  "e\030\001 \001(\014\022\014\n\004data\030\002 \001(\014\"\340\001\n\005_file\022\013\n\003cmd\030\001"
  " \001(\014\022\037\n\004st_s\030\002 \001(\0162\021._file.FILESTATUS\022\037\n"
  "\004st_e\030\003 \001(\0162\021._file.FILESTATUS\022\014\n\004data\030\004"
  " \001(\014\022\r\n\005total\030\005 \001(\003\022\n\n\002id\030\006 \001(\005\"_\n\nFILES"
  "TATUS\022\010\n\004NONE\020\000\022\t\n\005BEGIN\020\001\022\010\n\004BODY\020\002\022\007\n\003"
  "END\020\003\022\013\n\007REBEGIN\020\004\022\014\n\010BEGINEND\020\005\022\016\n\nREBE"
  "GINEND\020\006b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_datacode_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_datacode_2eproto = {
    false, false, 296, descriptor_table_protodef_datacode_2eproto,
    "datacode.proto",
    &descriptor_table_datacode_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_datacode_2eproto::offsets,
    file_level_metadata_datacode_2eproto, file_level_enum_descriptors_datacode_2eproto,
    file_level_service_descriptors_datacode_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_datacode_2eproto_getter() {
  return &descriptor_table_datacode_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_datacode_2eproto(&descriptor_table_datacode_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* _file_FILESTATUS_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_datacode_2eproto);
  return file_level_enum_descriptors_datacode_2eproto[0];
}
bool _file_FILESTATUS_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
constexpr _file_FILESTATUS _file::NONE;
constexpr _file_FILESTATUS _file::BEGIN;
constexpr _file_FILESTATUS _file::BODY;
constexpr _file_FILESTATUS _file::END;
constexpr _file_FILESTATUS _file::REBEGIN;
constexpr _file_FILESTATUS _file::BEGINEND;
constexpr _file_FILESTATUS _file::REBEGINEND;
constexpr _file_FILESTATUS _file::FILESTATUS_MIN;
constexpr _file_FILESTATUS _file::FILESTATUS_MAX;
constexpr int _file::FILESTATUS_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))

// ===================================================================

class proto_package::_Internal {
 public:
};

proto_package::proto_package(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto_package)
}
proto_package::proto_package(const proto_package& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  proto_package* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.name_){}
    , decltype(_impl_.data_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_name().empty()) {
    _this->_impl_.name_.Set(from._internal_name(), 
      _this->GetArenaForAllocation());
  }
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_data().empty()) {
    _this->_impl_.data_.Set(from._internal_data(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:proto_package)
}

inline void proto_package::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.name_){}
    , decltype(_impl_.data_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

proto_package::~proto_package() {
  // @@protoc_insertion_point(destructor:proto_package)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void proto_package::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.name_.Destroy();
  _impl_.data_.Destroy();
}

void proto_package::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void proto_package::Clear() {
// @@protoc_insertion_point(message_clear_start:proto_package)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.name_.ClearToEmpty();
  _impl_.data_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* proto_package::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes name = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_name();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes data = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_data();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* proto_package::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto_package)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes name = 1;
  if (!this->_internal_name().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_name(), target);
  }

  // bytes data = 2;
  if (!this->_internal_data().empty()) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_data(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto_package)
  return target;
}

size_t proto_package::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto_package)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes name = 1;
  if (!this->_internal_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_name());
  }

  // bytes data = 2;
  if (!this->_internal_data().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_data());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData proto_package::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    proto_package::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*proto_package::GetClassData() const { return &_class_data_; }


void proto_package::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<proto_package*>(&to_msg);
  auto& from = static_cast<const proto_package&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto_package)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _this->_internal_set_name(from._internal_name());
  }
  if (!from._internal_data().empty()) {
    _this->_internal_set_data(from._internal_data());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void proto_package::CopyFrom(const proto_package& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto_package)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool proto_package::IsInitialized() const {
  return true;
}

void proto_package::InternalSwap(proto_package* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.name_, lhs_arena,
      &other->_impl_.name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.data_, lhs_arena,
      &other->_impl_.data_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata proto_package::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_datacode_2eproto_getter, &descriptor_table_datacode_2eproto_once,
      file_level_metadata_datacode_2eproto[0]);
}

// ===================================================================

class _file::_Internal {
 public:
};

_file::_file(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:_file)
}
_file::_file(const _file& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _file* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.cmd_){}
    , decltype(_impl_.data_){}
    , decltype(_impl_.st_s_){}
    , decltype(_impl_.st_e_){}
    , decltype(_impl_.total_){}
    , decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.cmd_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.cmd_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_cmd().empty()) {
    _this->_impl_.cmd_.Set(from._internal_cmd(), 
      _this->GetArenaForAllocation());
  }
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_data().empty()) {
    _this->_impl_.data_.Set(from._internal_data(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.st_s_, &from._impl_.st_s_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.id_) -
    reinterpret_cast<char*>(&_impl_.st_s_)) + sizeof(_impl_.id_));
  // @@protoc_insertion_point(copy_constructor:_file)
}

inline void _file::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.cmd_){}
    , decltype(_impl_.data_){}
    , decltype(_impl_.st_s_){0}
    , decltype(_impl_.st_e_){0}
    , decltype(_impl_.total_){int64_t{0}}
    , decltype(_impl_.id_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.cmd_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.cmd_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

_file::~_file() {
  // @@protoc_insertion_point(destructor:_file)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void _file::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.cmd_.Destroy();
  _impl_.data_.Destroy();
}

void _file::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void _file::Clear() {
// @@protoc_insertion_point(message_clear_start:_file)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.cmd_.ClearToEmpty();
  _impl_.data_.ClearToEmpty();
  ::memset(&_impl_.st_s_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.id_) -
      reinterpret_cast<char*>(&_impl_.st_s_)) + sizeof(_impl_.id_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* _file::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes cmd = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_cmd();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // ._file.FILESTATUS st_s = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_st_s(static_cast<::_file_FILESTATUS>(val));
        } else
          goto handle_unusual;
        continue;
      // ._file.FILESTATUS st_e = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_st_e(static_cast<::_file_FILESTATUS>(val));
        } else
          goto handle_unusual;
        continue;
      // bytes data = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_data();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 total = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 40)) {
          _impl_.total_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 id = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 48)) {
          _impl_.id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* _file::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:_file)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes cmd = 1;
  if (!this->_internal_cmd().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_cmd(), target);
  }

  // ._file.FILESTATUS st_s = 2;
  if (this->_internal_st_s() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      2, this->_internal_st_s(), target);
  }

  // ._file.FILESTATUS st_e = 3;
  if (this->_internal_st_e() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      3, this->_internal_st_e(), target);
  }

  // bytes data = 4;
  if (!this->_internal_data().empty()) {
    target = stream->WriteBytesMaybeAliased(
        4, this->_internal_data(), target);
  }

  // int64 total = 5;
  if (this->_internal_total() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(5, this->_internal_total(), target);
  }

  // int32 id = 6;
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(6, this->_internal_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:_file)
  return target;
}

size_t _file::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:_file)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes cmd = 1;
  if (!this->_internal_cmd().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_cmd());
  }

  // bytes data = 4;
  if (!this->_internal_data().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_data());
  }

  // ._file.FILESTATUS st_s = 2;
  if (this->_internal_st_s() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_st_s());
  }

  // ._file.FILESTATUS st_e = 3;
  if (this->_internal_st_e() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_st_e());
  }

  // int64 total = 5;
  if (this->_internal_total() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_total());
  }

  // int32 id = 6;
  if (this->_internal_id() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_id());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData _file::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    _file::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*_file::GetClassData() const { return &_class_data_; }


void _file::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<_file*>(&to_msg);
  auto& from = static_cast<const _file&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:_file)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_cmd().empty()) {
    _this->_internal_set_cmd(from._internal_cmd());
  }
  if (!from._internal_data().empty()) {
    _this->_internal_set_data(from._internal_data());
  }
  if (from._internal_st_s() != 0) {
    _this->_internal_set_st_s(from._internal_st_s());
  }
  if (from._internal_st_e() != 0) {
    _this->_internal_set_st_e(from._internal_st_e());
  }
  if (from._internal_total() != 0) {
    _this->_internal_set_total(from._internal_total());
  }
  if (from._internal_id() != 0) {
    _this->_internal_set_id(from._internal_id());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void _file::CopyFrom(const _file& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:_file)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool _file::IsInitialized() const {
  return true;
}

void _file::InternalSwap(_file* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.cmd_, lhs_arena,
      &other->_impl_.cmd_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.data_, lhs_arena,
      &other->_impl_.data_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(_file, _impl_.id_)
      + sizeof(_file::_impl_.id_)
      - PROTOBUF_FIELD_OFFSET(_file, _impl_.st_s_)>(
          reinterpret_cast<char*>(&_impl_.st_s_),
          reinterpret_cast<char*>(&other->_impl_.st_s_));
}

::PROTOBUF_NAMESPACE_ID::Metadata _file::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_datacode_2eproto_getter, &descriptor_table_datacode_2eproto_once,
      file_level_metadata_datacode_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto_package*
Arena::CreateMaybeMessage< ::proto_package >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto_package >(arena);
}
template<> PROTOBUF_NOINLINE ::_file*
Arena::CreateMaybeMessage< ::_file >(Arena* arena) {
  return Arena::CreateMessageInternal< ::_file >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>