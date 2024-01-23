#pragma once
#include <cassert>
#include <functional>
#include <list>
#include <optional>
#include <type_traits>

template <class TData>
bool constexpr isArithmetic = std::is_arithmetic_v<TData>;  // NOLINT

template <class TData>
bool constexpr isPointer = std::is_pointer_v<TData>;  // NOLINT

template <class TData>
bool constexpr isEnum = std::is_enum_v<TData>;  // NOLINT

template <class TData>
bool constexpr isSimpleClass =  // NOLINT
    isArithmetic<TData> || isPointer<TData> || isEnum<TData>;

struct CByValue;
struct CByReference;

template <class TData, class TSendBy>
struct CDataSentByImpl;

template <class TData>
struct CDataSentByImpl<TData, CByValue> {
    using CType = TData;
};

template <class TData>
struct CDataSentByImpl<TData, CByReference> {
    using CType = const TData&;
};

template <class TData, class TSendBy>
using CDataSentBy = typename CDataSentByImpl<TData, TSendBy>::CType;

template <bool TFlag>
struct AutoSendByImpl;

template <>
struct AutoSendByImpl<true> {
    using CType = CByValue;
};

template <>
struct AutoSendByImpl<false> {
    using CType = CByReference;
};

template <class TData>
using AutoSendBy = std::conditional_t<std::is_same_v<TData, void>, void,
                                      typename AutoSendByImpl<isSimpleClass<TData>>::CType>;

template <class TData, class TSendBy>
struct CStoreWrapperImpl;

template <class TData>
struct CStoreWrapperImpl<TData, CByValue> {
    using CType = TData;
};

template <class TData>
struct CStoreWrapperImpl<TData, CByReference> {
    using CType = std::reference_wrapper<std::add_const_t<TData>>;
};

template <class TData, class TSendBy>
using CStoreWrapper = typename CStoreWrapperImpl<TData, TSendBy>::CType;

template <class TData, class TSendBy>
class CObserver;

template <class TData, class TSendBy>
class CObservable;

template <class TData, class TSendBy = AutoSendBy<TData>>
class CObserver {
    using CData = TData;
    using CSendBy = TSendBy;

    using CObservable = CObservable<CData, CSendBy>;

    using CDataSentBy = CDataSentBy<CData, CSendBy>;
    using CSignature = void(CDataSentBy);
    using CAction = std::function<CSignature>;

    friend CObservable;

public:
    using CArg = CDataSentBy;

    template <class T1, class T2, class T3>
    CObserver(T1&& onSubscribe, T2&& onNotify, T3&& onUnsubscribe)  // NOLINT
        : onSubscribe_(std::forward<T1>(onSubscribe)),
          onNotify_(std::forward<T2>(onNotify)),
          onUnsubscribe_(std::forward<T3>(onUnsubscribe)) {
        assert(onSubscribe_);
        assert(onNotify_);
        assert(onUnsubscribe_);
    }
    CObserver(const CObserver&) = delete;
    CObserver(CObserver&&) noexcept = delete;
    CObserver& operator=(const CObserver&) = delete;
    CObserver& operator=(CObserver&&) noexcept = delete;
    ~CObserver() {
        unsubscribe();
    }
    void unsubscribe();          // NOLINT
    bool isSubscribed() const {  // NOLINT
        return Observable_ != nullptr;
    }
    bool hasData() const;                 // NOLINT
    CDataSentBy data() const;             // NOLINT
    static void doNothing(CDataSentBy) {  // NOLINT
    }

private:
    void setObservable(CObservable* observable) {  // NOLINT
        assert(observable);
        Observable_ = observable;
    }

    CObservable* Observable_ = nullptr;  // NOLINT
    CAction onSubscribe_;                // NOLINT
    CAction onNotify_;                   // NOLINT
    CAction onUnsubscribe_;              // NOLINT
};

template <class TData, class TSendBy = AutoSendBy<TData>>  // NOLINT
class CObservable {
    using CData = TData;
    using CSendBy = TSendBy;

    using CObserver = CObserver<CData, CSendBy>;
    using CObserversContainer = std::list<CObserver*>;

    using CDataSentBy = CDataSentBy<CData, CSendBy>;
    using CStoreWrapper = CStoreWrapper<CData, CSendBy>;

    using CStoredData = std::optional<CStoreWrapper>;
    using CSignature = CStoredData();
    using CGetAction = std::function<CSignature>;

