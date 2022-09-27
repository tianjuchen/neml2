# batched material modeling prototype

Trying to get to the point where we can do some small strain
plasticity thing.

## Setup

You need [torch](https://pytorch.org/) to run and
[Catch2](https://github.com/catchorg/Catch2) to run the tests.
Catch2 we will likely package with the library, but Torch we may need to 
rely on the system as getting a version that works with your GPU can
be a pain.

To build and test:

```
cmake -DCMAKE_PREFIX_PATH=/path/to/torch/share/cmake .
make
make test
```

where `/path/to/torch/share/cmake` is the path to the `share/cmake`
folder for your torch install.

## Things to take a look at

Probably start looking at `SmallStrainIsotropicLinearElasticModel`
and work your way back.

## End goal

At least for me:

1. Setup a class structure for history-dependent models based on the
   user providing the implicit function definition of the model update.
2. Play around a bit with AD to get the partials.
3. Implement some small strain plasticity model.
4. Play around with making the model parameters as Torch `Variables`
   and getting parameter partials.

## Things I'm not happy with

A lot of stuff:

1. The `State` and `StateDerivative` classes should share more code.
2. The `Batched` and `Unbatched` classes should share more code.
3. I'm not content with the interface in `ConstitutiveModel`.  I want
   to
- Think a bit about return by value and return by reference.
- Think about how to provide the "tangent" and update with options
  for separate or simultaneous evaluation.
- Expand the API to let people ask for the dot product of the
  tangent with some state (i.e. the actual linearized update).
4. We need some finite different test classes to check derivatives of
   models and bits of models.

## Running on the GPU
I haven't tried this yet, but changing `TorchDefaults` in `types.h`
should let you run on the GPU without any other changes.  
We may need to think more about this in the future.