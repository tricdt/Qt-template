#pragma once
#include <QtCore/qglobal.h>

#define Q_PROPERTY_CREATE(TYPE, prop, Method)                                                      \
    Q_PROPERTY(TYPE prop READ prop WRITE set##Method NOTIFY prop##Changed)                         \
public:                                                                                            \
    Q_SIGNAL void prop##Changed();                                                                 \
    void set##Method(TYPE value)                                                                   \
    {                                                                                              \
        _##prop = std::move(value);                                                                \
        Q_EMIT prop##Changed();                                                                    \
    }                                                                                              \
    TYPE prop() const                                                                              \
    {                                                                                              \
        return _##prop;                                                                            \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE _##prop;

#define Q_PROPERTY_REF_CREATE(TYPE, M)                                                             \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed)                                        \
public:                                                                                            \
    Q_SIGNAL void p##M##Changed();                                                                 \
    void set##M(const TYPE& M)                                                                     \
    {                                                                                              \
        _p##M = M;                                                                                 \
        Q_EMIT p##M##Changed();                                                                    \
    }                                                                                              \
    const TYPE& get##M() const                                                                     \
    {                                                                                              \
        return _p##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE _p##M;

#define Q_PROPERTY_CREATE_Q_H(TYPE, prop, Method)                                                  \
    Q_PROPERTY(TYPE prop READ prop WRITE set##Method NOTIFY prop##Changed)                            \
public:                                                                                            \
    Q_SIGNAL void prop##Changed();                                                                 \
    void set##Method(TYPE value);                                                                           \
    TYPE prop() const;

#define Q_PROPERTY_REF_CREATE_Q_H(TYPE, M)                                                         \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed)                            \
public:                                                                                            \
    Q_SIGNAL void p##M##Changed();                                                                 \
    void set##M(const TYPE& M);                                                                    \
    const TYPE& get##M() const;

#define Q_PRIVATE_CREATE_Q_H(TYPE, M)                                                              \
public:                                                                                            \
    void set##M(TYPE M);                                                                           \
    TYPE get##M() const;

#define Q_PRIVATE_REF_CREATE_Q_H(TYPE, M)                                                          \
public:                                                                                            \
    void set##M(const TYPE& M);                                                                    \
    const TYPE& get##M() const;

#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M)                                                    \
    void CLASS::set##M(TYPE M)                                                                     \
    {                                                                                              \
        Q_D(CLASS);                                                                                \
        d->_p##M = std::move(M);                                                                   \
        Q_EMIT p##M##Changed();                                                                    \
    }                                                                                              \
    TYPE CLASS::get##M() const                                                                     \
    {                                                                                              \
        return d_ptr->_p##M;                                                                       \
    }

#define Q_PROPERTY_REF_CREATE_Q_CPP(CLASS, TYPE, M)                                                \
    void CLASS::set##M(const TYPE& M)                                                              \
    {                                                                                              \
        Q_D(CLASS);                                                                                \
        d->_p##M = M;                                                                              \
        Q_EMIT p##M##Changed();                                                                    \
    }                                                                                              \
    const TYPE& CLASS::get##M() const                                                              \
    {                                                                                              \
        return d_ptr->_p##M;                                                                       \
    }

#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M)                                                     \
    void CLASS::set##M(TYPE M)                                                                     \
    {                                                                                              \
        Q_D(CLASS);                                                                                \
        d->_p##M = std::move(M);                                                                   \
    }                                                                                              \
    TYPE CLASS::get##M() const                                                                     \
    {                                                                                              \
        return d_ptr->_p##M;                                                                       \
    }

#define Q_PRIVATE_REF_CREATE_Q_CPP(CLASS, TYPE, M)                                                 \
    void CLASS::set##M(const TYPE& M)                                                              \
    {                                                                                              \
        Q_D(CLASS);                                                                                \
        d->_p##M = M;                                                                              \
    }                                                                                              \
    const TYPE& CLASS::get##M() const                                                              \
    {                                                                                              \
        return d_ptr->_p##M;                                                                       \
    }

#define Q_PROPERTY_CREATE_D(TYPE, M)                                                               \
private:                                                                                           \
    TYPE _p##M;

#define Q_PRIVATE_CREATE_D(TYPE, M)                                                                \
private:                                                                                           \
    TYPE _p##M;

#define Q_PRIVATE_CREATE(TYPE, M)                                                                  \
public:                                                                                            \
    void set##M(TYPE M)                                                                            \
    {                                                                                              \
        _p##M = std::move(M);                                                                      \
    }                                                                                              \
    TYPE get##M() const                                                                            \
    {                                                                                              \
        return _p##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE _p##M;

#define Q_PRIVATE_REF_CREATE(TYPE, M)                                                              \
public:                                                                                            \
    void set##M(const TYPE& M)                                                                     \
    {                                                                                              \
        _p##M = M;                                                                                 \
    }                                                                                              \
    const TYPE& get##M() const                                                                     \
    {                                                                                              \
        return _p##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE _p##M;

#define Q_Q_CREATE(CLASS)                                                                          \
protected:                                                                                         \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr);                                   \
    QScopedPointer<CLASS##Private> d_ptr;                                                          \
                                                                                                   \
private:                                                                                           \
    Q_DISABLE_COPY(CLASS)                                                                          \
    Q_DECLARE_PRIVATE(CLASS);

#define Q_D_CREATE(CLASS)                                                                          \
protected:                                                                                         \
    CLASS* q_ptr;                                                                                  \
                                                                                                   \
private:                                                                                           \
    Q_DECLARE_PUBLIC(CLASS);
