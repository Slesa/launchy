#include <QtGui>
#include <QUrl>
#include <QFile>
#include <QRegExp>

#include "calcy.h"

#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/phoenix/functions.hpp>

//using namespace std;
using namespace boost::spirit;
using namespace phoenix;


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


bool DoCalculation(QString str, double& result) {
	// Our parser
	calculator calc;
	double n = 0;
    parse_info<const wchar_t *> info = parse((LPCTSTR) str.utf16(), calc[var(n) = arg1], space_p);

	if (info.full) {
		result = n;
		return true;
	} 
	return false;
}



void calcyPlugin::init()
{

}

void calcyPlugin::getID(uint* id)
{
	*id = HASH_CALCY;
}

void calcyPlugin::getName(QString* str)
{
	*str = "Calcy";
}

void calcyPlugin::getLabels(QList<InputData>* id)
{
	// Apply a "website" label if we think it's a website
	QString & text = id->last().getText();
	QRegExp reg(".*[\\-\\+\\*\\/]+[\\d\\s\\-\\+\\*\\/\\(\\)\\.]+");
	if (reg.indexIn(text) != -1) {
		id->last().setLabel(HASH_CALCY);
	}

		
	
}

void calcyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	if (id->last().hasLabel(HASH_CALCY)) {
		QString & text = id->last().getText();
		double res = 0.0;
		if (!DoCalculation(text, res))
			return;
		QString szRes = QString::number(res);
		results->push_front(CatItem(szRes + ".calcy", szRes, HASH_CALCY, getIcon()));
	}

}

QString calcyPlugin::getIcon()
{
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/plugins/icons/calcy.ico";
#endif
}




bool calcyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_LABELS:
			getLabels((QList<InputData>*) wParam);
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = true;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;

		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(calcy, calcyPlugin) 