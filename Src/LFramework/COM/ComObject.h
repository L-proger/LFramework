#pragma once

#include <LFramework/Detect/DetectOS.h>
#include <LFramework/Guid.h>
#include <LFramework/TypeTraits/FunctionTraits.h>

#include <cstdint>
#include <type_traits>
#include <atomic>
#include <string>
#include <vector>
#include <cstring>

#if (LF_TARGET_OS == LF_OS_WINDOWS) || (LF_TARGET_OS == LF_OS_CYGWIN)
#define LFRAMEWORK_COM_CALL __stdcall
#else
#define LFRAMEWORK_COM_CALL
#endif

namespace LFramework {

using InterfaceID = Guid;

//IUnknown
class IUnknown;

template<class TInterface, class TImplementer>
struct InterfaceRemap {};

template<typename Remap>
struct RemapTraits {

};

template<class TInterface, class TImplementer>
struct RemapTraits<InterfaceRemap<TInterface, TImplementer>> {
    using InterfaceType = TInterface;
    using ImplementerType = TImplementer;
};

template<class Interface>
struct InterfaceAbi {};




template<class InterfaceAbi>
bool InterfaceAbiInterfaceSupported(const InterfaceID& id) {
    if (InterfaceAbi::ID() == id) {
        return true;
    }
    if constexpr (!std::is_same_v<typename InterfaceAbi::Base, void>) {
        return InterfaceAbiInterfaceSupported<typename InterfaceAbi::Base>(id);
    }
    return false;
};

struct RemapChainItemBase {
    using InterfaceAbiInterfaceSupportedPtr = bool(*)(const InterfaceID&);
    RemapChainItemBase* next = nullptr;
    InterfaceAbiInterfaceSupportedPtr isSupported = nullptr;
    void* getRemapPtr() {
        return reinterpret_cast<std::uint8_t*>(&isSupported) + sizeof(isSupported);
    }
};

template<typename TRemap>
struct RemapChainItem : RemapChainItemBase {
    RemapChainItem(typename RemapTraits<TRemap>::ImplementerType* implementer) {
        remap._implementer = implementer;
        this->isSupported = &InterfaceAbiInterfaceSupported<InterfaceAbi<typename RemapTraits<TRemap>::InterfaceType>>;
        implementer->registerRemap(this);
    }
    TRemap remap;
};

template<class Implementer, class FirstInterface, class ... Interfaces>
struct ComRemapList : ComRemapList<Implementer, Interfaces...> {
    using Base = ComRemapList<Implementer, Interfaces...>;
    ComRemapList(Implementer* _this) : Base(_this), remapContainer{ _this } {}
    RemapChainItem<InterfaceRemap<FirstInterface, Implementer>> remapContainer;
};

template<class Implementer, class FirstInterface>
struct ComRemapList<Implementer, FirstInterface> {
    ComRemapList(Implementer* _this) : remapContainer{ _this } {}
    RemapChainItem<InterfaceRemap<FirstInterface, Implementer>> remapContainer;
};

template<class T>
struct IsInterface : std::conditional_t<std::is_base_of_v<InterfaceAbi<IUnknown>, InterfaceAbi<T>>, std::true_type, std::false_type> {};

template<class...T>
struct IsAllInterfaces : std::conjunction<IsInterface<T>...> {};

template<class TImplementer, class TBase, class ... TInterfaceList>
struct ComImplement : public TBase {
    using ComImplement_BaseType = TBase;
    using ComImplement_SelfType = ComImplement;
    using Implementer = TImplementer;
    ComRemapList<TImplementer, TInterfaceList...> _remaps = { reinterpret_cast<TImplementer*>(this) };
};


template<class T, class = void>
struct IsComImplement : std::false_type {};

template<class T>
struct IsComImplement<T, std::void_t<typename T::ComImplement_SelfType>> : std::true_type {};

template<class...T>
struct IsAllComImplement : std::conjunction<IsComImplement<T>...> {};

template<class TInterface, class ... TInterfaceList>
struct HasInterface : std::disjunction<std::is_base_of<InterfaceAbi<TInterface>, InterfaceAbi<TInterfaceList>>...> {};

template<class TImplementer, class TInterface>
class ComDelegate;


template<class T>
struct IsComDelegate : std::false_type {};

template<class TImplementer, class TInterface>
struct IsComDelegate<ComDelegate<TImplementer, TInterface>> : std::true_type {};


enum class Result : uint32_t {
    Ok = 0,
    NotImplemented = 0x80004001L,
    NoInterface = 0x80004002L,
    ErrorPointer = 0x80004003L,
    UnknownFailure = 0x80004005L,
    OutOfMemory = 0x8007000EL,
    InvalidArg = 0x80070057L,
    Pending = 0x8000000AL,
    AsyncOperationNotStarted = 0x80000019L,
    RpcTimeout = 0x8001011FL,
    RpcDisconnected = 0x80010108L
};

struct ComException : std::exception {
   ComException(Result code) : _errorCode(code) {

   }
   ComException() : ComException(Result::UnknownFailure) {

   }
   ComException(Result code, std::string message) :_errorCode(code), _message(std::move(message)) {

   }
   ComException(std::string message) : ComException(Result::UnknownFailure, std::move(message)) {

   }
   Result code() const {
       return _errorCode;
   }
   char const* what() const noexcept override  {
       return _message.c_str();
   }
   const std::string& message() const {
       return _message;
   }
private:
   Result _errorCode;
   std::string _message;
};

template<>
struct InterfaceAbi<IUnknown> {
public:
    using Base = void;
    static constexpr InterfaceID ID() { return { 0x00000000, 0x0000000, 0x000000C0, 0x46000000 }; }
    template<class TInterface>
    friend class ComPtr;
    friend class ComObject;
    virtual Result LFRAMEWORK_COM_CALL queryInterface(const InterfaceID& riid, void** ppvObject) = 0;
    virtual std::uint32_t LFRAMEWORK_COM_CALL addRef() = 0;
    virtual std::uint32_t LFRAMEWORK_COM_CALL release() = 0;
    ~InterfaceAbi() = delete;
};

template<class TImplementer>
struct InterfaceRemap<IUnknown, TImplementer> {
    virtual Result LFRAMEWORK_COM_CALL queryInterface(const InterfaceID& riid, void** ppvObject) { return _implementer->queryInterface(riid, ppvObject); }
    virtual std::uint32_t LFRAMEWORK_COM_CALL addRef() { return _implementer->addRef(); }
    virtual std::uint32_t LFRAMEWORK_COM_CALL release() { return _implementer->release(); }
    inline auto implementer() {
        if constexpr (IsComDelegate<TImplementer>::value) {
            return _implementer->getImplementer();
        } else {
            return _implementer;
        }
    }
    TImplementer* _implementer;
    ~InterfaceRemap() = default;
};

template <class TInterface, class TComImplement>
struct IsComImplementSupportsInterface : std::false_type {};

template <class TInterface, class TImplementer, class TBase, class ... TInterfaceList>
struct IsComImplementSupportsInterface<TInterface, ComImplement<TImplementer, TBase, TInterfaceList...>> : HasInterface<TInterface, TInterfaceList...> {};

class RefCountedObject {
public:
    virtual ~RefCountedObject() = default;
    std::uint32_t addRef() {
        return _refCount.fetch_add(1) + 1;
    }
    std::uint32_t release() {
        auto result = _refCount.fetch_sub(1);
        if (result == 1) {
            delete this;
        }
        return result - 1;
    }
private:
    std::atomic<unsigned long> _refCount{};
};

template<class TInterface>
class ComPtr;


class ComObject : public RefCountedObject {
public:
    template<typename TRemap>
    friend struct RemapChainItem;

