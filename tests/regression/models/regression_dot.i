[Solvers]
  [newton]
    type = NewtonNonlinearSolver
  []
[]

[Models]
  [Erate]
    type = SymR2ForceRate
    force = total_strain
  []
  [Eerate]
    type = ElasticStrainRate
  []
  [elasticity]
    type = CauchyStressRateFromElasticStrainRate
    E = 1e5
    nu = 0.3
  []
  [mandel_stress]
    type = IsotropicMandelStress
  []
  [isoharden]
    type = LinearIsotropicHardening
    K = 1000
  []
  [j2]
    type = J2StressMeasure
  []
  [yield]
    type = IsotropicHardeningYieldFunction
    stress_measure = j2
    yield_stress = 5
  []
  [direction]
    type = AssociativePlasticFlowDirection
    yield_function = yield
  []
  [eprate]
    type = AssociativeIsotropicPlasticHardening
    yield_function = yield
  []
  [hrate]
    type = PerzynaPlasticFlowRate
    eta = 100
    n = 2
  []
  [Eprate]
    type = PlasticStrainRate
  []
  [rate]
    type = ComposedModel
    models = 'Erate Eerate elasticity mandel_stress isoharden yield direction eprate hrate Eprate'
  []
[]