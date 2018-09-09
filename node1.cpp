#include "node1.h"

using namespace std;

//===========================================================================//
bool EmptyNode1::Evaluate(const Date& date, const string& event)
{
    return true;
}

//===========================================================================//
LogicalOperationNode1::LogicalOperationNode1(const LogicalOperation op,
                                           shared_ptr<Node1> left,
                                           shared_ptr<Node1> right)
    : m_op(op), m_left(left), m_right(right)
{
}

//---------------------------------------------------------------------------//
bool LogicalOperationNode1::Evaluate(const Date& date,
                                    const string& event)
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
DateComparisonNode1::DateComparisonNode1(const Comparison cmp,
                                       const Date& date)
    : Base(cmp, date)
{
}

//---------------------------------------------------------------------------//
bool DateComparisonNode1::Evaluate(const Date& date,
                                  const string& /*event*/)
{
    return Compare(date);
}

//===========================================================================//
EventComparisonNode1::EventComparisonNode1(const Comparison cmp,
                                         const string& event)
    : Base(cmp, event)
{
}

//---------------------------------------------------------------------------//
bool EventComparisonNode1::Evaluate(const Date& /*date*/,
                                   const string& event)
{
    return Compare(event);
}