    ComObject() {
        _headRemap.next = &_headRemap;
    }

    template<typename TInterface>
    ComPtr<TInterface> queryInterface() {
        void* result = nullptr;
        if(queryInterface(InterfaceAbi<TInterface>::ID(), &result) == Result::Ok){
            auto abiPtr = reinterpret_cast<InterfaceAbi<TInterface>*>(result);
            ComPtr<TInterface> result;
            result.attach(abiPtr);
            return result;
        }
        return nullptr;
    }

    Result queryInterface(const InterfaceID& riid, void** ppvObject) {
        if (ppvObject == nullptr) {
            return Result::ErrorPointer;
        }

        findInterface(riid, ppvObject);

        if (*ppvObject == nullptr) {
            return Result::NoInterface;
        } else {
            auto obj = (reinterpret_cast<InterfaceAbi<IUnknown>*>(*ppvObject));
            obj->addRef();
            return Result::Ok;
        }
    }
   
private:
    void findInterface(const InterfaceID& id, void** result) {
        RemapChainItemBase* current = &_headRemap;
        do {
            if (current->isSupported(id)) {
                *result = current->getRemapPtr();
                return;
            }
            current = current->next;
        } while (current != &_headRemap);

        *result = nullptr;
    }
    void registerRemap(RemapChainItemBase* remap) {
        remap->next = _headRemap.next;
        _headRemap.next = remap;
    }

