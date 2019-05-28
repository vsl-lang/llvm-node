#include "di-file.h"

Nan::Persistent<v8::FunctionTemplate> DIFileWrapper::functionTemplate {};

NAN_MODULE_INIT(DIFileWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DIFile").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->Inherit(Nan::New(DIScopeWrapper::functionTemplate));

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DIFile").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DIFileWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DIFile *file = static_cast<llvm::DIFile *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIFileWrapper { file };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> DIFileWrapper::of(llvm::DIFile *file) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DIFileWrapper* wrapper = new DIFileWrapper { file };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DIFile *DIFileWrapper::getDIFile() {
    return static_cast<llvm::DIFile*>(getDIScope());
}

bool DIFileWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
