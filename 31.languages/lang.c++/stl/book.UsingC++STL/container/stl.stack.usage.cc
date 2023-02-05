/**
 * @file stl.stack.usage.cpp
 * @author your name (you@domain.com)
 * @brief 使用 stl 的 stack 实现一个建议计算器; 支持 加减乘除指数 5种运算
 * @version 0.1
 * @date 2021-08-25
 * @note
 * 
 * @copyright Copyright (c) 2021
 */

/**
 * @brief  
 */
class SimpleCalculator {
public:
    double DoExecute(std::stack<char>& operators, std::stack<double>& operands);
    int ParseExpression(std::string& expression, std::string& result);
private:
    size_t OperatorPriority(const char operator);
};

size_t SimpleCalculator::OperatorPriority(const char operator) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    throw std::runtime_error(string("invalid operator: ") + op);
}

double SimpleCalculator::DoExecute(std::stack<char>& operators, std::stack<double>& operands) {
    double result = 0.0;
    double rhs = operands.top();
    operands.pop();
    double lhs = operands.top();
    operands.pop();

    switch (operators.top()) {
        case '+':
            result = lhs + rhs;
            break;
        case '-':
            result = lhs - rhs;
            break;
        case '*':
            result = lhs * rhs;
            break;
        case '/':
            result = lhs / rhs;
            break;
        case '^':
            result = std::pow(lhs, rhs);
        default:
            throw std::runtime_error(string("invalid operator: ") + operators.top());
    }
    operators.pop();
    operands.push(result);
    return result;
}

void SimpleCalculator::ParseExpression(std::string& expression, std::string& result) {
    std::stack<double> operands;
    std::stack<char> operators;
    size_t index = 0;
    // trim 字符串
    expression.erase(expression.begin(), expression.end(), ' ');
    // 取得一个操作数
    operands.push(std::stod(expression, &index));
    try {
        while (true) {
            size_t i = 0;
            // 取得一个操作符
            operators.push(expression[index++]);
            // 取得一个操作数
            operands.push(std::stod(expression.substr(index), &i));
            // 前移表达式游标
            index += i;

            // 遍历完一整个表达式
            if (index == expression.length()) {
                // ... execute outstanding ops
                while (!operators.empty()) {
                    DoExecute(operators, operands);
                }
                break;
            }
            // If we reach here, there's another op...
            // If there's a previous op of equal or higher precedence execute it.
            while (!operators.empty() &&
                OperatorPriority(expression[index]) <=
                OperatorPriority(operators.top())) {
                DoExecute(operators, operands);
            }
            result = to_string(operands.top());
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