    RemapChainItem<InterfaceRemap<IUnknown, ComObject>> _headRemap{ this };
};


template<class TImplementer, class TInterface>
constexpr bool IsInterfaceSupported() {
    if constexpr(!LFramework::IsInterface<TInterface>::value || !IsComImplement<TImplementer>::value){
        return false;
    }else{
        if constexpr(std::is_same_v<TImplementer, ComObject> && std::is_same_v<TInterface, LFramework::IUnknown>){
            return true;
        }else{
            if constexpr (LFramework::IsComImplementSupportsInterface<TInterface, typename TImplementer::ComImplement_SelfType>::value){
                return true;
            }else{
                return IsInterfaceSupported<typename TImplementer::ComImplement_BaseType, TInterface>();
            }
        }
    }
}


template<class TInterface>
class InterfaceWrapper {
public:
    using NotSpecialized = bool;
};






template<class TInterface, class = void>
class HasInterfaceWrapper : public std::true_type {};

template<class TInterface>
class HasInterfaceWrapper<TInterface, std::void_t<typename LFramework::InterfaceWrapper<TInterface>::NotSpecialized>> : public std::false_type {};

template<class TInterface>
class ComPtr {
public:
    using InterfacePtr = InterfaceAbi<TInterface>*;

    using PublicInterfacePtr = std::conditional_t<HasInterfaceWrapper<TInterface>::value, InterfaceWrapper<TInterface>*, InterfacePtr>;

    ComPtr() = default;

    ComPtr(InterfacePtr ptr) : _interface(ptr){
        if(_interface != nullptr){
            _interface->addRef();
        }
    }

    template<class U, class = std::enable_if_t<std::is_base_of_v<InterfaceAbi<TInterface>, InterfaceAbi<U>>>>
    ComPtr(ComPtr<U> ptr) : _interface(ptr.get()) {
        _interface->addRef();
    }

    ComPtr(const ComPtr& ptr) : _interface(ptr.get()) {
        if(_interface != nullptr){
            _interface->addRef();
        }
    }

    template<class U>
    ComPtr<U> queryInterface(){
        ComPtr<U> result;
        if(_interface->queryInterface(InterfaceAbi<U>::ID(), result.put()) == Result::Ok){
            return result;
        }
        return {};
    }

    ~ComPtr() {
        reset();
    }

    PublicInterfacePtr operator ->() {
        if constexpr(HasInterfaceWrapper<TInterface>::value){
            return reinterpret_cast<PublicInterfacePtr>(&_interface);
        }else{
            return reinterpret_cast<InterfacePtr>(_interface);
        }

    }

    template<class U, class = std::enable_if_t<std::is_base_of_v<InterfaceAbi<TInterface>, InterfaceAbi<U>>>>
    operator ComPtr<U>(){
        return ComPtr<U>(_interface);
    }

    ComPtr operator = (std::nullptr_t) {
        reset();
        return *this;
    }

    ComPtr operator = (const ComPtr& other) {
        reset();
        auto ptr = other._interface;
        if (ptr != nullptr) {
            ptr->addRef();
        }
        _interface = ptr;
        return *this;
    }


    bool operator == (const ComPtr& other) const {
        return _interface == other._interface;
    }

    bool operator != (const ComPtr& other) const {
        return _interface != other._interface;
    }
    bool operator == (std::nullptr_t) const {
        return _interface == nullptr;
    }

    bool operator != (std::nullptr_t) const {
        return _interface != nullptr;
    }

    InterfacePtr operator*() {
        return _interface;
    }

    const InterfacePtr operator*() const {
        return _interface;
    }

    operator bool() const {
        return (_interface != nullptr);
    }

    InterfacePtr get() const {
        return _interface;
    }

    void** put() {
        return reinterpret_cast<void**>(&_interface);
    }

    InterfacePtr detach() {
        auto result = _interface;
        _interface = nullptr;
        return result;
    }

    void attach(InterfacePtr ptr) {
        reset();
        _interface = ptr;
    }
    void reset() {
        auto ptr = _interface;
        if (ptr != nullptr) {
            _interface = nullptr;
            ptr->release();
        }
    }

    template<class TImplementer, class ... TArgs>
    static ComPtr create(TArgs&& ... args) {
        if constexpr(IsInterfaceSupported<TImplementer, TInterface>()){
            auto obj = new TImplementer(std::forward<TArgs>(args)...);
            return obj->template queryInterface<TInterface>();
        }else{
            static_assert(IsInterfaceSupported<TImplementer, TInterface>(), "Interface not supported");
        }
    }
private:
    InterfacePtr _interface = nullptr;
};


template<class TImplementer, class TInterface>
class ComDelegate : public ComImplement<ComDelegate<TImplementer, TInterface>, ComObject, TInterface> {
public:
    using DelegatedImplementer = TImplementer;
    typedef void (TImplementer::*DelegateDestroyCallback)();

