// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lobby.proto

#ifndef PROTOBUF_lobby_2eproto__INCLUDED
#define PROTOBUF_lobby_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace TLMP {
namespace LobbyMessages {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_lobby_2eproto();
void protobuf_AssignDesc_lobby_2eproto();
void protobuf_ShutdownFile_lobby_2eproto();

class Version;
class ClientPlayerName;
class BatchPlayerNames;
class PlayerLeft;
class ChatMessage;

// ===================================================================

class Version : public ::google::protobuf::Message {
 public:
  Version();
  virtual ~Version();
  
  Version(const Version& from);
  
  inline Version& operator=(const Version& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Version& default_instance();
  
  void Swap(Version* other);
  
  // implements Message ----------------------------------------------
  
  Version* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Version& from);
  void MergeFrom(const Version& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 version = 1;
  inline bool has_version() const;
  inline void clear_version();
  static const int kVersionFieldNumber = 1;
  inline ::google::protobuf::int32 version() const;
  inline void set_version(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:TLMP.LobbyMessages.Version)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 version_;
  friend void  protobuf_AddDesc_lobby_2eproto();
  friend void protobuf_AssignDesc_lobby_2eproto();
  friend void protobuf_ShutdownFile_lobby_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static Version* default_instance_;
};
// -------------------------------------------------------------------

class ClientPlayerName : public ::google::protobuf::Message {
 public:
  ClientPlayerName();
  virtual ~ClientPlayerName();
  
  ClientPlayerName(const ClientPlayerName& from);
  
  inline ClientPlayerName& operator=(const ClientPlayerName& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ClientPlayerName& default_instance();
  
  void Swap(ClientPlayerName* other);
  
  // implements Message ----------------------------------------------
  
  ClientPlayerName* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ClientPlayerName& from);
  void MergeFrom(const ClientPlayerName& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string playerName = 1;
  inline bool has_playername() const;
  inline void clear_playername();
  static const int kPlayerNameFieldNumber = 1;
  inline const ::std::string& playername() const;
  inline void set_playername(const ::std::string& value);
  inline void set_playername(const char* value);
  inline void set_playername(const char* value, size_t size);
  inline ::std::string* mutable_playername();
  
  // @@protoc_insertion_point(class_scope:TLMP.LobbyMessages.ClientPlayerName)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* playername_;
  static const ::std::string _default_playername_;
  friend void  protobuf_AddDesc_lobby_2eproto();
  friend void protobuf_AssignDesc_lobby_2eproto();
  friend void protobuf_ShutdownFile_lobby_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static ClientPlayerName* default_instance_;
};
// -------------------------------------------------------------------

class BatchPlayerNames : public ::google::protobuf::Message {
 public:
  BatchPlayerNames();
  virtual ~BatchPlayerNames();
  
  BatchPlayerNames(const BatchPlayerNames& from);
  
