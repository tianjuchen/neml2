#include "BatchedSymSymR4.h"
#include <ATen/ops/repeat_interleave.h>

BatchedSymSymR4::BatchedSymSymR4(TorchSize nbatch) :
    BatchedSymSymR4Base(nbatch)
{

}

BatchedSymSymR4::BatchedSymSymR4(const torch::Tensor & tensor) :
    BatchedSymSymR4Base(tensor)
{
  
}