    using CListeners = std::list<CObserver*>;

    friend CObserver;

public:
    using CGetType = CStoredData;
    using CReturn = CDataSentBy;

    template <class TF>
    CObservable(TF&& Data) : Data_(std::forward<TF>(Data)) {  // NOLINT
        assert(Data_);
    }
    CObservable(const CObservable&) = delete;
    CObservable(CObservable&&) noexcept = delete;
    CObservable& operator=(const CObservable&) = delete;
    CObservable& operator=(CObservable&&) noexcept = delete;
    ~CObservable() {
        unsubscribeAll();
    }
    void notify() const {                  // NOLINT
        CStoredData Data = Data_();        // NOLINT
        if (!Data.has_value())             // NOLINT
            return;                        // NOLINT
        for (CObserver* obs : Listeners_)  // NOLINT
            obs->onNotify_(*Data);         // NOLINT
    }
    void subscribe(CObserver* obs) {  // NOLINT
        assert(obs);
        if (obs->isSubscribed())  // NOLINT
            obs->unsubscribe();   // NOLINT
        Listeners_.push_back(obs);
        obs->setObservable(this);
        CStoredData Data = Data_();               // NOLINT
        if (Data.has_value())                     // NOLINT
            obs->onSubscribe_(std::move(*Data));  // NOLINT
    }
    void unsubscribeAll() {                     // NOLINT
        while (!Listeners_.empty())             // NOLINT
            Listeners_.front()->unsubscribe();  // NOLINT
    }

private:
    void detach_(CObserver* obs) {  // NOLINT
        assert(obs);
        CStoredData Data = Data_();                 // NOLINT
        if (Data.has_value())                       // NOLINT
            obs->onUnsubscribe_(std::move(*Data));  // NOLINT
        Listeners_.remove(obs);                     // NOLINT
    }
    CGetAction Data_;       // NOLINT
    CListeners Listeners_;  // NOLINT
};

template <class TData, class TSendBy>
void CObserver<TData, TSendBy>::unsubscribe() {
    if (!isSubscribed())  // NOLINT
        return;           // NOLINT
    Observable_->detach_(this);
    Observable_ = nullptr;
}

template <class TData, class TSendBy>
bool CObserver<TData, TSendBy>::hasData() const {
    return isSubscribed() && Observable_->Data_().has_value();
}

template <class TData, class TSendBy>
typename CObserver<TData, TSendBy>::CDataSentBy CObserver<TData, TSendBy>::data() const {
    assert(Observable_);
    return *Observable_->Data_();
}

template <>
class CObserver<void, void> {
    using CData = void;
    using CSendBy = void;

    using CObservable = CObservable<CData, CSendBy>;

    using CSignature = void();
    using CAction = std::function<CSignature>;

    friend CObservable;

public:
    using CArg = void;

    template <class T1, class T2, class T3>
    CObserver(T1&& onSubscribe, T2&& onNotify, T3&& onUnsubscribe)  // NOLINT
        : onSubscribe_(std::forward<T1>(onSubscribe)),
          onNotify_(std::forward<T2>(onNotify)),
          onUnsubscribe_(std::forward<T3>(onUnsubscribe)) {
        assert(onSubscribe_);
        assert(onNotify_);
        assert(onUnsubscribe_);
    }
    CObserver(const CObserver&) = delete;
    CObserver(CObserver&&) noexcept = delete;
    CObserver& operator=(const CObserver&) = delete;
    CObserver& operator=(CObserver&&) noexcept = delete;
    ~CObserver() {
        unsubscribe();
    }
    void unsubscribe();          // NOLINT
    bool isSubscribed() const {  // NOLINT
        return Observable_ != nullptr;
    }

    static void doNothing() {  // NOLINT
    }

private:
    void setObservable(CObservable* observable) {  // NOLINT
        assert(observable);
        Observable_ = observable;
    }

    CObservable* Observable_ = nullptr;  // NOLINT
    CAction onSubscribe_;                // NOLINT
    CAction onNotify_;                   // NOLINT
    CAction onUnsubscribe_;              // NOLINT
};

template <>
class CObservable<void, void> {
    using CData = void;
    using CSendBy = void;

    using CObserver = CObserver<CData, CSendBy>;
    using CObserversContainer = std::list<CObserver*>;

    using CDataSentBy = void;
    using CListeners = std::list<CObserver*>;

