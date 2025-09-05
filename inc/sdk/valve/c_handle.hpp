//
// Created by drev on 15/08/2025.
//

#pragma once


class base_handle;

#include "defs.hpp"

class handle_entity
{
public:
    virtual ~handle_entity() {}
    virtual void set_ref_e_handle(const base_handle& handle) = 0;
    virtual const base_handle& get_ref_e_handle() const = 0;
};

class base_handle
{
    friend class C_BaseEntityList;
public:
    base_handle();
    base_handle(const base_handle& other);
    base_handle(unsigned long value);
    base_handle(int iEntry, int iSerialNumber);

    void init(int iEntry, int iSerialNumber);
    void term();

    // Even if this returns true, Get() still can return return a non-null value.
    // This just tells if the handle has been initted with any values.
    bool is_valid() const;

    int get_entry_index() const;
    int get_serial_number() const;

    int to_int() const;
    bool operator !=(const base_handle& other) const;
    bool operator ==(const base_handle& other) const;
    bool operator ==(const handle_entity* pEnt) const;
    bool operator !=(const handle_entity* pEnt) const;
    bool operator <(const base_handle& other) const;
    bool operator <(const handle_entity* pEnt) const;

    // Assign a value to the handle.
    const base_handle& operator=(const handle_entity* pEntity);
    const base_handle& set(const handle_entity* pEntity);

    handle_entity* get() const;
protected:
    unsigned long  m_index;
};

template< class T >
class c_handle : public base_handle
{
public:

    c_handle();
    c_handle(int iEntry, int iSerialNumber);
    c_handle(const base_handle& handle);
    c_handle(T* pVal);

    static c_handle<T> from_index(int index);

    T* get() const;
    void set(const T* pVal);

    operator T* ();
    operator T* () const;

    bool operator !() const;
    bool operator==(T* val) const;
    bool operator!=(T* val) const;
    const base_handle& operator=(const T* val);

    T* operator->() const;
};

template<class T>
c_handle<T>::c_handle()
{
}

template<class T>
c_handle<T>::c_handle(int iEntry, int iSerialNumber)
{
    init(iEntry, iSerialNumber);
}

template<class T>
c_handle<T>::c_handle(const base_handle& handle)
    : base_handle(handle)
{
}

template<class T>
c_handle<T>::c_handle(T* pObj)
{
    term();
    Set(pObj);
}

template<class T>
inline c_handle<T> c_handle<T>::from_index(int index)
{
    c_handle<T> ret;
    ret.m_Index = index;
    return ret;
}

template<class T>
inline T* c_handle<T>::get() const
{
    return (T*)base_handle::get();
}

template<class T>
inline c_handle<T>::operator T* ()
{
    return get();
}

template<class T>
inline c_handle<T>::operator T* () const
{
    return get();
}

template<class T>
inline bool c_handle<T>::operator !() const
{
    return !get();
}

template<class T>
inline bool c_handle<T>::operator==(T* val) const
{
    return get() == val;
}

template<class T>
inline bool c_handle<T>::operator!=(T* val) const
{
    return get() != val;
}

template<class T>
void c_handle<T>::set(const T* pVal)
{
    base_handle::set(reinterpret_cast<const handle_entity*>(pVal));
}

template<class T>
inline const base_handle& c_handle<T>::operator=(const T* val)
{
    Set(val);
    return *this;
}

template<class T>
T* c_handle<T>::operator -> () const
{
    return get();
}

inline base_handle::base_handle()
{
    m_index = INVALID_EHANDLE_INDEX;
}

inline base_handle::base_handle(const base_handle& other)
{
    m_index = other.m_index;
}

inline base_handle::base_handle(unsigned long value)
{
    m_index = value;
}

inline base_handle::base_handle(int iEntry, int iSerialNumber)
{
    init(iEntry, iSerialNumber);
}

inline void base_handle::init(int iEntry, int iSerialNumber)
{
    m_index = (unsigned long)(iEntry | (iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS));
}

inline void base_handle::term()
{
    m_index = INVALID_EHANDLE_INDEX;
}

inline bool base_handle::is_valid() const
{
    return m_index != INVALID_EHANDLE_INDEX;
}

inline int base_handle::get_entry_index() const
{
    // There is a hack here: due to a bug in the original implementation of the
    // entity handle system, an attempt to look up an invalid entity index in
    // certain cirumstances might fall through to the the mask operation below.
    // This would mask an invalid index to be in fact a lookup of entity number
    // NUM_ENT_ENTRIES, so invalid ent indexes end up actually looking up the
    // last slot in the entities array. Since this slot is always empty, the
    // lookup returns NULL and the expected behavior occurs through this unexpected
    // route.
    // A lot of code actually depends on this behavior, and the bug was only exposed
    // after a change to NUM_SERIAL_NUM_BITS increased the number of allowable
    // static props in the world. So the if-stanza below detects this case and
    // retains the prior (bug-submarining) behavior.
    if (!is_valid())
        return NUM_ENT_ENTRIES - 1;
    return m_index & ENT_ENTRY_MASK;
}

inline int base_handle::get_serial_number() const
{
    return m_index >> NUM_SERIAL_NUM_SHIFT_BITS;
}

inline int base_handle::to_int() const
{
    return (int)m_index;
}

inline bool base_handle::operator !=(const base_handle& other) const
{
    return m_index != other.m_index;
}

inline bool base_handle::operator ==(const base_handle& other) const
{
    return m_index == other.m_index;
}

inline bool base_handle::operator ==(const handle_entity* pEnt) const
{
    return get() == pEnt;
}

inline bool base_handle::operator !=(const handle_entity* pEnt) const
{
    return get() != pEnt;
}

inline bool base_handle::operator <(const base_handle& other) const
{
    return m_index < other.m_index;
}

inline bool base_handle::operator <(const handle_entity* pEntity) const
{
    unsigned long otherIndex = (pEntity) ? pEntity->get_ref_e_handle().m_index : INVALID_EHANDLE_INDEX;
    return m_index < otherIndex;
}

inline const base_handle& base_handle::operator=(const handle_entity* pEntity)
{
    return set(pEntity);
}

inline const base_handle& base_handle::set(const handle_entity* pEntity)
{
    if (pEntity)
        *this = pEntity->get_ref_e_handle();
    else
        m_index = INVALID_EHANDLE_INDEX;

    return *this;
}