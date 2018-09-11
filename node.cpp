#include "node.h"

using namespace std;

//===========================================================================//
bool EmptyNode::Evaluate(const Date& /*date*/, const string& /*event*/) const
{
    return true;
}

//===========================================================================//
LogicalOperationNode::LogicalOperationNode(const LogicalOperation op,
                                           shared_ptr<Node> left,
                                           shared_ptr<Node> right)
    : m_op(op), m_left(left), m_right(right)
{
}

//---------------------------------------------------------------------------//
bool LogicalOperationNode::Evaluate(const Date& date,
                                    const string& event) const
{
    switch (m_op)
    {
    case LogicalOperation::And:
        return m_left->Evaluate(date, event) && m_right->Evaluate(date, event);
    case LogicalOperation::Or:
        return m_left->Evaluate(date, event) || m_right->Evaluate(date, event);
    default:
        throw logic_error("Unknown binary operation");
    }
}

//===========================================================================//
DateComparisonNode::DateComparisonNode(const Comparison cmp,
                                       const Date& date)
    : Base(cmp, date)
{
}

//---------------------------------------------------------------------------//
bool DateComparisonNode::Evaluate(const Date& date,
                                  const string& /*event*/) const
{
    return Compare(date);
}

//===========================================================================//
EventComparisonNode::EventComparisonNode(const Comparison cmp,
                                         const string& event)
    : Base(cmp, event)
{
}

//---------------------------------------------------------------------------//
bool EventComparisonNode::Evaluate(const Date& /*date*/,
                                   const string& event) const
{
    return Compare(event);
}
