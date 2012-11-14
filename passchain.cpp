#include "passchain.h"

#include <QDebug>

PassChain::~PassChain()
{
}

int PassChain::size() const
{
    return passes.size();
}

void PassChain::clear()
{
    passes.clear();
}

void PassChain::add(IPass *pass)
{
    if (pass != 0)
        passes.push_back(pass);
}

IplImage *PassChain::process(const IplImage *source)
{
    IplImage *result;

    if (source != 0)
    {
        result = cvCloneImage(source);

        // Exécution séquentielle de chacune des passes
        for (list<IPass*>::iterator beg = passes.begin(), end = passes.end(); beg != end; ++beg)
            result = (**beg)(result);
    }
    else
        result = 0;

    return result;

}
