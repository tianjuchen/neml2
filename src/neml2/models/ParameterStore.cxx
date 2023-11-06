// Copyright 2023, UChicago Argonne, LLC
// All Rights Reserved
// Software Name: NEML2 -- the New Engineering material Model Library, version 2
// By: Argonne National Laboratory
// OPEN SOURCE LICENSE (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "neml2/models/ParameterStore.h"
#include "neml2/models/NonlinearParameter.h"
#include "neml2/tensors/macros.h"

namespace neml2
{

template <typename T, typename>
const T &
ParameterStore::declare_parameter(const std::string & name, const std::string & input_option_name)
{
  if (_options.contains<T>(input_option_name))
    return declare_parameter(name, _options.get<T>(input_option_name));
  else if (_options.contains<CrossRef<T>>(input_option_name))
  {
    try
    {
      return declare_parameter(name, T(_options.get<CrossRef<T>>(input_option_name)));
    }
    catch (const NEMLException & e1)
    {
      try
      {
        // Handle the case of *nonlinear* parameter.
        // Note that nonlinear parameter should only exist inside a Model.
        auto model = dynamic_cast<Model *>(this);
        if (model)
        {
          auto & nl_param = Factory::get_object<NonlinearParameter<T>>(
              "Models", _options.get<CrossRef<T>>(input_option_name).raw());
          model->declare_input_variable<T>(nl_param.p);
          _nl_params.emplace(name, nl_param.p);
          return nl_param.get_value();
        }
      }
      catch (const NEMLException & e2)
      {
        std::cerr << e1.what() << std::endl;
        std::cerr << e2.what() << std::endl;
      }
    }
  }

  throw NEMLException(
      "Trying to register parameter named " + name + " from input option named " +
      input_option_name + " of type " + utils::demangle(typeid(T).name()) +
      ". Make sure you provided the correct parameter name, option name, and parameter type. Note "
      "that the parameter type can either be a plain type, a cross-reference, or an "
      "interpolation.");
}

#define instantiate_declare_parameter(T)                                                           \
  template const T & ParameterStore::declare_parameter<T>(const std::string &, const std::string &)
FOR_ALL_FIXEDDIMTENSOR(instantiate_declare_parameter);
} // namespace neml2