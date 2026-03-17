# Analog Anti-Aliasing Filter
There is an open question regarding the topology and design of the AAAF. Initially we favoured a Type II Chebyshev filter, however upon learning that this cannot be implemented on a Sallen-Key topology, we reconsider the filter design, or choose to implement it using a passive Cauer topology.
The options are as follows
- Type II Chebyshev filter implemented on a Cauer topology
- Type I Chebyshev filter implemented on a Sallen-Key topology
- Bessel filter implemented on a Sallen-Key Topology

| Type II Cheby - Cauer                                               | Type I Cheby - Sallen-Key                                                       | Bessel - Sallen-Key        |
| ------------------------------------------------------------------- | ------------------------------------------------------------------------------- | -------------------------- |
| No passband ripple                                                  | Little (possibly negligible) passband ripple                                    |                            |
|                                                                     |                                                                                 | Linear phase response      |
| Possibly lower order                                                |                                                                                 | Highest order solution     |
| High susceptibility to component deviations (inherent to LC ladder) | Lesser susceptibility to component deviations<br>(0.1% Thin-Film R, 1% NP0 cap) | Lesser susceptibility .... |
| Not Effected by Op-Amp Bandwidth                                    |                                                                                 |                            |
|                                                                     |                                                                                 |                            |
