#include "env-inl.h"
#include "node.h"
#include "node_buffer.h"
#include "node_external_reference.h"
#include "util.h"
#include "v8.h"
#include "xxhash.h"
#include "zlib.h"

namespace node {

namespace Hash {

using v8::ArrayBuffer;
using v8::ArrayBufferView;
using v8::BackingStore;
using v8::Context;
using v8::EscapableHandleScope;
using v8::FastApiTypedArray;
using v8::FunctionCallbackInfo;
using v8::Global;
using v8::HandleScope;
using v8::Int32;
using v8::Integer;
using v8::Isolate;
using v8::Just;
using v8::Local;
using v8::Maybe;
using v8::MaybeLocal;
using v8::NewStringType;
using v8::Nothing;
using v8::Number;
using v8::Object;
using v8::SharedArrayBuffer;
using v8::String;
using v8::Uint32;
using v8::Uint32Array;
using v8::Uint8Array;
using v8::Value;

namespace {

  static void XXHash32(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);

  Local<Object> buffer = args[0].As<Object>();
  size_t length = Buffer::Length(buffer);
  const char* data = Buffer::Data(buffer);

  uint32_t seed = 0;
  if (args.Length() > 1 && args[1]->IsUint32()) {
    seed = args[1]->Uint32Value(env->context()).FromJust();
  }

  uint32_t hash = XXHash32::hash(data, length, seed);
  args.GetReturnValue().Set(hash);
}

static void CRC32(const FunctionCallbackInfo<Value>& args) {
  Local<Object> buffer = args[0].As<Object>();
  size_t length = Buffer::Length(buffer);
  const char* data = Buffer::Data(buffer);

  uint32_t crc = crc32(0, reinterpret_cast<const Bytef*>(data), length);
  args.GetReturnValue().Set(crc);
}


static void Initialize(Local<Object> target,
                       Local<Value> unused,
                       Local<Context> context,
                       void* priv) {
  SetMethod(context, target, "xxhash32", XXHash32);
  SetMethod(context, target, "crc32", CRC32);
}

void RegisterExternalReferences(ExternalReferenceRegistry* registry) {

}
}

}

}

NODE_BINDING_CONTEXT_AWARE_INTERNAL(hash, node::Hash::Initialize)
NODE_BINDING_EXTERNAL_REFERENCE(hash,
                                node::Hash::RegisterExternalReferences)
