#ifndef NODE1_H
#define NODE1_H

#include <memory>

#include "node.h"
#include "date.h"
#include "flag.h"

//===========================================================================//
class Node1
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

//===========================================================================//
class EmptyNode1 : public Node1
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
class LogicalOperationNode1 : public Node1
{
public:
    LogicalOperationNode1(const LogicalOperation op,
                         std::shared_ptr<Node1> left,
                         std::shared_ptr<Node1> right);
    virtual bool Evaluate(const Date& date, const std::string& event) override;

private:
    LogicalOperation m_op;
    std::shared_ptr<Node1> m_left;
    std::shared_ptr<Node1> m_right;
};

//===========================================================================//
template <typename ValueType>
class ComparisonNode1 : public Node1
{
public:
    ComparisonNode1(const Comparison cmp, const ValueType& value);

protected:
    bool Compare(const ValueType& value);

private:
    Comparison m_cmp;
    ValueType m_value;
};

//---------------------------------------------------------------------------//
template <typename ValueType>
ComparisonNode1<ValueType>::ComparisonNode1(const Comparison cmp,
                                          const ValueType& value)
    : m_cmp(cmp), m_value(value)
{
}

//---------------------------------------------------------------------------//
template <typename ValueType>
bool ComparisonNode1<ValueType>::Compare(const ValueType& value)
{
    switch (m_cmp)
    {
    case Comparison::Less:
        return value < m_value;
    case Comparison::LessOrEqual:
        //throw std::logic_error("LessOrEqual");
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
class DateComparisonNode1 : public ComparisonNode1<Date>
{
    using Base = ComparisonNode1<Date>;
public:
    DateComparisonNode1(const Comparison cmp, const Date& date);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
class EventComparisonNode1 : public ComparisonNode1<std::string>
{
    using Base = ComparisonNode1<std::string>;
public:
    EventComparisonNode1(const Comparison cmp, const std::string& event);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

#endif // NODE1_H
