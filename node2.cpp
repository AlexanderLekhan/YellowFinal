#include "node2.h"

using namespace std;

//===========================================================================//
bool EmptyNode2::Evaluate(const Date& date, const string& event)
{
    return true;
}

//===========================================================================//
LogicalOperationNode2::LogicalOperationNode2(const LogicalOperation op,
                                           shared_ptr<Node2> left,
                                           shared_ptr<Node2> right)
    : m_op(op), m_left(left), m_right(right)
{
}

//---------------------------------------------------------------------------//
bool LogicalOperationNode2::Evaluate(const Date& date,
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
DateComparisonNode2::DateComparisonNode2(const Comparison cmp,
                                       const Date& date)
    : Base(cmp, date)
{
}

//---------------------------------------------------------------------------//
bool DateComparisonNode2::Evaluate(const Date& date,
                                  const string& /*event*/)
{
    return Compare(date);
}

//===========================================================================//
EventComparisonNode2::EventComparisonNode2(const Comparison cmp,
                                         const string& event)
    : Base(cmp, event)
{
}

//---------------------------------------------------------------------------//
bool EventComparisonNode2::Evaluate(const Date& /*date*/,
                                   const string& event)
{
    return Compare(event);
}
