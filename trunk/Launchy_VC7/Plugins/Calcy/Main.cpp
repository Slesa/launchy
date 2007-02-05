/*=============================================================================
    Copyright (c) 2002-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
////////////////////////////////////////////////////////////////////////////
//
//  Full calculator example demonstrating Phoenix
//  This is discussed in the "Closures" chapter in the Spirit User's Guide.
//
//  [ JDG 6/29/2002 ]
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/phoenix/functions.hpp>
#include <iostream>
#include <string>
#include "Main.h"
//#include "bigfp.h"

////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost::spirit;
using namespace phoenix;

////////////////////////////////////////////////////////////////////////////
//
//  Our calculator grammar using phoenix to do the semantics
//
//  Note:   The top rule propagates the expression result (value) upwards
//          to the calculator grammar self.val closure member which is
//          then visible outside the grammar (i.e. since self.val is the
//          member1 of the closure, it becomes the attribute passed by
//          the calculator to an attached semantic action. See the
//          driver code that uses the calculator below).
//
////////////////////////////////////////////////////////////////////////////
struct calc_closure : boost::spirit::closure<calc_closure, double>
{
    member1 val;
};

struct pow_
{
    template <typename X, typename Y>
    struct result { typedef X type; };

    template <typename X, typename Y>
    X operator()(X x, Y y) const
    {
        using namespace std;
        return pow(x, y);
    }
};

//  Notice how power(x, y) is lazily implemented using Phoenix function.
function<pow_> power;


struct calculator : public grammar<calculator, calc_closure::context_t>
{
    template <typename ScannerT>
    struct definition
    {
        definition(calculator const& self)
        {
            top = expression[self.val = arg1];

            expression
                =   term[expression.val = arg1]
                    >> *(   ('+' >> term[expression.val += arg1])
                        |   ('-' >> term[expression.val -= arg1])
                        )
                ;

            term
                =   factor[term.val = arg1]
                    >> *(   ('*' >> factor[term.val *= arg1])
                        |   ('/' >> factor[term.val /= arg1])
                        )
                ;

            factor
                =   ureal_p[factor.val = arg1]
                |   '(' >> expression[factor.val = arg1] >> ')'
                |   ('-' >> factor[factor.val = -arg1])
                |   ('+' >> factor[factor.val = arg1])
                ;
        }
//		const uint_parser<bigint, 10, 1, -1> bigint_parser;
        typedef rule<ScannerT, calc_closure::context_t> rule_t;
        rule_t expression, term, factor;
        rule<ScannerT> top;

        rule<ScannerT> const&
        start() const { return top; }
    };
};


bool DoCalculation(const TCHAR* str, double& result) {
// Our parser
calculator calc;
	double n = 0;
    parse_info<const wchar_t *> info = parse(str, calc[var(n) = arg1], space_p);

	if (info.full) {
		result = n;
		return true;
	} 
	return false;
}

