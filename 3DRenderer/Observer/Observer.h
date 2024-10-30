#pragma once
#include <cassert>
#include <functional>
#include <list>
#include <optional>
#include <type_traits>

namespace observer {

namespace NSObserverDetail {

template <class TData>
bool constexpr isArithmetic = std::is_arithmetic_v<TData>;

template <class TData>
bool constexpr isPointer = std::is_pointer_v<TData>;

template <class TData>
bool constexpr isEnum = std::is_enum_v<TData>;

template <class TData>
bool constexpr isSimpleClass = isArithmetic<TData> || isPointer<TData> || isEnum<TData>;
}  // namespace NSObserverDetail

struct CByValue;
struct CByReference;

namespace NSObserverDetail {

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

}  // namespace NSObserverDetail

template <class TData, class TSendBy>
class CObserver;

template <class TData, class TSendBy>
class CObservable;

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CObserver {
    using CData = TData;
    using CSendBy = TSendBy;

    using CMyObservable = CObservable<CData, CSendBy>;

    using CDataSentBy = NSObserverDetail::CDataSentBy<CData, CSendBy>;
    using CSignature = void(CDataSentBy);
    using CAction = std::function<CSignature>;

    friend CMyObservable;

public:
    using CArg = CDataSentBy;

    template <class T1, class T2, class T3>
    CObserver(T1&& onSubscribe, T2&& onNotify, T3&& onUnsubscribe)
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
    void unsubscribe();
    bool isSubscribed() const {
        return Observable_ != nullptr;
    }
    bool hasData() const;
    CDataSentBy data() const;
    static void doNothing(CDataSentBy) {
    }

private:
    void setObservable(CMyObservable* observable) {
        assert(observable);
        Observable_ = observable;
    }

    CMyObservable* Observable_ = nullptr;
    CAction onSubscribe_;
    CAction onNotify_;
    CAction onUnsubscribe_;
};

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CObservable {
    using CData = TData;
    using CSendBy = TSendBy;

    using CObserver1 = CObserver<CData, CSendBy>;
    using CObserversContainer = std::list<CObserver1*>;

    using CDataSentBy = NSObserverDetail::CDataSentBy<CData, CSendBy>;
    using CStoreWrapper = NSObserverDetail::CStoreWrapper<CData, CSendBy>;

    using CStoredData = std::optional<CStoreWrapper>;
    using CSignature = CStoredData();
    using CGetAction = std::function<CSignature>;

    using CListeners = std::list<CObserver1*>;

    friend CObserver1;

public:
    using CGetType = CStoredData;
    using CReturn = CDataSentBy;

    template <class TF>
    CObservable(TF&& Data) : Data_(std::forward<TF>(Data)) {
        assert(Data_);
    }
    CObservable(const CObservable&) = delete;
    CObservable(CObservable&&) noexcept = delete;
    CObservable& operator=(const CObservable&) = delete;
    CObservable& operator=(CObservable&&) noexcept = delete;
    ~CObservable() {
        unsubscribeAll();
    }
    void notify() const {
        CStoredData Data = Data_();
        if (!Data.has_value())
            return;
        for (CObserver1* obs : Listeners_)
            obs->onNotify_(*Data);
    }
    void subscribe(CObserver1* obs) {
        assert(obs);
        if (obs->isSubscribed())
            obs->unsubscribe();
        Listeners_.push_back(obs);
        obs->setObservable(this);
        CStoredData Data = Data_();
        if (Data.has_value())
            obs->onSubscribe_(std::move(*Data));
    }
    void unsubscribeAll() {
        while (!Listeners_.empty())
            Listeners_.front()->unsubscribe();
    }

private:
    void detach_(CObserver1* obs) {
        assert(obs);
        CStoredData Data = Data_();
        if (Data.has_value())
            obs->onUnsubscribe_(std::move(*Data));
        Listeners_.remove(obs);
    }
    CGetAction Data_;
    CListeners Listeners_;
};

template <class TData, class TSendBy>
void CObserver<TData, TSendBy>::unsubscribe() {
    if (!isSubscribed())
        return;
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

    using CMyObservable = CObservable<CData, CSendBy>;

    using CSignature = void();
    using CAction = std::function<CSignature>;

    friend CMyObservable;

public:
    using CArg = void;

    template <class T1, class T2, class T3>
    CObserver(T1&& onSubscribe, T2&& onNotify, T3&& onUnsubscribe)
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
    void unsubscribe();
    bool isSubscribed() const {
        return Observable_ != nullptr;
    }

    static void doNothing() {
    }

private:
    void setObservable(CMyObservable* observable) {
        assert(observable);
        Observable_ = observable;
    }

    CMyObservable* Observable_ = nullptr;
    CAction onSubscribe_;
    CAction onNotify_;
    CAction onUnsubscribe_;
};

template <>
class CObservable<void, void> {
    using CData = void;
    using CSendBy = void;