    friend CObserver;

public:
    using CReturn = CDataSentBy;

    CObservable() = default;
    CObservable(const CObservable&) = delete;
    CObservable(CObservable&&) noexcept = delete;
    CObservable& operator=(const CObservable&) = delete;
    CObservable& operator=(CObservable&&) noexcept = delete;
    ~CObservable() {
        unsubscribeAll();
    }
    void notify() const {                  // NOLINT
        for (CObserver* obs : Listeners_)  // NOLINT
            obs->onNotify_();              // NOLINT
    }
    void subscribe(CObserver* obs) {  // NOLINT
        assert(obs);
        if (obs->isSubscribed())  // NOLINT
            obs->unsubscribe();   // NOLINT
        Listeners_.push_back(obs);
        obs->setObservable(this);
        obs->onSubscribe_();
    }
    void unsubscribeAll() {                     // NOLINT
        while (!Listeners_.empty())             // NOLINT
            Listeners_.front()->unsubscribe();  // NOLINT
    }

private:
    void detach_(CObserver* obs) {  // NOLINT
        assert(obs);
        obs->onUnsubscribe_();
        Listeners_.remove(obs);
    }
    CListeners Listeners_;  // NOLINT
};

inline void CObserver<void, void>::unsubscribe() {
    if (!isSubscribed())  // NOLINT
        return;
    Observable_->detach_(this);
    Observable_ = nullptr;
}


template <class TData>
class CStorage {
    using CData = TData;
    using CStoredData = std::optional<CData>;

public:
    template <class... TArgs>
    explicit CStorage(TArgs&&... args) : Data_(std::forward<TArgs>(args)...) {
    }

protected:
    template <class... TArgs>
    void set(TArgs&&... args) {  // NOLINT
        Data_.emplace(std::forward<TArgs>(args)...);
    }

    CStoredData Data_{};
};

template <class TData, class TSendBy = AutoSendBy<TData>>
class CObservableMono : protected CObservable<TData, TSendBy> {
    using CData = TData;
    using CSendBy = TSendBy;

    using CBase = CObservable<CData, CSendBy>;

    using CObserver = CObserver<CData, CSendBy>;

public:
    using CBase::CGetType;
    using CBase::CReturn;

    using CBase::CBase;

    using CBase::notify;
    using CBase::unsubscribeAll;

    void subscribe(CObserver* obs) {  // NOLINT
        CBase::unsubscribeAll();
        CBase::subscribe(obs);
    }
};


template <class TData, class TSendBy, template <class T1, class T2> class TObservable>
class CObservableDataImpl : protected CStorage<TData>,
                            public TObservable<TData, TSendBy> {
    using CStorageBase = CStorage<TData>;
    using CObservableBase = TObservable<TData, TSendBy>;

public:
    template <class... TArgs>
    explicit CObservableDataImpl(TArgs&&... args)
        : CStorageBase(std::forward<TArgs>(args)...),
          CObservableBase([&Data = CStorageBase::Data_]() ->  // NOLINT
                          typename CObservableBase::CGetType { return Data; }) {
    }

    template <class... TArgs>
    void set(TArgs&&... args) {  // NOLINT
        CStorageBase::set(std::forward<TArgs>(args)...);
        CObservableBase::notify();
    }
};

template <class TData, class TSendBy = AutoSendBy<TData>>  // NOLINT
using CObservableData = CObservableDataImpl<TData, TSendBy, CObservable>;

template <class TData, class TSendBy = AutoSendBy<TData>>
using CObservableDataMono = CObservableDataImpl<TData, TSendBy, CObservableMono>;

template <class TData, class TSendBy = AutoSendBy<TData>>
class CInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T1, class T2>
    explicit CInput(T1&& OnSubscribe, T2&& OnNotify)  // NOLINT
        : CBase(std::forward<T1>(OnSubscribe), std::forward<T2>(OnNotify), CBase::doNothing) {
    }
};

template <class TData, class TSendBy = AutoSendBy<TData>>
class CHotInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T>
    explicit CHotInput(const T& Action)  // NOLINT
        : CBase(Action, Action, CBase::doNothing) {
    }
};

template <class TData, class TSendBy = AutoSendBy<TData>>  // NOLINT
class CColdInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T>
    explicit CColdInput(T&& Action)  // NOLINT
        : CBase(CBase::doNothing, std::forward<T>(Action), CBase::doNothing) {
    }
};