  inline BatchPlayerNames& operator=(const BatchPlayerNames& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const BatchPlayerNames& default_instance();
  
  void Swap(BatchPlayerNames* other);
  
  // implements Message ----------------------------------------------
  
  BatchPlayerNames* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BatchPlayerNames& from);
  void MergeFrom(const BatchPlayerNames& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated string playerNames = 1;
  inline int playernames_size() const;
  inline void clear_playernames();
  static const int kPlayerNamesFieldNumber = 1;
  inline const ::std::string& playernames(int index) const;
  inline ::std::string* mutable_playernames(int index);
  inline void set_playernames(int index, const ::std::string& value);
  inline void set_playernames(int index, const char* value);
  inline void set_playernames(int index, const char* value, size_t size);
  inline ::std::string* add_playernames();
  inline void add_playernames(const ::std::string& value);
  inline void add_playernames(const char* value);
  inline void add_playernames(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& playernames() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_playernames();
  
  // @@protoc_insertion_point(class_scope:TLMP.LobbyMessages.BatchPlayerNames)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::std::string> playernames_;
  friend void  protobuf_AddDesc_lobby_2eproto();
  friend void protobuf_AssignDesc_lobby_2eproto();
  friend void protobuf_ShutdownFile_lobby_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static BatchPlayerNames* default_instance_;
};
// -------------------------------------------------------------------

class PlayerLeft : public ::google::protobuf::Message {
 public:
  PlayerLeft();
  virtual ~PlayerLeft();
  
  PlayerLeft(const PlayerLeft& from);
  
  inline PlayerLeft& operator=(const PlayerLeft& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const PlayerLeft& default_instance();
  
  void Swap(PlayerLeft* other);
  
  // implements Message ----------------------------------------------
  
  PlayerLeft* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PlayerLeft& from);
  void MergeFrom(const PlayerLeft& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string playerName = 1;
  inline bool has_playername() const;
  inline void clear_playername();
  static const int kPlayerNameFieldNumber = 1;
  inline const ::std::string& playername() const;
  inline void set_playername(const ::std::string& value);
  inline void set_playername(const char* value);
  inline void set_playername(const char* value, size_t size);
  inline ::std::string* mutable_playername();
  
  // repeated string playersRemaining = 2;
  inline int playersremaining_size() const;
  inline void clear_playersremaining();
  static const int kPlayersRemainingFieldNumber = 2;
  inline const ::std::string& playersremaining(int index) const;
  inline ::std::string* mutable_playersremaining(int index);
  inline void set_playersremaining(int index, const ::std::string& value);
  inline void set_playersremaining(int index, const char* value);
  inline void set_playersremaining(int index, const char* value, size_t size);
  inline ::std::string* add_playersremaining();
  inline void add_playersremaining(const ::std::string& value);
  inline void add_playersremaining(const char* value);
  inline void add_playersremaining(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& playersremaining() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_playersremaining();
  
  // @@protoc_insertion_point(class_scope:TLMP.LobbyMessages.PlayerLeft)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* playername_;
  static const ::std::string _default_playername_;
  ::google::protobuf::RepeatedPtrField< ::std::string> playersremaining_;
  friend void  protobuf_AddDesc_lobby_2eproto();
  friend void protobuf_AssignDesc_lobby_2eproto();
  friend void protobuf_ShutdownFile_lobby_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static PlayerLeft* default_instance_;
};
// -------------------------------------------------------------------

class ChatMessage : public ::google::protobuf::Message {
 public:
  ChatMessage();
  virtual ~ChatMessage();
  
  ChatMessage(const ChatMessage& from);
  
  inline ChatMessage& operator=(const ChatMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ChatMessage& default_instance();
  
  void Swap(ChatMessage* other);
  
  // implements Message ----------------------------------------------
  
  ChatMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ChatMessage& from);
  void MergeFrom(const ChatMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string sender = 1;
  inline bool has_sender() const;
  inline void clear_sender();
  static const int kSenderFieldNumber = 1;
  inline const ::std::string& sender() const;
  inline void set_sender(const ::std::string& value);
  inline void set_sender(const char* value);
  inline void set_sender(const char* value, size_t size);
  inline ::std::string* mutable_sender();
  
  // required string message = 2;
  inline bool has_message() const;
  inline void clear_message();
  static const int kMessageFieldNumber = 2;
  inline const ::std::string& message() const;
  inline void set_message(const ::std::string& value);
  inline void set_message(const char* value);
  inline void set_message(const char* value, size_t size);
  inline ::std::string* mutable_message();
  
  // @@protoc_insertion_point(class_scope:TLMP.LobbyMessages.ChatMessage)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* sender_;
  static const ::std::string _default_sender_;
  ::std::string* message_;
  static const ::std::string _default_message_;
  friend void  protobuf_AddDesc_lobby_2eproto();
  friend void protobuf_AssignDesc_lobby_2eproto();
  friend void protobuf_ShutdownFile_lobby_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static ChatMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// Version

// required int32 version = 1;
inline bool Version::has_version() const {
  return _has_bit(0);
}
inline void Version::clear_version() {
  version_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 Version::version() const {
  return version_;
}
inline void Version::set_version(::google::protobuf::int32 value) {
  _set_bit(0);
  version_ = value;
}

// -------------------------------------------------------------------

// ClientPlayerName

// required string playerName = 1;
inline bool ClientPlayerName::has_playername() const {
  return _has_bit(0);
}
inline void ClientPlayerName::clear_playername() {
  if (playername_ != &_default_playername_) {
    playername_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& ClientPlayerName::playername() const {
  return *playername_;
}
inline void ClientPlayerName::set_playername(const ::std::string& value) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void ClientPlayerName::set_playername(const char* value) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void ClientPlayerName::set_playername(const char* value, size_t size) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ClientPlayerName::mutable_playername() {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  return playername_;
}

// -------------------------------------------------------------------

// BatchPlayerNames

// repeated string playerNames = 1;
inline int BatchPlayerNames::playernames_size() const {
  return playernames_.size();
}
inline void BatchPlayerNames::clear_playernames() {
  playernames_.Clear();
}
inline const ::std::string& BatchPlayerNames::playernames(int index) const {
  return playernames_.Get(index);
}
inline ::std::string* BatchPlayerNames::mutable_playernames(int index) {
  return playernames_.Mutable(index);
}
inline void BatchPlayerNames::set_playernames(int index, const ::std::string& value) {
  playernames_.Mutable(index)->assign(value);
}
inline void BatchPlayerNames::set_playernames(int index, const char* value) {
  playernames_.Mutable(index)->assign(value);
}
inline void BatchPlayerNames::set_playernames(int index, const char* value, size_t size) {
  playernames_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BatchPlayerNames::add_playernames() {
  return playernames_.Add();
}
inline void BatchPlayerNames::add_playernames(const ::std::string& value) {
  playernames_.Add()->assign(value);
}
inline void BatchPlayerNames::add_playernames(const char* value) {
  playernames_.Add()->assign(value);
}
inline void BatchPlayerNames::add_playernames(const char* value, size_t size) {
  playernames_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
BatchPlayerNames::playernames() const {
  return playernames_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
BatchPlayerNames::mutable_playernames() {
  return &playernames_;
}

// -------------------------------------------------------------------

// PlayerLeft

// required string playerName = 1;
inline bool PlayerLeft::has_playername() const {
  return _has_bit(0);
}
inline void PlayerLeft::clear_playername() {
  if (playername_ != &_default_playername_) {
    playername_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& PlayerLeft::playername() const {
  return *playername_;
}
inline void PlayerLeft::set_playername(const ::std::string& value) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void PlayerLeft::set_playername(const char* value) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void PlayerLeft::set_playername(const char* value, size_t size) {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  playername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* PlayerLeft::mutable_playername() {
  _set_bit(0);
  if (playername_ == &_default_playername_) {
    playername_ = new ::std::string;
  }
  return playername_;
}

// repeated string playersRemaining = 2;
inline int PlayerLeft::playersremaining_size() const {
  return playersremaining_.size();
}
inline void PlayerLeft::clear_playersremaining() {
  playersremaining_.Clear();
}
inline const ::std::string& PlayerLeft::playersremaining(int index) const {
  return playersremaining_.Get(index);
}
inline ::std::string* PlayerLeft::mutable_playersremaining(int index) {
  return playersremaining_.Mutable(index);
}
inline void PlayerLeft::set_playersremaining(int index, const ::std::string& value) {
  playersremaining_.Mutable(index)->assign(value);
}
inline void PlayerLeft::set_playersremaining(int index, const char* value) {
  playersremaining_.Mutable(index)->assign(value);
}
inline void PlayerLeft::set_playersremaining(int index, const char* value, size_t size) {
  playersremaining_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* PlayerLeft::add_playersremaining() {
  return playersremaining_.Add();
}
inline void PlayerLeft::add_playersremaining(const ::std::string& value) {
  playersremaining_.Add()->assign(value);
}
inline void PlayerLeft::add_playersremaining(const char* value) {
  playersremaining_.Add()->assign(value);
}
inline void PlayerLeft::add_playersremaining(const char* value, size_t size) {
  playersremaining_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
PlayerLeft::playersremaining() const {
  return playersremaining_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
PlayerLeft::mutable_playersremaining() {
  return &playersremaining_;
}

// -------------------------------------------------------------------

// ChatMessage

// required string sender = 1;
inline bool ChatMessage::has_sender() const {
  return _has_bit(0);
}
inline void ChatMessage::clear_sender() {
  if (sender_ != &_default_sender_) {
    sender_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& ChatMessage::sender() const {
  return *sender_;
}
inline void ChatMessage::set_sender(const ::std::string& value) {
  _set_bit(0);
  if (sender_ == &_default_sender_) {
    sender_ = new ::std::string;
  }
  sender_->assign(value);
}
inline void ChatMessage::set_sender(const char* value) {
  _set_bit(0);
  if (sender_ == &_default_sender_) {
    sender_ = new ::std::string;
  }
  sender_->assign(value);
}
inline void ChatMessage::set_sender(const char* value, size_t size) {
  _set_bit(0);
  if (sender_ == &_default_sender_) {
    sender_ = new ::std::string;
  }
  sender_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ChatMessage::mutable_sender() {
  _set_bit(0);
  if (sender_ == &_default_sender_) {
    sender_ = new ::std::string;
  }
  return sender_;
}

// required string message = 2;
inline bool ChatMessage::has_message() const {
  return _has_bit(1);
}
inline void ChatMessage::clear_message() {
  if (message_ != &_default_message_) {
    message_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& ChatMessage::message() const {
  return *message_;
}
inline void ChatMessage::set_message(const ::std::string& value) {
  _set_bit(1);
  if (message_ == &_default_message_) {
    message_ = new ::std::string;
  }
  message_->assign(value);
}
inline void ChatMessage::set_message(const char* value) {
  _set_bit(1);
  if (message_ == &_default_message_) {
    message_ = new ::std::string;
  }
  message_->assign(value);
}
inline void ChatMessage::set_message(const char* value, size_t size) {
  _set_bit(1);
  if (message_ == &_default_message_) {
    message_ = new ::std::string;
  }
  message_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ChatMessage::mutable_message() {
  _set_bit(1);
  if (message_ == &_default_message_) {
    message_ = new ::std::string;
  }
  return message_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace LobbyMessages
}  // namespace TLMP

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_lobby_2eproto__INCLUDED