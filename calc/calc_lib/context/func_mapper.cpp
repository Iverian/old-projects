#include <func_mapper.h>
#include <math_helper.h>

using namespace context;
using namespace std;
using namespace math;

#define f(name) { #name, func_t(name) }

unique_ptr<func_mapper> func_mapper::_inst(new func_mapper({
    f(sin),  f(cos),  f(tan),   f(asin),  f(acos),  f(atan), f(sinh),
    f(cosh), f(tanh), f(asinh), f(acosh), f(atanh), f(abs),  f(log),
    f(exp),  f(sqrt), f(ceil),  f(floor), f(round), f(sign), f(fraction),
}));

#undef f

const func_mapper &func_mapper::instance() { return *_inst; }

bool func_mapper::present(const string &p_name) const {
  return _map.find(p_name) != _map.end();
}

func_mapper::expr_t func_mapper::exec(const string &p_name,
                                      const vector<expr_t> &p_arg) const {
  expr_t retval;
  CHECK_IF(p_arg.empty(), err::too_few_arguments);
  if (p_arg.at(0).is_number())
    retval = expr_t(_map.at(p_name)(p_arg[0].to_number()));
  return retval;
}

size_t func_mapper::argcount(const string &p_name) const {
  return present(p_name) ? 1 : 0;
}

func_mapper::func_mapper(const initializer_list<cont_t::value_type> &p_list)
    : _map(p_list) {}