    using CObserver1 = CObserver<CData, CSendBy>;
    using CObserversContainer = std::list<CObserver1*>;

    using CDataSentBy = void;
    using CListeners = std::list<CObserver1*>;

    friend CObserver1;

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
    void notify() const {
        for (CObserver1* obs : Listeners_)
            obs->onNotify_();
    }
    void subscribe(CObserver1* obs) {
        assert(obs);
        if (obs->isSubscribed())
            obs->unsubscribe();
        Listeners_.push_back(obs);
        obs->setObservable(this);
        obs->onSubscribe_();
    }
    void unsubscribeAll() {
        while (!Listeners_.empty())
            Listeners_.front()->unsubscribe();
    }

private:
    void detach_(CObserver1* obs) {
        assert(obs);
        obs->onUnsubscribe_();
        Listeners_.remove(obs);
    }
    CListeners Listeners_;
};

inline void CObserver<void, void>::unsubscribe() {
    if (!isSubscribed())
        return;
    Observable_->detach_(this);
    Observable_ = nullptr;
}

namespace NSObserverDetail {

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
    void set(TArgs&&... args) {
        Data_.emplace(std::forward<TArgs>(args)...);
    }

    CStoredData Data_{};
};
}  // namespace NSObserverDetail

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CObservableMono : protected CObservable<TData, TSendBy> {
    using CData = TData;
    using CSendBy = TSendBy;

    using CBase = CObservable<CData, CSendBy>;

    using CObserver1 = CObserver<CData, CSendBy>;

public:
    using CBase::CGetType;
    using CBase::CReturn;

    using CBase::CBase;

    using CBase::notify;
    using CBase::unsubscribeAll;

    void subscribe(CObserver1* obs) {
        CBase::unsubscribeAll();
        CBase::subscribe(obs);
    }
};

namespace NSObserverDetail {

template <class TData, class TSendBy, template <class T1, class T2> class TObservable>
class CObservableDataImpl : protected NSObserverDetail::CStorage<TData>,
                            public TObservable<TData, TSendBy> {
    using CStorageBase = NSObserverDetail::CStorage<TData>;
    using CObservableBase = TObservable<TData, TSendBy>;

public:
    template <class... TArgs>
    explicit CObservableDataImpl(TArgs&&... args)
        : CStorageBase(std::forward<TArgs>(args)...),
          CObservableBase([&Data = CStorageBase::Data_]() ->
                          typename CObservableBase::CGetType { return Data; }) {
    }

    template <class... TArgs>
    void set(TArgs&&... args) {
        CStorageBase::set(std::forward<TArgs>(args)...);
        CObservableBase::notify();
    }
};
}  // namespace NSObserverDetail

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
using CObservableData = NSObserverDetail::CObservableDataImpl<TData, TSendBy, CObservable>;

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
using CObservableDataMono = NSObserverDetail::CObservableDataImpl<TData, TSendBy, CObservableMono>;

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T1, class T2>
    explicit CInput(T1&& OnSubscribe, T2&& OnNotify)
        : CBase(std::forward<T1>(OnSubscribe), std::forward<T2>(OnNotify), CBase::doNothing) {
    }
};

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CHotInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T>
    explicit CHotInput(const T& Action) : CBase(Action, Action, CBase::doNothing) {
    }
};

template <class TData, class TSendBy = NSObserverDetail::AutoSendBy<TData>>
class CColdInput : public CObserver<TData, TSendBy> {
    using CBase = CObserver<TData, TSendBy>;

public:
    template <class T>
    explicit CColdInput(T&& Action)
        : CBase(CBase::doNothing, std::forward<T>(Action), CBase::doNothing) {
    }
};

}  // namespace observer
