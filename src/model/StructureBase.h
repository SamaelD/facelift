/**********************************************************************
**
** Copyright (C) 2018 Luxoft Sweden AB
**
** This file is part of the FaceLift project
**
** Permission is hereby granted, free of charge, to any person
** obtaining a copy of this software and associated documentation files
** (the "Software"), to deal in the Software without restriction,
** including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software,
** and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
** BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
** ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
** SPDX-License-Identifier: MIT
**
**********************************************************************/

#pragma once

#include <QObject>
#include <QVariant>
#include <QDataStream>
#include <QQmlEngine>
#include <QTextStream>
#include <array>

#include "FaceliftCommon.h"

#if defined(FaceliftModelLib_LIBRARY)
#  define FaceliftModelLib_EXPORT Q_DECL_EXPORT
#else
#  define FaceliftModelLib_EXPORT Q_DECL_IMPORT
#endif

namespace facelift {

class FaceliftModelLib_EXPORT StructureBase
{
    Q_GADGET

public:
    // Q_PROPERTIES defined here are not visible in subclasses, for some reason (Qt bug ?)

    static constexpr int ROLE_ID = 1000;
    static constexpr int ROLE_BASE = ROLE_ID + 1;

    StructureBase();

    virtual ~StructureBase();

    template<typename T = QVariant>
    T userData() const
    {
        if (!m_userData.canConvert<T>()) {
            qCCritical(LogModel) << "Cannot convert type" << m_userData.typeName()
                        << "to" << QVariant::fromValue(T()).typeName();
        }
        return m_userData.value<T>();
    }

    template<typename T>
    void setUserData(const T &value)
    {
        m_userData.setValue<T>(value);
    }

protected:
    QVariant m_userData;
};



template<typename ... FieldTypes>
class Structure : public StructureBase
{

public:
    virtual ~Structure()
    {
    }

    typedef std::tuple<FieldTypes ...> FieldTupleTypes;
    static constexpr size_t FieldCount = sizeof ... (FieldTypes);

    typedef std::array<const char *, FieldCount> FieldNames;

    const FieldTupleTypes &asTuple() const
    {
        return m_values;
    }

    FieldTupleTypes &asTuple()
    {
        return m_values;
    }

    void setValue(FieldTupleTypes value)
    {
        m_values = value;
    }

    void copyFrom(const Structure &other)
    {
        setValue(other.m_values);
        m_userData = other.m_userData;
    }

    bool operator==(const Structure &right) const
    {
        return (m_values == right.m_values);
    }

protected:
    template<std::size_t I = 0, typename ... Tp>
    inline typename std::enable_if<I == (sizeof ... (Tp) - 1), void>::type
    static toStringWithFields(const std::tuple<Tp ...> &t, const FieldNames &names, QTextStream &outStream)
    {
        using TupleElementType = typename std::tuple_element<I, std::tuple<Tp ...> >::type;
        outStream << names[I] << "=" << TypeHandler<TupleElementType>::toString(std::get<I>(t));
    }

    template<std::size_t I = 0, typename ... Tp>
    inline typename std::enable_if<(I < (sizeof ... (Tp) - 1)), void>::type
    static toStringWithFields(const std::tuple<Tp ...> &t, const FieldNames &names, QTextStream &outStream)
    {
        using TupleElementType = typename std::tuple_element<I, std::tuple<Tp ...> >::type;
        outStream << names[I] << "=" << TypeHandler<TupleElementType>::toString(std::get<I>(t));
        outStream << ", ";
        toStringWithFields<I + 1, Tp ...>(t, names, outStream);
    }

    QString toStringWithFields(const QString &structName, const FieldNames &names) const
    {
        QString s;
        QTextStream outStream(&s);
        outStream << structName << " { ";

        toStringWithFields(m_values, names, outStream);
        outStream << " }";

        return s;
    }

    FieldTupleTypes m_values = {};
};



class FaceliftModelLib_EXPORT StructureFactoryBase : public QObject
{

    Q_OBJECT

public:
    StructureFactoryBase(QQmlEngine *engine);

    template<typename Type>
    static QObject *getter(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(jsEngine);
        return new Type(qmlEngine);
    }

};


}

