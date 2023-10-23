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

#include "neml2/models/Interpolation.h"

namespace neml2
{
template <typename T>
OptionSet
Interpolation<T>::expected_options()
{
  OptionSet options = NonlinearParameter<T>::expected_options();
  options.set<LabeledAxisAccessor>("argument");
  options.set<CrossRef<Scalar>>("abscissa");
  options.set<CrossRef<T>>("ordinate");
  return options;
}

template <typename T>
Interpolation<T>::Interpolation(const OptionSet & options)
  : NonlinearParameter<T>(options),
    x(this->template declare_input_variable<Scalar>(options.get<LabeledAxisAccessor>("argument"))),
    _abscissa(this->template declare_parameter<Scalar>("A", "abscissa")),
    _ordinate(this->template declare_parameter<T>("O", "ordinate"))
{
  this->setup();
}

template <typename T>
void
Interpolation<T>::set_value(const LabeledVector & in,
                            LabeledVector * out,
                            LabeledMatrix * dout_din,
                            LabeledTensor3D * d2out_din2) const
{
  auto xv = in.get<Scalar>(x);

  auto & yv = this->_p[0];
  T dyv_dxv, d2yv_dxv2;
  interpolate(
      xv, out ? &yv : nullptr, dout_din ? &dyv_dxv : nullptr, d2out_din2 ? &d2yv_dxv2 : nullptr);

  if (out)
    out->set(yv, this->p);

  if (dout_din)
    dout_din->set(dyv_dxv, this->p, x);

  if (d2out_din2)
    d2out_din2->set(d2yv_dxv2, this->p, x, x);
}

instantiate_all_FixedDimTensor(Interpolation);
} // namespace neml2