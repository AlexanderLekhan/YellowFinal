#ifndef NODE2_H
#define NODE2_H

#include <memory>

#include "node.h"
#include "date.h"
#include "flag.h"

//===========================================================================//
class Node2
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

//===========================================================================//
class EmptyNode2 : public Node2
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
class LogicalOperationNode2 : public Node2
{
public:
    LogicalOperationNode2(const LogicalOperation op,
                         std::shared_ptr<Node2> left,
                         std::shared_ptr<Node2> right);
    virtual bool Evaluate(const Date& date, const std::string& event) override;

private:
    LogicalOperation m_op;
    std::shared_ptr<Node2> m_left;
    std::shared_ptr<Node2> m_right;
};

//===========================================================================//
template <typename ValueType>
class ComparisonNode2 : public Node2
{
public:
    ComparisonNode2(const Comparison cmp, const ValueType& value);

protected:
    bool Compare(const ValueType& value);

private:
    Comparison m_cmp;
    ValueType m_value;
};

//---------------------------------------------------------------------------//
template <typename ValueType>
ComparisonNode2<ValueType>::ComparisonNode2(const Comparison cmp,
                                          const ValueType& value)
    : m_cmp(cmp), m_value(value)
{
}

//---------------------------------------------------------------------------//
template <typename ValueType>
bool ComparisonNode2<ValueType>::Compare(const ValueType& value)
{
    switch (m_cmp)
    {
    case Comparison::Less:
        return value < m_value;
    case Comparison::LessOrEqual:
        //throw std::logic_error("LessOrEqual");
        //g_flag = true;
        return !(m_value < value);
    case Comparison::Greater:
        return m_value < value;
    case Comparison::GreaterOrEqual:
        //throw std::logic_error("GreaterOrEqual");
        return !(value < m_value);
    case Comparison::Equal:
        return !(value < m_value || m_value < value);
    case Comparison::NotEqual:
        return value < m_value || m_value < value;
    default:
        throw std::logic_error("Unknown comparison");
    }
}

//===========================================================================//
class DateComparisonNode2 : public ComparisonNode2<Date>
{
    using Base = ComparisonNode2<Date>;
public:
    DateComparisonNode2(const Comparison cmp, const Date& date);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
class EventComparisonNode2 : public ComparisonNode2<std::string>
{
    using Base = ComparisonNode2<std::string>;
public:
    EventComparisonNode2(const Comparison cmp, const std::string& event);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

#endif // NODE2_H