    ComDelegate(TImplementer* implementer, DelegateDestroyCallback destroyCallback)
        : _implementer(implementer), _destroyCallback(destroyCallback) {
        if constexpr (std::is_base_of_v<RefCountedObject, TImplementer>) {
            _implementer->addRef();
        }
    }
    ~ComDelegate() {
        if (_destroyCallback != nullptr) {
            ((_implementer)->*(_destroyCallback))();
        }
        if constexpr (std::is_base_of_v<RefCountedObject, TImplementer>) {
            _implementer->release();
        }
    }
    TImplementer* getImplementer() {
        return _implementer;
    }
private:
    DelegateDestroyCallback _destroyCallback;
    TImplementer* _implementer;
};


template<class TInterface, class TImplementer>
ComPtr<TInterface> makeComDelegate(TImplementer* implementer, typename ComDelegate<TImplementer, TInterface>::DelegateDestroyCallback delegateDestroyCallback = nullptr) {
    return ComPtr<TInterface>::template create<ComDelegate<TImplementer, TInterface>>(implementer, delegateDestroyCallback);
}


class ArrayOutMarshaler {
public:
    using SizeType = std::uint32_t;
    typedef void* ContextPtr;
    typedef void* (LFRAMEWORK_COM_CALL *ContainerResizeCallback)(ContextPtr context, SizeType size);

    ContextPtr context;
    ContainerResizeCallback callback;

    template<typename T>
    ArrayOutMarshaler(std::vector<T>& buffer) {
        context = &buffer;
        callback = &ArrayOutMarshaler::resizeCallback<T>;
    }

    ArrayOutMarshaler(std::string& buffer) {
        context = &buffer;
        callback = &ArrayOutMarshaler::resizeCallback;
    }
    template<typename T>
    void operator = (const std::vector<T>& source) {
        void* data = callback(context, static_cast<SizeType>(source.size()));
        if (data != nullptr && !source.empty()) {
            memcpy(data, source.data(), sizeof(T) * source.size());
        }
    }
    void operator = (const std::string& source) {
        void* data = callback(context, static_cast<SizeType>(source.size()));
        if (data != nullptr && !source.empty()) {
            memcpy(data, source.data(), source.size());
        }
    }
private:
    template<typename T>
    static void* LFRAMEWORK_COM_CALL resizeCallback(ContextPtr context, SizeType size) {
        auto container = reinterpret_cast<std::vector<T>*>(context);
        container->resize(size);
        return size == 0 ? nullptr : container->data();
    }
    static void* LFRAMEWORK_COM_CALL resizeCallback(ContextPtr context, SizeType size) {
        auto container = reinterpret_cast<std::string*>(context);
        container->resize(size);
        return size == 0 ? nullptr : container->data();
    }
    ArrayOutMarshaler() = delete;
};

class ArrayInMarshaler {
public:
    using SizeType = std::uint32_t;
    const void* data;
    SizeType itemsCount;

    ArrayInMarshaler(const std::string& source) {
        data = source.data();
        itemsCount = static_cast<SizeType>(source.size());
    }
    template<typename T>
    ArrayInMarshaler(const std::vector<T>& source) {
        data = source.data();
        itemsCount = static_cast<SizeType>(source.size());
    }
    operator std::string() const {
        return std::string(reinterpret_cast<const char*>(data), itemsCount);
    }
    template<typename T>
    operator std::vector<T>() const {
        std::vector<T> result;
        if (itemsCount != 0) {
            result.resize(itemsCount);
            memcpy(result.data(), data, sizeof(T) * itemsCount);
        }
        return result;
    }
private:
    ArrayInMarshaler() = delete;
};


template<>
class InterfaceWrapper<IUnknown> {
public:
    template<typename TInterface, typename = typename std::enable_if<std::is_base_of<InterfaceAbi<IUnknown>, InterfaceAbi<TInterface>>::value>::type>
    ComPtr<TInterface> queryInterface() {
        if (_abi == nullptr) {
            return {};
        }
        ComPtr<TInterface> result{};
        reinterpret_cast<InterfaceAbi<IUnknown>*>(_abi)->queryInterface(InterfaceAbi<TInterface>::ID(), reinterpret_cast<void**>(&result));
        return result;
    }
protected:
    void* _abi = nullptr;
};


}
