#ifndef EXPRESSION_COMPUTER
#define EXPRESSION_COMPUTER

#include <QStack>

class ExpressionComputer
{
public:
  ExpressionComputer() {}

  double compute(const QString &expression);

private:
  QStack<double> _numericStack;
};

#endif
