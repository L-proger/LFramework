#pragma once

#include <LFramework/Detect/DetectOS.h>
#include <LFramework/Guid.h>
#include <LFramework/TypeTraits/FunctionTraits.h>

#include <cstdint>
#include <type_traits>
#include <atomic>

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

template<>
struct InterfaceAbi<IUnknown> {
public:
    using Base = void;
    static constexpr InterfaceID ID() { return { 0x00000000, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46} }; }
    template<class TInterface>
    friend class ComPtr;
    friend class ComObject;
private:
    virtual Result LFRAMEWORK_COM_CALL queryInterface(const InterfaceID& riid, void** ppvObject) = 0;
    virtual std::uint32_t LFRAMEWORK_COM_CALL addRef() = 0;
    virtual std::uint32_t LFRAMEWORK_COM_CALL release() = 0;
};

template<class TImplementer>
struct InterfaceRemap<IUnknown, TImplementer> {
    virtual Result LFRAMEWORK_COM_CALL queryInterface(const InterfaceID& riid, void** ppvObject) { return _implementer->queryInterface(riid, ppvObject); }
    virtual std::uint32_t LFRAMEWORK_COM_CALL addRef() { return _implementer->addRef(); }
    virtual std::uint32_t LFRAMEWORK_COM_CALL release() { return _implementer->release(); }
    TImplementer* _implementer;
};

template <class TInterface, class TComImplement>
struct IsComImplementSupportsInterface : std::false_type {};

template <class TInterface, class TImplementer, class TBase, class ... TInterfaceList>
struct IsComImplementSupportsInterface<TInterface, ComImplement<TImplementer, TBase, TInterfaceList...>> : HasInterface<TInterface, TInterfaceList...> {};

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


class ComObject  {
public:
    template<typename TRemap>
    friend struct RemapChainItem;

    ComObject() {
        _headRemap.next = &_headRemap;
    }

    virtual ~ComObject() = default;

    template<typename TInterface>
    InterfaceAbi<TInterface>* queryInterface() {
        void* result = nullptr;
        if(queryInterface(InterfaceAbi<TInterface>::ID(), &result) == Result::Ok){
            return reinterpret_cast<InterfaceAbi<TInterface>*>(result);
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
    std::atomic<unsigned long> _refCount{};
};






template<class TInterface>
class ComPtr {
public:
    using InterfacePtr = InterfaceAbi<TInterface>*;

    ComPtr() = default;

    ComPtr(InterfacePtr ptr) : _interface(ptr){
        if(_interface != nullptr){
            _interface->addRef();
        }
    }

    template<class U, class = std::enable_if_t<std::is_base_of_v<InterfaceAbi<TInterface>, InterfaceAbi<U>>>>
    ComPtr(ComPtr<U> ptr) : _interface(ptr._interface) {
        _interface->addRef();
    }

    ComPtr(const ComPtr& ptr) : _interface(ptr._interface) {
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

    InterfacePtr operator ->() {
        return _interface;
    }

    template<class U, class = std::enable_if_t<std::is_base_of_v<InterfaceAbi<TInterface>, InterfaceAbi<U>>>>
    operator ComPtr<U>(){
        return ComPtr<U>(_interface);
    }

    ComPtr operator = (std::nullptr_t) {
        reset();
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
            ComPtr result;
            result.attach(obj->template queryInterface<TInterface>());
            return result;
        }else{
            static_assert(IsInterfaceSupported<TImplementer, TInterface>(), "Intarfece not supported");
        }
    }
private:
    InterfacePtr _interface = nullptr;
};


}
