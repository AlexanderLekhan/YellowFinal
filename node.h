#ifndef NODE_H
#define NODE_H

#include <memory>

#include "date.h"

//===========================================================================//
class Node
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

//===========================================================================//
class EmptyNode : public Node
{
public:
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
enum class LogicalOperation
{
    And,
    Or
};

//===========================================================================//
class LogicalOperationNode : public Node
{
public:
    LogicalOperationNode(const LogicalOperation op,
                         std::shared_ptr<Node> left,
                         std::shared_ptr<Node> right);
    virtual bool Evaluate(const Date& date, const std::string& event) override;

private:
    LogicalOperation m_op;
    std::shared_ptr<Node> m_left;
    std::shared_ptr<Node> m_right;
};

//===========================================================================//
enum class Comparison
{
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

//===========================================================================//
template <typename ValueType>
class ComparisonNode : public Node
{
public:
    ComparisonNode(const Comparison cmp, const ValueType& value);

protected:
    bool Compare(const ValueType& value);

private:
    Comparison m_cmp;
    ValueType m_value;
};

//---------------------------------------------------------------------------//
template <typename ValueType>
ComparisonNode<ValueType>::ComparisonNode(const Comparison cmp,
                                          const ValueType& value)
    : m_cmp(cmp), m_value(value)
{
}

//---------------------------------------------------------------------------//
template <typename ValueType>
bool ComparisonNode<ValueType>::Compare(const ValueType& value)
{
    switch (m_cmp)
    {
    case Comparison::Less:
        return value < m_value;
    case Comparison::LessOrEqual:
        return !(m_value < value);
    case Comparison::Greater:
        return m_value < value;
    case Comparison::GreaterOrEqual:
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
class DateComparisonNode : public ComparisonNode<Date>
{
    using Base = ComparisonNode<Date>;
public:
    DateComparisonNode(const Comparison cmp, const Date& date);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

//===========================================================================//
class EventComparisonNode : public ComparisonNode<std::string>
{
    using Base = ComparisonNode<std::string>;
public:
    EventComparisonNode(const Comparison cmp, const std::string& event);
    virtual bool Evaluate(const Date& date, const std::string& event) override;
};

#endif // NODE_H
